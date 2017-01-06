/*
 * PlayerMoneyRequest.h
 *
 *  Created on: 02/02/2010
 *      Author: victor
 */

#ifndef PLAYERMONEYREQUEST_H_
#define PLAYERMONEYREQUEST_H_

#include "server/zone/packets/MessageCallback.h"
#include "PlayerMoneyResponseMessage.h"

class PlayerMoneyRequestMessageCallback : public MessageCallback {
public:
	PlayerMoneyRequestMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server) {

	}

	void parse(Message* message) {
	}

	void run() {
		ManagedReference<CreatureObject*> player = client->getPlayer();

		if (player == NULL)
			return;

		PlayerMoneyResponseMessage* reply = new  PlayerMoneyResponseMessage(player);
		client->sendMessage(reply);
	}

};


#endif /* PLAYERMONEYREQUEST_H_ */
