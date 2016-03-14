/*
 * ChatManagerImplementation.cpp
 *
 *  Created on: 28/07/2009
 *      Author: victor
 */

#include "server/chat/ChatManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/Zone.h"

#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/name/NameManager.h"
#include "server/zone/managers/player/PlayerMap.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/creature/PetManager.h"

#include "server/zone/packets/chat/ChatRoomList.h"
#include "server/zone/packets/chat/ChatRoomMessage.h"
#include "server/zone/packets/object/SpatialChat.h"
#include "server/zone/packets/object/Emote.h"
#include "server/zone/packets/chat/ChatInstantMessageToCharacter.h"
#include "server/zone/packets/chat/ChatInstantMessageToClient.h"
#include "server/zone/packets/chat/ChatOnSendInstantMessage.h"
#include "server/zone/packets/chat/ChatOnSendRoomMessage.h"
#include "server/zone/packets/chat/ChatOnCreateRoom.h"
#include "server/zone/packets/chat/ChatOnDestroyRoom.h"
#include "server/zone/packets/chat/ChatOnEnteredRoom.h"
#include "server/zone/packets/chat/ChatPersistentMessageToClient.h"
#include "server/zone/packets/chat/ChatQueryRoomResults.h"
#include "server/zone/packets/chat/ChatOnReceiveRoomInvitation.h"
#include "server/zone/packets/chat/ChatOnInviteToRoom.h"
#include "server/zone/packets/chat/ChatOnUninviteFromRoom.h"
#include "server/zone/packets/chat/ChatOnAddModeratorToRoom.h"
#include "server/zone/packets/chat/ChatOnRemoveModeratorFromRoom.h"
#include "server/zone/packets/chat/ChatOnBanFromRoom.h"
#include "server/zone/packets/chat/ChatOnUnbanFromRoom.h"

#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/guild/GuildObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/intangible/PetControlDevice.h"

#include "server/chat/StringIdChatParameter.h"
#include "server/chat/PersistentMessage.h"
#include "server/chat/ChatMessage.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"

#include "server/chat/room/ChatRoom.h"
#include "server/chat/room/ChatRoomMap.h"
#include "server/chat/SendMailTask.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"

ChatManagerImplementation::ChatManagerImplementation(ZoneServer* serv, int initsize) : ManagedServiceImplementation() {
	server = serv;
	playerManager = NULL;
	setLoggingName("ChatManager");

	loadMailDatabase();

	playerMap = new PlayerMap(initsize);
	roomMap = new ChatRoomMap(10000);
	mute = false;

	loadSocialTypes();
	loadSpatialChatTypes();

}

void ChatManagerImplementation::finalize() {
}

void ChatManagerImplementation::loadMailDatabase() {
	info("Checking mail for expiration");

	ObjectDatabase* playerMailDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("mail", true);

	if (playerMailDatabase == NULL) {
		error("Could not load the player mail database.");
		return;
	}

	int i = 0, j = 0;

	try {
		ObjectDatabaseIterator iterator(playerMailDatabase);

		uint64 objectID;
		uint32 timeStamp, currentTime = System::getTime();
		ObjectInputStream* objectData = new ObjectInputStream(2000);

		while (i < 25000 && iterator.getNextKeyAndValue(objectID, objectData)) {
			if (!Serializable::getVariable<uint32>(STRING_HASHCODE("PersistentMessage.timeStamp"), &timeStamp, objectData)) {
				objectData->clear();
				continue;
			}

			j++;

			if (currentTime - timeStamp > PM_LIFESPAN) {
				Reference<PersistentMessage*> mail = Core::getObjectBroker()->lookUp(objectID).castTo<PersistentMessage*>();

				if (mail != NULL) {
					i++;

					ObjectManager::instance()->destroyObjectFromDatabase(objectID);
				}
			}

			if (ConfigManager::instance()->isProgressMonitorActivated())
				printf("\r\tChecking mail for expiration [%d] / [?]\t", j);

			objectData->clear();
		}

		delete objectData;
	} catch (DatabaseException& e) {
		error("Database exception in ChatManager::loadMailDatabase(): " + e.getMessage());
	}

	info("Deleted " + String::valueOf(i) + " mails due to expiration.", true);
}

void ChatManagerImplementation::loadSocialTypes() {
	IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/chat/social_types.iff");

	if (iffStream == NULL) {
		error("Could not load social types.");
		return;
	}

	DataTableIff dtiff;
	dtiff.readObject(iffStream);

	delete iffStream;

	for (int i = 0; i < dtiff.getTotalRows(); ++i) {
		DataTableRow* row = dtiff.getRow(i);

		String key;
		row->getCell(0)->getValue(key);
		socialTypes.put(i + 1, key);
	}

	info("Loaded " + String::valueOf(socialTypes.size()) + " social types.", true);
}

void ChatManagerImplementation::loadSpatialChatTypes() {
	TemplateManager* templateManager = TemplateManager::instance();
	IffStream* iffStream = templateManager->openIffFile("chat/spatial_chat_types.iff");

	if (iffStream == NULL) {
		error("Could not open chat/spatial_chat_types.iff");
		return;
	}

	iffStream->openForm('SPCT');

	Chunk* version = iffStream->openForm('0000');

	Chunk* data = iffStream->openChunk('TYPS');
	int i = 0;

	while (data->hasData()) {
		String key;
		data->readString(key);
		i++;

		spatialChatTypes.put(key, i);
	}

	iffStream->closeChunk('TYPS');

	Chunk* version2 = iffStream->openForm('VOLS');

	for (int j = 0; j < version2->getChunksSize(); j++) {
		Chunk* data2 = version2->getNextChunk();

		String name;
		data2->readString(name);

		uint16 distance = 0;

		distance = data2->readShort();

		if (name.isEmpty()) {
			defaultSpatialChatDistance = distance;
		} else {
			uint32 chatType = spatialChatTypes.get(name);

			spatialChatDistances.put(chatType, distance);
		}
	}

	iffStream->closeForm('VOLS');

	Chunk* temp = iffStream->openForm('PRIV');
	iffStream->closeForm('PRIV');

	Chunk* version3 = iffStream->openForm('SKCS');

	for (int j = 0; j < version3->getChunksSize(); j++) {
		Chunk* skillData = version3->getNextChunk();

		String type, skillNeeded;
		skillData->readString(type);
		skillData->readString(skillNeeded);

		uint32 chatType = spatialChatTypes.get(type);

		spatialChatTypeSkillNeeded.put(chatType, skillNeeded);
	}

	iffStream->closeForm('SKCS');

	iffStream->closeForm('0000');
	iffStream->closeForm('SPCT');

	delete iffStream;
}

void ChatManagerImplementation::initiateRooms() {
	gameRooms.setNullValue(NULL);

	ChatRoom* mainRoom = createRoom("SWG");
	mainRoom->setPrivate();
	gameRooms.put("SWG", mainRoom);

	galaxyRoom = createRoom(server->getGalaxyName(), mainRoom);
	galaxyRoom->setPrivate();

	systemRoom = createRoom("system", galaxyRoom);
	systemRoom->setPrivate();

	groupRoom = createRoom("group", systemRoom);
	groupRoom->setPrivate();

	guildRoom = createRoom("guild", systemRoom);
	guildRoom->setPrivate();

	ChatRoom* generalRoom = createRoom("Chat", galaxyRoom);
	generalRoom->setCanEnter(true);
	generalRoom->setAllowSubrooms(true);
	generalRoom->setTitle("public chat for this server, can create rooms here");

	auctionRoom = createRoom("Auction", galaxyRoom);
	auctionRoom->setCanEnter(true);
	auctionRoom->setChatRoomType(ChatRoom::AUCTION);

}

void ChatManagerImplementation::initiatePlanetRooms() {
	for (int i = 0; i < server->getZoneCount(); ++i) {
		ManagedReference<Zone*> zone = server->getZone(i);

		if (zone == NULL)
			continue;

		Locker locker(zone);

		ChatRoom* zoneRoom = createRoom(zone->getZoneName(), galaxyRoom);
		zoneRoom->setPrivate();

		ChatRoom* planetaryChat = createRoom("Planet", zoneRoom);
		planetaryChat->setPrivate();

		planetaryChat->setChatRoomType(ChatRoom::PLANET);
		planetaryChat->setOwnerID(zone->getObjectID());

		zone->setPlanetChatRoom(planetaryChat);
	}

}

