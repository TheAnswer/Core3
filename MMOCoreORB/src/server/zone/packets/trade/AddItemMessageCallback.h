/*
 * AddItemMessageCallback.h
 *
 *  Created on: 07/06/2010
 *      Author: victor
 */

#ifndef ADDITEMMESSAGECALLBACK_H_
#define ADDITEMMESSAGECALLBACK_H_


#include "../MessageCallback.h"
#include "server/zone/managers/player/PlayerManager.h"

class AddItemMessageCallback : public MessageCallback {
	uint64 id;

public:
	AddItemMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server), id(0) {

	}

	void parse(Message* message) {
		id = message->parseLong();
	}

	void run() {
		ManagedReference<CreatureObject*> player = static_cast<CreatureObject*>(client->getPlayer().get().get());

		if (player == NULL)
			return;

		PlayerManager* playerManager = server->getZoneServer()->getPlayerManager();
		playerManager->handleAddItemToTradeWindow(player, id);
	}

};


#endif /* ADDITEMMESSAGECALLBACK_H_ */
