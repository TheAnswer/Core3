/*
 * ConfirmDivorceSuiCallback.h
 *
 *  Created on: 6/8/2014
 *      Author: Klivian
 */

#ifndef CONFIRMDIVORCESUICALLBACK_H_
#define CONFIRMDIVORCESUICALLBACK_H_

#include "server/zone/objects/player/sui/SuiCallback.h"

class ConfirmDivorceSuiCallback : public SuiCallback {

public:

	ConfirmDivorceSuiCallback(ZoneServer* server)
		: SuiCallback(server) {
	}

	void run(CreatureObject* creature, SuiBox* sui, uint32 eventIndex, Vector<UnicodeString>* args) {
		bool cancelPressed = (eventIndex == 1);

		PlayerManager* playerManager = creature->getZoneServer()->getPlayerManager();
		if( playerManager == NULL )
			return;

		if( !cancelPressed ){
			playerManager->grantDivorce( creature );
		}

	}
};

#endif /* CONFIRMDIVORCESUICALLBACK_H_ */