void ChatManagerImplementation::loadPersistentRooms() {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	info("Loading and validating chat rooms from chatrooms.db", true);

	ObjectDatabase* chatRoomDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("chatrooms", true);

	if (chatRoomDatabase == NULL) {
		error("Could not load the chat rooms database.");
		return;
	}

	int loaded = 0;
	int repaired = 0;
	int deleted = 0;
	bool galaxyNameChanged = false;

	try {
		ObjectDatabaseIterator iterator(chatRoomDatabase);

		uint64 objectID = 0;
		String currentGalaxy = server->getGalaxyName();

		while (iterator.getNextKey(objectID)) {

			//Pull the chatroom from the database.
			Reference<ChatRoom*> room = Core::getObjectBroker()->lookUp(objectID).castTo<ChatRoom*>();
			ObjectDatabaseManager::instance()->commitLocalTransaction();

			if (room == NULL) {
				error("Chat room was NULL when attempting to load objectID: " + String::valueOf(objectID));
				ObjectManager::instance()->destroyObjectFromDatabase(objectID);
				continue;
			}

			//Validate the Galaxy name in the room path.
			String oldPath = room->getFullPath();
			String pathGalaxy = oldPath.subString(oldPath.indexOf('.') + 1, oldPath.length());
			pathGalaxy = pathGalaxy.subString(0, pathGalaxy.indexOf('.'));

			if (pathGalaxy != currentGalaxy) { //Room needs its path corrected.
				if (!galaxyNameChanged) {
					info("Detected a Galaxy name change to: '" + currentGalaxy + "'", true);
					info("Attempting to repair chat room paths...", true);
					galaxyNameChanged = true; //Display these messages only once.
				}

				String newPath = oldPath.replaceFirst(pathGalaxy, currentGalaxy);
				info("Old path = " + oldPath, false);
				info("New path = " + newPath, false);

				if (getChatRoomByFullPath(newPath) == NULL) {
					room->setFullPath(newPath);
					info("Successfully updated room to the new path.", false);
					repaired++;
				} else {
					info("There is already a room at the new path! Deleting this room from the database.", false);
					ObjectManager::instance()->destroyObjectFromDatabase(objectID);
					deleted++;
					continue;
				}
			}

			//Reset transient parent's room ID on the room.
			if (!room->hasPersistentParent()) {
				String parentPath = room->getFullPath();
				parentPath = parentPath.subString(0, parentPath.lastIndexOf('.'));

				ManagedReference<ChatRoom*> parent = getChatRoomByFullPath(parentPath);
				if (parent != NULL) {
					room->setParentRoomID(parent->getRoomID());
					parent->addSubRoom(room->getName().toLowerCase(), room->getRoomID());
				}
			}

			//Re-initialize the chat room.
			room->setChatManager(_this.getReferenceUnsafeStaticCast());
			room->setZoneServer(server);
			addRoom(room);
			loaded++;
		}

	} catch (DatabaseException& e) {
		error("Database exception in ChatManager::loadPersistentRooms(): " + e.getMessage());
	}

	int totalDeleted = deleted + checkRoomPaths();
	int expired = checkRoomExpirations();

	info("Load room results: Loaded = " + String::valueOf(loaded - totalDeleted - expired) + " Repaired = " +
	String::valueOf(repaired) + " Deleted = " + String::valueOf(totalDeleted) + " Expired = " + String::valueOf(expired), true);

}

int ChatManagerImplementation::checkRoomPaths() {
	//ChatManager locked

	Vector<ChatRoom*> roomsToDelete;
	int deleted = 0;

	HashTableIterator<unsigned int, ManagedReference<ChatRoom* > > iter = roomMap->iterator();

	while (iter.hasNext()) {
		ChatRoom* room = iter.next();

		if (room == NULL)
			continue;

		//Check if room's path is broken.
		if (room->getChatRoomType() == ChatRoom::CUSTOM) {
			if (getChatRoomByFullPath(room->getFullPath()) == NULL)
				roomsToDelete.add(room);
		}
	}

	//Delete rooms with broken paths.
	for (int i = 0; i < roomsToDelete.size(); ++i) {
		ChatRoom* badRoom = roomsToDelete.get(i);
		if (badRoom != NULL) {
			error("Broken path detected! Deleting room: " + badRoom->getFullPath());
			roomMap->remove(badRoom->getRoomID());
			ObjectManager::instance()->destroyObjectFromDatabase(badRoom->_getObjectID());
			deleted++;
		}
	}

	return deleted;

}

int ChatManagerImplementation::checkRoomExpirations() {
	//ChatManager locked

	int expired = 0;

	HashTableIterator<unsigned int, ManagedReference<ChatRoom* > > iter = roomMap->iterator();

	while (iter.hasNext()) {
		ChatRoom* room = iter.next();

		if (room == NULL)
			continue;

		if (ROOMEXPIRATIONTIME > 0 && room->getLastJoinTime() / (1000*60*60)  > ROOMEXPIRATIONTIME) { //in hours
			destroyRoom(room);
			expired++;
		}
	}

	return expired;

}

ChatRoom* ChatManagerImplementation::createRoom(const String& roomName, ChatRoom* parent) {
	ManagedReference<ChatRoom*> room = cast<ChatRoom*>(ObjectManager::instance()->createObject("ChatRoom", 0 , ""));

	if (parent != NULL) {
		room->init(server, parent, roomName);
		parent->addSubRoom(room->getName().toLowerCase(), room->getRoomID());

		if (parent->isPersistent())
			room->setParentIsPersistent(true);
	} else //Only the top "SWG" room should not have a parent.
		room->init(server, NULL, roomName);

	addRoom(room);

	return room;
}

ChatRoom* ChatManagerImplementation::createPersistentRoom(const String& roomName, ChatRoom* parent) {
	//There should always be a valid parent room for a persistent room.
	if (parent == NULL)
		return NULL;

	ManagedReference<ChatRoom*> room = cast<ChatRoom*>(ObjectManager::instance()->createObject("ChatRoom", 1 , "chatrooms"));

	room->init(server, parent, roomName);
	if (parent->isPersistent())
		room->setParentIsPersistent(true);

	parent->addSubRoom(room->getName().toLowerCase(), room->getRoomID());
	addRoom(room);

	return room;
}

ChatRoom* ChatManagerImplementation::getChatRoomByGamePath(ChatRoom* game, const String& path) {
	StringTokenizer tokenizer(path);
	tokenizer.setDelimeter(".");

	String channel;
	ChatRoom* room = game;

	while (tokenizer.hasMoreTokens()) {
		tokenizer.getStringToken(channel);

		unsigned int roomID = room->getSubRoom(channel.toLowerCase());
		room = getChatRoom(roomID);

		if (room == NULL)
			return NULL;
	}

	if (room == game)
		return NULL;
	else
		return room;
}

ChatRoom* ChatManagerImplementation::getChatRoomByFullPath(const String& path) {
	StringTokenizer tokenizer(path);
	tokenizer.setDelimeter(".");

	if (!tokenizer.hasMoreTokens())
		return NULL;

	String game;
	tokenizer.getStringToken(game);

	ChatRoom* gameRoom = getGameRoom(game);

	if (gameRoom == NULL)
		return NULL;

	if (!tokenizer.hasMoreTokens())
		return NULL;

	String gamePath;
	tokenizer.finalToken(gamePath);

	return getChatRoomByGamePath(gameRoom, gamePath);
}

void ChatManagerImplementation::destroyRoom(ChatRoom* room) {
	if (room == NULL)
		return;

	ManagedReference<ChatManager*> chatManager = _this.getReferenceUnsafeStaticCast();

	EXECUTE_TASK_2(room, chatManager, {
		//Notify all players to remove the room from their Channel Browser.
		ChatOnDestroyRoom* msg = new ChatOnDestroyRoom("SWG", room_p->getGalaxyName(), room_p->getOwnerName(), room_p->getRoomID());
		chatManager_p->broadcastMessage(msg);

		//Clear everyone out of the room on the server side.
		Locker locker(room_p);
		room_p->removeAllPlayers();

		//Check if this room has existing sub rooms.
		if (room_p->getSubRoomsSize() > 0) //Disable the room but don't delete it.
			chatManager_p->disableRoom(room_p);
		else //Safe to delete the room.
			chatManager_p->deleteRoom(room_p);

		locker.release();

		//Remove from the owner's list of created rooms.
		ManagedReference<CreatureObject*> owner = room_p->getZoneServer()->getObject(room_p->getOwnerID()).castTo<CreatureObject*>();
		if (owner != NULL) {
			Locker olocker(owner);
			PlayerObject* ghost = owner->getPlayerObject();
			if (ghost != NULL)
				ghost->removeOwnedChatRoom(room_p->getRoomID());
		}
	});

}

void ChatManagerImplementation::deleteRoom(ChatRoom* room) {
	//room locked
	ManagedReference<ChatRoom*> parent = room->getParent();

	if (parent != NULL) {
		parent->removeSubRoom(room->getName());

		if (parent->isDisabled()) {
			if (parent->getSubRoomsSize() < 1) {
				ManagedReference<ChatManager*> chatManager = _this.getReferenceUnsafeStaticCast();
				EXECUTE_TASK_2(parent, chatManager, {
					Locker locker(parent_p);
					chatManager_p->deleteRoom(parent_p);
				});
			}
		}
	}

	Locker clocker(_this.getReferenceUnsafeStaticCast(), room);
	roomMap->remove(room->getRoomID());
	ObjectManager::instance()->destroyObjectFromDatabase(room->_getObjectID());
}

void ChatManagerImplementation::disableRoom(ChatRoom* room) {
	//room prelocked

	room->setDisabled(true);

	room->removeAllModerators();
	room->removeAllInvited();
	room->removeAllBanned();
}

void ChatManagerImplementation::enableRoom(CreatureObject* player, ChatRoom* room, int requestID) {
	//ChatManager locked

	Locker clocker (room, _this.getReferenceUnsafeStaticCast());
	room->setDisabled(false);
	room->addModerator(player);
	clocker.release();

	Locker plocker(player, _this.getReferenceUnsafeStaticCast());

	int error = 0; //"[RoomName] You have created the channel."
	ChatOnCreateRoom* packet = new ChatOnCreateRoom(room, requestID, error);
	player->sendMessage(packet);

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
	if (ghost != NULL)
		ghost->addOwnedChatRoom(room->getRoomID());

}

void ChatManagerImplementation::destroyRooms() {
	Locker _locker(_this.getReferenceUnsafeStaticCast());

	HashTableIterator<unsigned int, ManagedReference<ChatRoom* > > iter = roomMap->iterator();

	while (iter.hasNext()) {
		ChatRoom* room = iter.next();
		room->finalize();
	}

	roomMap->removeAll();

	gameRooms.removeAll();
}



