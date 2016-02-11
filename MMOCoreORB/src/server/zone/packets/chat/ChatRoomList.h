/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CHATROOMLIST_H_
#define CHATROOMLIST_H_

#include "engine/engine.h"

#include "server/chat/room/ChatRoom.h"
#include "../MessageCallback.h"

#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"

#include "server/chat/ChatManager.h"

namespace server {
namespace zone {
namespace packets {
namespace chat {

class ChatRoomList : public BaseMessage {
	int channelCounter;

public:
	ChatRoomList() : BaseMessage() {
		insertShort(0x02);
		insertInt(0x70DEB197);  // Opcode

		insertInt(0); //List Count

		channelCounter = 0;

		setCompression(true);
	}

	void addChannel(ChatRoom* channel) {
		channelCounter++;

		insertInt(channel->getRoomID());

		if (channel->isPublic())
			insertInt(0);
		else
			insertInt(1);

		if (!channel->isModerated())
			insertByte(0);
		else
			insertByte(1);

		insertAscii(channel->getFullPath());

		insertAscii("SWG");
		insertAscii(channel->getGalaxyName());
		insertAscii(channel->getOwnerName());

		//This struct is a ChatAvatarId
		insertAscii("SWG");
		insertAscii(channel->getGalaxyName());
		insertAscii(channel->getCreator());

		insertUnicode(channel->getTitle());

		insertInt(0); //Moderator & Player lists not needed in this packet.
		insertInt(0);

	}

	void insertChannelListCount() {
		insertInt(10, channelCounter);
	}

};

class ChatRequestRoomListCallback : public MessageCallback {
public:
	ChatRequestRoomListCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server) {

	}

	void parse(Message* message) {

	}

	void run() {
		ZoneServer* zoneServer = server->getZoneServer();
		ChatManager* chatManager = zoneServer->getChatManager();

		ManagedReference<SceneObject*> strongRef = client->getPlayer();

		if (strongRef == NULL)
			return;

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>(strongRef.get());

		if (player != NULL)
			chatManager->sendRoomList(player);
	}
};

}
}
}
}

using namespace server::zone::packets::chat;

#endif /*CHATROOMLIST_H_*/
