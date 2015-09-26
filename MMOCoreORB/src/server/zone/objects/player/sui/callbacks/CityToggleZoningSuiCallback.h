
#ifndef CITYTOGGLEZONINGSUICALLBACK_H_
#define CITYTOGGLEZONINGSUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
	class CreatureObject;
}
namespace region {
	class CityRegion;
}
}
}
}

using namespace server::zone::objects::creature;
using namespace server::zone::objects::region;

class CityToggleZoningSuiCallback : public SuiCallback {
	ManagedWeakReference<CityRegion*> cityRegion;

public:
	CityToggleZoningSuiCallback(ZoneServer* server, CityRegion* city) : SuiCallback(server) {
		cityRegion = city;
	}

	void run(CreatureObject* player, SuiBox* suiBox, bool cancelPressed, Vector<UnicodeString>* args) {
		ManagedReference<CityRegion*> city = cityRegion.get();

		if (city == NULL || !suiBox->isMessageBox() || player == NULL || cancelPressed) {
			return;
		}

		PlayerObject* ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		bool isAdmin = false;

		if ((ghost->getAdminLevel() == 15))
			isAdmin = true;

		if (!city->isMayor(player->getObjectID()) && !isAdmin) {
			return;
		}

		if (!player->hasSkill("social_politician_novice") && !isAdmin) {
			player->sendSystemMessage("@city/city:zoning_skill"); // You must be a Politician to enable city zoning.
			return;
		}

		Locker clocker(city, player);

		CityManager* cityManager = server->getCityManager();
		cityManager->toggleZoningEnabled(city, player);
	}
};

#endif /* CITYTOGGLEZONINGSUICALLBACK_H_ */
