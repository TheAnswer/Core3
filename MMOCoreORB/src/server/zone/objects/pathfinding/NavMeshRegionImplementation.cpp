#include "server/zone/objects/pathfinding/NavMeshRegion.h"
#include "templates/tangible/SharedStructureObjectTemplate.h"
#include "server/zone/managers/collision/NavMeshManager.h"
#include "server/zone/Zone.h"

void NavMeshRegionImplementation::initializeTransientMembers() {

}

AABB NavMeshRegionImplementation::getBoundingBox() {
    return meshBounds;
}


void NavMeshRegionImplementation::setRadius(float f) {
    RegionImplementation::setRadius(f);
    float x = getPositionX();
    float y = getPositionY();
    Vector3 center(x, 0, y);
    Vector3 radius(f, f, f);
    meshBounds = AABB(center-radius, center+radius);
}

void NavMeshRegionImplementation::updateNavMesh(const AABB& bounds) {
   // navmeshRegion->originalBounds = Sphere(center, r);

    if (navMesh == NULL || !navMesh->isLoaded()) {
        NavMeshManager::instance()->enqueueJob(zone, _this.getReferenceUnsafeStaticCast(), meshBounds);
    } else {
        NavMeshManager::instance()->enqueueJob(zone, _this.getReferenceUnsafeStaticCast(), bounds);
    }


}

void NavMeshRegionImplementation::initializeNavRegion(Vector3& position, float radius, Zone* zone, String& name, bool buildMesh) {
    meshName = zone->getZoneName()+"_"+name+".navmesh";
    navMesh = new RecastNavMesh("navmeshes/"+meshName);
    initializePosition(position[0], position[1], position[2]);
    setRadius(radius);
    setZone(zone);
    if (!navMesh->isLoaded() && buildMesh) {
        updateNavMesh(getBoundingBox());
    }

}

void NavMeshRegionImplementation::initialize() {
    meshName = zone->getZoneName()+"_"+String::valueOf(getObjectID())+".navmesh";
}

void NavMeshRegionImplementation::notifyEnter(SceneObject* object) {

    if(disableUpdates)
        return;

    if(!containedObjects.contains(object) && object->isStructureObject() && !object->getObjectTemplate()->getTemplateFileName().contains("construction_")) {

        info(object->getObjectTemplate()->getTemplateFileName() + " caused navmesh rebuild", true);
        Vector3 position = object->getWorldPosition();
        const BaseBoundingVolume *volume = object->getBoundingVolume();
        if(volume) {
            AABB bbox = volume->getBoundingBox();
            float len = bbox.extents()[bbox.longestAxis()];
            len = (len / 32.0f) * 32;
            Vector3 extents(len, len, len);
            bbox = AABB(position-extents, position+extents);
            info("Rebuilding from structure extents :\n" + bbox.getMinBound()->toString() + ", " + bbox.getMaxBound()->toString(), true);
            Core::getTaskManager()->scheduleTask([=]{
                updateNavMesh(bbox);
            }, "updateCityNavmesh", 500);
        }
    }

    containedObjects.add(object);
}

void NavMeshRegionImplementation::notifyExit(SceneObject* object) {

    if(disableUpdates)
        return;

    if(containedObjects.contains(object) && object->isStructureObject() && !object->getObjectTemplate()->getTemplateFileName().contains("construction_")) {

        info(object->getObjectTemplate()->getTemplateFileName() + " caused navmesh rebuild", true);
        Vector3 position = object->getWorldPosition();
        const BaseBoundingVolume *volume = object->getBoundingVolume();
        if(volume) {
            AABB bbox = volume->getBoundingBox();
            float len = bbox.extents()[bbox.longestAxis()];
            len = (len / 32.0f) * 32;
            Vector3 extents(len, len, len);
            bbox = AABB(position-extents, position+extents);
            info("Rebuilding from structure extents :\n" + bbox.getMinBound()->toString() + ", " + bbox.getMaxBound()->toString(), true);
            Core::getTaskManager()->scheduleTask([=]{
                updateNavMesh(bbox);
            }, "updateCityNavmesh", 500);
        }
    }

    containedObjects.add(object);
}