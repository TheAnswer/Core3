/*
 * ActiveAreaImplementation.cpp
 *
 *  Created on: 02/06/2010
 *      Author: victor
 */

#include "server/zone/objects/area/ActiveArea.h"
#include "events/ActiveAreaEvent.h"
#include "server/zone/objects/area/areashapes/AreaShape.h"
#include "server/zone/objects/region/SpawnArea.h"
#include "server/zone/managers/creature/SpawnAreaMap.h"

bool ActiveAreaImplementation::containsPoint(float px, float py, uint64 cellid) const {
	if (cellObjectID != 0 && cellObjectID != cellid)
		return false;

	return containsPoint(px, py);
}

bool ActiveAreaImplementation::containsPoint(float px, float py) const {
	if (areaShape == nullptr) {
		float dx = getPositionX() - px;
		float dy = getPositionY() - py;

		float squaredLength = dx * dx + dy * dy;
		return squaredLength <= (radius * radius);
	}

	return areaShape->containsPoint(px, py);
}

void ActiveAreaImplementation::enqueueEnterEvent(SceneObject* obj) {
#ifdef WITH_STM
	notifyEnter(obj);
#else
	Reference<Task*> task = new ActiveAreaEvent(_this.getReferenceUnsafeStaticCast(), obj, ActiveAreaEvent::ENTEREVENT);
	obj->executeOrderedTask(task);

#endif
}

void ActiveAreaImplementation::enqueueExitEvent(SceneObject* obj) {
#ifdef WITH_STM
	notifyExit(obj);
#else
	Reference<Task*> task = new ActiveAreaEvent(_this.getReferenceUnsafeStaticCast(), obj, ActiveAreaEvent::EXITEVENT);
	obj->executeOrderedTask(task);

#endif
}

void ActiveAreaImplementation::notifyEnter(SceneObject* obj) {
	if (cellObjectID == 0 || cellObjectID == obj->getParentID())
		notifyObservers(ObserverEventType::ENTEREDAREA, obj);

	if (obj->isPlayerCreature()) {
		if (attachedScenery.size() > 0) {
			ManagedReference<SceneObject*> sceno = obj;
			Vector<ManagedReference<SceneObject* > > scene = attachedScenery;

			Core::getTaskManager()->executeTask([=] () {
				for (int i = 0; i < scene.size(); i++) {
					SceneObject* scenery = scene.get(i);
					Locker locker(scenery);

					scenery->sendTo(sceno, true);
				}
			}, "SendSceneryLambda");
		}

		if (obj->isDebuggingRegions())
			sendDebugMessage(obj, true);
	}
}

void ActiveAreaImplementation::notifyExit(SceneObject* obj) {
	if (cellObjectID == 0 || cellObjectID != obj->getParentID())
		notifyObservers(ObserverEventType::EXITEDAREA, obj);

	if (obj->isPlayerCreature()) {
		if (attachedScenery.size() > 0) {
			ManagedReference<SceneObject*> sceno = obj;
			Vector<ManagedReference<SceneObject* > > scene = attachedScenery;

			Core::getTaskManager()->executeTask([=] () {
				for (int i = 0; i < scene.size(); i++) {
					SceneObject* scenery = scene.get(i);
					Locker locker(scenery);

					scenery->sendDestroyTo(sceno);
				}
			}, "SendDestroySceneryLambda");
		}

		if (obj->isDebuggingRegions())
			sendDebugMessage(obj, false);
	}
}

void ActiveAreaImplementation::sendDebugMessage(SceneObject* object, bool entry) {
	if (object == nullptr || !object->isPlayerCreature())
		return;

	CreatureObject* creature = object->asCreatureObject();

	if (creature == nullptr)
		return;

	StringBuffer debugMsg;

	if (entry) {
		debugMsg << "Entering Region:  " << areaName << "  -  ";
	} else {
		debugMsg << "Exiting Region:  " << areaName << "  -  ";
	}

	String shapeString = areaShape == nullptr ? " null shape " : areaShape->isRectangularAreaShape() ? " rectangle " : areaShape->isCircularAreaShape() ? " circle "
		: areaShape->isRingAreaShape() ? " ring " : " none ";

	debugMsg << " Region Shape = " << shapeString;
	debugMsg << " Radius = " << getRadius() << "  ";

	Vector3 playerCoords = creature->getWorldPosition();
	debugMsg << " Player Coords: X = " << playerCoords.getX() <<  "  Z = " << playerCoords.getZ() <<"  Y = " << playerCoords.getY() << " ";

	Vector3 coords = getWorldPosition();
	debugMsg << " Area Coords - X = " << coords.getX() <<  " Z = " << coords.getZ() <<"  Y = " << coords.getY() << " ";

	StringBuffer regionTypes;

	if (isUndefinedRegion())
		regionTypes << "UNDEFINEDAREA ";
	if (isSpawnArea())
		regionTypes << "SPAWNAREA ";
	if (isNoSpawnArea())
		regionTypes << "NOSPAWNAREA ";
	if (isWorldSpawnArea())
		regionTypes << "WORLDSPAWNAREA ";
	if (isNoWorldSpawnArea())
		regionTypes << "NOWORLDSPAWNAREA ";
	if (isNoBuildZone())
		regionTypes << "NOBUILDZONEAREA ";
	if (isCampingArea())
		regionTypes << "CAMPINGAREA ";
	if (isCityRegion())
		regionTypes << "CITY ";
	if (shouldBuildNavmesh())
		regionTypes << "NAVAREA ";
	if (isNamedRegion())
		regionTypes << "NAMEDREGION";


	debugMsg << " Region Types: (" << regionTypes.toString() << ")";

	creature->sendSystemMessage(debugMsg.toString());
}

void ActiveAreaImplementation::setZone(Zone* zone) {
	this->zone = zone;
}

bool ActiveAreaImplementation::intersectsWith(ActiveArea* area) const {
	if (areaShape == nullptr) {
		return false;
	}

	return areaShape->intersectsWith(area->getAreaShape());
}

void ActiveAreaImplementation::initializeChildObject(SceneObject* controllerObject) {
	ManagedReference<SceneObject*> objectParent = controllerObject->getParent().get();

	if (objectParent != nullptr && objectParent->isCellObject()) {
		setCellObjectID(objectParent->getObjectID());
	}
}

void ActiveAreaImplementation::addAreaFlag(uint32 flag) {
	if (!(areaFlags & flag)) {
		areaFlags |= flag;
	}
}

void ActiveAreaImplementation::removeAreaFlag(uint32 flag) {
	if (areaFlags & flag) {
		areaFlags &= ~flag;
	}
}