void ChatManagerImplementation::handleChatRoomMessage(CreatureObject* sender, const UnicodeString& message, unsigned int roomID, unsigned int counter) {
	String name = sender->getFirstName();
	String fullName = "";

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		fullName = getTaggedName(senderGhost, name);
	}

	ChatRoom* channel = getChatRoom(roomID);

	if (channel == NULL)
		return;

	if (!channel->hasPlayer(sender))
		return;

	Zone* zone = sender->getZone();
	if( zone == NULL ){
		return;
	}

	//Check for moderated (muted) room.
	if (channel->isModerated() && !channel->hasModerator(sender)) {
		int error = 9; //"Your message to [Room Path] was not sent because you are not a moderator."
		BaseMessage* amsg = new ChatOnSendRoomMessage(counter, error);
		sender->sendMessage(amsg);
		return;
	}

	UnicodeString formattedMessage(formatMessage(message));

	ManagedReference<ChatRoom*> planetRoom = zone->getPlanetChatRoom();

	BaseMessage* msg = new ChatRoomMessage(fullName, server->getGalaxyName(), formattedMessage, roomID);

	// Auction Chat and Planet Chat should adhere to player ignore list
	if(auctionRoom != NULL && auctionRoom->getRoomID() == roomID) {
		channel->broadcastMessageCheckIgnore(msg, name);
	} else if (planetRoom != NULL && planetRoom->getRoomID() == roomID) {
		channel->broadcastMessageCheckIgnore(msg, name);
	} else {
		channel->broadcastMessage(msg);
	}

	BaseMessage* amsg = new ChatOnSendRoomMessage(counter);
	channel->broadcastMessage(amsg);

}

void ChatManagerImplementation::handleChatEnterRoomById(CreatureObject* player, uint32 roomID, int requestID, bool bypassSecurity) {
	if (player == NULL)
		return;

	//Check if room exists.
	ManagedReference<ChatRoom*> room = getChatRoom(roomID);
	if (room == NULL) {
		int error = 6; //"Chatroom [Room path] does not exist!"
		ChatOnEnteredRoom* coer = new ChatOnEnteredRoom(server->getGalaxyName(), player->getFirstName(), roomID, error, requestID);
		player->sendMessage(coer);
		return;
	}

	//Check if player is allowed to join.
	int result = ChatManager::SUCCESS;

	if (!bypassSecurity)
		result = room->checkEnterPermission(player);

	ChatOnEnteredRoom* coer = new ChatOnEnteredRoom(server->getGalaxyName(), player->getFirstName(), roomID, result, requestID);

	if (result != ChatManager::SUCCESS) {
		player->sendMessage(coer);
		return;
	}

	//Add player to the room and notify everyone in room.
	room->addPlayer(player);
	room->broadcastMessage(coer);

}

void ChatManagerImplementation::handleSocialInternalMessage(CreatureObject* sender, const UnicodeString& arguments) {
	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}
	}

	Zone* zone = sender->getZone();

	if (zone == NULL)
		return;

	StringTokenizer tokenizer(arguments.toString());
	uint64 targetid;
	uint32 emoteid, unkint, unkint2;

	try {
		targetid = tokenizer.getLongToken();
		emoteid = tokenizer.getIntToken();
		unkint = tokenizer.getIntToken();
		unkint2 = tokenizer.getIntToken();
	} catch (const Exception& e) {
		return;
	}

	//bool readlock = !zone->isLockedByCurrentThread();

	bool showtext = true;

	if (unkint2 == 0)
		showtext = false;

	String firstName;

	if (sender->isPlayerCreature())
		firstName = (cast<CreatureObject*>(sender))->getFirstName().toLowerCase();

	CloseObjectsVector* vec = (CloseObjectsVector*) sender->getCloseObjects();

	SortedVector<QuadTreeEntry* > closeEntryObjects(200, 50);

	if (vec != NULL) {
		vec->safeCopyTo(closeEntryObjects);
	} else {
		sender->info("Null closeobjects vector in ChatManager::handleSocialInternalMessage", true);
		zone->getInRangeObjects(sender->getWorldPositionX(), sender->getWorldPositionX(), 128, &closeEntryObjects, true);
	}

	float range = defaultSpatialChatDistance;

	for (int i = 0; i < closeEntryObjects.size(); ++i) {
		SceneObject* object = cast<SceneObject*>(closeEntryObjects.get(i));

		if (object->isPlayerCreature()) {
			CreatureObject* creature = cast<CreatureObject*>(object);

			Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

			if (ghost == NULL)
				continue;

			if (!ghost->isIgnoring(firstName) && creature->isInRange(sender, range)) {
				Emote* emsg = new Emote(creature, sender, targetid, emoteid, showtext);
				creature->sendMessage(emsg);

			}
		}
	}

}

void ChatManagerImplementation::sendRoomList(CreatureObject* player) {
	Locker _locker(_this.getReferenceUnsafeStaticCast());

	ChatRoomList* crl = new ChatRoomList();

	HashTableIterator<unsigned int, ManagedReference<ChatRoom* > > iter = roomMap->iterator();

	while (iter.hasNext()) {
		ChatRoom* room = iter.next();
		if (room != NULL && !room->isDisabled()) {
			if (room->isPrivate()) {
				if (!room->hasInvited(player))
					continue;
			}

			crl->addChannel(room);
		}
	}

	crl->insertChannelListCount();
	player->sendMessage(crl);

}

void ChatManagerImplementation::addPlayer(CreatureObject* player) {
	Locker _locker(_this.getReferenceUnsafeStaticCast());

	String name = player->getFirstName().toLowerCase();
	playerMap->put(name, player, false);
}

CreatureObject* ChatManagerImplementation::getPlayer(const String& name) {
	Locker _locker(_this.getReferenceUnsafeStaticCast());

	CreatureObject* player = NULL;

	try {
		String lName = name.toLowerCase();

		player = playerMap->get(lName, false);
	} catch (Exception& e) {
		System::out << e.getMessage();
		e.printStackTrace();
	}

	return player;
}

CreatureObject* ChatManagerImplementation::removePlayer(const String& name) {
	Locker _locker(_this.getReferenceUnsafeStaticCast());

	String lName = name.toLowerCase();

	CreatureObject* player = playerMap->remove(lName, false);

	return player;
}

void ChatManagerImplementation::broadcastGalaxy(const String& message, const String& faction) {
	Locker locker(_this.getReferenceUnsafeStaticCast());

	playerMap->resetIterator(false);

	uint32 factionCRC = faction.hashCode();

	while (playerMap->hasNext(false)) {
		ManagedReference<CreatureObject*> playerObject = playerMap->getNextValue(false);

		if (playerObject->getFaction() == factionCRC || playerObject->getPlayerObject()->hasGodMode())
			playerObject->sendSystemMessage(message);
	}
}

void ChatManagerImplementation::broadcastGalaxy(CreatureObject* player, const String& message) {
	String firstName = "SKYNET";

	if (player != NULL)
		firstName = player->getFirstName();

	StringBuffer fullMessage;
	fullMessage << "[" << firstName << "] " << message;

	Locker locker(_this.getReferenceUnsafeStaticCast());
	//playerMap->lock();

	playerMap->resetIterator(false);

	while (playerMap->hasNext(false)) {
		ManagedReference<CreatureObject*> playerObject = playerMap->getNextValue(false);

		playerObject->sendSystemMessage(fullMessage.toString());
	}
}

void ChatManagerImplementation::broadcastMessage(BaseMessage* message) {
	Locker _lock(_this.getReferenceUnsafeStaticCast());

	playerMap->resetIterator(false);

	while (playerMap->hasNext(false)) {
		ManagedReference<CreatureObject*> player = playerMap->getNextValue(false);

		if (player == NULL || !player->isOnline())
			continue;

		player->sendMessage(message->clone());
	}

	delete message;
	message = NULL;
}

