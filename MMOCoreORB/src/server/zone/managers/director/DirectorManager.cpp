/*
 * DirectorManager.cpp
 *
 *  Created on: 26/05/2011
 *      Author: victor
 */

#include "DirectorManager.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/objects/creature/LuaCreatureObject.h"
#include "server/zone/objects/scene/LuaSceneObject.h"
#include "server/zone/objects/building/LuaBuildingObject.h"
#include "server/zone/objects/intangible/LuaIntangibleObject.h"
#include "server/zone/objects/intangible/ControlDevice.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/objects/player/LuaPlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/tangible/LuaTangibleObject.h"
#include "server/zone/objects/region/LuaCityRegion.h"
#include "server/zone/packets/cell/UpdateCellPermissionsMessage.h"
#include "server/zone/managers/structure/tasks/DestroyStructureTask.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/zone/managers/faction/FactionManager.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/collision/PathFinderManager.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "templates/manager/TemplateManager.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/managers/name/NameManager.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/director/ScreenPlayObserver.h"
#include "server/zone/managers/director/PersistentEvent.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/player/creation/PlayerCreationManager.h"
#include "server/zone/managers/reaction/ReactionManager.h"
#include "server/ServerCore.h"
#include "server/chat/ChatManager.h"
#include "server/chat/ChatMessage.h"
#include "server/zone/managers/loot/LootManager.h"
#include "templates/params/ObserverEventType.h"
#include "templates/params/creature/CreatureAttribute.h"
#include "templates/params/creature/CreatureState.h"
#include "templates/params/creature/CreaturePosture.h"
#include "server/zone/objects/creature/ai/LuaAiAgent.h"
#include "server/zone/objects/area/LuaActiveArea.h"
#include "server/zone/objects/creature/conversation/ConversationScreen.h"
#include "server/zone/objects/creature/conversation/ConversationTemplate.h"
#include "server/zone/objects/creature/conversation/LuaConversationScreen.h"
#include "server/zone/objects/creature/conversation/LuaConversationTemplate.h"
#include "server/zone/objects/player/sessions/LuaConversationSession.h"
#include "server/zone/objects/tangible/terminal/startinglocation/StartingLocationTerminal.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/managers/sui/LuaSuiManager.h"
#include "server/zone/managers/skill/LuaSkillManager.h"
#include "server/zone/objects/player/sui/LuaSuiBox.h"
#include "server/zone/objects/scene/components/LuaObjectMenuResponse.h"
#include "server/zone/objects/scene/variables/ContainerPermissions.h"
#include "server/zone/objects/tangible/deed/Deed.h"
#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/managers/jedi/JediManager.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/managers/creature/AiMap.h"
#include "server/chat/LuaStringIdChatParameter.h"
#include "server/zone/objects/tangible/ticket/TicketObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"
#include "server/zone/packets/scene/PlayClientEffectLocMessage.h"
#include "server/zone/managers/player/BadgeList.h"
#include "server/zone/managers/player/LuaQuestInfo.h"
#include "server/zone/objects/tangible/misc/FsPuzzlePack.h"
#include "server/zone/objects/tangible/misc/FsCsObject.h"
#include "server/zone/objects/tangible/misc/CustomIngredient.h"
#include "server/zone/objects/tangible/misc/FsCraftingComponentObject.h"
#include "server/zone/objects/tangible/misc/FsBuffItem.h"
#include "server/zone/objects/player/sui/LuaSuiPageData.h"
#include "server/zone/objects/player/sui/SuiBoxPage.h"
#include "server/zone/objects/tangible/powerup/PowerupObject.h"
#include "server/zone/objects/resource/ResourceSpawn.h"
#include "server/zone/objects/tangible/component/Component.h"
#include "server/zone/objects/pathfinding/NavArea.h"
#include "server/zone/objects/player/sui/listbox/LuaSuiListBox.h"
#include "server/zone/objects/tangible/component/lightsaber/LightsaberCrystalComponent.h"
#include "server/zone/objects/creature/variables/LuaSkill.h"
#include "server/zone/objects/intangible/TheaterObject.h"
#include "server/zone/objects/tangible/misc/ContractCrate.h"
#include "server/zone/managers/crafting/schematicmap/SchematicMap.h"

int DirectorManager::DEBUG_MODE = 0;
int DirectorManager::ERROR_CODE = NO_ERROR;

SynchronizedHashTable<uint32, Reference<PersistentEvent*> > DirectorManager::persistentEvents;

DirectorManager::DirectorManager() : Logger("DirectorManager") {
	if (!DEBUG_MODE)
		info("loading..", true);

	sharedMemory = new DirectorSharedMemory();
	sharedMemory->setNullValue(0);

	screenPlays.setNullValue(false);
	screenPlays.setNoDuplicateInsertPlan();

	questStatuses.setNullValue(nullptr);
	questStatuses.setNoDuplicateInsertPlan();

	questVectorMaps.setNullValue(nullptr);
	questVectorMaps.setNoDuplicateInsertPlan();

	masterScreenPlayVersion.set(0);
}

DirectorManager::~DirectorManager() {
}

void DirectorManager::loadPersistentEvents() {
	info("Loading persistent events from events.db");

	ObjectDatabaseManager* dbManager = ObjectDatabaseManager::instance();
	ObjectDatabase* eventDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("events", true);

	if (eventDatabase == nullptr) {
		error("Could not load the event database.");
		return;
	}

	int i = 0;

	try {
		ObjectDatabaseIterator iterator(eventDatabase);

		uint64 objectID;

		while (iterator.getNextKey(objectID)) {
			Reference<PersistentEvent*> event = Core::getObjectBroker()->lookUp(objectID).castTo<PersistentEvent*>();
			Locker locker(event);

			++i;

			Reference<PersistentEvent*> oldEvent = persistentEvents.put(event->getEventName().hashCode(), event);

			if (event != nullptr && oldEvent != nullptr) {
				error("duplicate persistent event " + event->getEventName() + " loading from database!");
			} else if (event != nullptr) {
				event->loadTransientTask();
			} else {
				error("could not load PersistentEvent!");
			}

			if (ConfigManager::instance()->isProgressMonitorActivated())
				printf("\r\tLoading persistent events [%d] / [?]\t", i);
		}
	} catch (DatabaseException& e) {
		error("Database exception in DirectorManager::loadPersistentEvents(): "	+ e.getMessage());
	}

	info(true) << i << " persistent events loaded.";
}

void DirectorManager::loadPersistentStatus() {
	info("Loading persistent quest status from questdata.db");

	ObjectDatabaseManager* dbManager = ObjectDatabaseManager::instance();
	ObjectDatabase* statusDatabase = dbManager->loadObjectDatabase("questdata", true);

	if (statusDatabase == nullptr) {
		error("Could not load the questdata database.");
		return;
	}

	try {
		ObjectDatabaseIterator iterator(statusDatabase);

		uint64 objectID;

		while (iterator.getNextKey(objectID)) {
			Reference<QuestStatus*> status = Core::getObjectBroker()->lookUp(objectID).castTo<QuestStatus*>();

			if (status != nullptr) {
				questStatuses.put(status->getKey(), status);
				continue;
			}

			Reference<QuestVectorMap*> questMap = Core::getObjectBroker()->lookUp(objectID).castTo<QuestVectorMap*>();

			if (questMap != nullptr)
				questVectorMaps.put(questMap->getKey(), questMap);
		}
	} catch (DatabaseException& e) {
		error("Database exception in DirectorManager::loadPersistentStatus(): "	+ e.getMessage());
	}

	info(true) << questStatuses.size() + questVectorMaps.size() << " persistent statuses loaded.";
}

void DirectorManager::setQuestStatus(const String& keyString, const String& valString) {
	ManagedReference<QuestStatus*> status = questStatuses.get(keyString);

	if (status == nullptr) {
		status = new QuestStatus();
		status->setKey(keyString);
		questStatuses.put(keyString, status);

		ObjectManager::instance()->persistObject(status, 1, "questdata");
	}

	status->setStatus(valString);
}

String DirectorManager::getQuestStatus(const String& keyString) const {
	String str = "";

	Reference<QuestStatus*> status = questStatuses.get(keyString);
	if (status != nullptr)
		str = status->getStatus();

	return str;
}

void DirectorManager::removeQuestStatus(const String& key) {
	ManagedReference<QuestStatus*> status = nullptr;

	status = questStatuses.get(key);

	questStatuses.drop(key);

	if (status != nullptr)
		ObjectManager::instance()->destroyObjectFromDatabase(status->_getObjectID());
}

String DirectorManager::readStringSharedMemory(const String& key) {
#ifndef WITH_STM
	DirectorManager::instance()->rlock();
#endif

	String data = DirectorManager::instance()->sharedMemory->getString(key);

#ifndef WITH_STM
	DirectorManager::instance()->runlock();
#endif

	return data;
}

uint64 DirectorManager::readSharedMemory(const String& key) {
#ifndef WITH_STM
	DirectorManager::instance()->rlock();
#endif

	uint64 data = DirectorManager::instance()->sharedMemory->get(key);

#ifndef WITH_STM
	DirectorManager::instance()->runlock();
#endif

	return data;
}

Vector<Reference<ScreenPlayTask*> > DirectorManager::getObjectEvents(SceneObject* obj) const {
	Vector<Reference<ScreenPlayTask*> > eventList;

	for (int i = 0; i < screenplayTasks.size(); i++) {
		Reference<ScreenPlayTask*> task = screenplayTasks.get(i);

		if (task->getSceneObject() == obj) {
			eventList.emplace(std::move(task));
		}
	}

	return eventList;
}

void DirectorManager::printTraceError(lua_State* L, const String& error) {
	Lua* lua = instance()->getLuaInstance();
	luaL_traceback(L, L, error.toCharArray(), 0);
	String trace = lua_tostring(L, -1);
	lua->error(trace);
}

String DirectorManager::getStringSharedMemory(const String& key) const {
	return sharedMemory->getString(key);
}

void DirectorManager::startGlobalScreenPlays() {
	info("Starting global screenplays.", true);

	for (int i = 0; i < screenPlays.size(); ++i) {
		String screenPlay = screenPlays.elementAt(i).getKey();
		bool start = screenPlays.elementAt(i).getValue();

		if (start)
			startScreenPlay(nullptr, screenPlay);
	}
}

void DirectorManager::setupLuaPackagePath(Lua* luaEngine) {
	lua_State* L = luaEngine->getLuaState();
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	String currentPath = lua_tostring(L, -1);
	currentPath = currentPath.concat(";scripts/?.lua");
	lua_pop(L, 1);
	lua_pushstring(L, currentPath.toCharArray());
	lua_setfield(L, -2, "path");
	lua_pop(L, 1);
}

