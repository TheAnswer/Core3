/*
 * ChatPersistentMessageToServerCallback.h
 *
 *  Created on: 13/03/2010
 *      Author: victor
 */

#ifndef CHATPERSISTENTMESSAGETOSERVERCALLBACK_H_
#define CHATPERSISTENTMESSAGETOSERVERCALLBACK_H_


#include "../MessageCallback.h"
#include "server/chat/ChatManager.h"
#include "server/zone/managers/city/CityManager.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/guild/GuildObject.h"

#include "server/chat/StringIdChatParameterVector.h"
#include "server/chat/WaypointChatParameterVector.h"

#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/region/CitizenList.h"

#include "server/zone/packets/chat/ChatOnSendPersistentMessage.h"

class ChatPersistentMessageToServerCallback : public MessageCallback {
	UnicodeString header;
	UnicodeString body;
	String recipientName;
	uint32 sequence;

	StringIdChatParameterVector stringIdParameters;
	WaypointChatParameterVector waypointParameters;

public:
	ChatPersistentMessageToServerCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server) {

		sequence = 0;
	}

	void parse(Message* message) {
		message->parseUnicode(body);

		int parametersSize = message->parseInt() * 2;

		if (parametersSize > 0) {
			int initialOffset = message->getOffset();

			while (message->getOffset() < initialOffset + parametersSize) {
				uint16 appendedByte = message->parseShort();
				message->shiftOffset(1); //Skip the initial type byte. We'll switch on the second type.
				int type = message->parseInt();

				switch (type) {
				default:
				case ChatParameter::STRINGID:
				{
					StringIdChatParameter param;
					param.parse(message);
					stringIdParameters.add(param);
					break;
				}
				case ChatParameter::WAYPOINT:
				{
					WaypointChatParameter param;
					param.parse(message);
					waypointParameters.add(param);
					break;
				}
				}

				if (appendedByte > 0)
					message->shiftOffset(1);
			}
		}

		sequence = message->parseInt();
		message->parseUnicode(header); //mail subject
		message->shiftOffset(4); //skip spacer
		message->parseAscii(recipientName);
	}

	int sendMail(const String& recipient) {
		ManagedReference<SceneObject*> scene = client->getPlayer();

		if (scene == NULL)
			return 0;

		CreatureObject* player = cast<CreatureObject*>(scene.get());

		if (player == NULL)
			return 0;

		if (recipient == "guild") {
			ManagedReference<GuildObject*> guild = player->getGuildObject().get();

			if (guild == NULL)
				return 0;

			if (!guild->hasMailPermission(player->getObjectID())) {
				player->sendSystemMessage("@guild:generic_fail_no_permission");
				return 0;
			}

			Locker locker(guild);

			int max = guild->getTotalMembers();

			Vector<uint64> guildMembers(max, 5);

			for (int i = 0; i < max; ++i) {
				uint64 oid = guild->getMember(i);

				guildMembers.add(oid);
			}

			locker.release();

			for (int i = 0; i < guildMembers.size(); ++i) {
				ManagedReference<SceneObject*> receiver = server->getZoneServer()->getObject(guildMembers.get(i));

				if (receiver == NULL || !receiver->isPlayerCreature())
					continue;

				CreatureObject* receiverPlayer = cast<CreatureObject*>(receiver.get());

				sendMailToPlayer(receiverPlayer->getFirstName());
			}

			return 0;
		}
		else if (recipient == "citizens") {
			PlayerObject* ghost = player->getPlayerObject();
			if (ghost == NULL)
				return 0;

			// Pull the player's residence
			uint64 declaredOidResidence = ghost->getDeclaredResidence();
			ManagedReference<BuildingObject*> declaredResidence = player->getZoneServer()->getObject(declaredOidResidence).castTo<BuildingObject*>();
			if (declaredResidence == NULL){
				player->sendSystemMessage("@error_message:insufficient_permissions");
				return 0;
			}

			// Player must be the mayor of the city where he resides
			ManagedReference<CityRegion*> declaredCity = declaredResidence->getCityRegion();
			if( declaredCity != NULL && declaredCity->isMayor(player->getObjectID()) ){

				Locker cityLocker( declaredCity );

				CitizenList* citizenList = declaredCity->getCitizenList();
				if (citizenList == NULL)
					return 0;

				Vector<String> players;

				for (int i = 0; i < citizenList->size(); ++i) {
					uint64 citizenID = citizenList->get(i);

					ManagedReference<SceneObject*> receiver = player->getZoneServer()->getObject(citizenID);

					if (receiver == NULL || !receiver->isPlayerCreature())
						continue;

					CreatureObject* receiverPlayer = cast<CreatureObject*>(receiver.get());

					players.add(receiverPlayer->getFirstName());
				}

				cityLocker.release();

				for (int i = 0; i < players.size(); ++i) {
					sendMailToPlayer(players.get(i));
				}

				return 0;
			}
			else{
				player->sendSystemMessage("@error_message:insufficient_permissions");
				return 0;
			}

		}

		return sendMailToPlayer(recipient);
	}

	int sendMailToPlayer(const String& recipientName) {
		ManagedReference<SceneObject*> scene = client->getPlayer();

		if (scene == NULL)
			return 0;

		CreatureObject* player = cast<CreatureObject*>(scene.get());

		if (player == NULL)
			return 0;
			
		ChatManager* chatManager = server->getChatManager();

		uint64 receiverObjectID = server->getPlayerManager()->getObjectID(recipientName);

		if (receiverObjectID == 0) {
			return 0;
		}

		ManagedReference<SceneObject*> receiver = server->getZoneServer()->getObject(receiverObjectID);
		ManagedReference<PlayerObject*> sender = NULL;
		sender = player->getPlayerObject();

		if (receiver == NULL || !receiver->isPlayerCreature() || sender == NULL)
			return 0;

		bool privileged = false;

		if (sender->isPrivileged())
			privileged = true;

		Locker locker(receiver);

		CreatureObject* receiverPlayer = cast<CreatureObject*>(receiver.get());
		ManagedReference<PlayerObject*> ghost = receiverPlayer->getPlayerObject();

		if (ghost == NULL || (ghost->isIgnoring(player->getFirstName().toLowerCase()) && !privileged)) {
			StringIdChatParameter err("ui_pm", "recipient_ignored_prose");
			err.setTT(recipientName);
			player->sendSystemMessage(err);

			return ChatManager::IM_IGNORED;
		}

		return chatManager->sendMail(player->getFirstName(), header, body, recipientName, &stringIdParameters, &waypointParameters);
	}

	void run() {
		ManagedReference<SceneObject*> scene = client->getPlayer();

		if (scene == NULL)
			return;

		CreatureObject* player = cast<CreatureObject*>(scene.get());

		if (player == NULL)
			return;

		int result = 0;

		StringTokenizer tokenizer(recipientName);
		tokenizer.setDelimeter(";");

		if (tokenizer.hasMoreTokens()) {
			String receiver;

			int incr = 0;

			while (tokenizer.hasMoreTokens()) {
				tokenizer.getStringToken(receiver);

				sendMail(receiver);

				if ((incr++) > 80) {
					player->error("recipient count exceeded!");
					return;
				}
			}
		} else {
			result = sendMailToPlayer(recipientName);
		}

		ChatOnSendPersistentMessage* cospm = new ChatOnSendPersistentMessage(sequence, result);
		player->sendMessage(cospm);
	}

};


#endif /* CHATPERSISTENTMESSAGETOSERVERCALLBACK_H_ */
