/*
 * DonateDefenseSuiCallback.h
 *
 *  Created on: Nov 30, 2012
 *      Author: root
 */

#ifndef DONATEDEFENSESUICALLBACK_H_
#define DONATEDEFENSESUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"
#include "server/zone/managers/gcw/GCWManager.h"

class DonateDefenseSuiCallback : public SuiCallback {

public:
	DonateDefenseSuiCallback(ZoneServer* server)
		: SuiCallback(server) {

	}

	void run(CreatureObject* player, SuiBox* suiBox, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		if (cancelPressed || !suiBox->isListBox() || player == nullptr)
			return;

		ManagedReference<SceneObject*> obj = suiBox->getUsingObject().get();

		if (obj == nullptr || !obj->isBuildingObject())
			return;

		SuiListBox* listBox = cast<SuiListBox*>(suiBox);

		int indx = Integer::valueOf(args->get(0).toString());

		uint64 objectid = listBox->getMenuObjectID(indx);

		ManagedReference<BuildingObject*> building = cast<BuildingObject*>(obj.get());

		GCWManager* gcwMan = player->getZone()->getGCWManager();

		if (gcwMan == nullptr)
			return;

		gcwMan->performDefenseDonation(building, player, objectid);

	}
};



#endif /* DONATEDEFENSESUICALLBACK_H_ */