void DirectorManager::initializeLuaEngine(Lua* luaEngine) {
	if (DEBUG_MODE)
		setLogLevel(Logger::DEBUG);

	debug("initializeLuaEngine started");

	luaEngine->init();
	luaEngine->setLoggingName("DirectorManagerLuaInstance");
	luaEngine->setGlobalLogging(true);

	if (DEBUG_MODE) {
		luaEngine->setLogLevel(Logger::DEBUG);
	} else {
		luaEngine->setLogLevel(Logger::INFO);
	}

	luaEngine->setFileLogger("log/lua.log", true, ConfigManager::instance()->getRotateLogAtStart());
	luaEngine->setLogJSON(ConfigManager::instance()->getLuaLogJSON());
	luaEngine->setRotateLogSizeMB(ConfigManager::instance()->getRotateLogSizeMB());

	if (luaEngine->getLogJSON()) {
		luaEngine->setLogSynchronized(true);
	}

	setupLuaPackagePath(luaEngine);

	//luaEngine->registerFunction("includeFile", includeFile);
	luaEngine->registerFunction("includeFile", includeFile);
	luaEngine->registerFunction("createEvent", createEvent);
	luaEngine->registerFunction("createEventActualTime", createEventActualTime);
	luaEngine->registerFunction("createServerEvent", createServerEvent);
	luaEngine->registerFunction("hasServerEvent", hasServerEvent);
	luaEngine->registerFunction("rescheduleServerEvent", rescheduleServerEvent);
	luaEngine->registerFunction("getServerEventID", getServerEventID);
	luaEngine->registerFunction("getServerEventTimeLeft", getServerEventTimeLeft);
	luaEngine->registerFunction("createObserver", createObserver);
	luaEngine->registerFunction("dropObserver", dropObserver);
	luaEngine->registerFunction("hasObserver", hasObserver);
	luaEngine->registerFunction("spawnMobile", spawnMobile);
	luaEngine->registerFunction("spawnEventMobile", spawnEventMobile);
	luaEngine->registerFunction("spatialChat", spatialChat);
	luaEngine->registerFunction("spatialMoodChat", spatialMoodChat);
	luaEngine->registerFunction("getRandomNumber", getRandomNumber);
	luaEngine->registerFunction("forcePeace", forcePeace);
	luaEngine->registerFunction("readSharedMemory", readSharedMemory);
	luaEngine->registerFunction("writeSharedMemory", writeSharedMemory);
	luaEngine->registerFunction("deleteSharedMemory", deleteSharedMemory);
	luaEngine->registerFunction("readStringSharedMemory", readStringSharedMemory);
	luaEngine->registerFunction("writeStringSharedMemory", writeStringSharedMemory);
	luaEngine->registerFunction("deleteStringSharedMemory", deleteStringSharedMemory);
	luaEngine->registerFunction("spawnSceneObject", spawnSceneObject);
	luaEngine->registerFunction("spawnActiveArea", spawnActiveArea);
	luaEngine->registerFunction("spawnBuilding", spawnBuilding);
	luaEngine->registerFunction("destroyBuilding", destroyBuilding);
	luaEngine->registerFunction("getSceneObject", getSceneObject);
	luaEngine->registerFunction("getCreatureObject", getCreatureObject);
	luaEngine->registerFunction("addStartingItemsInto", addStartingItemsInto);
	luaEngine->registerFunction("addStartingWeaponsInto", addStartingWeaponsInto);
	luaEngine->registerFunction("setAuthorizationState", setAuthorizationState);
	luaEngine->registerFunction("giveItem", giveItem);
	luaEngine->registerFunction("giveControlDevice", giveControlDevice);
	luaEngine->registerFunction("checkTooManyHirelings", checkTooManyHirelings);
	luaEngine->registerFunction("checkInt64Lua", checkInt64Lua);
	luaEngine->registerFunction("getChatMessage", getChatMessage);
	luaEngine->registerFunction("getStringId", getStringId);
	luaEngine->registerFunction("getRankName", getRankName);
	luaEngine->registerFunction("getRankCost", getRankCost);
	luaEngine->registerFunction("getRankDelegateRatioFrom", getRankDelegateRatioFrom);
	luaEngine->registerFunction("getRankDelegateRatioTo", getRankDelegateRatioTo);
	luaEngine->registerFunction("isHighestRank", isHighestRank);
	luaEngine->registerFunction("getFactionPointsCap", getFactionPointsCap);
	luaEngine->registerFunction("registerScreenPlay", registerScreenPlay);
	luaEngine->registerFunction("getZoneByName", getZoneByName);
	luaEngine->registerFunction("isZoneEnabled", isZoneEnabled);
	luaEngine->registerFunction("getContainerObjectByTemplate", getContainerObjectByTemplate);
	luaEngine->registerFunction("updateCellPermission", updateCellPermission);
	luaEngine->registerFunction("updateCellPermissionGroup", updateCellPermissionGroup);
	luaEngine->registerFunction("getQuestInfo", getQuestInfo);
	luaEngine->registerFunction("getPlayerQuestID", getPlayerQuestID);

	// call for createLoot(SceneObject* container, const String& lootGroup, int level)
	luaEngine->registerFunction("createLoot", createLoot);
	luaEngine->registerFunction("createLootSet", createLootSet);
	luaEngine->registerFunction("createLootFromCollection", createLootFromCollection);

	luaEngine->registerFunction("getRegion", getRegion);
	luaEngine->registerFunction("writeScreenPlayData", writeScreenPlayData);
	luaEngine->registerFunction("readScreenPlayData", readScreenPlayData);
	luaEngine->registerFunction("deleteScreenPlayData", deleteScreenPlayData);
	luaEngine->registerFunction("clearScreenPlayData", clearScreenPlayData);
	luaEngine->registerFunction("getObjectTemplatePathByCRC", getObjectTemplatePathByCRC);
	luaEngine->registerFunction("getTimestamp", getTimestamp);
	luaEngine->registerFunction("getTimestampMilli", getTimestampMilli);
	luaEngine->registerFunction("getFormattedTime", getFormattedTime);
	luaEngine->registerFunction("getSpawnPoint", getSpawnPoint);
	luaEngine->registerFunction("getSpawnPointInArea", getSpawnPointInArea);
	luaEngine->registerFunction("getSpawnArea", getSpawnArea);
	luaEngine->registerFunction("makeCreatureName", makeCreatureName);
	luaEngine->registerFunction("getGCWDiscount", getGCWDiscount);
	luaEngine->registerFunction("getTerrainHeight", getTerrainHeight);
	luaEngine->registerFunction("awardSkill", awardSkill);
	luaEngine->registerFunction("getCityRegionAt", getCityRegionAt);
	luaEngine->registerFunction("setDungeonTicketAttributes", setDungeonTicketAttributes);
	luaEngine->registerFunction("setQuestStatus", setQuestStatus);
	luaEngine->registerFunction("getQuestStatus", getQuestStatus);
	luaEngine->registerFunction("removeQuestStatus", removeQuestStatus);
	luaEngine->registerFunction("getControllingFaction", getControllingFaction);
	luaEngine->registerFunction("getImperialScore", getImperialScore);
	luaEngine->registerFunction("getRebelScore", getRebelScore);
	luaEngine->registerFunction("getWinningFactionDifficultyScaling", getWinningFactionDifficultyScaling);
	luaEngine->registerFunction("playClientEffectLoc", playClientEffectLoc);
	luaEngine->registerFunction("getQuestVectorMap", getQuestVectorMap);
	luaEngine->registerFunction("createQuestVectorMap", createQuestVectorMap);
	luaEngine->registerFunction("removeQuestVectorMap", removeQuestVectorMap);
	luaEngine->registerFunction("creatureTemplateExists", creatureTemplateExists);
	luaEngine->registerFunction("printLuaError", printLuaError);
	luaEngine->registerFunction("getPlayerByName", getPlayerByName);
	luaEngine->registerFunction("sendMail", sendMail);
	luaEngine->registerFunction("spawnTheaterObject", spawnTheaterObject);
	luaEngine->registerFunction("getSchematicItemName", getSchematicItemName);
	luaEngine->registerFunction("getBadgeListByType", getBadgeListByType);

	//Navigation Mesh Management
	luaEngine->registerFunction("createNavMesh", createNavMesh);

	luaEngine->setGlobalInt("POSITIONCHANGED", ObserverEventType::POSITIONCHANGED);
	luaEngine->setGlobalInt("CLOSECONTAINER", ObserverEventType::CLOSECONTAINER);
	luaEngine->setGlobalInt("OBJECTDESTRUCTION", ObserverEventType::OBJECTDESTRUCTION);
	luaEngine->setGlobalInt("OBJECTDISABLED", ObserverEventType::OBJECTDISABLED);
	luaEngine->setGlobalInt("SAMPLE", ObserverEventType::SAMPLE);
	luaEngine->setGlobalInt("CONVERSE", ObserverEventType::CONVERSE);
	luaEngine->setGlobalInt("KILLEDCREATURE", ObserverEventType::KILLEDCREATURE);
	luaEngine->setGlobalInt("OBJECTREMOVEDFROMZONE", ObserverEventType::OBJECTREMOVEDFROMZONE);
	luaEngine->setGlobalInt("ENTEREDAREA", ObserverEventType::ENTEREDAREA);
	luaEngine->setGlobalInt("EXITEDAREA", ObserverEventType::EXITEDAREA);
	luaEngine->setGlobalInt("DESTINATIONREACHED", ObserverEventType::DESTINATIONREACHED);
	luaEngine->setGlobalInt("SPECIALATTACK", ObserverEventType::SPECIALATTACK);
	luaEngine->setGlobalInt("CALLFORHELP", ObserverEventType::CALLFORHELP);
	luaEngine->setGlobalInt("NEWBIETUTORIALZOOMCAMERA", ObserverEventType::NEWBIETUTORIALZOOMCAMERA);
	luaEngine->setGlobalInt("CHAT", ObserverEventType::CHAT);
	luaEngine->setGlobalInt("NEWBIETUTORIALHOLOCRON", ObserverEventType::NEWBIETUTORIALHOLOCRON);
	luaEngine->setGlobalInt("OBJECTINRANGEMOVED", ObserverEventType::OBJECTINRANGEMOVED);
	luaEngine->setGlobalInt("PLAYERCHANGEDTARGET", ObserverEventType::PLAYERCHANGEDTARGET);
	luaEngine->setGlobalInt("STARTCONVERSATION", ObserverEventType::STARTCONVERSATION);
	luaEngine->setGlobalInt("SELECTCONVERSATION", ObserverEventType::SELECTCONVERSATION);
	luaEngine->setGlobalInt("STOPCONVERSATION", ObserverEventType::STOPCONVERSATION);
	luaEngine->setGlobalInt("OPENCONTAINER", ObserverEventType::OPENCONTAINER);
	luaEngine->setGlobalInt("NEWBIEOPENINVENTORY", ObserverEventType::NEWBIEOPENINVENTORY);
	luaEngine->setGlobalInt("NEWBIECLOSEINVENTORY", ObserverEventType::NEWBIECLOSEINVENTORY);
	luaEngine->setGlobalInt("OBJECTRADIALUSED", ObserverEventType::OBJECTRADIALUSED);
	luaEngine->setGlobalInt("DAMAGERECEIVED", ObserverEventType::DAMAGERECEIVED);
	luaEngine->setGlobalInt("OBJECTNAMECHANGED", ObserverEventType::OBJECTNAMECHANGED);
	luaEngine->setGlobalInt("SURVEY", ObserverEventType::SURVEY);
	luaEngine->setGlobalInt("GETATTRIBUTESBATCHCOMMAND", ObserverEventType::GETATTRIBUTESBATCHCOMMAND);
	luaEngine->setGlobalInt("HEALINGRECEIVED", ObserverEventType::HEALINGRECEIVED);
	luaEngine->setGlobalInt("STARTCOMBAT", ObserverEventType::STARTCOMBAT);
	luaEngine->setGlobalInt("DEFENDERADDED", ObserverEventType::DEFENDERADDED);
	luaEngine->setGlobalInt("DEFENDERDROPPED", ObserverEventType::DEFENDERDROPPED);
	luaEngine->setGlobalInt("OBJECTRADIALOPENED", ObserverEventType::OBJECTRADIALOPENED);
	luaEngine->setGlobalInt("ENTEREDBUILDING", ObserverEventType::ENTEREDBUILDING);
	luaEngine->setGlobalInt("EXITEDBUILDING", ObserverEventType::EXITEDBUILDING);
	luaEngine->setGlobalInt("SPATIALCHATSENT", ObserverEventType::SPATIALCHATSENT);
	luaEngine->setGlobalInt("ITEMLOOTED", ObserverEventType::ITEMLOOTED);
	luaEngine->setGlobalInt("MEDPACKUSED", ObserverEventType::MEDPACKUSED);
	luaEngine->setGlobalInt("BADGEAWARDED", ObserverEventType::BADGEAWARDED);
	luaEngine->setGlobalInt("FACTIONBASEFLIPPED", ObserverEventType::FACTIONBASEFLIPPED);
	luaEngine->setGlobalInt("LOOTCREATURE", ObserverEventType::LOOTCREATURE);
	luaEngine->setGlobalInt("SCREENPLAYSTATECHANGED", ObserverEventType::SCREENPLAYSTATECHANGED);
	luaEngine->setGlobalInt("CREATUREDESPAWNED", ObserverEventType::CREATUREDESPAWNED);
	luaEngine->setGlobalInt("AIMESSAGE", ObserverEventType::AIMESSAGE);
	luaEngine->setGlobalInt("STARTENTERTAIN", ObserverEventType::STARTENTERTAIN);
	luaEngine->setGlobalInt("CHANGEENTERTAIN", ObserverEventType::CHANGEENTERTAIN);
	luaEngine->setGlobalInt("STOPENTERTAIN", ObserverEventType::STOPENTERTAIN);
	luaEngine->setGlobalInt("FLOURISH", ObserverEventType::FLOURISH);
	luaEngine->setGlobalInt("CONTAINERCONTENTSCHANGED", ObserverEventType::CONTAINERCONTENTSCHANGED);
	luaEngine->setGlobalInt("WASLISTENEDTO", ObserverEventType::WASLISTENEDTO);
	luaEngine->setGlobalInt("WASWATCHED", ObserverEventType::WASWATCHED);
	luaEngine->setGlobalInt("PARENTCHANGED", ObserverEventType::PARENTCHANGED);
	luaEngine->setGlobalInt("LOGGEDIN", ObserverEventType::LOGGEDIN);
	luaEngine->setGlobalInt("LOGGEDOUT", ObserverEventType::LOGGEDOUT);
	luaEngine->setGlobalInt("ZONESWITCHED", ObserverEventType::ZONESWITCHED);
	luaEngine->setGlobalInt("TUNEDCRYSTAL", ObserverEventType::TUNEDCRYSTAL);
	luaEngine->setGlobalInt("PROTOTYPECREATED", ObserverEventType::PROTOTYPECREATED);
	luaEngine->setGlobalInt("SLICED", ObserverEventType::SLICED);

	luaEngine->setGlobalInt("UPRIGHT", CreaturePosture::UPRIGHT);
	luaEngine->setGlobalInt("PRONE", CreaturePosture::PRONE);
	luaEngine->setGlobalInt("POSTURESITTING", CreaturePosture::SITTING);
	luaEngine->setGlobalInt("KNOCKEDDOWN", CreaturePosture::KNOCKEDDOWN);
	luaEngine->setGlobalInt("CROUCHED", CreaturePosture::CROUCHED);
	luaEngine->setGlobalInt("STATESITTINGONCHAIR", CreatureState::SITTINGONCHAIR);

	//Waypoint Colors
	luaEngine->setGlobalInt("WAYPOINTBLUE", WaypointObject::COLOR_BLUE);
	luaEngine->setGlobalInt("WAYPOINTGREEN", WaypointObject::COLOR_GREEN);
	luaEngine->setGlobalInt("WAYPOINTYELLOW", WaypointObject::COLOR_YELLOW);
	luaEngine->setGlobalInt("WAYPOINTPURPLE", WaypointObject::COLOR_PURPLE);
	luaEngine->setGlobalInt("WAYPOINTWHITE", WaypointObject::COLOR_WHITE);
	luaEngine->setGlobalInt("WAYPOINTORANGE", WaypointObject::COLOR_ORANGE);

	//Waypoint Special Types
	luaEngine->setGlobalInt("WAYPOINTTHEMEPARK", WaypointObject::SPECIALTYPE_THEMEPARK);
	luaEngine->setGlobalInt("WAYPOINTRACETRACK", WaypointObject::SPECIALTYPE_RACETRACK);
	luaEngine->setGlobalInt("WAYPOINTTREASUREMAP", WaypointObject::SPECIALTYPE_TREASUREMAP);
	luaEngine->setGlobalInt("WAYPOINTQUESTTASK", WaypointObject::SPECIALTYPE_QUESTTASK);

	luaEngine->setGlobalInt("HEALTH", CreatureAttribute::HEALTH);
	luaEngine->setGlobalInt("CONSTITUTION", CreatureAttribute::CONSTITUTION);
	luaEngine->setGlobalInt("STAMINA", CreatureAttribute::STAMINA);
	luaEngine->setGlobalInt("ACTION", CreatureAttribute::ACTION);
	luaEngine->setGlobalInt("STRENGTH", CreatureAttribute::STRENGTH);
	luaEngine->setGlobalInt("QUICKNESS", CreatureAttribute::QUICKNESS);
	luaEngine->setGlobalInt("MIND", CreatureAttribute::MIND);
	luaEngine->setGlobalInt("FOCUS", CreatureAttribute::FOCUS);
	luaEngine->setGlobalInt("WILLPOWER", CreatureAttribute::WILLPOWER);

	luaEngine->setGlobalInt("POISONED", CreatureState::POISONED);
	luaEngine->setGlobalInt("DISEASED", CreatureState::DISEASED);
	luaEngine->setGlobalInt("ONFIRE", CreatureState::ONFIRE);
	luaEngine->setGlobalInt("BLEEDING", CreatureState::BLEEDING);
	luaEngine->setGlobalInt("PEACE", CreatureState::PEACE);

	luaEngine->setGlobalInt("NONE", CreatureFlag::NONE);
	luaEngine->setGlobalInt("ATTACKABLE", CreatureFlag::ATTACKABLE);
	luaEngine->setGlobalInt("AGGRESSIVE", CreatureFlag::AGGRESSIVE);
	luaEngine->setGlobalInt("OVERT", CreatureFlag::OVERT);
	luaEngine->setGlobalInt("TEF", CreatureFlag::TEF);
	luaEngine->setGlobalInt("PLAYER", CreatureFlag::PLAYER);
	luaEngine->setGlobalInt("ENEMY", CreatureFlag::ENEMY);
	luaEngine->setGlobalInt("WILLBEDECLARED", CreatureFlag::WILLBEDECLARED);
	luaEngine->setGlobalInt("WASDECLARED", CreatureFlag::WASDECLARED);
	luaEngine->setGlobalInt("SCANNING_FOR_CONTRABAND", CreatureFlag::SCANNING_FOR_CONTRABAND);
	luaEngine->setGlobalInt("IGNORE_FACTION_STANDING", CreatureFlag::IGNORE_FACTION_STANDING);

	luaEngine->setGlobalInt("CONVERSABLE", OptionBitmask::CONVERSE);
	luaEngine->setGlobalInt("AIENABLED", OptionBitmask::AIENABLED);
	luaEngine->setGlobalInt("INVULNERABLE", OptionBitmask::INVULNERABLE);
	luaEngine->setGlobalInt("FACTIONAGGRO", OptionBitmask::FACTIONAGGRO);
	luaEngine->setGlobalInt("INTERESTING", OptionBitmask::INTERESTING);
	luaEngine->setGlobalInt("JTLINTERESTING", OptionBitmask::JTLINTERESTING);

	luaEngine->setGlobalInt("OPEN", ContainerPermissions::OPEN);
	luaEngine->setGlobalInt("MOVEIN", ContainerPermissions::MOVEIN);
	luaEngine->setGlobalInt("MOVEOUT", ContainerPermissions::MOVEOUT);
	luaEngine->setGlobalInt("WALKIN", ContainerPermissions::WALKIN);
	luaEngine->setGlobalInt("MOVECONTAINER", ContainerPermissions::MOVECONTAINER);

	// Transfer Error Codes
	luaEngine->setGlobalInt("TRANSFERCANADD", TransferErrorCode::SUCCESS);
	luaEngine->setGlobalInt("TRANSFERCANTADD", TransferErrorCode::CANTADD);
	luaEngine->setGlobalInt("TRANSFERCANTREMOVE", TransferErrorCode::CANTREMOVE);
	luaEngine->setGlobalInt("TRANSFERSUCCESS", 1);
	luaEngine->setGlobalInt("TRANSFERFAIL", 0);

	// NPC reaction chat types / states
	luaEngine->setGlobalInt("REACTION_HI", ReactionManager::HI);
	luaEngine->setGlobalInt("REACTION_BYE", ReactionManager::BYE);
	luaEngine->setGlobalInt("REACTION_NICE", ReactionManager::NICE);
	luaEngine->setGlobalInt("REACTION_MID", ReactionManager::MID);
	luaEngine->setGlobalInt("REACTION_MEAN", ReactionManager::MEAN);

	luaEngine->setGlobalLong("FACTIONNEUTRAL", Factions::FACTIONNEUTRAL);
	luaEngine->setGlobalLong("FACTIONIMPERIAL", Factions::FACTIONIMPERIAL);
	luaEngine->setGlobalLong("FACTIONREBEL", Factions::FACTIONREBEL);

	// Badges
	const auto badges = BadgeList::instance()->getMap();
	for (const auto& entry : *badges) {
		const auto badge = entry.getValue();

		const String val = badge->getKey().toUpperCase();

		luaEngine->setGlobalInt(val, badge->getIndex());
	}

	// SUI Window Types (WIP)
	luaEngine->setGlobalInt("NEWSNET_INFO", SuiWindowType::NEWSNET_INFO);

	Luna<LuaCellObject>::Register(luaEngine->getLuaState());
	Luna<LuaBuildingObject>::Register(luaEngine->getLuaState());
	Luna<LuaCreatureObject>::Register(luaEngine->getLuaState());
	Luna<LuaSceneObject>::Register(luaEngine->getLuaState());
	Luna<LuaConversationScreen>::Register(luaEngine->getLuaState());
	Luna<LuaConversationSession>::Register(luaEngine->getLuaState());
	Luna<LuaConversationTemplate>::Register(luaEngine->getLuaState());
	Luna<LuaIntangibleObject>::Register(luaEngine->getLuaState());
	Luna<LuaControlDevice>::Register(luaEngine->getLuaState());
	Luna<LuaPlayerObject>::Register(luaEngine->getLuaState());
	Luna<LuaAiAgent>::Register(luaEngine->getLuaState());
	Luna<LuaActiveArea>::Register(luaEngine->getLuaState());
	Luna<LuaTangibleObject>::Register(luaEngine->getLuaState());
	Luna<LuaSuiManager>::Register(luaEngine->getLuaState());
	Luna<LuaSuiBox>::Register(luaEngine->getLuaState());
	Luna<LuaObjectMenuResponse>::Register(luaEngine->getLuaState());
	Luna<LuaDeed>::Register(luaEngine->getLuaState());
	Luna<LuaCityRegion>::Register(luaEngine->getLuaState());
	Luna<LuaStringIdChatParameter>::Register(luaEngine->getLuaState());
	Luna<LuaTicketObject>::Register(luaEngine->getLuaState());
	Luna<LuaQuestInfo>::Register(luaEngine->getLuaState());
	Luna<LuaFsPuzzlePack>::Register(luaEngine->getLuaState());
	Luna<LuaFsCsObject>::Register(luaEngine->getLuaState());
	Luna<LuaFsBuffItem>::Register(luaEngine->getLuaState());
	Luna<LuaResourceSpawn>::Register(luaEngine->getLuaState());
	Luna<LuaCustomIngredient>::Register(luaEngine->getLuaState());
	Luna<LuaFsCraftingComponentObject>::Register(luaEngine->getLuaState());
	Luna<LuaSuiPageData>::Register(luaEngine->getLuaState());
	Luna<LuaQuestVectorMap>::Register(luaEngine->getLuaState());
	Luna<LuaSuiBoxPage>::Register(luaEngine->getLuaState());
	Luna<LuaPowerupObject>::Register(luaEngine->getLuaState());
	Luna<LuaWaypointObject>::Register(luaEngine->getLuaState());
	Luna<LuaComponent>::Register(luaEngine->getLuaState());
	Luna<LuaSuiListBox>::Register(luaEngine->getLuaState());
	Luna<LuaLightsaberCrystalComponent>::Register(luaEngine->getLuaState());
	Luna<LuaSkill>::Register(luaEngine->getLuaState());
	Luna<LuaSkillManager>::Register(luaEngine->getLuaState());
	Luna<LuaContractCrate>::Register(luaEngine->getLuaState());
}

