/*
 * DestroyMissionObjectiveImplementation.cpp
 *
 *  Created on: 16/07/2010
 *      Author: victor
 */

#include "DestroyMissionObjective.h"
#include "server/zone/objects/area/MissionSpawnActiveArea.h"

#include "server/zone/objects/waypoint/WaypointObject.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/mission/MissionManager.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/terrain/TerrainManager.h"
#include "MissionObject.h"
#include "MissionObserver.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/templates/mobile/LairTemplate.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"

void DestroyMissionObjectiveImplementation::setLairTemplateToSpawn(const String& sp) {
	lairTemplate = sp;
}

void DestroyMissionObjectiveImplementation::destroyObjectFromDatabase() {
	MissionObjectiveImplementation::destroyObjectFromDatabase();

	ManagedReference<MissionSpawnActiveArea* > spawnActiveArea = this->spawnActiveArea;

	if (spawnActiveArea != NULL) {
		Locker locker(spawnActiveArea);

		spawnActiveArea->destroyObjectFromWorld(true);
		spawnActiveArea->destroyObjectFromDatabase(true);

		this->spawnActiveArea = NULL;
	}
}

void DestroyMissionObjectiveImplementation::activate() {
	MissionObjectiveImplementation::activate();
	ManagedReference<MissionObject* > mission = this->mission.get();

	if ((hasObservers() && lairObject != NULL) || mission == NULL) {
		return;
	}

	if (spawnActiveArea == NULL) {
		spawnActiveArea = ( Core::lookupObject<ZoneServer>("ZoneServer")->createObject(String("object/mission_spawn_area.iff").hashCode(), 1)).castTo<MissionSpawnActiveArea*>();
		spawnActiveArea->setMissionObjective(_this.get());
	}

	if (spawnActiveArea->getZone() == NULL) {
		String planetName = mission->getStartPlanet();

		Zone* zone = Core::lookupObject<ZoneServer>("ZoneServer")->getZone(planetName);

		if (zone == NULL)
			return;

		spawnActiveArea->initializePosition(mission->getStartPositionX(), 0, mission->getStartPositionY());
		spawnActiveArea->setRadius(128.f);
		zone->transferObject(spawnActiveArea, -1, false);
		//info("inserting to zone " + zone->getPlanetName(), true);
	}

	WaypointObject* waypoint = mission->getWaypointToMission();

	if (waypoint == NULL)
		waypoint = mission->createWaypoint();

	waypoint->setPlanetCRC(mission->getStartPlanet().hashCode());
	waypoint->setPosition(mission->getStartPositionX(), 0, mission->getStartPositionY());
	waypoint->setActive(true);

	mission->updateMissionLocation();
}

Vector3 DestroyMissionObjectiveImplementation::findValidSpawnPosition(Zone* zone) {
	Vector3 position;

	float newX = spawnActiveArea->getPositionX() + (256.0f - (float) System::random(512));
	float newY = spawnActiveArea->getPositionY() + (256.0f - (float) System::random(512));

	float height = zone->getHeight(newX, newY);

	float waterHeight;
	TerrainManager* terrain = zone->getPlanetManager()->getTerrainManager();

	float distance = 128;
	int tries = 0;

	position.set(newX, height, newY);

	while ((!zone->isWithinBoundaries(position) ||
			(terrain->getWaterHeight(newX, newY, waterHeight) && waterHeight > height)
			|| CollisionManager::checkSphereCollision(position, 25.f , zone) ||
			zone->getPlanetManager()->isInObjectsNoBuildZone(newX, newY, 25)) && tries < 128) {
		newX = spawnActiveArea->getPositionX() + (distance - (float) System::random(distance * 2));
		newY = spawnActiveArea->getPositionY() + (distance - (float) System::random(distance * 2));
		height = zone->getHeight(newX, newY);

		position.set(newX, height, newY);

		++tries;

		//Increase distance every tenth try.
		if (tries % 16 == 0) {
			distance = distance * 2;
		}
	}

	if (tries == 128) {
		//Failed to find a spawn point for the lair, fail mission.
		getPlayerOwner().get()->sendSystemMessage("@mission/mission_generic:failed");
		fail();
	}

	//info("found with tries " + String::valueOf(tries), true);

	return position;
}