void ChatManagerImplementation::broadcastChatMessage(CreatureObject* player, const UnicodeString& message,  uint64 target, uint32 moodType, uint32 spatialChatType, int languageID) {
	Zone* zone = player->getZone();
	PlayerObject* myGhost = NULL;
	bool godMode = false;

	if (zone == NULL)
		return;

	String firstName;

	if (player->isPlayerCreature()) {
		CreatureObject* playerCreature = cast<CreatureObject*>(player);
		if (playerCreature)
		{
			firstName = playerCreature->getFirstName().toLowerCase();
			myGhost = playerCreature->getPlayerObject();
		}

		if (myGhost) {
			if (spatialChatTypeSkillNeeded.contains(spatialChatType) && !myGhost->hasAbility(spatialChatTypeSkillNeeded.get(spatialChatType)))
				return;

			if (myGhost->hasGodMode())
				godMode = true;
		}
	}

	StringIdChatParameter* param = NULL;

	if (message[0] == '@' && message.indexOf(":") != -1) {
		param = new StringIdChatParameter(message.toString());
	}

	CloseObjectsVector* closeObjects = (CloseObjectsVector*) player->getCloseObjects();

	SortedVector<QuadTreeEntry*> closeEntryObjects(200, 50);

	if (closeObjects != NULL) {
		closeObjects->safeCopyTo(closeEntryObjects);
	} else {
		player->info("Null closeobjects vector in ChatManager::broadcastChatMessage", true);
		zone->getInRangeObjects(player->getWorldPositionX(), player->getWorldPositionY(), 128, &closeEntryObjects, true);
	}

	float range = defaultSpatialChatDistance;

	float specialRange = spatialChatDistances.get(spatialChatType);
	if (specialRange != -1) {
		range = specialRange;
	}

	try {
		for (int i = 0; i < closeEntryObjects.size(); ++i) {
			SceneObject* object = cast<SceneObject*>(closeEntryObjects.get(i));

			if (player->isInRange(object, range)) {

				//Notify observers that are expecting spatial chat.
				if (object->getObserverCount(ObserverEventType::SPATIALCHATRECEIVED)) {
					ManagedReference<ChatMessage*> chatMessage = new ChatMessage();
					chatMessage->setString(message.toString());

					EXECUTE_TASK_3(object, chatMessage, player, {
						if (player_p == NULL || object_p == NULL)
							return;

						Locker locker(object_p);

						SortedVector<ManagedReference<Observer*> > observers = object_p->getObservers(ObserverEventType::SPATIALCHATRECEIVED);
						for (int oc = 0; oc < observers.size(); oc++) {
							Observer* observer = observers.get(oc);
							Locker clocker(observer, object_p);
							if (observer->notifyObserverEvent(ObserverEventType::SPATIALCHATRECEIVED, object_p, chatMessage_p, player_p->getObjectID()) == 1)
								object_p->dropObserver(ObserverEventType::SPATIALCHATRECEIVED, observer);
						}
					});
				}

				if (object->isPlayerCreature()) {
					CreatureObject* creature = cast<CreatureObject*>(object);
					PlayerObject* ghost = creature->getPlayerObject();

					if (ghost == NULL)
						continue;

					if (!ghost->isIgnoring(firstName) || godMode) {
						SpatialChat* cmsg = NULL;

						if (param == NULL) {
							cmsg = new SpatialChat(player->getObjectID(), creature->getObjectID(), message, target, moodType, spatialChatType, languageID);
						} else {
							cmsg = new SpatialChat(player->getObjectID(), creature->getObjectID(), *param, target, moodType, spatialChatType);
						}

						creature->sendMessage(cmsg);
					}
				}
				else if( object->isPet() ){
					AiAgent* pet = cast<AiAgent*>(object);

					if (pet == NULL )
						continue;

					if( pet->isDead() || pet->isIncapacitated() )
						continue;

					PetManager* petManager = server->getPetManager();
					Locker clocker(pet, player);
					petManager->handleChat( player, pet, message.toString() );

				}
			}
		}

		if (param != NULL) {
			delete param;
			param = NULL;
		}

	} catch (...) {
		//zone->runlock();

		//		zone->runlock(readlock);

		if (param != NULL) {
			delete param;
			param = NULL;
		}

		throw;
	}

	//zone->runlock();

}

void ChatManagerImplementation::broadcastChatMessage(CreatureObject* player, StringIdChatParameter& message, uint64 target, uint32 moodType, uint32 spatialChatType, int languageID) {
	Zone* zone = player->getZone();
	PlayerObject* myGhost = NULL;
	bool godMode = false;

	if (zone == NULL)
		return;

	String firstName;

	if (player->isPlayerCreature() /*|| !((Player *)player)->isChatMuted() */) {
		CreatureObject* playerCreature = cast<CreatureObject*>(player);

		if (playerCreature)
		{
			firstName = playerCreature->getFirstName().toLowerCase();
			myGhost = playerCreature->getPlayerObject();
		}

		if (myGhost)
		{
			if (spatialChatTypeSkillNeeded.contains(spatialChatType) && !myGhost->hasAbility(spatialChatTypeSkillNeeded.get(spatialChatType)))
				return;

			if (myGhost->hasGodMode())
				godMode = true;
		}
	}

	CloseObjectsVector* closeObjects = (CloseObjectsVector*) player->getCloseObjects();

	SortedVector<QuadTreeEntry*> closeEntryObjects(200, 50);

	if (closeObjects != NULL) {
		closeObjects->safeCopyTo(closeEntryObjects);
	} else {
		player->info("Null closeobjects vector in ChatManager::broadcastChatMessage(StringId)", true);
		zone->getInRangeObjects(player->getWorldPositionX(), player->getWorldPositionY(), ZoneServer::CLOSEOBJECTRANGE, &closeEntryObjects, true);
	}

	try {
		for (int i = 0; i < closeEntryObjects.size(); ++i) {
			SceneObject* object = cast<SceneObject*>(closeEntryObjects.get(i));

			if (player->isInRange(object, 128)) {

				//Notify observers that are expecting spatial chat.
				if (object->getObserverCount(ObserverEventType::SPATIALCHATRECEIVED)) {
					ManagedReference<ChatMessage*> chatMessage = new ChatMessage();
					chatMessage->setString(message.toString());

					object->notifyObservers(ObserverEventType::SPATIALCHATRECEIVED, chatMessage, player->getObjectID());
				}

				if (object->isPlayerCreature()) {
					CreatureObject* creature = cast<CreatureObject*>(object);
					PlayerObject* ghost = creature->getPlayerObject();

					if (ghost == NULL)
						continue;

					if (!ghost->isIgnoring(firstName) || godMode) {
						SpatialChat* cmsg = new SpatialChat(player->getObjectID(), creature->getObjectID(), message, target, moodType, spatialChatType);

						creature->sendMessage(cmsg);
					}
				}
			}
		}
	} catch (...) {
		throw;
	}
}

void ChatManagerImplementation::handleSpatialChatInternalMessage(CreatureObject* player, const UnicodeString& args) {
	if (player->isPlayerCreature()) {
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		if (ghost == NULL)
			return;

		if (ghost->isMuted()) {
			String reason = ghost->getMutedReason();

			if (reason != "")
				player->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				player->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}
	}

	try {
		UnicodeTokenizer tokenizer(args);

		uint64 targetid = tokenizer.getLongToken();
		uint32 spatialChatType = tokenizer.getIntToken();
		uint32 moodType = tokenizer.getIntToken();
		uint32 unk2 = tokenizer.getIntToken();
		uint32 languageID = tokenizer.getIntToken();

		UnicodeString msg;

		tokenizer.finalToken(msg);

		UnicodeString formattedMessage(formatMessage(msg));
		broadcastChatMessage(player, formattedMessage, targetid, moodType, spatialChatType, languageID);

		ManagedReference<ChatMessage*> cm = new ChatMessage();
		cm->setString(formattedMessage.toString());
		player->notifyObservers(ObserverEventType::CHAT, cm, 0);

	} catch (Exception& e) {
		StringBuffer msg;
		msg << "Exception in ChatManagerImplementation::handleMessage " << e.getMessage() << "\n";
		System::out << msg.toString();
		e.printStackTrace();
	}
}

//TODO: Refactor into a sendInstantMessage() method that returns a returnCode.
void ChatManagerImplementation::handleChatInstantMessageToCharacter(ChatInstantMessageToCharacter* message) {
	ManagedReference<SceneObject*> scene = message->getClient()->getPlayer();

	if (scene == NULL)
		return;

	CreatureObject* sender = cast<CreatureObject*>(scene.get());

	bool godMode = false;

	if (sender == NULL)
		return;

	String name = sender->getFirstName();

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = NULL;
		senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->hasGodMode())
			godMode = true;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		name = getTaggedName(senderGhost, name);
	}

	uint32 sequence = message->getSequence();

	// Pull out first name
	String fname = message->getName();
	int spc = fname.indexOf(" ");
	if (spc != -1) {
		fname = fname.subString(0, spc);
	}

	ManagedReference<CreatureObject*> receiver = getPlayer(fname);

	if (receiver == NULL || !receiver->isOnline()) {
		BaseMessage* amsg = new ChatOnSendInstantMessage(sequence, IM_OFFLINE);
		sender->sendMessage(amsg);

		return;
	}

	if (receiver->getPlayerObject()->isIgnoring(sender->getFirstName()) && !godMode) {
		BaseMessage* amsg = new ChatOnSendInstantMessage(sequence, IM_IGNORED);
		sender->sendMessage(amsg);

		return;
	}

	UnicodeString text = message->getMessage();

	if (text.length() > IM_MAXSIZE) {
		BaseMessage* amsg = new ChatOnSendInstantMessage(sequence, IM_TOOLONG);
		sender->sendMessage(amsg);

		return;
	}

	text = formatMessage(text);

	BaseMessage* msg = new ChatInstantMessageToClient("SWG", sender->getZoneServer()->getGalaxyName(), name, text);
	receiver->sendMessage(msg);

	BaseMessage* amsg = new ChatOnSendInstantMessage(message->getSequence(), IM_SUCCESS);
	sender->sendMessage(amsg);
}

ChatRoom* ChatManagerImplementation::createGroupRoom(uint64 groupID, CreatureObject* creator) {
	// Pre: creator locked;
	// Post: creator locked.

	ManagedReference<ChatRoom*> groupChatRoom;

	StringBuffer name;
	name << groupID;

	ChatRoom* newGroupRoom = createRoom(name.toString(), groupRoom);
	newGroupRoom->setPrivate();

	groupChatRoom = createRoom("GroupChat", newGroupRoom);

	groupChatRoom->setTitle(name.toString());
	groupChatRoom->setPrivate();
	groupChatRoom->setCanEnter(true);
	groupChatRoom->setChatRoomType(ChatRoom::GROUP);
	groupChatRoom->setOwnerID(groupID);

	groupChatRoom->sendTo(creator);
	handleChatEnterRoomById(creator, groupChatRoom->getRoomID(), -1, true);

	return groupChatRoom;
}

