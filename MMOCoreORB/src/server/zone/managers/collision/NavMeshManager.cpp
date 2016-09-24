#include "NavMeshManager.h"
#include "pathfinding/RecastNavMesh.h"
#include "pathfinding/RecastNavMeshBuilder.h"
#include "server/zone/managers/planet/PlanetManager.h"



void NavMeshManager::enqueueJob(Zone* zone, NavmeshRegion* region, AABB areaToBuild) {
    Locker locker(&jobQueueMutex);
    Reference<NavMeshJob*> job = runningJobs.get(region->name);

    if (job) {
        job->addArea(areaToBuild);
        return;
    }

    job = jobs.get(region->name);

    if (job == NULL) {
        job = new NavMeshJob(region, zone);
    }

    job->addArea(areaToBuild);
    jobs.put(region->name, job);

    checkJobs();
}

void NavMeshManager::checkJobs() {

    Locker locker(&jobQueueMutex);

    while (runningJobs.size() < maxConcurrentJobs && jobs.size() > 0) {

        Reference<NavMeshJob*> job = jobs.get(0);
        const String& name = job->getRegion()->name;
        jobs.drop(name);
        runningJobs.put(name, job);

        Core::getTaskManager()->executeTask([=]{
            startJob(job);

            Locker locker(&jobQueueMutex);
            runningJobs.drop(name);
        }, "updateNavMesh");
    }
}


void NavMeshManager::startJob(Reference<NavMeshJob*> job) {

    if(job == NULL) {
        return;
    }

    Reference<Zone*> zone = job->getZone();
    if (!zone) {
        Locker locker(&jobQueueMutex);
        jobs.put(job->getRegion()->name, job);
        return;
    }

    job->getMutex().lock();
    //copy and clear this vector otherwise our scene data may not be correct if a zone was added during the build process
    Vector <AABB> dirtyZones = Vector<AABB>(job->getAreas());
    job->getAreas().removeAll();
    job->getMutex().unlock();

    NavmeshRegion *region = job->getRegion();
    const Sphere &bSphere = region->meshBounds;

    float range = bSphere.getRadius();
    const Vector3 center = Vector3(bSphere.getCenter()[0], 0, -bSphere.getCenter()[2]);

    String filename = region->name;

    SortedVector <ManagedReference<QuadTreeEntry *>> closeObjects;
    zone->getInRangeSolidObjects(center.getX(), center.getZ(), range, &closeObjects, true);
    Matrix4 identity;

    Vector <Reference<MeshData *>> meshData;

    for (int i = 0; i < closeObjects.size(); i++) {
        SceneObject *sceno = closeObjects.get(i).castTo<SceneObject *>();
        if (sceno)
            meshData.addAll(sceno->getTransformedMeshData(&identity));
    }

    RecastNavMeshBuilder *builder = NULL;

    Vector3 radius(range, 0, range);
    AABB box = AABB(center - radius, center + radius);

    builder = new RecastNavMeshBuilder(zone, filename);


    // lower our resolution a bit for player cities
    RecastSettings &settings = builder->getRecastConfig();
    settings.m_cellSize = 0.2f;
    settings.m_cellHeight = 0.2f;
    settings.m_tileSize = 64.0f;

    builder->initialize(meshData, box, 4.0f);
    // This will take a very long time to complete
    Reference<RecastNavMesh*> navmesh = region->getNavMesh();
    if (navmesh == NULL || !navmesh->isLoaded()) {
        builder->build();
    } else if (dirtyZones.size() > 0) {
        for (int i = dirtyZones.size() - 1; i >= 0; i--) {
            const AABB &area = dirtyZones.get(i);
            builder->rebuildArea(area, navmesh);
        }
    }

    info("Region->name: " + region->name);

    builder->saveAll(region->name);

    navmesh = new RecastNavMesh(filename);
    region->setNavMesh(navmesh);

    if(job->getAreas().size() > 0) {
        Locker locker(&jobQueueMutex);
        jobs.put(region->name, job);
    }

    delete builder;

    checkJobs();
}

bool NavMeshManager::AABBEncompasessAABB(const AABB& lhs, const AABB& rhs) {
    const Vector3 &lMin = *lhs.getMinBound();
    const Vector3 &lMax = *lhs.getMaxBound();

    const Vector3 &rMin = *rhs.getMinBound();
    const Vector3 &rMax = *rhs.getMaxBound();

    if (rMin[0] >= lMin[0] && rMin[2] >= lMin[2] && rMax[0] <= lMax[0] && rMax[2] <= lMax[2])
        return true;

    return false;
}
