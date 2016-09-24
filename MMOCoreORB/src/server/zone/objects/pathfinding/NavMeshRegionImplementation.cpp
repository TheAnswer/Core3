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

void NavMeshRegionImplementation::initializeNavRegion(Vector3& position, float radius, Zone* zone, String& name) {
    meshName = zone->getZoneName()+"_"+name+".navmesh";
    navMesh = new RecastNavMesh(meshName);
    initializePosition(position[0], position[1], position[2]);
    setRadius(radius);
    setZone(zone);
    if (!navMesh->isLoaded()) {
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
        const SharedStructureObjectTemplate *stemp = dynamic_cast<SharedStructureObjectTemplate*>(object->getObjectTemplate());

        StructureFootprint* structureFootprint = stemp->getStructureFootprint();
        if(volume) {
            AABB bbox = volume->getBoundingBox();
            info("Rebuilding from structure extents :\n" + bbox.getMinBound()->toString() + ", " + bbox.getMaxBound()->toString(), true);
            bbox = AABB(position-(bbox.extents()), position+(bbox.extents()));
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
        const SharedStructureObjectTemplate *stemp = dynamic_cast<SharedStructureObjectTemplate*>(object->getObjectTemplate());

        StructureFootprint* structureFootprint = stemp->getStructureFootprint();
        if(volume) {
            AABB bbox = volume->getBoundingBox();
            info("Rebuilding from structure extents :\n" + bbox.getMinBound()->toString() + ", " + bbox.getMaxBound()->toString(), true);
            bbox = AABB(position-(bbox.extents()), position+(bbox.extents()));
            info("Rebuilding from structure extents :\n" + bbox.getMinBound()->toString() + ", " + bbox.getMaxBound()->toString(), true);
            Core::getTaskManager()->scheduleTask([=]{
                updateNavMesh(bbox);
            }, "updateCityNavmesh", 500);
        }
    }

    containedObjects.add(object);
}