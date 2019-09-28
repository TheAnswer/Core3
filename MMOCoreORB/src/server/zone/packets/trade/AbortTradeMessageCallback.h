/*
 * AbortTradeMessageCallback.h
 *
 *  Created on: 07/06/2010
 *      Author: victor
 */

#ifndef ABORTTRADEMESSAGECALLBACK_H_
#define ABORTTRADEMESSAGECALLBACK_H_

#include "server/zone/packets/MessageCallback.h"
#include "server/zone/managers/player/PlayerManager.h"

class AbortTradeMessageCallback : public MessageCallback {


public:
	AbortTradeMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server) {

	}

	void parse(Message* message) {

	}

	void run() {
		ManagedReference<CreatureObject*> player = client->getPlayer();

		if (player == nullptr)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->handleAbortTradeMessage(player);
	}

};


#endif /* ABORTTRADEMESSAGECALLBACK_H_ */
