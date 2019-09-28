/*
 * DespawnCreatureTask.h
 *
 *  Created on: 12/07/2010
 *      Author: victor
 */

#ifndef DESPAWNCREATURETASK_H_
#define DESPAWNCREATURETASK_H_

#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/Zone.h"

class DespawnCreatureTask : public Task {
	ManagedReference<AiAgent*> creature;

public:
	DespawnCreatureTask(AiAgent* cr) {
		creature = cr;

		auto zone = cr->getZone();

		if (zone != nullptr) {
			setCustomTaskQueue(zone->getZoneName());
		}
	}

	void run() {
		Locker locker(creature);

		Zone* zone = creature->getZone();

		creature->removePendingTask("despawn");

		if (zone == nullptr)
			return;

		creature->destroyObjectFromWorld(false);
		creature->notifyDespawn(zone);

		//creature->printReferenceHolders();

		/*PatrolPoint* homeLocation = creature->getHomeLocation();

		if (homeLocation->getPosit)*/
	}
};


#endif /* DESPAWNCREATURETASK_H_ */