void ChatManagerImplementation::handleGroupChat(CreatureObject* sender, const UnicodeString& message) {
	String name = sender->getFirstName();

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		name = getTaggedName(senderGhost, name);
	}

	ManagedReference<GroupObject*> group = sender->getGroup();
	if (group == NULL) {
		sender->sendSystemMessage("@combat_effects:not_in_group"); // You must be in a group to perform this command.
		return;
	}

	StringTokenizer args(message.toString());
	if (!args.hasMoreTokens()) {
		sender->sendSystemMessage("@ui:im_no_message"); // You need to include a message!
		return;
	}

	UnicodeString formattedMessage(formatMessage(message));

	sender->unlock();

	try {
		group->wlock();

		ManagedReference<ChatRoom*> room = group->getChatRoom();

		if (room != NULL) {
			BaseMessage* msg = new ChatRoomMessage(name, server->getGalaxyName(), formattedMessage, room->getRoomID());
			group->broadcastMessage(msg);
		}

		group->unlock();
	} catch (...) {
		group->unlock();

		throw;
	}

	sender->wlock();
}

void ChatManagerImplementation::handleGuildChat(CreatureObject* sender, const UnicodeString& message) {
	String name = sender->getFirstName();

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		name = getTaggedName(senderGhost, name);
	}

	ManagedReference<GuildObject*> guild = sender->getGuildObject().get();
	if (guild == NULL) {
		sender->sendSystemMessage("@error_message:not_in_guild"); // You are not in a guild.
		return;
	}

	StringTokenizer args(message.toString());
	if (!args.hasMoreTokens()) {
		sender->sendSystemMessage("@ui:im_no_message"); // You need to include a message!
		return;
	}

	UnicodeString formattedMessage(formatMessage(message));

	ManagedReference<ChatRoom*> room = guild->getChatRoom();
	if (room != NULL) {
		BaseMessage* msg = new ChatRoomMessage(name, server->getGalaxyName(), formattedMessage, room->getRoomID());
		room->broadcastMessage(msg);
	}

}

void ChatManagerImplementation::handlePlanetChat(CreatureObject* sender, const UnicodeString& message) {
	String name = sender->getFirstName();
	String fullName = "";

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		fullName = getTaggedName(senderGhost, name);
	}

	Zone* zone = sender->getZone();
	if( zone == NULL ){
		return;
	}

	StringTokenizer args(message.toString());
	if (!args.hasMoreTokens()) {
		sender->sendSystemMessage("@ui:im_no_message"); // You need to include a message!
		return;
	}

	UnicodeString formattedMessage(formatMessage(message));

	ManagedReference<ChatRoom*> room = zone->getPlanetChatRoom();

	if (room != NULL) {
		BaseMessage* msg = new ChatRoomMessage(fullName, server->getGalaxyName(), formattedMessage, room->getRoomID());
		room->broadcastMessageCheckIgnore(msg, name);
	}

}

void ChatManagerImplementation::handleAuctionChat(CreatureObject* sender, const UnicodeString& message) {
	String name = sender->getFirstName();
	String fullName = "";

	if (sender->isPlayerCreature()) {
		ManagedReference<PlayerObject*> senderGhost = sender->getPlayerObject();

		if (senderGhost == NULL)
			return;

		if (senderGhost->isMuted()) {
			String reason = senderGhost->getMutedReason();

			if (reason != "")
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support for '" + reason + "'.");
			else
				sender->sendSystemMessage("Your chat abilities are currently disabled by Customer Support.");

			return;
		}

		fullName = getTaggedName(senderGhost, name);
	}

	StringTokenizer args(message.toString());
	if (!args.hasMoreTokens()) {
		sender->sendSystemMessage("@ui:im_no_message"); // You need to include a message!
		return;
	}

	UnicodeString formattedMessage(formatMessage(message));

	if (auctionRoom != NULL) {
		BaseMessage* msg = new ChatRoomMessage(fullName, server->getGalaxyName(), formattedMessage, auctionRoom->getRoomID());
		auctionRoom->broadcastMessageCheckIgnore(msg, name);
	}

}

void ChatManagerImplementation::sendMail(const String& sendername, const UnicodeString& header, const UnicodeString& body, const String& name) {
	uint64 receiverObjectID = playerManager->getObjectID(name);
	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;

	if (receiverObjectID == 0) {
		error("unexistent name for persistent message");
		return;
	}

	ManagedReference<SceneObject*> receiver = server->getObject(receiverObjectID);

	if (receiver == NULL) {
		error("NULL receiver in send mail");

		return;
	}

	if (!receiver->isPlayerCreature()) {
		error("not player in send mail");

		return;
	}

	CreatureObject* player = cast<CreatureObject*>(receiver.get());
	PlayerObject* ghost = player->getPlayerObject();

	/*StringIdChatParameter test("base_player", "sale_fee");
	test.setDI(100);
	test.setUnknownByte(1);*/

	ManagedReference<PersistentMessage*> mail = new PersistentMessage();
	mail->setSenderName(sendername);
	mail->setSubject(header);
	mail->setBody(body);
	//mail->setParameterizedBody(test);
	mail->setReceiverObjectID(receiverObjectID);
	mail->setTimeStamp(currentTime);

	ObjectManager::instance()->persistObject(mail, 1, "mail");

	/*Locker _locker(player);

	ghost->addPersistentMessage(mail->getObjectID());

	if (player->isOnline())
		mail->sendTo(player, false);*/

	Reference<SendMailTask*> sendMailTask = new SendMailTask(player, mail, sendername);
	sendMailTask->execute();
}

int ChatManagerImplementation::sendMail(const String& sendername, const UnicodeString& subject, const UnicodeString& body, const String& recipientName, StringIdChatParameterVector* stringIdParameters, WaypointChatParameterVector* waypointParameters) {
	uint64 receiverObjectID = playerManager->getObjectID(recipientName);
	ManagedReference<SceneObject*> obj = server->getObject(receiverObjectID);
	ManagedReference<CreatureObject*> sender = NULL;
	bool godMode = false;

	if (obj == NULL || !obj->isPlayerCreature())
		return IM_OFFLINE;

	if (body.length() > PM_MAXSIZE)
		return IM_TOOLONG;

	sender = playerManager->getPlayer(sendername.toLowerCase());

	if (sender)
	{
		if (sender->isPlayerCreature()) {
			ManagedReference<PlayerObject*> senderPlayer = NULL;
			senderPlayer = sender->getPlayerObject();

			if (senderPlayer == NULL)
				return IM_OFFLINE;

			if (senderPlayer->hasGodMode())
				godMode = true;
		}
	}

	CreatureObject* receiver = cast<CreatureObject*>(obj.get());
	PlayerObject* receiverPlayerObject = receiver->getPlayerObject();

	if ((receiverPlayerObject == NULL) || (receiverPlayerObject->isIgnoring(sendername) && !godMode))
		return IM_IGNORED;

	ManagedReference<PersistentMessage*> mail = new PersistentMessage();
	mail->setSenderName(sendername);
	mail->setSubject(subject);
	mail->setBody(body);

	for (int i = 0; i < stringIdParameters->size(); ++i) {
		StringIdChatParameter* param = &stringIdParameters->get(i);

		if (param == NULL)
			continue;

		mail->addStringIdParameter(*param);
	}

	for (int i = 0; i < waypointParameters->size(); ++i) {
		WaypointChatParameter* param = &waypointParameters->get(i);

		if (param == NULL)
			continue;

		mail->addWaypointParameter(*param);
	}

	mail->setReceiverObjectID(receiverObjectID);

	ObjectManager::instance()->persistObject(mail, 1, "mail");

	/*
	PlayerObject* ghost = receiver->getPlayerObject();

	ghost->addPersistentMessage(mail->getObjectID());

	if (receiver->isOnline())
		mail->sendTo(receiver, false);
	 */

	Reference<SendMailTask*> sendMailTask = new SendMailTask(receiver, mail, sendername);
	sendMailTask->execute();

	return IM_SUCCESS;
}

int ChatManagerImplementation::sendMail(const String& sendername, const UnicodeString& subject, StringIdChatParameter& body, const String& recipientName, WaypointObject* waypoint) {
	uint64 receiverObjectID = playerManager->getObjectID(recipientName);
	ManagedReference<SceneObject*> obj = server->getObject(receiverObjectID);
	ManagedReference<CreatureObject*> sender = NULL;
	bool godMode = false;

	if (obj == NULL || !obj->isPlayerCreature())
		return IM_OFFLINE;

	CreatureObject* receiver = cast<CreatureObject*>(obj.get());
	sender = playerManager->getPlayer(sendername.toLowerCase());

	if (sender)
	{
		if (sender->isPlayerCreature()) {
			ManagedReference<PlayerObject*> senderPlayer = NULL;
			senderPlayer = sender->getPlayerObject();

			if (senderPlayer == NULL)
				return IM_OFFLINE;

			if (senderPlayer->hasGodMode())
				godMode = true;
		}
	}

	PlayerObject* ghost = receiver->getPlayerObject();

	if (ghost == NULL ||
			(ghost->isIgnoring(sendername) && !godMode))
		return IM_IGNORED;

	ManagedReference<PersistentMessage*> mail = new PersistentMessage();
	mail->setSenderName(sendername);
	mail->setSubject(subject);
	mail->addStringIdParameter(body);

	if (waypoint != NULL) {
		WaypointChatParameter waypointParam(waypoint);
		mail->addWaypointParameter(waypointParam);
	}

	mail->setReceiverObjectID(receiverObjectID);

	ObjectManager::instance()->persistObject(mail, 1, "mail");

	Reference<SendMailTask*> sendMailTask = new SendMailTask(receiver, mail, sendername);
	sendMailTask->execute();
/*
	PlayerObject* ghost = receiver->getPlayerObject();

	ghost->addPersistentMessage(mail->getObjectID());

	if (receiver->isOnline())
		mail->sendTo(receiver, false);

		*/

	return IM_SUCCESS;
}