int DirectorManager::loadScreenPlays(Lua* luaEngine) {
	Timer loadTimer;
	loadTimer.start();

	bool res = luaEngine->runFile("scripts/screenplays/screenplays.lua");

	if (!DEBUG_MODE) {
		auto elapsed = loadTimer.stopMs();

		info() << Thread::getCurrentThread()->getName()
			<< " loaded "
			<< instance()->screenPlays.size()
			<< " screenplays in "
			<< elapsed
			<< " ms.";
	}

	if (!res)
		return 1;

	return 0;
}

void DirectorManager::reloadScreenPlays() {
	masterScreenPlayVersion.increment();
}

int DirectorManager::writeScreenPlayData(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		printTraceError(L, "incorrect number of arguments passed to DirectorManager::writeScreenPlayData");

		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String data = lua_tostring(L, -1);
	String variable = lua_tostring(L, -2);
	String screenPlay = lua_tostring(L, -3);

	SceneObject* player = (SceneObject*) lua_touserdata(L, -4);

	if (player == nullptr || !player->isPlayerCreature()) {
		String err = "Attempted to write screen play data to a non-player Scene Object using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);
		return 0;
	}

	Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost == nullptr) {
		String err = "Attempted to write screen play data for a null ghost using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);
		return 0;
	}

	ghost->setScreenPlayData(screenPlay, variable, data);

	return 0;
}

int DirectorManager::createLoot(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createLoot";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* container = (SceneObject*)lua_touserdata(L, -4);
	String lootGroup = lua_tostring(L, -3);
	int level = lua_tonumber(L, -2);
	bool maxCondition = lua_toboolean(L, -1);

	if (container == nullptr || lootGroup == "")
		return 0;

	LootManager* lootManager = ServerCore::getZoneServer()->getLootManager();
	TransactionLog trx(TrxCode::LUASCRIPT, container);
	trx.addContextFromLua(L);
	if (lootManager->createLoot(trx,container, lootGroup, level, maxCondition)) {
		trx.commit(true);
	} else {
		trx.abort() << __FUNCTION__ << " failed: lootGroup=" << lootGroup << "; level=" << level << "; maxCondition=" << maxCondition;
	}

	return 0;
}

int DirectorManager::createLootSet(lua_State* L) {
	if (checkArgumentCount(L,5) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createLootSet";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* container = (SceneObject*)lua_touserdata(L, -5);
	String lootGroup = lua_tostring(L, -4);
	int level = lua_tonumber(L, -3);
	bool maxCondition = lua_toboolean(L, -2);
	int setSize = lua_tonumber(L, -1);

	if (container == nullptr || lootGroup == "")
		return 0;

	LootManager* lootManager = ServerCore::getZoneServer()->getLootManager();
	TransactionLog trx(TrxCode::LUASCRIPT, container);
	trx.addContextFromLua(L);
	if (lootManager->createLootSet(trx, container, lootGroup, level, maxCondition, setSize)) {
		trx.commit(true);
	} else {
		trx.abort() << __FUNCTION__ << " failed: lootGroup=" << lootGroup << "; level=" << level << "; maxCondition=" << maxCondition << "; setSize=" << setSize;
	}

	return 0;
}

int DirectorManager::createLootFromCollection(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createLootFromCollection";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* container = (SceneObject*)lua_touserdata(L, -3);

	if (container == nullptr)
		return 0;

	int level = lua_tonumber(L, -1);
	lua_pop(L, 1);

	LuaObject luaObject(L);

	LootGroupCollection lootCollection;
	lootCollection.readObject(&luaObject, level);

	luaObject.pop();

	LootManager* lootManager = ServerCore::getZoneServer()->getLootManager();
	TransactionLog trx(TrxCode::LUASCRIPT, container);
	trx.addContextFromLua(L);
	if (lootManager->createLootFromCollection(trx, container, &lootCollection, level)) {
		trx.commit(true);
	} else {
		trx.abort() << __FUNCTION__ << " failed: level=" << level;
	}

	return 0;
}

int DirectorManager::getTimestamp(lua_State* L) {
	Time now;
	lua_pushinteger(L, now.getTime());

	return 1;
}

int DirectorManager::getTimestampMilli(lua_State* L) {
	Time now;
	lua_pushinteger(L, now.getMiliTime());

	return 1;
}

int DirectorManager::getFormattedTime(lua_State* L) {
	Time now;
	lua_pushstring(L, now.getFormattedTime().toCharArray());

	return 1;
}

int DirectorManager::readScreenPlayData(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::readScreenPlayData";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String variable = lua_tostring(L, -1);
	String screenPlay = lua_tostring(L, -2);
	SceneObject* player = (SceneObject*) lua_touserdata(L, -3);

	if (player == nullptr || !player->isPlayerCreature()) {
		String err = "Attempted to read screen play data from a non-player Scene Object using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);

		lua_pushstring(L, "");

		return 1;
	}

	Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost == nullptr) {
		String err = "Attempted to read screen play data for a null ghost using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);

		lua_pushstring(L, "");

		return 1;
	}

	//readScreenPlayData(player, screenPlay, variable)

	lua_pushstring(L, ghost->getScreenPlayData(screenPlay, variable).toCharArray());

	return 1;
}

