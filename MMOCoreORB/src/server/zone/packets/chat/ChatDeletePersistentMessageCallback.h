/*
 * ChatDeletePersistentMessageCallback.h
 *
 *  Created on: 13/03/2010
 *      Author: victor
 */

#ifndef CHATDELETEPERSISTENTMESSAGECALLBACK_H_
#define CHATDELETEPERSISTENTMESSAGECALLBACK_H_


#include "../MessageCallback.h"
#include "server/chat/ChatManager.h"

class ChatDeletePersistentMessageCallback : public MessageCallback {
	uint32 mailid;

public:
	ChatDeletePersistentMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server), mailid(0) {
	}

	void parse(Message* message) {
		mailid = message->parseInt();
	}

	void run() {
		ManagedReference<CreatureObject*> player = cast<CreatureObject*>( client->getPlayer().get().get());

		if (player == NULL)
			return;

		ChatManager* chatManager = server->getChatManager();
		chatManager->deletePersistentMessage(player, mailid);
	}

};



#endif /* CHATDELETEPERSISTENTMESSAGECALLBACK_H_ */
