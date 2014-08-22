/*
 * AiMoveEvent.h
 *
 *  Created on: 10/07/2010
 *      Author: victor
 */

#ifndef AiMoveEvent_H_
#define AiMoveEvent_H_

#include "server/zone/objects/creature/AiAgent.h"
#include "server/zone/managers/creature/AiMap.h"
#include "../PatrolPoint.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace events {

class AiMoveEvent : public Task {
	ManagedWeakReference<AiAgent*> creature;

public:
	AiMoveEvent(AiAgent* pl) : Task(1000) {
		creature = pl;
		AiMap::instance()->activeMoveEvents.increment();
	}

	~AiMoveEvent() {
		AiMap::instance()->activeMoveEvents.decrement();
	}

	void run() {
		AiMap::instance()->scheduledMoveEvents.decrement();

		ManagedReference<AiAgent*> strongRef = creature.get();

		if (strongRef == NULL)
			return;

		Locker locker(strongRef);

		strongRef->doMovement();
	}

	void schedule(uint64 delay = 0) {
		AiMap::instance()->scheduledMoveEvents.increment();

		try {
			Task::schedule(delay);
		} catch (...) {
			AiMap::instance()->scheduledMoveEvents.decrement();
		}
	}

	bool cancel() {
		bool ret = false;

		if ((ret = Task::cancel())) {
			AiMap::instance()->scheduledMoveEvents.decrement();
		}

		return ret;
	}

	void clearCreatureObject() {
		creature = NULL;
	}

};

}
}
}
}
}

using namespace server::zone::objects::creature::events;



#endif /* AiMoveEvent_H_ */
