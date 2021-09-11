/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef COMMANDQUEUEACTIONEVENT_H_
#define COMMANDQUEUEACTIONEVENT_H_

#include "server/zone/objects/creature/CreatureObject.h"

class CommandQueueActionEvent : public Task {
	ManagedWeakReference<CreatureObject*> creature;
	int type;
public:
	const static int IMMEDIATE = 1;
	const static int NORMAL = 2;

public:
	CommandQueueActionEvent(CreatureObject* pl, int type = NORMAL) : Task() {
		creature = pl;
		this->type = type;
	}

	void run() {
		ManagedReference<CreatureObject*> creature = this->creature.get();

		if (creature == nullptr)
			return;

		try {
			Locker creatureLocker(creature);

			//player->info("activating command queue action");

			if (type == NORMAL)
				creature->activateQueueAction();
			else
				creature->activateImmediateAction();

			//player->info("command queue action activated");

		} catch (Exception& e) {
			creature->error(e.getMessage());
			e.printStackTrace();
		} catch (...) {
			//creature = nullptr;

			throw;
		}
	}
};

#endif /*COMMANDQUEUEACTIONEVENT_H_*/
