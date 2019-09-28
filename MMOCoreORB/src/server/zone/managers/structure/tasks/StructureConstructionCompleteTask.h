/*
 * StructureConstructionCompleteTask.h
 *
 *  Created on: Jun 13, 2011
 *      Author: crush
 */


#ifndef STRUCTURECONSTRUCTIONCOMPLETETASK_H_
#define STRUCTURECONSTRUCTIONCOMPLETETASK_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/sessions/PlaceStructureSession.h"

class StructureConstructionCompleteTask : public Task {
	ManagedWeakReference<CreatureObject*> creatureObject;

public:
	StructureConstructionCompleteTask(CreatureObject* creature) : Task() {
		creatureObject = creature;
	}

	void run() {
		ManagedReference<CreatureObject*> creature = creatureObject.get();

		if (creature == nullptr)
			return;

		Locker lock(creature);

		ManagedReference<PlaceStructureSession*> session = creature->getActiveSession(SessionFacadeType::PLACESTRUCTURE).castTo<PlaceStructureSession*>();

		if (session == nullptr)
			return;

		session->completeSession();
	}
};

#endif /*STRUCTURECONSTRUCTIONCOMPLETETASK_H_*/