void DestroyMissionObjectiveImplementation::spawnLair() {
	ManagedReference<MissionObject* > mission = this->mission.get();

	ManagedReference<MissionSpawnActiveArea* > spawnActiveArea = this->spawnActiveArea;

	if (spawnActiveArea == NULL)
		return;

	if (lairObject != NULL && lairObject->getZone() != NULL)
		return;

	Zone* zone = spawnActiveArea->getZone();

	Locker locker(spawnActiveArea);

	spawnActiveArea->destroyObjectFromWorld(true);

	Vector3 pos = findValidSpawnPosition(zone);

	ManagedReference<WaypointObject*> waypoint = mission->getWaypointToMission();

	if (waypoint == NULL) {
		waypoint = mission->createWaypoint();
	}

	waypoint->setPosition(pos.getX(), 0, pos.getY());
	mission->updateMissionLocation();

	mission->setStartPosition(pos.getX(), pos.getY());

	//TODO: find correct string id
	ManagedReference<CreatureObject*> player = getPlayerOwner();

	player->sendSystemMessage("Transmission Received: Mission Target has been located.  Mission waypoint has been updated to exact location");

	LairTemplate* lair = CreatureTemplateManager::instance()->getLairTemplate(lairTemplate.hashCode());

	unsigned int faction = lair->getFaction();

	if (lairObject == NULL) {
		CreatureManager* creatureManager = zone->getCreatureManager();

		lairObject = creatureManager->spawnLair(lairTemplate.hashCode(), difficulty, pos.getX(), pos.getZ(), pos.getY(), faction);

		if (lairObject != NULL) {
			ManagedReference<MissionObserver*> observer = new MissionObserver(_this.get());
			addObserver(observer, true);

			Locker locker(lairObject);

			lairObject->registerObserver(ObserverEventType::OBJECTDESTRUCTION, observer);
		}

	}

	if (lairObject != NULL && lairObject->getZone() == NULL) {
		zone->transferObject(lairObject, -1, true);
	}
}

void DestroyMissionObjectiveImplementation::abort() {
	MissionObjectiveImplementation::abort();

	if (hasObservers()) {
		ManagedReference<MissionObserver*> observer = getObserver(0);

		ManagedReference<CreatureObject*> player = getPlayerOwner();

		ManagedReference<TangibleObject*> lair = lairObject;

		if (lair != NULL) {
			Locker locker(lair);

			lair->dropObserver(ObserverEventType::OBJECTDESTRUCTION, observer);
			lair->destroyObjectFromWorld(true);

			lair = NULL;

			dropObserver(observer, true);
		}
			//spawnActiveArea->destroyObjectFromDatabase(true);
	}

	ManagedReference<MissionSpawnActiveArea* > spawnActiveArea = this->spawnActiveArea;

	if (spawnActiveArea != NULL) {
		Locker locker(spawnActiveArea);

		spawnActiveArea->destroyObjectFromWorld(true);
	}
}

void DestroyMissionObjectiveImplementation::complete() {

	MissionObjectiveImplementation::complete();
}

int DestroyMissionObjectiveImplementation::notifyObserverEvent(MissionObserver* observer, uint32 eventType, Observable* observable, ManagedObject* arg1, int64 arg2) {
	if (eventType == ObserverEventType::OBJECTDESTRUCTION) {

		complete();
	}

	return 1;
}

Vector3 DestroyMissionObjectiveImplementation::getEndPosition() {
	ManagedReference<MissionObject* > mission = this->mission.get();

	Vector3 missionEndPoint;

	missionEndPoint.setX(mission->getStartPositionX());
	missionEndPoint.setY(mission->getStartPositionY());

	Zone* zone = getPlayerOwner().get()->getZone();

	if (zone != NULL) {
		missionEndPoint.setZ(zone->getHeight(missionEndPoint.getX(), missionEndPoint.getY()));
	}

	return missionEndPoint;
}
