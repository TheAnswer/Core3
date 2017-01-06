/*
 * AiWaitEvent.h
 *
 *  Created on: Nov 14, 2010
 *      Author: da
 */

#ifndef AIWAITEVENT_H_
#define AIWAITEVENT_H_

#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/managers/creature/AiMap.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace ai {
namespace events {

class AiWaitEvent : public Task {
	ManagedWeakReference<AiAgent*> creature;

public:
	AiWaitEvent(AiAgent* pl) : Task(10000) {
		creature = pl;
		AiMap::instance()->activeWaitEvents.increment();
	}

	virtual ~AiWaitEvent() {
		AiMap::instance()->activeWaitEvents.decrement();
	}

	void run() {
		ManagedReference<AiAgent*> strongRef = creature.get();

		if (strongRef == NULL)
			return;

		Locker locker(strongRef);

		strongRef->activateMovementEvent();
	}

};

}
}
}
}
}
}

using namespace server::zone::objects::creature::events;

#endif /* AIWAITEVENT_H_ */