int DirectorManager::deleteScreenPlayData(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::deleteScreenPlayData";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String variable = lua_tostring(L, -1);
	String screenPlay = lua_tostring(L, -2);
	SceneObject* player = (SceneObject*) lua_touserdata(L, -3);

	if (player == nullptr || !player->isPlayerCreature()) {
		String err = "Attempted to delete screen play data for a non-player Scene Object using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);
		return 0;
	}

	Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost == nullptr) {
		String err = "Attempted to delete screen play data for a null ghost using screenplay " + screenPlay + " and variable " + variable;
		printTraceError(L, err);
		return 0;
	}

	ghost->deleteScreenPlayData(screenPlay, variable);

	return 0;
}

int DirectorManager::clearScreenPlayData(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::clearScreenPlayData";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String screenPlay = lua_tostring(L, -1);
	SceneObject* player = (SceneObject*) lua_touserdata(L, -2);

	if (player == nullptr || !player->isPlayerCreature()) {
		String err = "Attempted to clear screen play data for a non-player Scene Object using screenplay " + screenPlay;
		printTraceError(L, err);
		return 0;
	}

	Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost == nullptr) {
		String err = "Attempted to clear screen play data for a null ghost using screenplay " + screenPlay;
		printTraceError(L, err);
		return 0;
	}

	ghost->clearScreenPlayData(screenPlay);

	return 0;
}

int DirectorManager::registerScreenPlay(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::registerScreenPlay";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String name = lua_tostring(L, -2);
	bool start = lua_toboolean(L, -1);

	instance()->wlock();

	instance()->screenPlays.put(name, start);

	instance()->unlock();

	return 0;
}

int DirectorManager::checkInt64Lua(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::checkInt64Lua";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	instance()->info(true) << "Lua version: " << *lua_version(L);

	uint64 data = lua_tointeger(L, -1);

	uint64 bigNumber = 0x01000000;
	bigNumber += bigNumber << 32;

	instance()->fatal(data >= bigNumber, "Lua not using lnum patch with 64 bit integers, please patch lua!!");

	lua_pop(L, 1);

	return 0;
}

int DirectorManager::includeFile(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::includeFile";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String filename = Lua::getStringParameter(L);

	int oldError = ERROR_CODE;

	bool ret = Lua::runFile("scripts/screenplays/" + filename, L);

	if (!ret) {
		ERROR_CODE = GENERAL_ERROR;

		DirectorManager::instance()->error("running file: scripts/screenplays/" + filename);
	} else if (!oldError && ERROR_CODE) {
		DirectorManager::instance()->error("running file: scripts/screenplays/" + filename);
	}

	return 0;
}

int DirectorManager::readSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::readSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = Lua::getStringParameter(L);

	uint64 data = instance()->readSharedMemory(key);

	lua_pushinteger(L, data);

	return 1;
}

int DirectorManager::deleteSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::deleteSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = Lua::getStringParameter(L);

#ifndef WITH_STM
	DirectorManager::instance()->wlock();
#endif

	DirectorManager::instance()->sharedMemory->remove(key);

#ifndef WITH_STM
	DirectorManager::instance()->unlock();
#endif

	return 0;
}

int DirectorManager::writeSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::writeSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = lua_tostring(L, -2);
	uint64 data = lua_tointeger(L, -1);

#ifndef WITH_STM
	DirectorManager::instance()->wlock();
#endif

	DirectorManager::instance()->sharedMemory->put(key, data);

#ifndef WITH_STM
	DirectorManager::instance()->unlock();
#endif

	return 0;
}

int DirectorManager::readStringSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::readStringSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = Lua::getStringParameter(L);

	String data = instance()->readStringSharedMemory(key);

	lua_pushstring(L, data.toCharArray());

	return 1;
}

int DirectorManager::deleteStringSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::deleteStringSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = Lua::getStringParameter(L);

#ifndef WITH_STM
	DirectorManager::instance()->wlock();
#endif

	DirectorManager::instance()->sharedMemory->removeString(key);

#ifndef WITH_STM
	DirectorManager::instance()->unlock();
#endif

	return 0;
}

int DirectorManager::writeStringSharedMemory(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::writeStringSharedMemory";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String key = lua_tostring(L, -2);
	String data = lua_tostring(L, -1);

#ifndef WITH_STM
	DirectorManager::instance()->wlock();
#endif

	DirectorManager::instance()->sharedMemory->putString(key, data);

#ifndef WITH_STM
	DirectorManager::instance()->unlock();
#endif

	return 0;
}

int DirectorManager::createEvent(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 5 && numberOfArguments != 6) {
		String err = "incorrect number of arguments passed to DirectorManager::createEvent";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String args = lua_tostring(L, -1);
	SceneObject* obj = (SceneObject*) lua_touserdata(L, -2);
	String key = lua_tostring(L, -3);
	String play = lua_tostring(L, -4);
	uint32 mili = lua_tonumber(L, -5);

	//System::out << "scheduling task with mili:" << mili << endl;

	Reference<ScreenPlayTask*> task = new ScreenPlayTask(obj, key, play, args);
	DirectorManager::instance()->screenplayTasks.put(task);

	if (numberOfArguments == 6) {
		bool save = lua_toboolean(L, -6);

		if (save && obj != nullptr) {
			Time expireTime;
			uint64 currentTime = expireTime.getMiliTime();

			Reference<PersistentEvent*> pevent = new PersistentEvent();
			pevent->setObject(obj);
			pevent->setKey(key);
			pevent->setScreenplay(play);
			pevent->setArgs(args);
			pevent->setMiliDiff(mili);
			pevent->setCurTime(currentTime);
			pevent->setScreenplayTask(task);

			StringBuffer eventName;
			eventName << key << ":" << play << obj->getObjectID();

			String eventStringName = eventName.toString();

			pevent->setEventName(eventStringName);

			ObjectManager::instance()->persistObject(pevent, 1, "events");

			task->setPersistentEvent(pevent.get());

			if (persistentEvents.put(eventStringName.hashCode(), pevent) != nullptr) {
				String err = "Duplicate persistent event for " + eventStringName;
				printTraceError(L, err);
			}
		}

		task->schedule(mili);
	} else {
		task->schedule(mili);
	}

	return 0;
}

int DirectorManager::createEventActualTime(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createEventActualTime";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* obj = (SceneObject*) nullptr;
	String key = lua_tostring(L, -1);
	String play = lua_tostring(L, -2);
	uint32 timeInMinutes = lua_tonumber(L, -3);
	Reference<ScreenPlayTask*> task = new ScreenPlayTask(obj, key, play, "");
	Time actualTime = Time(timeInMinutes);
	Time now;
	uint64 days=now.getMiliTime()/(24*60*60000);
	uint64 dModifier = now.getMiliTime() - (days * (24*60*60000));
	uint64 interval =(24*60*60000);
	if (actualTime.getMiliTime()<= dModifier){
		interval =(24*60*60000) - (dModifier - actualTime.getMiliTime());
	}
	DirectorManager::instance()->screenplayTasks.put(task);

	task->schedule(interval);

	return 0;
}

int DirectorManager::createServerEvent(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createServerEvent";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String eventName = lua_tostring(L, -1);
	String key = lua_tostring(L, -2);
	String play = lua_tostring(L, -3);
	uint64 mili = lua_tonumber(L, -4);

	Reference<PersistentEvent*> pEvent = getServerEvent(eventName);

	if (pEvent != nullptr) {
		String err = "Server event " + eventName + " already exists, exiting...";
		printTraceError(L, err);
		ERROR_CODE = GENERAL_ERROR;
		return 0;
	}

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime();

	Reference<ScreenPlayTask*> task = new ScreenPlayTask(nullptr, key, play, "");

	Reference<PersistentEvent*> pevent = new PersistentEvent();
	pevent->setMiliDiff(mili);
	pevent->setCurTime(currentTime);
	pevent->setEventName(eventName);
	pevent->setKey(key);
	pevent->setScreenplay(play);
	pevent->setScreenplayTask(task);

	if (persistentEvents.put(eventName.hashCode(), pevent) != nullptr) {
		String err = "Persistent event with " + eventName + " already exists!";
		printTraceError(L, err);
	}

	task->setPersistentEvent(pevent);

	ObjectManager::instance()->persistObject(pevent, 1, "events");

	task->schedule(mili);

	lua_pushinteger(L, pevent->_getObjectID());

	return 1;
}

int DirectorManager::hasServerEvent(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::hasServerEvent";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String eventName = lua_tostring(L, -1);

	Reference<PersistentEvent*> pEvent = getServerEvent(eventName);

	if (pEvent != nullptr)
		lua_pushboolean(L, true);
	else
		lua_pushboolean(L, false);

	return 1;
}

int DirectorManager::rescheduleServerEvent(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::rescheduleServerEvent";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String eventName = lua_tostring(L, -2);
	uint32 mili = lua_tonumber(L, -1);

	Reference<PersistentEvent*> pEvent = getServerEvent(eventName);

	if (pEvent == nullptr) {
		String err = "Unable to find server event " + eventName + " in DirectorManager::rescheduleServerEvent";
		printTraceError(L, err);
		return 0;
	}

	Reference<ScreenPlayTask*> task = pEvent->getScreenplayTask().get();

	if (task == nullptr) {
		String err = "Unable to find task for server event " + eventName + " in DirectorManager::rescheduleServerEvent";
		printTraceError(L, err);
		return 0;
	}

	Time curTime;
	uint64 currentTime = curTime.getMiliTime();

	pEvent->setMiliDiff(mili);
	pEvent->setCurTime(currentTime);
	task->reschedule(mili);

	return 0;
}

int DirectorManager::getServerEventTimeLeft(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getServerEventTimeLeft";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint64 objectID = lua_tointeger(L, -1);

	Reference<PersistentEvent*> pEvent = Core::getObjectBroker()->lookUp(objectID).castTo<PersistentEvent*>();

	if (pEvent == nullptr)
		lua_pushnil(L);
	else {
		Time curTime;
		uint64 currentTime = curTime.getMiliTime();
		uint64 origTime = pEvent->getCurTime();
		uint64 timeStamp = pEvent->getMiliDiff();
		uint64 timeLeft = origTime + timeStamp - currentTime;

		lua_pushinteger(L, timeLeft);
	}

	return 1;
}

int DirectorManager::getServerEventID(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getServerEventID";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String eventName = lua_tostring(L, -1);

	Reference<PersistentEvent*> pEvent = getServerEvent(eventName);

	if (pEvent == nullptr) {
		lua_pushnil(L);
	} else {
		lua_pushnumber(L, pEvent->_getObjectID());
	}

	return 1;
}

void DirectorManager::dropServerEventReference(const String& eventName) {
	persistentEvents.remove(eventName.hashCode());
}

Reference<PersistentEvent*> DirectorManager::getServerEvent(const String& eventName) {
	return persistentEvents.get(eventName.hashCode());
}

int DirectorManager::getChatMessage(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getChatMessage";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	ChatMessage* cm = (ChatMessage*)lua_touserdata(L, -1);

	String text = "";

	if (cm != nullptr)
		text = cm->toString();

	lua_pushstring(L, text.toCharArray());

	return 1;
}

int DirectorManager::spatialChat(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::spatialChat";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	ZoneServer* zoneServer = ServerCore::getZoneServer();
	ChatManager* chatManager = zoneServer->getChatManager();

	ManagedReference<CreatureObject*> creature = (CreatureObject*)lua_touserdata(L, -2);

	if (lua_islightuserdata(L, -1)) {
		StringIdChatParameter* message = (StringIdChatParameter*)lua_touserdata(L, -1);

		if (creature != nullptr && message != nullptr) {
			Reference<StringIdChatParameter*> param = new StringIdChatParameter(*message);

			Core::getTaskManager()->executeTask([=] () {
				Locker locker(creature);

				chatManager->broadcastChatMessage(creature, *param.get(), 0, 0, creature->getMoodID());
			}, "BroadcastChatLambda");
		}
	} else {
		String message = lua_tostring(L, -1);

		if (creature != nullptr) {
			Core::getTaskManager()->executeTask([=] () {
				Locker locker(creature);

				chatManager->broadcastChatMessage(creature, message, 0, 0, creature->getMoodID());
			}, "BroadcastChatLambda2");
		}
	}

	return 0;
}

int DirectorManager::spatialMoodChat(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::spatialMoodChat";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	ZoneServer* zoneServer = ServerCore::getZoneServer();
	ChatManager* chatManager = zoneServer->getChatManager();

	SceneObject* scene = (SceneObject*)lua_touserdata(L, -4);

	CreatureObject* creature = dynamic_cast<CreatureObject*>(scene);

	assert(creature);

	int moodType = lua_tonumber(L, -2);
	int chatType = lua_tonumber(L, -1);

	if (creature == nullptr)
		return 0;

	Locker locker(creature);

	if (lua_isuserdata(L, -3)) {
		StringIdChatParameter* message = (StringIdChatParameter*)lua_touserdata(L, -3);

		chatManager->broadcastChatMessage(creature, *message, 0, chatType, moodType);
	} else {
		String message = lua_tostring(L, -3);

		chatManager->broadcastChatMessage(creature, message, 0, chatType, moodType);
	}

	return 0;
}

int DirectorManager::getSceneObject(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getSceneObject";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint64 objectID = lua_tointeger(L, -1);
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Reference<SceneObject*> object = zoneServer->getObject(objectID);

	if (object == nullptr) {
		lua_pushnil(L);
	} else {
		lua_pushlightuserdata(L, object.get());
		object->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
	}

	return 1;
}