void ChatManagerImplementation::loadMail(CreatureObject* player) {
	Locker _locker(player);

	PlayerObject* ghost = player->getPlayerObject();

	SortedVector<uint64>* messages = ghost->getPersistentMessages();

	for (int i = messages->size() - 1; i >= 0 ; --i) {
		uint64 messageObjectID = messages->get(i);

		Reference<PersistentMessage*> mail = Core::getObjectBroker()->lookUp(messageObjectID).castTo<PersistentMessage*>();

		if (mail == NULL) {
			messages->drop(messageObjectID);
			continue;
		}

		mail->sendTo(player, false);
	}
}

void ChatManagerImplementation::handleRequestPersistentMsg(CreatureObject* player, uint32 mailID) {
	Locker _locker(player);

	PlayerObject* ghost = player->getPlayerObject();

	SortedVector<uint64>* messages = ghost->getPersistentMessages();

	uint64 messageObjectID = -1;

	for (int i = 0; i < messages->size(); ++i) {
		messageObjectID = messages->get(i);

		if (Long::hashCode(messageObjectID) == mailID)
			break;
	}

	if (messageObjectID == -1) {
		error("could not find mail in handleRequestPersistentMsg");
		return;
	}

	Reference<PersistentMessage*> mail = Core::getObjectBroker()->lookUp(messageObjectID).castTo<PersistentMessage*>();

	if (mail == NULL) {
		messages->drop(messageObjectID);
		return;
	}

	_locker.release();

	mail->setStatus('R');
	mail->sendTo(player, true);
}

void ChatManagerImplementation::deletePersistentMessage(CreatureObject* player, uint32 mailID) {
	Locker _locker(player);

	PlayerObject* ghost = player->getPlayerObject();

	SortedVector<uint64>* messages = ghost->getPersistentMessages();

	uint64 messageObjectID = -1;

	for (int i = 0; i < messages->size(); ++i) {
		messageObjectID = messages->get(i);

		if (Long::hashCode(messageObjectID) == mailID)
			break;
	}

	if (messageObjectID == -1) {
		error("could not find mail in deletePersistentMessage");
		return;
	}

	messages->drop(messageObjectID);

	_locker.release();

	ObjectManager::instance()->destroyObjectFromDatabase(messageObjectID);
}

UnicodeString ChatManagerImplementation::formatMessage(const UnicodeString& message) {
	UnicodeString text = message;

	while (text.indexOf("\\>") >= 0) {
		int index = text.indexOf("\\>");
		UnicodeString sub = "\\" + text.subString(index, index + 2);
		text = text.replaceFirst(sub,"");
	}

	return text;
}

String ChatManagerImplementation::getTaggedName(PlayerObject* ghost, const String& name) {
	String taggedName = name;

	if (ghost->hasGodMode()) {
		String tag = PermissionLevelList::instance()->getPermissionTag(ghost->getAdminLevel()).toString();
		taggedName = name + " [" + tag + "]";
	}

	return taggedName;
}

void ChatManagerImplementation::handleChatCreateRoom(CreatureObject* player, uint8 permissionFlag, uint8 moderationFlag, const String& roomPath, const String& roomTitle, int requestID) {
	Locker _lock(_this.getReferenceUnsafeStaticCast());

	//This request is sent by clients to make a new room and to join a room if the client doesn't have the roomID.

	/* Error Codes:
	 * 0: [RoomName] You have created the channel. (successful creation)
	 * 6: Cannot create the channel named '[RoomPathName]' because the name is invalid.
	 * 24: NO MESSAGE (room already exists)
	 * Default: Channel '[RoomPathName]' creation failed for an unknown reason.*/

	int error = 0;

	//Check if player has reached their max allowed rooms yet.
	Locker clocker(player, _this.getReferenceUnsafeStaticCast());
	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
	if (ghost == NULL || ghost->getOwnedChatRoomCount() >= MAXCUSTOMCHATROOMS) {
		error = 1;
		sendChatOnCreateRoomError(player, requestID, error);
		return;
	}

	clocker.release();

	//Attempt to create the new room as a subroom of the second last path node (the parent).
	ManagedReference<ChatRoom*> newRoom = createPersistentRoomByFullPath(player, roomPath, roomTitle, requestID);

	if (newRoom == NULL)
		return;

	//Set the room flags as specified by the player.
	if (permissionFlag == 0)
		newRoom->setPrivate();
	else
		newRoom->setPublic();

	if (moderationFlag == 0)
		newRoom->setModerated(false);
	else
		newRoom->setModerated(true);

	newRoom->setTitle(roomTitle);

	//Initialize the creator and owner of the new room.
	newRoom->setCreator(player->getFirstName());
	newRoom->setOwnerName(player->getFirstName());
	newRoom->setOwnerID(player->getObjectID());
	newRoom->addModerator(player);
	if (newRoom->isPrivate())
		newRoom->addInvited(player);

	newRoom->setCanEnter(true);
	newRoom->setChatRoomType(ChatRoom::CUSTOM);

	Locker plocker(player, _this.getReferenceUnsafeStaticCast());
	ghost->addOwnedChatRoom(newRoom->getRoomID());

	ChatOnCreateRoom* packet = new ChatOnCreateRoom(newRoom, requestID, error);
	player->sendMessage(packet);

}

ChatRoom* ChatManagerImplementation::createPersistentRoomByFullPath(CreatureObject* player, const String& path, const String& title, int requestID) {
	//ChatManager locked

	StringTokenizer tokenizer(path);
	tokenizer.setDelimeter(".");

	String game;
	tokenizer.getStringToken(game);

	ChatRoom* gameRoom = getGameRoom(game);

	if (gameRoom == NULL)
		return NULL;

	ManagedReference<ChatRoom*> parent = gameRoom;

	String channel;
	String persistentPath = "";
	int persistentNodes = 0;

	while (tokenizer.hasMoreTokens()) {

		//Check Persistent room nodes in path does not exceed maximum.
		if (persistentNodes >= ChatManager::MAXPERSISTENTNODES) {
			int error = 6; //"Cannot create the channel named '[RoomPathName]' because the name is invalid."
			sendChatOnCreateRoomError(player, requestID, error);
			return NULL;
		}

		//Get name of next node in path.
		tokenizer.getStringToken(channel);
		String channelLower = channel.toLowerCase(); //Subrooms are stored in lower case to prevent close duplicates.
		unsigned int subRoomID = parent->getSubRoom(channelLower);
		ChatRoom* subRoom = getChatRoom(subRoomID);

		if (subRoom == NULL) { //Found first node that doesn't already exist.
			persistentPath = persistentPath + channelLower;

			if (tokenizer.hasMoreTokens()) { //Do not create a room that is not the last node in the entered path.
				int error = 6; //"Cannot create the channel named '[RoomPathName]' because the name is invalid."
				sendChatOnCreateRoomError(player, requestID, error);
				return NULL;

			} else //First non-existent node in path is the last node, try to create it in parent.
				break;

		} else { //Node exists, check if it's the last path node.
			if (!tokenizer.hasMoreTokens()) {
				if (subRoom->isDisabled() && subRoom->getOwnerID() == player->getObjectID()) { //Allow owner to re-enable the room.
					enableRoom(player, subRoom, requestID);
					return NULL;

				} else { //Only the owner can re-enable the room.
					int error = 24; //NO MESSAGE (room already exists)
					sendChatOnCreateRoomError(player, requestID, error);
					return NULL;
				}

			} else {
				parent = subRoom;
				if (parent->isPersistent()) {
					persistentPath = persistentPath + channelLower;
					persistentNodes++;
				}
			}
		}
	}

	if (parent == gameRoom) {
		int error = 6; //"Cannot create the channel named '[RoomPathName]' because the name is invalid."
		sendChatOnCreateRoomError(player, requestID, error);
		return NULL;
	}

	//Check permission to create channel in the parent.
	if (!parent->subroomsAllowed() && !parent->hasModerator(player)) {
		int error = 1; //"Channel '[RoomPathName]' creation failed for an unknown reason.""
		sendChatOnCreateRoomError(player, requestID, error);
		return NULL;
	}

	NameManager* nameManager = NameManager::instance();
	bool nameOK = true;

	//Validate the room name on its own (need to compare to special cases).
	if (channel.length() > ChatManager::MAXCHATROOMNAMELENGTH || nameManager->validateChatRoomName(channel) != NameManagerResult::ACCEPTED)
		nameOK = false;

	//Check for profane room title.
	if (nameOK && nameManager->isProfane(title))
		nameOK = false;

	//Check for profane path combinations on the path's persistent portion.
	if (nameOK && nameManager->isProfane(persistentPath))
		nameOK = false;

	if (!nameOK) {
		int error = 6; //"Cannot create the channel named '[RoomPathName]' because the name is invalid."
		sendChatOnCreateRoomError(player, requestID, error);
		return NULL;
	}

	ChatRoom* newRoom = createPersistentRoom(channel, parent);

	return newRoom;

}

