/*
 * ChatQueryRoomCallback.h
 *
 *  Created on: Oct 10, 2015
 *      Author: Anakis
 */

#ifndef CHATQUERYROOMCALLBACK_H_
#define CHATQUERYROOMCALLBACK_H_


#include "server/zone/packets/MessageCallback.h"
#include "server/chat/ChatManager.h"

class ChatQueryRoomCallback : public MessageCallback {
	int requestID;
	String roomPath;


public:
	ChatQueryRoomCallback(ZoneClientSession* client, ZoneProcessServer* server) : MessageCallback(client, server) {
		requestID = -1;
		roomPath = "";
	}

	void parse(Message* message) {
		requestID = message->parseInt();
		message->parseAscii(roomPath);
	}

	void run() {
		ManagedReference<SceneObject*> scene = client->getPlayer();

		if (scene == NULL)
			return;

		CreatureObject* player = cast<CreatureObject*>(scene.get());

		if (player == NULL)
			return;

		ChatManager* chatManager = server->getChatManager();
		chatManager->handleChatQueryRoom(player, roomPath, requestID);

	}

};


#endif /* CHATQUERYROOMCALLBACK_H_ */
