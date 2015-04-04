/*
 * GiveMoneyMessageCallback.h
 *
 *  Created on: 07/06/2010
 *      Author: victor
 */

#ifndef GIVEMONEYMESSAGECALLBACK_H_
#define GIVEMONEYMESSAGECALLBACK_H_

#include "../MessageCallback.h"
#include "server/zone/managers/player/PlayerManager.h"

class GiveMoneyMessageCallback : public MessageCallback {
	uint32 money;

public:
	GiveMoneyMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server), money(0) {

	}

	void parse(Message* message) {
		money = message->parseInt();
	}

	void run() {
		ManagedReference<CreatureObject*> player = static_cast<CreatureObject*>(client->getPlayer().get().get());

		if (player == NULL)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->handleGiveMoneyMessage(player, money);
	}

};


#endif /* GIVEMONEYMESSAGECALLBACK_H_ */
