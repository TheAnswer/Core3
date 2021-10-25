/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FISHINGSPLASHEVENT_H_
#define FISHINGSPLASHEVENT_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/managers/minigames/FishingManager.h"

namespace server {
namespace zone {
namespace managers {
namespace minigames {
namespace events {

class FishingSplashEvent : public Task {
	ManagedReference<CreatureObject*> player;
	ManagedReference<SceneObject*> splash;

public:
	FishingSplashEvent(CreatureObject* player, SceneObject* splash) : Task(1000) {
		this->player = player;
		this->splash = splash;
	}

	void run() {
		try {
			Locker _locker(player);

			ManagedReference<FishingManager*> manager = player->getZoneProcessServer()->getFishingManager();
			Locker splashLocker(splash);
			manager->removeSplash(splash);
		} catch (...) {
			// player = nullptr;

			throw;
		}

		// player = nullptr;
	}

	SceneObject* getSplash() {
		return splash;
	}
};
} // namespace events
} // namespace minigames
} // namespace managers
} // namespace zone
} // namespace server

using namespace server::zone::managers::minigames::events;

#endif /* FISHINGSPLASHEVENT_H_ */