void ChatManagerImplementation::sendChatOnCreateRoomError(CreatureObject* player, int requestID, int error) {
	ChatOnCreateRoom* errorPacket = new ChatOnCreateRoom(player, requestID, error);
	player->sendMessage(errorPacket);

}

String ChatManagerImplementation::getRoomNameFromPath(const String& path) {
	StringTokenizer tokenizer(path);
	tokenizer.setDelimeter(".");

	String name = path;

	while (tokenizer.hasMoreTokens())
		tokenizer.getStringToken(name);

	return name;

}

void ChatManagerImplementation::handleChatDestroyRoom(CreatureObject* player, uint32 roomID, int requestID) {
	/* Error codes:
	 * 0: Success: [RoomName] You have destroyed the channel.
	 * Default: You don't have permission to delete '[RoomPathName]'.
	 */

	ManagedReference<ChatRoom*> room = getChatRoom(roomID);
	if (room != NULL && room->getOwnerID() == player->getObjectID())
		destroyRoom(room);

	else {
		int error = 1;
		ChatOnDestroyRoom* errorPacket = new ChatOnDestroyRoom(roomID, error, requestID);
		player->sendMessage(errorPacket);
	}

}

void ChatManagerImplementation::handleChatLeaveRoom(CreatureObject* player, const String& roomPath) {
	ChatRoom* room = getChatRoomByFullPath(roomPath);

	if (room == NULL)
		return;

	Locker clocker(room, player);
	room->removePlayer(player);

}

void ChatManagerImplementation::handleChatQueryRoom(CreatureObject* player, const String& roomPath, int requestID) {
	ChatRoom* room = getChatRoomByFullPath(roomPath);
	if (player == NULL || room == NULL)
		return;

	Locker lock(room);

	ChatQueryRoomResults* packet = new ChatQueryRoomResults(room, requestID);
	player->sendMessage(packet);

}

void ChatManagerImplementation::broadcastQueryResultsToRoom(ChatRoom* room) {
	//Room prelocked

	if (room == NULL)
		return;

	ChatQueryRoomResults* notification = new ChatQueryRoomResults(room);
	room->broadcastMessage(notification);

}

