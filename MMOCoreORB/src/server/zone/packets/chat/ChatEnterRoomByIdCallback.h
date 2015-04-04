/*
 * ChatEnterRoomByIdCallback.h
 *
 *  Created on: Nov 5, 2010
 *      Author: oru
 */

#ifndef CHATENTERROOMBYIDCALLBACK_H_
#define CHATENTERROOMBYIDCALLBACK_H_


#include "../MessageCallback.h"
#include "server/chat/ChatManager.h"

class ChatEnterRoomByIdCallback : public MessageCallback {
	uint32 counter;
	uint32 roomID;

public:
	ChatEnterRoomByIdCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server), counter(0), roomID(0) {
	}

	void parse(Message* message) {
		counter = message->parseInt();
		roomID = message->parseInt();
	}

	void run() {
		ManagedReference<CreatureObject*> player = cast<CreatureObject*>( client->getPlayer().get().get());

		if (player == NULL)
			return;

		ChatManager* chatManager = server->getChatManager();
		chatManager->handleChatEnterRoomById(player, counter, roomID);
	}

};




#endif /* CHATENTERROOMBYIDCALLBACK_H_ */
