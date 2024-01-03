/*
 * ShuttleZoneComponent.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: crush
 */

#include "ShuttleZoneComponent.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/building/tasks/ScheduleShuttleTask.h"

//#define SHUTTLE_TIMER_DEBUG

void ShuttleZoneComponent::notifyInsertToZone(SceneObject* sceneObject, Zone* zone) const {
	GroundZoneComponent::notifyInsertToZone(sceneObject, zone);

	if (sceneObject == nullptr) {
		error() << "ShuttleZoneComponent::notifyInsertToZone -- inserted object is null";
		return;
	}

	if (!sceneObject->isCreatureObject()) {
		error() << "ShuttleZoneComponent::notifyInsertToZone -- inserted object is not a Creature Object";
		return;
	}

	CreatureObject* shuttle = cast<CreatureObject*>( sceneObject);

	Reference<ScheduleShuttleTask*> task = new ScheduleShuttleTask(shuttle, zone);

	uint32 startDiff = zone->getZoneServer()->getStartTimestamp()->miliDifference();

	// Shuttles delayed 5 minutes for server start
	int bootDelay = ConfigManager::instance()->getInt("Core3.ShuttleZoneComponent.BootDelay", 5 * 60 * 1000);

	int delay = bootDelay - startDiff;

	if (delay <= 0)
		delay = 50;

#ifdef SHUTTLE_TIMER_DEBUG
	info(true) << "ScheduleShuttleTask for " << zone->getZoneName() << " scheduled due to server loading: militime since server start - " << startDiff << "  shuttle dealy time - " << delay << " miliseconds.";
#endif

	task->schedule(delay);
}

void ShuttleZoneComponent::notifyRemoveFromZone(SceneObject* sceneObject) const {
	GroundZoneComponent::notifyRemoveFromZone(sceneObject);
}
