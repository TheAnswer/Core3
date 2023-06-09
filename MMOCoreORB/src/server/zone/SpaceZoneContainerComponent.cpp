/*
 * SpaceZoneContainerComponent.cpp
 *
 *  Created on: 03/09/2011
 *      Author: TheAnswer
 */

#include "SpaceZoneContainerComponent.h"

#include "server/zone/SpaceZone.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "templates/building/SharedBuildingObjectTemplate.h"
#include "server/zone/objects/intangible/TheaterObject.h"

bool SpaceZoneContainerComponent::transferObject(SceneObject* sceneObject, SceneObject* object, int containmentType, bool notifyClient, bool allowOverflow, bool notifyRoot) const {
	if (sceneObject == nullptr) {
		return false;
	}

	Zone* newZone = cast<Zone*>(sceneObject);

	if (newZone == nullptr || !newZone->isSpaceZone()) {
		return false;
	}

	SpaceZone* newSpaceZone = dynamic_cast<SpaceZone*>(sceneObject);

	if (newSpaceZone == nullptr)
		return false;

	SpaceZone* spaceZone = object->getSpaceZone();

	/*
	if (object->isActiveArea())
		return insertActiveArea(newSpaceZone, dynamic_cast<ActiveArea*>(object));
	*/
	Locker zoneLocker(newSpaceZone);

	if (object->isInOctTree() && newSpaceZone != spaceZone) {
		object->error("trying to insert object to newSpaceZone but is already in another space zone");

		object->destroyObjectFromWorld(true);

		return false;
	}

	ManagedReference<SceneObject*> parent = object->getParent().get();

	if (parent != nullptr/* && parent->isCellObject()*/) {
		uint64 parentID = object->getParentID();

		if (containmentType == -2)
			parent->removeObject(object, sceneObject, false);
		else
			parent->removeObject(object, sceneObject, true);

		if (object->getParent() != nullptr && parent->containsChildObject(object))
			return false;
		else
			object->setParent(nullptr, false);

		if (parent->isCellObject()) {
			// Ship Object?
			/*
			ManagedReference<BuildingObject*> build = cast<BuildingObject*>(parent->getParent().get().get());

			if (build != nullptr) {
				CreatureObject* creature = cast<CreatureObject*>(object);

				if (creature != nullptr)
					build->onExit(creature, parentID);
			}*/
		}
	} else {
		object->setParent(nullptr, false);
	}

	object->setSpaceZone(newSpaceZone);
	spaceZone = newSpaceZone;

	spaceZone->addSceneObject(object);

	if (notifyClient)
		object->sendToOwner(true);

	if (parent == nullptr)
		object->initializePosition(object->getPositionX(), object->getPositionZ(), object->getPositionY());

	spaceZone->insert(object);

	spaceZone->inRange(object, ZoneServer::SPACEOBJECTRANGE);

	zoneLocker.release();

	object->notifyInsertToZone(spaceZone);

	object->notifyObservers(ObserverEventType::PARENTCHANGED, nullptr);

	return true;
}

bool SpaceZoneContainerComponent::removeObject(SceneObject* sceneObject, SceneObject* object, SceneObject* destination, bool notifyClient) const {
	SpaceZone* spaceZone = dynamic_cast<SpaceZone*>(sceneObject);

	ManagedReference<SceneObject*> parent = object->getParent().get();

	try {
		Locker locker(object);

		if (spaceZone == nullptr)
			return false;

		object->debug("removing from space zone");

		Locker zoneLocker(spaceZone);

		if (parent != nullptr) {
			parent->removeObject(object, nullptr, false);
		} else
			spaceZone->remove(object);

		SpaceZone* oldZone = spaceZone;

		auto closeObjects = object->getCloseObjects();

		if (closeObjects != nullptr) {
			SortedVector<ManagedReference<TreeEntry*> > closeSceneObjects;

			SpaceZoneComponent::removeAllObjectsFromCOV(closeObjects, closeSceneObjects, sceneObject, object);
		} else {
#ifdef COV_DEBUG
			object->info("Null closeobjects vector in SpaceZoneContainerComponent::removeObject", true);
#endif
			SortedVector<ManagedReference<TreeEntry*> > closeSceneObjects;

			spaceZone->getInRangeObjects(object->getPositionX(), object->getPositionY(), object->getPositionZ(), ZoneServer::SPACEOBJECTRANGE, &closeSceneObjects, false);

			for (int i = 0; i < closeSceneObjects.size(); ++i) {
				TreeEntry* obj = closeSceneObjects.get(i);

				if (obj != nullptr && obj != object && obj->getCloseObjects() != nullptr)
					obj->removeInRangeObject(object);
			}
		}

		spaceZone = nullptr;

		oldZone->dropSceneObject(object);

		zoneLocker.release();

		SortedVector<ManagedReference<SceneObject*> >* childObjects = object->getChildObjects();

		//Remove all outdoor child objects from zone
		for (int i = 0; i < childObjects->size(); ++i) {
			ManagedReference<SceneObject*> outdoorChild = childObjects->get(i);

			if (outdoorChild == nullptr)
				continue;

			if (outdoorChild->isInOctTree()) {
				Locker locker(outdoorChild);

				outdoorChild->destroyObjectFromWorld(true);
			}
		}

	} catch (Exception& e) {
		object->error("exception in SpaceZoneContainerComponent::removeObject: " + e.getMessage());
	}

	object->notifyObservers(ObserverEventType::OBJECTREMOVEDFROMZONE, nullptr, 0);

	VectorMap<uint32, ManagedReference<Facade*> >* objectActiveSessions = object->getObjectActiveSessions();

	while (objectActiveSessions->size()) {
		ManagedReference<Facade*> facade = objectActiveSessions->remove(0).getValue();

		if (facade == nullptr)
			continue;

		facade->cancelSession();
	}

	object->debug("removed from zone");

	object->notifyRemoveFromZone();

	object->setSpaceZone(nullptr);

	return true;
}