void ChatManagerImplementation::handleChatInvitePlayer(CreatureObject* inviter, const String& inviteeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: You have invited %TT to %TU. (SUCCESS)
	 * 1: Failed to invite %TT to %TU. (FAIL)
	 * 4: Failed to invite %TT to %TU: avatar not found. (NOAVATAR)
	 * 6: Failed to invite %TT to %TU: room does not exist. (NOROOM)
	 * 9: Failed to invite %TT to %TU: room is not private. (NOTPRIVATE)
	 * 16: Failed to invite %TT to %TU: you are not a moderator. (NOPERMISSION)
	 */

	if (inviter == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	} else if (room->isPublic()) {
		sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::NOTPRIVATE, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(inviter)) {
		sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the invitee.
	ManagedReference<CreatureObject*> invitee = playerManager->getPlayer(inviteeName);
	if (invitee == NULL || inviteeName != invitee->getFirstName()) { //Enforce proper capitalization to prevent duplicate list entries on client.
		sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	}

	Locker locker(room);

	//Remove ban if applicable.
	if (room->hasBanned(invitee)) {
		room->removeBanned(invitee);
		ChatOnUnbanFromRoom* unbanNotification = new ChatOnUnbanFromRoom(inviter, inviteeName, roomPath, ChatManager::SUCCESS);
		room->broadcastMessage(unbanNotification); //"%TT has been unbanned from %TU."
	}

	//Add invitee and send invitation.
	if (!room->hasInvited(invitee)) {
		room->addInvited(invitee);
		room->sendTo(invitee); //Add to invitee's Channel Browser.

		ChatOnReceiveRoomInvitation* invitation = new ChatOnReceiveRoomInvitation(server->getGalaxyName(), inviter->getFirstName(), roomPath);
		invitee->sendMessage(invitation);
	} else {
		sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	//Notify the inviter of the successful invite.
	sendChatOnInviteResult(inviter, inviteeName, roomPath, ChatManager::SUCCESS, requestID);

	//Update the room list for all players in the room.
	broadcastQueryResultsToRoom(room);

}

void ChatManagerImplementation::sendChatOnInviteResult(CreatureObject* inviter, const String& inviteeName, const String& roomPath, int error, int requestID) {
	ChatOnInviteToRoom* notification = new ChatOnInviteToRoom(inviter, inviteeName, roomPath, error, requestID);
	inviter->sendMessage(notification);
}

void ChatManagerImplementation::handleChatUninvitePlayer(CreatureObject* uninviter, const String& uninviteeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: Success: You uninvited [Target] from [RoomPathName]. (SUCCESS)
	 * 1: Failed to uninvite %TT to %TU. (FAIL)
	 * 4: Failed to uninvite [Target] from [RoomPathName]: avatar not found. (NOAVATAR)
	 * 6: Failed to uninvite [Target] from [RoomPathName]: room does not exist. (NOROOM)
	 * 9: Failed to uninvite [Target] from [RoomPathName]: room is not private. (NOTPRIVATE)
	 * 13: Failed to uninvite [Target] from [RoomPathName]: that avatar is not currently invited. (NOTINVITED)
	 * 16: Failed to uninvite [Target] from [RoomPathName]: you are not a moderator. (NOPERMISSION)
	 */

	if (uninviter == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	} else if (room->isPublic()){
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::NOTPRIVATE, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(uninviter)) {
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the uninvitee.
	ManagedReference<CreatureObject*> uninvitee = playerManager->getPlayer(uninviteeName);
	if (uninvitee == NULL) {
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	} else if (uninvitee->getObjectID() == room->getOwnerID()) { //Sorry buddy, you cannot uninvite the channel owner!
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	Locker locker(room);

	//Remove moderator status if applicable.
	if (room->hasModerator(uninvitee)) {
		room->removeModerator(uninvitee);
		ChatOnRemoveModeratorFromRoom* deopNotification = new ChatOnRemoveModeratorFromRoom(uninviter, uninviteeName, roomPath, ChatManager::SUCCESS);
		room->broadcastMessage(deopNotification); //"Moderator status has been revoked from %TU."
	}

	//Remove invite and kick from the room.
	if (room->hasInvited(uninvitee)) {
		if (room->hasPlayer(uninvitee)) {
			Locker clocker(uninvitee, room);
			room->removePlayer(uninvitee);

			//Send kick notification to uninvitee.
			StringIdChatParameter notification;
			notification.setStringId("guild", "kick_target"); //"%TU has removed you from %TT."
			notification.setTU(uninviter->getFirstName());
			notification.setTT(roomPath);
			uninvitee->sendSystemMessage(notification);
		}

		room->removeInvited(uninvitee);

	} else {
		sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::NOTINVITED, requestID);
		return;
	}

	//Notify the uninviter of the successful uninvite.
	sendChatOnUninviteResult(uninviter, uninviteeName, roomPath, ChatManager::SUCCESS, requestID);

	//Update the room list for all players in the room.
	broadcastQueryResultsToRoom(room);

	//Remove the room from uninvitee's channel browser.
	room->sendDestroyTo(uninvitee); //Done last in the off chance they uninvited themself so the error displays the proper room path.

}

void ChatManagerImplementation::sendChatOnUninviteResult(CreatureObject* uninviter, const String& uninviteeName, const String& roomPath, int error, int requestID) {
	ChatOnUninviteFromRoom* notification = new ChatOnUninviteFromRoom(uninviter, uninviteeName, roomPath, error, requestID);
	uninviter->sendMessage(notification);
}

void ChatManagerImplementation::handleChatKickPlayer(CreatureObject* kicker, const String& kickeeName, const String& roomPath) {
	//Nothing locked

	if (kicker == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL)
		return;

	//Get the player to be kicked.
	ManagedReference<CreatureObject*> kickee = getPlayer(kickeeName); //locks ChatManager
	if (kickee == NULL || kickee->getObjectID() == room->getOwnerID()) //Sorry friend, you cannot kick the channel owner!
		return;

	//Check for moderator permission.
	if (!room->hasModerator(kicker))
		return;

	//Remove the player from the room.
	if (room->hasPlayer(kickee)) {
		Locker locker(room);
		Locker clocker(kickee, room);
		room->removePlayer(kickee);
	}

	//Send kick notification to kickee.
	StringIdChatParameter kickeeMsg("guild", "kick_target"); //"%TU has removed you from %TT."
	kickeeMsg.setTU(kicker->getFirstName());
	kickeeMsg.setTT(roomPath);
	kickee->sendSystemMessage(kickeeMsg);

	//Send kick notification to kicker.
	StringIdChatParameter kickerMsg("guild", "kick_self"); //"You remove %TU from %TT."
	kickerMsg.setTU(kickee->getFirstName());
	kickerMsg.setTT(roomPath);
	kicker->sendSystemMessage(kickerMsg);

}

void ChatManagerImplementation::handleChatAddModerator(CreatureObject* oper, const String& opeeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: Success: Moderator status has been granted to [Target]. (SUCCESS)
	 * 1: Failed to op [Game.Server.Target] in [RoomPathName]. (FAIL)
	 * 4: Failed to op [Game.Server.Target] in [RoomPathName]: avatar not found. (NOAVATAR)
	 * 6: Failed to op [Game.Server.Target] in [RoomPathName]: room does not exist. (NOROOM)
	 * 16: Failed to op [Game.Server.Target] in [RoomPathName]: You are not a moderator. (NOPERMISSION)
	 */

	if (oper == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(oper)) {
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the op'ee
	ManagedReference<CreatureObject*> opee = playerManager->getPlayer(opeeName);
	if (opee == NULL) {
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	} else if (opee->getObjectID() == room->getOwnerID()) { //Sorry guy, you cannot op the channel owner!
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	Locker locker(room);

	//Check if already op'd.
	if (room->hasModerator(opee)) {
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	//Remove ban if applicable.
	if (room->hasBanned(opee)) {
		room->removeBanned(opee);
		ChatOnUnbanFromRoom* unbanNotification = new ChatOnUnbanFromRoom(oper, opeeName, roomPath, ChatManager::SUCCESS);
		room->broadcastMessage(unbanNotification); //"%TT has been unbanned from %TU."
	}

	//Add invite and send invitation if applicable.
	if (room->isPrivate()) {
		if (!room->hasInvited(opee)) {
			room->addInvited(opee);
			room->sendTo(opee); //Shows private room in their Channel Browser.

			//Update the room list for all players in the room.
			broadcastQueryResultsToRoom(room);

			//Send invitation.
			ChatOnReceiveRoomInvitation* invitation = new ChatOnReceiveRoomInvitation(server->getGalaxyName(), oper->getFirstName(), roomPath);
			opee->sendMessage(invitation);
		}
	}

	//Make the op'ee a moderator.
	room->addModerator(opee);

	//Notify the room of the successful op.
	ChatOnAddModeratorToRoom* opNotification = new ChatOnAddModeratorToRoom(oper, opeeName, roomPath, ChatManager::SUCCESS);
	room->broadcastMessage(opNotification);

	if (!room->hasPlayer(oper))
		sendChatOnAddModeratorResult(oper, opeeName, roomPath, ChatManager::SUCCESS, requestID);

}

void ChatManagerImplementation::sendChatOnAddModeratorResult(CreatureObject* oper, const String& opeeName, const String& roomPath, int error, int requestID) {
	ChatOnAddModeratorToRoom* notification = new ChatOnAddModeratorToRoom(oper, opeeName, roomPath, error, requestID);
	oper->sendMessage(notification);
}

void ChatManagerImplementation::handleChatRemoveModerator(CreatureObject* deoper, const String& deopeeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: You deop'd %TT in %TU. (SUCCESS)
	 * 1: Failed to deop [Target] in [RoomPathName]. (FAIL)
	 * 4: Failed to deop [Target] in [RoomPathName]: avatar not found. (NOAVATAR)
	 * 6: Failed to deop [Target] in [RoomPathName]: room does not exist. (NOROOM)
	 * 16: Failed to deop [Target] in [RoomPathName]: You are not a moderator. (NOPERMISSION)
	 */

	if (deoper == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(deoper)) {
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the deopee.
	ManagedReference<CreatureObject*> deopee = playerManager->getPlayer(deopeeName);
	if (deopee == NULL) {
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	} else if (deopee->getObjectID() == room->getOwnerID()) { //Sorry buddy, you cannot deop the channel owner!
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	Locker locker(room);

	//Remove deopee from moderator list if applicable.
	if (room->hasModerator(deopee))
		room->removeModerator(deopee);
	else {
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	//Notify the room of the successful deop.
	ChatOnRemoveModeratorFromRoom* deopNotification = new ChatOnRemoveModeratorFromRoom(deoper, deopeeName, roomPath, ChatManager::SUCCESS);
	room->broadcastMessage(deopNotification);

	if (!room->hasPlayer(deoper))
		sendChatOnRemoveModeratorResult(deoper, deopeeName, roomPath, ChatManager::SUCCESS, requestID);

}

void ChatManagerImplementation::sendChatOnRemoveModeratorResult(CreatureObject* deoper, const String& deopeeName, const String& roomPath, int error, int requestID) {
	ChatOnRemoveModeratorFromRoom* notification = new ChatOnRemoveModeratorFromRoom(deoper, deopeeName, roomPath, error, requestID);
	deoper->sendMessage(notification);
}

void ChatManagerImplementation::handleChatBanPlayer(CreatureObject* banner, const String& baneeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: Success: [Target] has been banned from [RoomName]. (SUCCESS)
	 * 1: Ban [Target] from [RoomName] failed: Unknown Error (FAIL)
	 * 4: Ban [Target] from [RoomName] failed: Avatar not found. (NOAVATAR)
	 * 6: Ban [Target] from [RoomName] failed: Room does not exist. (NOROOM)
	 * 12: Ban [Target] from [RoomName] failed: Already Banned. (INVALIDBANSTATE)
	 * 16: Ban [Target] from [RoomName] failed: Insufficient Privileges (NOPERMISSION)
	 */

	if (banner == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(banner)) {
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the banee.
	ManagedReference<CreatureObject*> banee = playerManager->getPlayer(baneeName);
	if (banee == NULL) {
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	} else if (banee->getObjectID() == room->getOwnerID()) { //Sorry friend, you cannot ban the channel owner!
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	Locker locker(room);

	//Check if already banned.
	if (room->hasBanned(banee)) {
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::INVALIDBANSTATE, requestID);
		return;
	}

	//Remove moderator status if applicable.
	if (room->hasModerator(banee)) {
		room->removeModerator(banee);
		ChatOnRemoveModeratorFromRoom* deopNotification = new ChatOnRemoveModeratorFromRoom(banner, baneeName, roomPath, ChatManager::SUCCESS);
		room->broadcastMessage(deopNotification); //"Moderator status has been revoked from %TU."
	}

	//Remove invite if applicable.
	if (room->isPrivate()) {
		if (room->hasInvited(banee)) {
			room->removeInvited(banee);
			//Update the room list for all players in the room.
			broadcastQueryResultsToRoom(room);
		}
	}

	//Kick player if applicable.
	if (room->hasPlayer(banee)) {
		Locker clocker(banee, room);
		room->removePlayer(banee);

		//Send kick notification to banee.
		StringIdChatParameter notification;
		notification.setStringId("guild", "kick_target"); //"%TU has removed you from %TT."
		notification.setTU(banner->getFirstName());
		notification.setTT(roomPath);
		banee->sendSystemMessage(notification);
	}

	//Ban the banee from the room.
	room->addBanned(banee);

	//Notify the room of the successful ban.
	ChatOnBanFromRoom* banNotification = new ChatOnBanFromRoom(banner, baneeName, roomPath, ChatManager::SUCCESS);
	room->broadcastMessage(banNotification);

	if (!room->hasPlayer(banner))
		sendChatOnBanResult(banner, baneeName, roomPath, ChatManager::SUCCESS, requestID);

	//Remove the room from banee's channel browser.
	if (room->isPrivate())
		room->sendDestroyTo(banee); //Done last in the off chance they banned themself so the error displays the proper room path.

}


void ChatManagerImplementation::sendChatOnBanResult(CreatureObject* banner, const String& baneeName, const String& roomPath, int error, int requestID) {
	ChatOnBanFromRoom* notification = new ChatOnBanFromRoom(banner, baneeName, roomPath, error, requestID);
	banner->sendMessage(notification);
}

void ChatManagerImplementation::handleChatUnbanPlayer(CreatureObject* unbanner, const String& unbaneeName, const String& roomPath, int requestID) {
	//Nothing locked

	/* Error Codes:
	 * 0: Success: [Target] has been unbanned from [RoomName]. (SUCCESS)
	 * 1: Unban [Target] from [RoomName] failed: Unknown Error (FAIL)
	 * 4: Unban [Target] from [RoomName] failed: Avatar not found. (NOAVATAR)
	 * 6: Unban [Target] from [RoomName] failed: Room does not exist. (NOROOM)
	 * 12: Unban [Target] from [RoomName] failed: Not Banned. (INVALIDBANSTATE)
	 * 16: Unban [Target] from [RoomName] failed: Insufficient Privileges (NOPERMISSION)
	 */

	if (unbanner == NULL)
		return;

	//Validate the room.
	ManagedReference<ChatRoom*> room = getChatRoomByFullPath(roomPath);
	if (room == NULL) {
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::NOROOM, requestID);
		return;
	}

	//Check for moderator permission.
	if (!room->hasModerator(unbanner)) {
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::NOPERMISSION, requestID);
		return;
	}

	//Get the unbanee.
	ManagedReference<CreatureObject*> unbanee = playerManager->getPlayer(unbaneeName);
	if (unbanee == NULL) {
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::NOAVATAR, requestID);
		return;
	} else if (unbanee->getObjectID() == room->getOwnerID()) { //Sorry guy, you cannot unban the channel owner!
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::FAIL, requestID);
		return;
	}

	Locker locker(room);

	//Remove unbanee from banned list if applicable.
	if (room->hasBanned(unbanee))
		room->removeBanned(unbanee);
	else {
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::INVALIDBANSTATE, requestID);
		return;
	}

	//Notify the room of the successful unban.
	ChatOnUnbanFromRoom* unbanNotification = new ChatOnUnbanFromRoom(unbanner, unbaneeName, roomPath, ChatManager::SUCCESS);
	room->broadcastMessage(unbanNotification);

	if (!room->hasPlayer(unbanner))
		sendChatOnUnbanResult(unbanner, unbaneeName, roomPath, ChatManager::SUCCESS, requestID);

}

void ChatManagerImplementation::sendChatOnUnbanResult(CreatureObject* unbanner, const String& unbaneeName, const String& roomPath, int error, int requestID) {
	ChatOnUnbanFromRoom* notification = new ChatOnUnbanFromRoom(unbanner, unbaneeName, roomPath, error, requestID);
	unbanner->sendMessage(notification);
}