int DirectorManager::getRandomNumber(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 1 && numberOfArguments != 2) {
		String err = "incorrect number of arguments passed to DirectorManager::getRandomNumber";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}
	int random;

	if (numberOfArguments == 1) {
		int max = lua_tointeger(L, -1);
		random = System::random(max - 1) + 1;
	} else {
		int min = lua_tointeger(L, -2);
		int max = lua_tointeger(L, -1);
		random = min + System::random(max - min);
	}
	lua_pushinteger(L, random);

	return 1;
}


int DirectorManager::getRegion(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRegion";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String regionName = lua_tostring(L, -1);
	String zoneName = lua_tostring(L, -2);
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Zone* zone = zoneServer->getZone(zoneName);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	CreatureManager* creatureManager = zone->getCreatureManager();

	SceneObject* spawnArea = creatureManager->getSpawnArea(regionName);

	if (spawnArea == nullptr)
		lua_pushnil(L);
	else {
		spawnArea->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
		lua_pushlightuserdata(L, spawnArea);
	}

	return 1;
}

int DirectorManager::getCreatureObject(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getCreatureObject";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint64 objectID = lua_tointeger(L, -1);
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Reference<SceneObject*> object = zoneServer->getObject(objectID);

	if (object != nullptr && object->isCreatureObject()) {
		lua_pushlightuserdata(L, object.get());
		object->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::getContainerObjectByTemplate(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getContainerObjectByTemplate";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* container = (SceneObject*)lua_touserdata(L, -3);
	String objectTemplate = lua_tostring(L, -2);
	bool checkChildren = lua_toboolean(L, -1);

	uint32 objectCRC = objectTemplate.hashCode();

	if (container == nullptr) {
		instance()->info("getContainerObjectByTemplate: SceneObject nullptr", true);
		lua_pushnil(L);

		return 1;
	}

	SceneObject* sco = nullptr;
	SceneObject* child = nullptr;

	for (int i=0; i< container->getContainerObjectsSize(); i++) {
		sco = container->getContainerObject(i);

		if (sco == nullptr)
			continue;

		if (sco->getServerObjectCRC() == objectCRC) {
			sco->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
			lua_pushlightuserdata(L, sco);
			return 1;
		}

		if (checkChildren && sco->getContainerObjectsSize() > 0) {
			for (int j=0; j < sco->getContainerObjectsSize(); j++) {
				SceneObject* child = sco->getContainerObject(j);

				if (child == nullptr)
					continue;

				if (child->getServerObjectCRC() == objectCRC) {
					child->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
					lua_pushlightuserdata(L, child);
					return 1;
				}
			}
		}
	}

	lua_pushnil(L);

	return 1;
}

int DirectorManager::updateCellPermission(lua_State* L) {
	//realObject->info("getting values",true);
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::updateCellPermission";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* sco = (SceneObject*)lua_touserdata(L, -3);
	bool allowEntry = lua_toboolean(L, -2);
	CreatureObject* obj = (CreatureObject*)lua_touserdata(L, -1);

	//sco->info("allowentry:" + String::valueOf(allowEntry), true);
	if (obj == nullptr) {
		instance()->info("Object nullptr", true);
		return 0;
	}


	//sco->info("values not nullptr", true);

	if (sco == nullptr) {
		obj->info("Cell nullptr", true);
		return 0;
	}

	if (!sco->isCellObject()) {
		sco->info("Unknown entity error: Cell", true);
		return 0;
	}

	if (!obj->isCreatureObject()) {
		//sco->info("Unknown entity error: Creature", true);
		obj->info("Unknown entity error: Creature", true);
		return 0;
	}

	//sco->info("checks are fine", true);

	BaseMessage* perm = new UpdateCellPermissionsMessage(sco->getObjectID(), allowEntry);
	obj->sendMessage(perm);

	return 0;
}

int DirectorManager::updateCellPermissionGroup(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::updateCellPermissionGroup";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	//realObject->info("getting values",true);
	SceneObject* sco = (SceneObject*)lua_touserdata(L, -3);
	int allowEntry = lua_tonumber(L, -2);
	CreatureObject* obj = (CreatureObject*)lua_touserdata(L, -1);
	//realObject->info("allowentry:" + String::valueOf(allowEntry), true);
	if (obj == nullptr)
		return 0;

	//realObject->info("values not nullptr", true);

	if (!sco->isCellObject()) {
		sco->info("Unknown entity error: Cell", true);
		return 0;
	}

	if (!obj->isCreatureObject()) {
		//sco->info("Unknown entity error: Creature", true);
		obj->info("Unknown entity error: Creature", true);
		return 0;
	}

	BaseMessage* perm = new UpdateCellPermissionsMessage(sco->getObjectID(), allowEntry);

	//sco->info("checks are fine", true);
	if (obj->isGrouped()) {
		// do group
		GroupObject* group = obj->getGroup();
		if (group != nullptr) {
			group->broadcastMessage(perm);
		} else {
			delete perm;
		}
	} else {
		// do single creature
		obj->sendMessage(perm);
	}

	return 0;
}

int DirectorManager::forcePeace(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::forcePeace";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	CreatureObject* creatureObject = (CreatureObject*)lua_touserdata(L, -1);

	if (creatureObject != nullptr) {
		Locker locker(creatureObject);

		ThreatMap* threatMap = creatureObject->getThreatMap();

		if (threatMap != nullptr)
			threatMap->removeAll();

		CombatManager::instance()->forcePeace(creatureObject);
	}

	return 0;
}

int DirectorManager::addStartingItemsInto(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::addStartingItemsInto";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	CreatureObject* creatureObject = (CreatureObject*)lua_touserdata(L, -2);
	SceneObject* sceneObject = (SceneObject*)lua_touserdata(L, -1);

	if (creatureObject != nullptr && sceneObject != nullptr) {
		PlayerCreationManager* pcm = PlayerCreationManager::instance();
		pcm->addStartingItemsInto(creatureObject, sceneObject);
	}

	return 0;
}

int DirectorManager::addStartingWeaponsInto(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::addStartingWeaponsInto";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	CreatureObject* creatureObject = (CreatureObject*)lua_touserdata(L, -2);
	SceneObject* sceneObject = (SceneObject*)lua_touserdata(L, -1);

	//SceneObject* sceneObject = creatureObject->getSlottedObject("inventory");

	if (creatureObject != nullptr && sceneObject != nullptr) {
		PlayerCreationManager* pcm = PlayerCreationManager::instance();
		pcm->addStartingWeaponsInto(creatureObject, sceneObject);
	}

	return 0;
}

int DirectorManager::giveItem(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 3 && numberOfArguments != 4) {
		String err = "incorrect number of arguments passed to DirectorManager::giveItem";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* obj;
	String objectString;
	int slot = -1;
	bool overload = false;

	if (numberOfArguments == 3) {
		obj = (SceneObject*) lua_touserdata(L, -3);
		objectString = lua_tostring(L, -2);
		slot = lua_tointeger(L, -1);
	} else {
		obj = (SceneObject*) lua_touserdata(L, -4);
		objectString = lua_tostring(L, -3);
		slot = lua_tointeger(L, -2);
		overload = lua_toboolean(L, -1);
	}

	if (obj == nullptr)
		return 0;

	ZoneServer* zoneServer = obj->getZoneServer();

	ManagedReference<SceneObject*> item = zoneServer->createObject(objectString.hashCode(), 1);

	if (item != nullptr && obj != nullptr) {
		if (obj->transferObject(item, slot, true, overload)) {
			item->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA

			ManagedReference<SceneObject*> parent = item->getParentRecursively(SceneObjectType::PLAYERCREATURE);
			if (parent != nullptr && parent->isPlayerCreature()) {
				item->sendTo(parent, true);
			}

			lua_pushlightuserdata(L, item.get());
		} else {
			item->destroyObjectFromDatabase(true);
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::giveControlDevice(lua_State* L) {
	if (checkArgumentCount(L, 5) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::giveControlDevice";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* datapad = (SceneObject*) lua_touserdata(L, -5);
	String objectString = lua_tostring(L, -4);
	String controlledObjectPath = lua_tostring(L, -3);
	int slot = lua_tointeger(L, -2);
	bool mobile = lua_toboolean(L, -1);

	if (datapad == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	ZoneServer* zoneServer = datapad->getZoneServer();
	Zone* zone = datapad->getZone();

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	ManagedReference<ControlDevice*> controlDevice = zoneServer->createObject(objectString.hashCode(), 1).castTo<ControlDevice*>();

	if (controlDevice == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	Locker locker(controlDevice);

	ManagedReference<TangibleObject*> controlledObject = nullptr;
	ManagedReference<CreatureObject*> player = (datapad->getParent().get()).castTo<CreatureObject*>();

	if (mobile) {
		CreatureManager* creatureManager = zone->getCreatureManager();
		CreatureTemplate* creoTempl = CreatureTemplateManager::instance()->getTemplate(controlledObjectPath.hashCode());

		if (creoTempl == nullptr) {
			controlDevice->destroyObjectFromDatabase(true);
			lua_pushnil(L);
			return 1;
		}

		String templateToSpawn = creatureManager->getTemplateToSpawn(controlledObjectPath.hashCode());
		controlledObject = creatureManager->createCreature(templateToSpawn.hashCode(), true, controlledObjectPath.hashCode());

		if (controlledObject == nullptr) {
			controlDevice->destroyObjectFromDatabase(true);
			lua_pushnil(L);
			return 1;
		}

		Locker locker2(controlledObject);

		if (!controlledObject->isAiAgent()) {
			controlDevice->destroyObjectFromDatabase(true);
			controlledObject->destroyObjectFromDatabase(true);
			lua_pushnil(L);
			return 1;
		}

		AiAgent* pet = controlledObject.castTo<AiAgent*>();
		pet->loadTemplateData(creoTempl);

	} else {
		controlledObject = zoneServer->createObject(controlledObjectPath.hashCode(), 1).castTo<TangibleObject*>();

		if (controlledObject == nullptr) {
			controlDevice->destroyObjectFromDatabase(true);
			lua_pushnil(L);
			return 1;
		}

		Locker locker2(controlledObject);

		SharedObjectTemplate* temp = controlledObject->getObjectTemplate();
		controlledObject->loadTemplateData(temp);
	}

	controlDevice->setControlledObject(controlledObject);
	StringId s;
	s.setStringId(controlledObject->getObjectName()->getFullPath());
	controlDevice->setObjectName(s, false);

	PetControlDevice* petControlDevice = cast<PetControlDevice*>(controlDevice.get());
	if( petControlDevice != nullptr ){
		petControlDevice->setDefaultCommands();
	}

	if (datapad->transferObject(controlDevice, slot, true)) {
		controlDevice->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
		lua_pushlightuserdata(L, controlDevice.get());
	} else {
		controlDevice->destroyObjectFromDatabase(true);
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::checkTooManyHirelings(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::checkTooManyHirelings";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* datapad = (SceneObject*) lua_touserdata(L, -1);

	if (datapad == nullptr) {
		lua_pushboolean(L, true);
		return 1;
	}

	int numberOfHirelings = 0;

	for (int i = 0; i < datapad->getContainerObjectsSize(); i++) {
		Reference<SceneObject*> object = datapad->getContainerObject(i);

		if (object != nullptr && object->isPetControlDevice()) {
			PetControlDevice* device = object.castTo<PetControlDevice*>();

			if (device->getPetType() == PetManager::FACTIONPET)
				numberOfHirelings++;
		}
	}

	ManagedReference<PlayerManager*> playerManager = ServerCore::getZoneServer()->getPlayerManager();

	bool result = numberOfHirelings >= playerManager->getBaseStoredFactionPets();

	lua_pushboolean(L, result);

	return 1;
}

int DirectorManager::setAuthorizationState(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::setAuthorizationState";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* terminal = (SceneObject*)lua_touserdata(L, -2);
	bool state = lua_toboolean(L, -1);

	//SceneObject* sceneObject = creatureObject->getSlottedObject("inventory");

	if (terminal == nullptr) {
		instance()->info("setAuthorizationState: Terminal is nullptr");
		return 0;
	}

	if (terminal->getGameObjectType() != SceneObjectType::NEWBIETUTORIALTERMINAL) {
		instance()->info("setAuthorizationState: Wrong SceneObjectType:" + String::valueOf(terminal->getGameObjectType()));
		return 0;
	}

	ManagedReference<StartingLocationTerminal*> item = (StartingLocationTerminal*)terminal;
	item->setAuthorizationState(state);

	return 0;
}

int DirectorManager::spawnMobile(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 8 && numberOfArguments != 9) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnMobile";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	bool randomRespawn = false;
	uint64 parentID;
	float x, y, z, heading;
	int respawnTimer;
	String mobile, zoneid;

	if (numberOfArguments == 8) {
		parentID = lua_tointeger(L, -1);
		heading = lua_tonumber(L, -2);
		y = lua_tonumber(L, -3);
		z = lua_tonumber(L, -4);
		x = lua_tonumber(L, -5);
		respawnTimer = lua_tointeger(L, -6);
		mobile = lua_tostring(L, -7);
		zoneid = lua_tostring(L, -8);
	} else {
		randomRespawn = lua_toboolean(L, -1);
		parentID = lua_tointeger(L, -2);
		heading = lua_tonumber(L, -3);
		y = lua_tonumber(L, -4);
		z = lua_tonumber(L, -5);
		x = lua_tonumber(L, -6);
		respawnTimer = lua_tointeger(L, -7);
		mobile = lua_tostring(L, -8);
		zoneid = lua_tostring(L, -9);
	}

	ZoneServer* zoneServer = ServerCore::getZoneServer();

	Zone* zone = zoneServer->getZone(zoneid);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	CreatureManager* creatureManager = zone->getCreatureManager();

	/*StringBuffer msg;
	msg << "trying to spawn with mobile: " << mobile << " x:" << x;
	DirectorManager::instance()->info(msg.toString(), true);*/

	CreatureObject* creature = creatureManager->spawnCreature(mobile.hashCode(), 0, x, z, y, parentID);

	if (creature == nullptr) {
		String err = "could not spawn mobile " + mobile;
		printTraceError(L, err);

		lua_pushnil(L);
	} else {
		Locker locker(creature);

		creature->updateDirection(Math::deg2rad(heading));

		if (creature->isAiAgent()) {
			AiAgent* ai = cast<AiAgent*>(creature);
			ai->setRespawnTimer(respawnTimer);

			if (randomRespawn)
				ai->setRandomRespawn(true);

			// TODO (dannuic): this is a temporary measure until we add an AI setting method to DirectorManager -- make stationary the default
			ai->activateLoad("stationary");
		}

		creature->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
		lua_pushlightuserdata(L, creature);
	}

	return 1;
	//public native CreatureObject spawnCreature(unsigned int templateCRC, float x, float z, float y, unsigned long parentID = 0);
}

int DirectorManager::spawnEventMobile(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 8) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnEventMobile";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint64 parentID;
	float x, y, z, heading;
	int respawnTimer, level;
	String mobile, zoneid;

	if (numberOfArguments == 8) {
		parentID = lua_tointeger(L, -1);
		heading = lua_tonumber(L, -2);
		y = lua_tonumber(L, -3);
		z = lua_tonumber(L, -4);
		x = lua_tonumber(L, -5);
		level = lua_tointeger(L, -6);
		mobile = lua_tostring(L, -7);
		zoneid = lua_tostring(L, -8);
	}

	ZoneServer* zoneServer = ServerCore::getZoneServer();

	Zone* zone = zoneServer->getZone(zoneid);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	CreatureManager* creatureManager = zone->getCreatureManager();

	CreatureObject* creature = creatureManager->spawnCreatureAsEventMob(mobile.hashCode(), level, x, z, y, parentID);

	if (creature == nullptr) {
		String err = "could not spawn mobile " + mobile;
		printTraceError(L, err);

		lua_pushnil(L);
	} else {
		Locker locker(creature);

		creature->updateDirection(Math::deg2rad(heading));

		creature->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA
		lua_pushlightuserdata(L, creature);
	}

	return 1;
}

int DirectorManager::spawnBuilding(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 5) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnBuilding";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float x, y, angle;
	uint64 parentID;
	String script, zoneID;

	angle = lua_tointeger(L, -1);
	y = lua_tonumber(L, -2);
	x = lua_tonumber(L, -3);
	script = lua_tostring(L, -4);
	CreatureObject* creature = (CreatureObject*)lua_touserdata(L, -5);

	SharedStructureObjectTemplate* serverTemplate = dynamic_cast<SharedStructureObjectTemplate*>(TemplateManager::instance()->getTemplate(script.hashCode()));

	if (serverTemplate == nullptr) {
		String err = "Unable to find template for building " + script;
		printTraceError(L, err);
		lua_pushnil(L);
	} else {
		StructureObject* structure = StructureManager::instance()->placeStructure(creature, script, x, y, 0, 0);
		if (structure == nullptr) {
			String err = "Unable to spawn building " + script;
			printTraceError(L, err);
			lua_pushnil(L);
		} else {
			structure->_setUpdated(true);
			lua_pushlightuserdata(L, structure);
		}
	}
	return 1;
}

int DirectorManager::destroyBuilding(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 1) {
		String err = "incorrect number of arguments passed to DirectorManager::destroyBuilding";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}
	uint64 objectID = lua_tointeger(L, -1);
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Reference<SceneObject*> object = zoneServer->getObject(objectID);

	if (object == nullptr)
		return 0;

	ManagedReference<StructureObject*> building = object.castTo<StructureObject*>();

	if (building == nullptr || !building->isStructureObject())
		return 0;

	Reference<Task*> pendingTask = building->getPendingTask("destruction");

	if (pendingTask != nullptr)
		return 0;

	Reference<DestroyStructureTask*> task = new DestroyStructureTask(building);
	task->execute();
	return 1;
}

int DirectorManager::spawnSceneObject(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 10 && numberOfArguments != 7) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnSceneObject";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float dz, dy, dx, dw, x, y, z;
	uint64 parentID;
	String script, zoneID;

	if (numberOfArguments == 10) {
		dz = lua_tonumber(L, -1);
		dy = lua_tonumber(L, -2);
		dx = lua_tonumber(L, -3);
		dw = lua_tonumber(L, -4);
		parentID = lua_tointeger(L, -5);
		y = lua_tonumber(L, -6);
		z = lua_tonumber(L, -7);
		x = lua_tonumber(L, -8);
		script = lua_tostring(L, -9);
		zoneID = lua_tostring(L, -10);
	} else {
		Quaternion direction;
		direction.setHeadingDirection(lua_tonumber(L, -1));
		dz = direction.getZ();
		dy = direction.getY();
		dx = direction.getX();
		dw = direction.getW();
		parentID = lua_tointeger(L, -2);
		y = lua_tonumber(L, -3);
		z = lua_tonumber(L, -4);
		x = lua_tonumber(L, -5);
		script = lua_tostring(L, -6);
		zoneID = lua_tostring(L, -7);
	}

	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Zone* zone = zoneServer->getZone(zoneID);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	ManagedReference<SceneObject*> object = zoneServer->createObject(script.hashCode(), 0);

	if (object != nullptr) {
		Locker objLocker(object);

		object->initializePosition(x, z, y);
		object->setDirection(dw, dx, dy, dz);

		if (object->isBuildingObject()) {
			BuildingObject* building = object->asBuildingObject();

			building->createCellObjects();
		}

		Reference<SceneObject*> cellParent = nullptr;

		if (parentID != 0) {
			cellParent = zoneServer->getObject(parentID);

			if (cellParent != nullptr && !cellParent->isCellObject()) {
				//error("trying to set a parent that is not a cell to creature");
				cellParent = nullptr;
			}
		}

		if (cellParent != nullptr) {
			cellParent->broadcastObject(object, true);
			cellParent->transferObject(object, -1, true);
		} else {
			zone->transferObject(object, -1, true);
		}

		object->createChildObjects();

		object->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA

		lua_pushlightuserdata(L, object.get());
	} else {
		String err = "could not spawn template " + script;
		printTraceError(L, err);
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::spawnActiveArea(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 7) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnActiveArea";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint64 cellID = lua_tointeger(L, -1);
	int radius = lua_tointeger(L, -2);
	float y = lua_tonumber(L, -3);
	float z = lua_tonumber(L, -4);
	float x = lua_tonumber(L, -5);
	String script = lua_tostring(L, -6);
	String zoneID = lua_tostring(L, -7);

	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Zone* zone = zoneServer->getZone(zoneID);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	ManagedReference<SceneObject*> object = zoneServer->createObject(script.hashCode(), 0);

	if (object != nullptr && object->isActiveArea()) {
		ActiveArea* area = object.castTo<ActiveArea*>();

		if (area == nullptr) {
			lua_pushnil(L);
			return 1;
		}

		Locker locker(area);

		area->initializePosition(x, z, y);
		area->setRadius(radius);

		Reference<SceneObject*> cellParent = nullptr;

		if (cellID != 0) {
			cellParent = zoneServer->getObject(cellID);

			if (cellParent == nullptr || !cellParent->isCellObject()) {
				cellID = 0;
			}
		}

		area->setCellObjectID(cellID);

		Locker zoneLocker(zone);

		zone->transferObject(area, -1, true);

		area->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA

		lua_pushlightuserdata(L, area);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::createObserver(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 4 && numberOfArguments != 5) {
		String err = "incorrect number of arguments passed to DirectorManager::createObserver";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* sceneObject;
	String key, play;
	uint32 eventType;
	int persistence = 0;

	if (numberOfArguments == 4) {
		sceneObject = (SceneObject*) lua_touserdata(L, -1);
		key = lua_tostring(L, -2);
		play = lua_tostring(L, -3);
		eventType = lua_tointeger(L, -4);
	} else {
		persistence = lua_tointeger(L, -1);
		sceneObject = (SceneObject*) lua_touserdata(L, -2);
		key = lua_tostring(L, -3);
		play = lua_tostring(L, -4);
		eventType = lua_tointeger(L, -5);
	}

	if (sceneObject == nullptr)
		return 0;

	ManagedReference<ScreenPlayObserver*> observer = dynamic_cast<ScreenPlayObserver*>(ObjectManager::instance()->createObject("ScreenPlayObserver", persistence, ""));
	observer->setScreenPlay(play);
	observer->setScreenKey(key);
	observer->setObserverType(ObserverType::SCREENPLAY);

	sceneObject->registerObserver(eventType, observer);

	return 0;
}

int DirectorManager::hasObserver(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 2) {
		String err = "incorrect number of arguments passed to DirectorManager::hasObserver";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* sceneObject = (SceneObject*) lua_touserdata(L, -1);
	uint32 eventType = lua_tointeger(L, -2);

	SortedVector<ManagedReference<Observer* > > observers = sceneObject->getObservers(eventType);

	lua_pushboolean(L, observers.size() > 0);

	return 1;
}

int DirectorManager::dropObserver(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 2 && numberOfArguments != 4) {
		String err = "incorrect number of arguments passed to DirectorManager::dropObserver";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* sceneObject = nullptr;
	uint32 eventType = 0;

	if (numberOfArguments == 2) {
		sceneObject = (SceneObject*) lua_touserdata(L, -1);
		eventType = lua_tointeger(L, -2);

		if (sceneObject == nullptr)
			return 0;

		SortedVector<ManagedReference<Observer* > > observers = sceneObject->getObservers(eventType);
		for (int i = 0; i < observers.size(); i++) {
			Observer* observer = observers.get(i).get();
			if (observer != nullptr && observer->isObserverType(ObserverType::SCREENPLAY)) {
				sceneObject->dropObserver(eventType, observer);

				if (observer->isPersistent())
					ObjectManager::instance()->destroyObjectFromDatabase(observer->_getObjectID());
			}
		}
	} else {
		sceneObject = (SceneObject*) lua_touserdata(L, -1);
		String key = lua_tostring(L, -2);
		String play = lua_tostring(L, -3);
		eventType = lua_tointeger(L, -4);

		if (sceneObject == nullptr)
			return 0;

		SortedVector<ManagedReference<Observer* > > observers = sceneObject->getObservers(eventType);
		for (int i = 0; i < observers.size(); i++) {
			Observer* observer = observers.get(i).get();
			if (observer != nullptr && observer->isObserverType(ObserverType::SCREENPLAY)) {
				ManagedReference<ScreenPlayObserver*> spObserver = dynamic_cast<ScreenPlayObserver*>(observer);

				if (spObserver->getScreenPlay() == play && spObserver->getScreenKey() == key) {
					sceneObject->dropObserver(eventType, observer);

					if (observer->isPersistent())
						ObjectManager::instance()->destroyObjectFromDatabase(observer->_getObjectID());
				}
			}
		}
	}

	return 0;
}

Lua* DirectorManager::getLuaInstance() {
	Lua* lua = localLua.get();
	uint32* version = localScreenPlayVersion.get();

	if (version == nullptr) {
		version = new uint32;
		*version = 0;
		localScreenPlayVersion.set(version);
	}

	if (lua == nullptr) {
		lua = new Lua();
		initializeLuaEngine(lua);
		loadScreenPlays(lua);
		JediManager::instance()->loadConfiguration(lua);
		AiMap::instance()->initialize(lua);
		if (!AiMap::instance()->isLoaded())
			AiMap::instance()->loadTemplates(lua);

		localLua.set(lua);
	}

	if (*version != masterScreenPlayVersion.get()) {
		loadScreenPlays(lua);
		*version = masterScreenPlayVersion.get();
	}

	return lua;
}

int DirectorManager::runScreenPlays() {
	Lua* lua = localLua.get();
	uint32* version = localScreenPlayVersion.get();
	int ret = 0;

	if (version == nullptr) {
		version = new uint32;
		*version = 0;
		localScreenPlayVersion.set(version);
	}

	if (lua == nullptr) {
		lua = new Lua();
		initializeLuaEngine(lua);
		ret = loadScreenPlays(lua);
		JediManager::instance()->loadConfiguration(lua);
		AiMap::instance()->initialize(lua);
		if (!AiMap::instance()->isLoaded())
			AiMap::instance()->loadTemplates(lua);

		localLua.set(lua);
	}

	if (*version != masterScreenPlayVersion.get()) {
		ret = loadScreenPlays(lua);
		*version = masterScreenPlayVersion.get();
	}

	return ret || ERROR_CODE;
}

void DirectorManager::startScreenPlay(CreatureObject* creatureObject, const String& screenPlayName) {
	Lua* lua = getLuaInstance();

	LuaFunction startScreenPlay(lua->getLuaState(), screenPlayName, "start", 0);
	startScreenPlay << creatureObject;

	startScreenPlay.callFunction();
}

ConversationScreen* DirectorManager::getNextConversationScreen(const String& luaClass, ConversationTemplate* conversationTemplate, CreatureObject* conversingPlayer, int selectedOption, CreatureObject* conversingNPC) {
	Lua* lua = getLuaInstance();

	LuaFunction runMethod(lua->getLuaState(), luaClass, "getNextConversationScreen", 1);
	runMethod << conversationTemplate;
	runMethod << conversingPlayer;
	runMethod << selectedOption;
	runMethod << conversingNPC;

	runMethod.callFunction();

	ConversationScreen* result = (ConversationScreen*) lua_touserdata(lua->getLuaState(), -1);

	lua_pop(lua->getLuaState(), 1);

	return result;
}

ConversationScreen* DirectorManager::runScreenHandlers(const String& luaClass, ConversationTemplate* conversationTemplate, CreatureObject* conversingPlayer, CreatureObject* conversingNPC, int selectedOption, ConversationScreen* conversationScreen) {
	Lua* lua = getLuaInstance();

	LuaFunction runMethod(lua->getLuaState(), luaClass, "runScreenHandlers", 1);
	runMethod << conversationTemplate;
	runMethod << conversingPlayer;
	runMethod << conversingNPC;
	runMethod << selectedOption;
	runMethod << conversationScreen;

	runMethod.callFunction();

	ConversationScreen* result = (ConversationScreen*) lua_touserdata(lua->getLuaState(), -1);

	lua_pop(lua->getLuaState(), 1);

	return result;
}

void DirectorManager::activateEvent(ScreenPlayTask* task) {
	ManagedReference<SceneObject*> obj = task->getSceneObject();
	const String& play = task->getScreenPlay();
	const String& key = task->getTaskKey();
	const String& args = task->getArgs();

	Reference<PersistentEvent*> persistentEvent = task->getPersistentEvent();

	if (persistentEvent != nullptr) {
		persistentEvent->setEventExecuted(true);

		dropServerEventReference(persistentEvent->getEventName());

		ObjectManager::instance()->destroyObjectFromDatabase(persistentEvent->_getObjectID());
	} /*else {
		StringBuffer msg;
		msg << "no PersistentEvent object in the task " << play << ":" << task;

		error(msg.toString());
	}*/

	Lua* lua = getLuaInstance();

	try {
		LuaFunction startScreenPlay(lua->getLuaState(), play, key, 0);
		startScreenPlay << obj.get();
		startScreenPlay << args.toCharArray();

		startScreenPlay.callFunction();
	} catch (Exception& e) {
		StringBuffer msg;
		msg << "exception while running lua task " << play << ":" << key;
		error(msg.toString());

		e.printStackTrace();
	}

	screenplayTasks.drop(task);
}

int DirectorManager::createConversationScreen(lua_State* L) {
	ConversationScreen* screen = new ConversationScreen();

	lua_pushlightuserdata(L, screen);

	return 1;
}

int DirectorManager::getStringId(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getStringId";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String stringid = lua_tostring(L, -1);
	String stringvalue = StringIdManager::instance()->getStringId(stringid.hashCode()).toString();
	lua_pushstring(L, stringvalue.toCharArray());

	return 1;
}

int DirectorManager::getRankName(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRankName";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	String rankName = FactionManager::instance()->getRankName(rank);

	lua_pushstring(L, rankName.toCharArray());

	return 1;
}

int DirectorManager::getRankCost(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRankCost";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	lua_pushinteger(L, FactionManager::instance()->getRankCost(rank));

	return 1;
}

int DirectorManager::getRankDelegateRatioFrom(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRankDelegateRatioFrom";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	lua_pushinteger(L, FactionManager::instance()->getRankDelegateRatioFrom(rank));

	return 1;
}

int DirectorManager::getRankDelegateRatioTo(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRankDelegateRatioTo";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	lua_pushinteger(L, FactionManager::instance()->getRankDelegateRatioTo(rank));

	return 1;
}

int DirectorManager::isHighestRank(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::isHighestRank";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	bool result = FactionManager::instance()->isHighestRank(rank);

	lua_pushboolean(L, result);

	return 1;
}

int DirectorManager::getFactionPointsCap(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getFactionPointsCap";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int rank = lua_tointeger(L, -1);

	lua_pushinteger(L, FactionManager::instance()->getFactionPointsCap(rank));

	return 1;
}

int DirectorManager::getObjectTemplatePathByCRC(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getObjectTemplatePathByCRC";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	uint32 crc = lua_tointeger(L, -1);

	lua_pushstring(L, TemplateManager::instance()->getTemplateFile(crc).toCharArray());

	return 1;
}

int DirectorManager::getZoneByName(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getZoneByName";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneid = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneid);

	if (zone == nullptr) {
		lua_pushnil(L);
	} else {
		lua_pushlightuserdata(L, zone);
	}

	return 1;
}

int DirectorManager::isZoneEnabled(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::isZoneEnabled";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneid = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneid);

	lua_pushboolean(L, (zone != nullptr));

	return 1;
}

Vector3 DirectorManager::generateSpawnPoint(String zoneName, float x, float y, float minimumDistance, float maximumDistance, float extraNoBuildRadius, float sphereCollision, bool forceSpawn) {
	bool found = false;
	Vector3 position(0, 0, 0);
	int retries = 40;
	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Zone* zone = zoneServer->getZone(zoneName);

	if (zone == nullptr) {
		return position;
	}

	while (!found && retries > 0) {
		float distance = minimumDistance + System::random(maximumDistance - minimumDistance);
		float angle = System::random(360);
		float newAngle = angle * (M_PI / 180.0f);

		float newX = x + (cos(newAngle) * distance); // client has x/y inverted
		float newY = y + (sin(newAngle) * distance);

		newX = (newX < -8150) ? -8150 : newX;
		newX = (newX > 8150) ? 8150 : newX;

		newY = (newY < -8150) ? -8150 : newY;
		newY = (newY > 8150) ? 8150 : newY;

		float newZ = zone->getHeight(newX, newY);

		position = Vector3(newX, newY, newZ);

		found = forceSpawn == true || (zone->getPlanetManager()->isSpawningPermittedAt(position.getX(), position.getY(), extraNoBuildRadius) &&
				!CollisionManager::checkSphereCollision(position, sphereCollision, zone));

		retries--;
	}

	if (!found) {
		position = Vector3(0, 0, 0);
	}

	return position;
}

int DirectorManager::getSpawnPoint(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 5 && numberOfArguments != 6) {
		String err = "incorrect number of arguments passed to DirectorManager::getSpawnPoint";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float maximumDistance, minimumDistance, y, x;
	String zoneName;
	bool forceSpawn = false;

	if (numberOfArguments == 5) {
		maximumDistance = lua_tonumber(L, -1);
		minimumDistance = lua_tonumber(L, -2);
		y = lua_tonumber(L, -3);
		x = lua_tonumber(L, -4);
		zoneName = lua_tostring(L, -5);
	} else {
		forceSpawn = lua_toboolean(L, -1);
		maximumDistance = lua_tonumber(L, -2);
		minimumDistance = lua_tonumber(L, -3);
		y = lua_tonumber(L, -4);
		x = lua_tonumber(L, -5);
		zoneName = lua_tostring(L, -6);
	}

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);

	if (zone == nullptr) {
		String err = "Zone is nullptr in DirectorManager::getSpawnPoint. zoneName = " + zoneName;
		printTraceError(L, err);
		return 0;
	}

	bool found = false;
	Vector3 position;
	int retries = 50;

	while (!found && retries > 0) {
		position = generateSpawnPoint(zoneName, x, y, minimumDistance, maximumDistance, 5.0, 20, false);

		if (position != Vector3(0, 0, 0))
			found = true;

		retries--;
	}

	if (!found && forceSpawn)
		position = generateSpawnPoint(zoneName, x, y, minimumDistance, maximumDistance, 5.0, 20, true);

	if (position != Vector3(0, 0, 0)) {
		lua_newtable(L);
		lua_pushnumber(L, position.getX());
		lua_pushnumber(L, position.getZ());
		lua_pushnumber(L, position.getY());
		lua_rawseti(L, -4, 3);
		lua_rawseti(L, -3, 2);
		lua_rawseti(L, -2, 1);

		return 1;
	} else {
		String err = "Unable to generate spawn point in DirectorManager::getSpawnPoint, x: " + String::valueOf(x) + ", y: " + String::valueOf(y) +
				", zone: " + zoneName + ", minDist: " + String::valueOf(minimumDistance) + ", maxDist: " + String::valueOf(maximumDistance);
		printTraceError(L, err);
		return 0;
	}
}

int DirectorManager::getSpawnArea(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 7 && numberOfArguments != 8) {
		String err = "incorrect number of arguments passed to DirectorManager::getSpawnArea";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float maximumHeightDifference, areaSize, maximumDistance, minimumDistance, y, x;
	Zone* zone = nullptr;
	bool forceSpawn = false;
	String zoneName;

	if (numberOfArguments == 8) {
		forceSpawn = lua_toboolean(L, -1);
		maximumHeightDifference = lua_tonumber(L, -2);
		areaSize = lua_tonumber(L, -3);
		maximumDistance = lua_tonumber(L, -4);
		minimumDistance = lua_tonumber(L, -5);
		y = lua_tonumber(L, -6);
		x = lua_tonumber(L, -7);
		zoneName = lua_tostring(L, -8);
	} else {
		maximumHeightDifference = lua_tonumber(L, -1);
		areaSize = lua_tonumber(L, -2);
		maximumDistance = lua_tonumber(L, -3);
		minimumDistance = lua_tonumber(L, -4);
		y = lua_tonumber(L, -5);
		x = lua_tonumber(L, -6);
		zoneName = lua_tostring(L, -7);
	}

	zone = ServerCore::getZoneServer()->getZone(zoneName);

	if (zone == nullptr) {
		String err = "Zone is nullptr in DirectorManager::getSpawnArea using zone name: " + zoneName;
		printTraceError(L, err);
		return 0;
	}

	bool found = false;
	Vector3 position;
	int retries = 50;

	while (!found && retries > 0) {
		position = generateSpawnPoint(zoneName, x, y, minimumDistance, maximumDistance, areaSize + 5.0, areaSize + 20, false);

		int x0 = position.getX() - areaSize;
		int x1 = position.getX() + areaSize;
		int y0 = position.getY() - areaSize;
		int y1 = position.getY() + areaSize;

		found = zone->getPlanetManager()->getTerrainManager()->getHighestHeightDifference(x0, y0, x1, y1) <= maximumHeightDifference;
		retries--;
	}

	if (!found && forceSpawn) {
		position = generateSpawnPoint(zoneName, x, y, minimumDistance, maximumDistance, areaSize + 5.0, areaSize + 20, true);

		int x0 = position.getX() - areaSize;
		int x1 = position.getX() + areaSize;
		int y0 = position.getY() - areaSize;
		int y1 = position.getY() + areaSize;

		found = zone->getPlanetManager()->getTerrainManager()->getHighestHeightDifference(x0, y0, x1, y1) <= maximumHeightDifference;
	}

	if (found) {
		lua_newtable(L);
		lua_pushnumber(L, position.getX());
		lua_pushnumber(L, position.getZ());
		lua_pushnumber(L, position.getY());
		lua_rawseti(L, -4, 3);
		lua_rawseti(L, -3, 2);
		lua_rawseti(L, -2, 1);

		return 1;
	} else {
		String err = "Unable to generate spawn point in DirectorManager::getSpawnArea, x: " + String::valueOf(x) + ", y: " + String::valueOf(y) +
				", zone: " + zoneName + ", areaSize: " + String::valueOf(areaSize) + ", minDist: " + String::valueOf(minimumDistance) + ", maxDist: " +
				String::valueOf(maximumDistance) + ", maxHeightDiff: " + String::valueOf(maximumHeightDifference);
		printTraceError(L, err);
		return 0;
	}
}

int DirectorManager::makeCreatureName(lua_State* L) {
	ZoneServer* zoneServer = ServerCore::getZoneServer();

	String name = "";
	if (zoneServer != nullptr) {
		NameManager* nameManager = zoneServer->getNameManager();
		name = nameManager->makeCreatureName();
	}

	lua_pushstring(L, name.toCharArray());
	return 1;
}


int DirectorManager::getGCWDiscount(lua_State* L){
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getGCWDiscount";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	CreatureObject* creature = (CreatureObject*)lua_touserdata(L, -1);
	if (creature == nullptr)
		return 0;

	Zone* zone = creature->getZone();
	if (zone == nullptr)
		return 0;

	GCWManager* gcwMan = zone->getGCWManager();
	if (gcwMan == nullptr)
		return 0;

	lua_pushnumber(L, gcwMan->getGCWDiscount(creature));
	return 1;
}

int DirectorManager::getTerrainHeight(lua_State* L){
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getGCWDiscount";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float y = lua_tonumber(L, -1);
	float x = lua_tonumber(L, -2);
	CreatureObject* creatureObject = (CreatureObject*) lua_touserdata(L, -3);

	if(creatureObject == nullptr || creatureObject->getZone() == nullptr)
		return 0;

	lua_pushnumber(L, creatureObject->getZone()->getHeight(x, y));
	return 1;
}

int DirectorManager::checkArgumentCount(lua_State* L, int args) {
	int parameterCount = lua_gettop(L);

	if (parameterCount < args) {
		return 1;
	} else if (parameterCount > args)
		return 2;

	return 0;
}

int DirectorManager::awardSkill(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::awardSkill";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	CreatureObject* creature = (CreatureObject*)lua_touserdata(L, -2);
	String skillName = lua_tostring(L, -1);

	if(creature == nullptr)
		return 0;

	SkillManager::instance()->awardSkill(skillName, creature, true, true, true);

	return 0;
}

int DirectorManager::getCityRegionAt(lua_State* L) {
	if (checkArgumentCount(L, 3) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getCityRegionAt";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneid = lua_tostring(L, -3);
	float x = lua_tonumber(L, -2);
	float y = lua_tonumber(L, -1);

	auto zone =  ServerCore::getZoneServer()->getZone(zoneid);

	if (zone != nullptr) {
		PlanetManager* planetManager = zone->getPlanetManager();

		CityRegion* cityRegion = planetManager->getRegionAt(x, y);

		if (cityRegion != nullptr) {
			lua_pushlightuserdata(L, cityRegion);
		} else {
			lua_pushnil(L);
		}
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::setDungeonTicketAttributes(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::setDungeonTicketAttributes";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	SceneObject* scene = (SceneObject*) lua_touserdata(L, -4);
	String ticketDepartPlanet = lua_tostring(L, -3);
	String ticketDepartPoint = lua_tostring(L, -2);
	String ticketArrivePoint = lua_tostring(L, -1);


	if (scene == nullptr)
		return 0;

	ManagedReference<TangibleObject*> tano = cast<TangibleObject*>(scene);

	TicketObject* tObj = tano.castTo<TicketObject*>();

	tObj->setDeparturePlanet(ticketDepartPlanet);
	tObj->setDeparturePoint(ticketDepartPoint);
	tObj->setArrivalPoint(ticketArrivePoint);

	return 0;
}

int DirectorManager::setQuestStatus(lua_State* L) {
	if (checkArgumentCount(L, 2) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::setQuestStatus";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String valueString = lua_tostring(L, -1);
	String keyString = lua_tostring(L, -2);

	instance()->setQuestStatus(keyString, valueString);

	return 0;
}

int DirectorManager::getQuestStatus(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getQuestStatus";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String keyString = lua_tostring(L, -1);

	String str = instance()->getQuestStatus(keyString);

	if (str == "")
		lua_pushnil(L);
	else
		lua_pushstring(L, str.toCharArray());

	return 1;
}

int DirectorManager::removeQuestStatus(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::removeQuestStatus";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String keyString = lua_tostring(L, -1);

	instance()->removeQuestStatus(keyString);

	return 0;
}

int DirectorManager::getControllingFaction(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getControllingFaction";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneName = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);
	if (zone == nullptr) {
		lua_pushinteger(L, 0);
		return 1;
	}

	GCWManager* gcwMan = zone->getGCWManager();
	if (gcwMan == nullptr) {
		lua_pushinteger(L, 0);
	} else {
		lua_pushinteger(L, gcwMan->getWinningFaction());
	}

	return 1;
}

int DirectorManager::getImperialScore(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getImperialScore";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneName = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);
	if (zone == nullptr) {
		lua_pushinteger(L, 0);
		return 1;
	}

	GCWManager* gcwMan = zone->getGCWManager();
	if (gcwMan == nullptr) {
		lua_pushinteger(L, 0);
	} else {
		lua_pushinteger(L, gcwMan->getImperialScore());
	}

	return 1;
}

int DirectorManager::getRebelScore(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getRebelScore";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneName = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);
	if (zone == nullptr) {
		lua_pushinteger(L, 0);
		return 1;
	}

	GCWManager* gcwMan = zone->getGCWManager();
	if (gcwMan == nullptr) {
		lua_pushinteger(L, 0);
	} else {
		lua_pushinteger(L, gcwMan->getRebelScore());
	}

	return 1;
}

int DirectorManager::getWinningFactionDifficultyScaling(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getWinningFactionDifficultyScaling";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneName = lua_tostring(L, -1);

	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);
	if (zone == nullptr) {
		lua_pushinteger(L, 0);
		return 1;
	}

	GCWManager* gcwMan = zone->getGCWManager();
	if (gcwMan == nullptr) {
		lua_pushinteger(L, 0);
	} else {
		lua_pushinteger(L, gcwMan->getWinningFactionDifficultyScaling());
	}

	return 1;
}

int DirectorManager::playClientEffectLoc(lua_State* L) {
	uint64 playerId = lua_tointeger(L, -7);
	String effect = lua_tostring(L, -6);
	String zone = lua_tostring(L, -5);
	float x = lua_tonumber(L, -4);
	float z = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -2);
	int cell = lua_tonumber(L, -1);

	ZoneServer* zoneServer = ServerCore::getZoneServer();

	ManagedReference<CreatureObject*> creature = zoneServer->getObject(playerId).castTo<CreatureObject*>();

	if (creature == nullptr)
		return 0;

	PlayClientEffectLoc* effectLoc = new PlayClientEffectLoc(effect, zone, x, z, y, cell);
	creature->broadcastMessage(effectLoc, true);

	return 1;
}

int DirectorManager::getPlayerQuestID(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getPlayerQuestID";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	ManagedReference<PlayerManager*> playerManager = ServerCore::getZoneServer()->getPlayerManager();
	String questName = lua_tostring(L, -1);
	int questID = playerManager->getPlayerQuestID(questName);

	if (questID >= 0)
		lua_pushinteger(L, questID);
	else
		lua_pushnil(L);

	return 1;
}

int DirectorManager::getQuestInfo(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getQuestInfo";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	int questID = 0;
	ManagedReference<PlayerManager*> playerManager = ServerCore::getZoneServer()->getPlayerManager();

	if (lua_isnumber(L, -1)) {
		questID = lua_tointeger(L, -1);
	} else {
		String questName = lua_tostring(L, -1);
		questID = playerManager->getPlayerQuestID(questName);
	}

	QuestInfo* questInfo = playerManager->getQuestInfo(questID);

	if (questInfo == nullptr)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, questInfo);

	return 1;
}

int DirectorManager::getQuestVectorMap(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getQuestVectorMap";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String keyString = lua_tostring(L, -1);

	Reference<QuestVectorMap*> questMap = instance()->getQuestVectorMap(keyString);

	if (questMap == nullptr)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, questMap.get());

	return 1;
}

int DirectorManager::createQuestVectorMap(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::createQuestVectorMap";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String keyString = lua_tostring(L, -1);

	Reference<QuestVectorMap*> questMap = instance()->createQuestVectorMap(keyString);

	if (questMap == nullptr)
		lua_pushnil(L);
	else
		lua_pushlightuserdata(L, questMap);

	return 1;
}

int DirectorManager::removeQuestVectorMap(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::removeQuestVectorMap";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String keyString = lua_tostring(L, -1);

	instance()->removeQuestVectorMap(keyString);

	return 0;
}


QuestVectorMap* DirectorManager::createQuestVectorMap(const String& keyString) {
	Reference<QuestVectorMap*> questMap = questVectorMaps.get(keyString);

	if (questMap == nullptr) {
		questMap = new QuestVectorMap();
		questMap->setKey(keyString);
		questVectorMaps.put(keyString, questMap);

		ObjectManager::instance()->persistObject(questMap, 1, "questdata");
	}

	return questMap;
}

QuestVectorMap* DirectorManager::getQuestVectorMap(const String& keyString) {
	Reference<QuestVectorMap*> questMap = questVectorMaps.get(keyString);

	return questMap;
}

void DirectorManager::removeQuestVectorMap(const String& keyString) {
	Reference<QuestVectorMap*> questMap = nullptr;

	questMap = questVectorMaps.get(keyString);

	questVectorMaps.drop(keyString);

	if (questMap != nullptr)
		ObjectManager::instance()->destroyObjectFromDatabase(questMap->_getObjectID());
}

int DirectorManager::createNavMesh(lua_State *L) {

    if (checkArgumentCount(L, 6) == 1) {
        String err = "incorrect number of arguments passed to DirectorManager::createNavMesh";
        printTraceError(L, err);
        ERROR_CODE = INCORRECT_ARGUMENTS;
        return 0;
    }
    String name  = lua_tostring(L, -1);
    bool dynamic = lua_toboolean(L, -2);
    float radius = lua_tonumber(L, -3);
    float z      = lua_tonumber(L, -4);
    float x      = lua_tonumber(L, -5);
    String zoneName  = lua_tostring(L, -6);

    Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);

    if (zone == nullptr) {
       instance()-> error("Zone == nullptr in DirectorManager::createNavMesh (" + zoneName + ")");
        ERROR_CODE = INCORRECT_ARGUMENTS;
        return 0;
    }

    ManagedReference<NavArea*> navArea = ServerCore::getZoneServer()->createObject(STRING_HASHCODE("object/region_navmesh.iff"), "navareas", 0).castTo<NavArea*>();
    if (name.length() == 0) {
        name = String::valueOf(navArea->getObjectID());
    }

    Core::getTaskManager()->scheduleTask([=]{
        String str = name;
        Vector3 position = Vector3(x, 0, z);

		Locker locker(navArea);

        navArea->disableMeshUpdates(!dynamic);
        navArea->initializeNavArea(position, radius, zone, str);
        zone->transferObject(navArea, -1, false);
        zone->getPlanetManager()->addNavArea(str, navArea);
    }, "create_lua_navmesh", 1000);
    lua_pushlightuserdata(L, navArea);
    return 1;
}

int DirectorManager::creatureTemplateExists(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::creatureTemplateExists";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String templateName = lua_tostring(L, -1);

	bool result = CreatureTemplateManager::instance()->getTemplate(templateName) != nullptr;

	lua_pushboolean(L, result);

	return 1;
}

int DirectorManager::printLuaError(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::printLuaError";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String error = lua_tostring(L, -1);

	printTraceError(L, error);

	return 0;
}

int DirectorManager::getSpawnPointInArea(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getSpawnPointInArea";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String zoneName  = lua_tostring(L, -4);
	Zone* zone = ServerCore::getZoneServer()->getZone(zoneName);
	if (zone == nullptr) {
		instance()-> error("Zone == nullptr in DirectorManager::getSpawnPointInArea (" + zoneName + ")");
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	float x = lua_tonumber(L, -3);
	float y = lua_tonumber(L, -2);
	float radius = lua_tonumber(L, -1);

	Sphere sphere(Vector3(x, y, zone->getHeightNoCache(x, y)), radius);
	Vector3 result;

	if (PathFinderManager::instance()->getSpawnPointInArea(sphere, zone, result)) {
		lua_newtable(L);
		lua_pushnumber(L, result.getX());
		lua_pushnumber(L, result.getZ());
		lua_pushnumber(L, result.getY());
		lua_rawseti(L, -4, 3);
		lua_rawseti(L, -3, 2);
		lua_rawseti(L, -2, 1);
		return 1;
	} else {
		String err = "Unable to generate spawn point in DirectorManager::getSpawnPointInArea, x: " + String::valueOf(x) + ", y: " + String::valueOf(y) + ", zone: " + zoneName + ", radius: " + String::valueOf(radius);
		printTraceError(L, err);
		return 0;
	}
}

int DirectorManager::getPlayerByName(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getPlayerByName";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String playerName = lua_tostring(L, -1);

	ManagedReference<PlayerManager*> playerManager = ServerCore::getZoneServer()->getPlayerManager();

	CreatureObject* player = playerManager->getPlayer(playerName);

	if (player != nullptr) {
		lua_pushlightuserdata(L, player);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::sendMail(lua_State* L) {
	if (checkArgumentCount(L, 4) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::sendMail";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String recipient = lua_tostring(L, -1);
	String body = lua_tostring(L, -2);
	String subject = lua_tostring(L, -3);
	String senderName = lua_tostring(L, -4);

	ManagedReference<ChatManager*> chatManager = ServerCore::getZoneServer()->getChatManager();

	if (chatManager != nullptr)
		chatManager->sendMail(senderName, subject, body, recipient);

	return 0;
}

int DirectorManager::spawnTheaterObject(lua_State* L) {
	int numberOfArguments = lua_gettop(L);
	if (numberOfArguments != 5) {
		String err = "incorrect number of arguments passed to DirectorManager::spawnTheaterObject";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	bool flatten = lua_toboolean(L, -1);
	float y = lua_tonumber(L, -2);
	float z = lua_tonumber(L, -3);
	float x = lua_tonumber(L, -4);
	String zoneID = lua_tostring(L, -5);

	ZoneServer* zoneServer = ServerCore::getZoneServer();
	Zone* zone = zoneServer->getZone(zoneID);

	if (zone == nullptr) {
		lua_pushnil(L);
		return 1;
	}

	ManagedReference<SceneObject*> object = zoneServer->createObject(STRING_HASHCODE("object/intangible/theater/base_theater.iff"), 0);

	if (object != nullptr && object->isTheaterObject()) {
		TheaterObject* theater = object.castTo<TheaterObject*>();

		if (theater == nullptr) {
			lua_pushnil(L);
			return 1;
		}

		Locker locker(theater);

		theater->initializePosition(x, z, y);

		if (flatten)
			theater->setShouldFlatten(true);

		Locker zoneLocker(zone);

		zone->transferObject(theater, -1, true);

		theater->_setUpdated(true); //mark updated so the GC doesnt delete it while in LUA

		lua_pushlightuserdata(L, theater);
	} else {
		lua_pushnil(L);
	}

	return 1;
}

int DirectorManager::getSchematicItemName(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getSchematicItemName";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String itemName = "";
	String templatePath = Lua::getStringParameter(L);

	DraftSchematic* schematicTemplate = SchematicMap::instance()->get(templatePath.hashCode());

	if (schematicTemplate != nullptr)
		itemName = schematicTemplate->getCustomName();

	lua_pushstring(L, itemName.toCharArray());

	return 1;
}

int DirectorManager::getBadgeListByType(lua_State* L) {
	if (checkArgumentCount(L, 1) == 1) {
		String err = "incorrect number of arguments passed to DirectorManager::getBadgeListByType";
		printTraceError(L, err);
		ERROR_CODE = INCORRECT_ARGUMENTS;
		return 0;
	}

	String type = Lua::getStringParameter(L);

	lua_newtable(L);

	int count = 0;

	const auto badgeList = BadgeList::instance()->getMap();

	for (int i = 0; i < badgeList->size(); i++) {
		const Badge* badge = badgeList->get(i);

		if (badge->getTypeString() == type) {
			count++;
			lua_pushinteger(L, badge->getIndex());
			lua_rawseti(L, -2, count);
		}
	}

	return 1;
}
