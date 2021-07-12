/*
 * PlayerManagerImplementation.cpp
 *
 *  Created on: 18/07/2009
 *      Author: TheAnswer
 */

#include "server/zone/managers/player/PlayerManager.h"
#include <utility>
#include <mutex>

#include "server/zone/packets/charcreation/ClientCreateCharacterCallback.h"
#include "server/zone/packets/charcreation/ClientCreateCharacterFailed.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/managers/name/NameManager.h"
#include "templates/manager/TemplateManager.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/faction/FactionManager.h"
#include "server/zone/managers/frs/FrsManager.h"
#include "server/db/ServerDatabase.h"
#include "server/chat/ChatManager.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/skill/Performance.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/objects/intangible/VehicleControlDevice.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "server/zone/objects/creature/VehicleObject.h"
#include "server/login/packets/ErrorMessage.h"
#include "server/zone/packets/player/LogoutMessage.h"
#include "server/zone/objects/player/sessions/TradeSession.h"
#include "server/zone/objects/player/sessions/ProposeUnitySession.h"
#include "server/zone/objects/player/sessions/VeteranRewardSession.h"
#include "templates/params/OptionBitmask.h"
#include "server/zone/managers/player/JukeboxSong.h"
#include "server/zone/managers/player/QuestInfo.h"

#include "server/zone/objects/intangible/ShipControlDevice.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "templates/building/CloningBuildingObjectTemplate.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/wearables/ArmorObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/objects/tangible/wearables/WearableObject.h"

#include "server/zone/objects/player/events/PlayerIncapacitationRecoverTask.h"
#include "server/zone/objects/creature/events/ProposeUnityExpiredTask.h"
#include "server/zone/objects/player/events/ForceMeditateTask.h"
#include "server/zone/objects/player/events/MeditateTask.h"
#include "server/zone/objects/player/events/LogoutTask.h"
#include "server/zone/objects/player/sessions/EntertainingSession.h"
#include "templates/building/CloneSpawnPoint.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/managers/skill/SkillManager.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "server/zone/managers/planet/PlanetManager.h"

#include "server/zone/packets/trade/AbortTradeMessage.h"
#include "server/zone/packets/trade/AcceptTransactionMessage.h"
#include "server/zone/packets/trade/UnAcceptTransactionMessage.h"
#include "server/zone/packets/trade/AddItemMessage.h"
#include "server/zone/packets/trade/TradeCompleteMessage.h"
#include "server/zone/packets/trade/GiveMoneyMessage.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/zone/packets/tangible/TangibleObjectDeltaMessage3.h"
#include "server/zone/packets/player/PlayMusicMessage.h"
#include "server/zone/packets/object/StartingLocationListMessage.h"

#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/managers/director/DirectorManager.h"
#include "server/zone/objects/player/sui/callbacks/CloningRequestSuiCallback.h"
#include "server/zone/objects/tangible/tool/CraftingStation.h"
#include "server/zone/objects/tangible/tool/CraftingTool.h"

#include "server/zone/Zone.h"
#include "server/zone/managers/player/creation/PlayerCreationManager.h"
#include "server/ServerCore.h"
#include "server/login/account/Account.h"

#include "server/zone/objects/player/sui/callbacks/PlayerTeachSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/PlayerTeachConfirmSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/ProposeUnitySuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/SelectUnityRingSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/SelectVeteranRewardSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/ConfirmVeteranRewardSuiCallback.h"
#include "server/zone/objects/player/sui/callbacks/ConfirmDivorceSuiCallback.h"

#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/objects/creature/buffs/PowerBoostBuff.h"
#include "server/zone/objects/creature/buffs/ForceWeakenDebuff.h"
#include "server/zone/objects/creature/ai/Creature.h"
#include "server/zone/objects/creature/ai/NonPlayerCreatureObject.h"
#include "server/zone/objects/creature/events/DespawnCreatureTask.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/objects/creature/events/BurstRunNotifyAvailableEvent.h"
#include "server/zone/objects/creature/ai/DroidObject.h"
#include "server/zone/objects/tangible/components/droid/DroidPlaybackModuleDataComponent.h"
#include "server/zone/objects/player/badges/Badge.h"
#include "server/zone/objects/building/TutorialBuildingObject.h"
#include "server/zone/managers/frs/FrsManager.h"
#include "server/zone/objects/player/events/OnlinePlayerLogTask.h"
#include <sys/stat.h>
#include "server/zone/objects/transaction/TransactionLog.h"
#include "server/zone/objects/creature/commands/TransferItemMiscCommand.h"

PlayerManagerImplementation::PlayerManagerImplementation(ZoneServer* zoneServer, ZoneProcessServer* impl,
					bool trackOnlineUsers) : Logger("PlayerManager") {

	playerLoggerFilename = "log/player.log";
	playerLoggerLines = ConfigManager::instance()->getMaxLogLines();
	playerLogger.setLoggingName("PlayerLogger");
	playerLogger.setFileLogger(playerLoggerFilename, true);

	server = zoneServer;
	processor = impl;

	nameMap = new CharacterNameMap();

	DirectorManager::instance()->getLuaInstance()->runFile("scripts/screenplays/checklnum.lua");

	loadLuaConfig();
	loadStartingLocations();
	loadQuestInfo();
	loadPermissionLevels();
	loadXpBonusList();

	onlinePlayersLogOnSessionChange = false;

	setGlobalLogging(true);
	setLogging(false);

	if (ServerCore::truncateDatabases()) {
		try {
			const static String query = "TRUNCATE TABLE characters";

			Reference<ResultSet*> res = ServerDatabase::instance()->executeQuery(query);

			info("characters table truncated", true);
		} catch (const Exception& e) {
			error(e.getMessage());
		}
	}

	loadNameMap();

	if (ServerCore::hasArgument("listadmins")) {
		info("generating admin list...", true);

		auto admins = generateAdminList();

		for (auto& entry : admins) {
			auto name = entry.getKey();
			auto level = entry.getValue();

			info(true) << "Player: " << name << " level: " << level;
		}
	}

	int onlineLogSeconds = ConfigManager::instance()->getOnlineLogSeconds();

	if (onlineLogSeconds > 0 && trackOnlineUsers) {
		onlinePlayerLogSum = 0;

		Core::getTaskManager()->executeTask([=] () {
			rescheduleOnlinePlayerLogTask(onlineLogSeconds);
		}, "startOnlinePlayerLogTask");
	}
}

void PlayerManagerImplementation::stopOnlinePlayerLogTask() {
	auto onlinePlayerLogTask = this->onlinePlayerLogTask.get();

	if (onlinePlayerLogTask != nullptr) {
		onlinePlayerLogTask->cancel();
	}
}

bool PlayerManagerImplementation::rescheduleOnlinePlayerLogTask(int logSecs) {
	auto onlinePlayerLogTask = this->onlinePlayerLogTask.get();

	if (logSecs <= -1) {
		if (onlinePlayerLogTask != nullptr) {
			onlinePlayerLogTask->cancel();
			this->onlinePlayerLogTask = onlinePlayerLogTask = nullptr;
		}
		info("Loging online players disabled.", true);
		return true;
	}

	if (logSecs < 10) {
		error() << "rescheduleOnlinePlayerLogTask: attempt to set log schedule too low: " << logSecs;
		return false;
	}

	if (onlinePlayerLogTask == nullptr) {
		this->onlinePlayerLogTask = onlinePlayerLogTask = new OnlinePlayerLogTask();
	} else {
		onlinePlayerLogTask->cancel();
	}

	onlinePlayersLogOnSessionChange = ConfigManager::instance()->getBool("Core3.LogOnlineOnSessionChange", true);

	onlinePlayerLogTask->schedulePeriodic(0, logSecs * 1000);

	info(true) << "Loging online players every " << logSecs << " seconds.";

	return true;
}

bool PlayerManagerImplementation::createPlayer(ClientCreateCharacterCallback* callback) {
	PlayerCreationManager* pcm = PlayerCreationManager::instance();
	return pcm->createCharacter(callback);
}

void PlayerManagerImplementation::loadLuaConfig() {
	info("Loading configuration script.");

	Lua* lua = new Lua();
	lua->init();

	lua->runFile("scripts/managers/player_manager.lua");

	allowSameAccountPvpRatingCredit = lua->getGlobalInt("allowSameAccountPvpRatingCredit");
	onlineCharactersPerAccount = lua->getGlobalInt("onlineCharactersPerAccount");
	performanceBuff = lua->getGlobalInt("performanceBuff");
	medicalBuff = lua->getGlobalInt("medicalBuff");
	performanceDuration = lua->getGlobalInt("performanceDuration");
	medicalDuration = lua->getGlobalInt("medicalDuration");

	groupExpMultiplier = lua->getGlobalFloat("groupExpMultiplier");

	globalExpMultiplier = lua->getGlobalFloat("globalExpMultiplier");

	baseStoredCreaturePets = lua->getGlobalInt("baseStoredCreaturePets");
	baseStoredFactionPets = lua->getGlobalInt("baseStoredFactionPets");
	baseStoredDroids = lua->getGlobalInt("baseStoredDroids");
	baseStoredVehicles = lua->getGlobalInt("baseStoredVehicles");
	baseStoredShips = lua->getGlobalInt("baseStoredShips");

	veteranRewardAdditionalMilestones = lua->getGlobalInt("veteranRewardAdditionalMilestones");

	LuaObject rewardMilestonesLua = lua->getGlobalObject("veteranRewardMilestones");
	for (int i = 1; i <= rewardMilestonesLua.getTableSize(); ++i) {
		veteranRewardMilestones.add(rewardMilestonesLua.getIntAt(i));
	}
	rewardMilestonesLua.pop();

	LuaObject rewardsListLua = lua->getGlobalObject("veteranRewards");
	int size = rewardsListLua.getTableSize();

	lua_State* L = rewardsListLua.getLuaState();

	for (int i = 0; i < size; ++i) {
		lua_rawgeti(L, -1, i + 1);
		LuaObject a(L);

		VeteranReward reward;
		reward.parseFromLua(&a);
		veteranRewards.add(reward);

		a.pop();
	}

	rewardsListLua.pop();

	info(true) << "Loaded " << veteranRewards.size() << " veteran rewards.";

	LuaObject jboxSongs = lua->getGlobalObject("jukeboxSongs");

	if (jboxSongs.isValidTable()) {
		for (int i = 1; i <= jboxSongs.getTableSize(); ++i) {
			LuaObject songData = jboxSongs.getObjectAt(i);

			if (songData.isValidTable()) {
				String songStringID = songData.getStringAt(1);
				String songPath = songData.getStringAt(2);

				Reference<JukeboxSong*> data = new JukeboxSong(songStringID, songPath);

				jukeboxSongs.add(data);
			}

			songData.pop();
		}

	}

	jboxSongs.pop();

	delete lua;
	lua = nullptr;
}

void PlayerManagerImplementation::loadStartingLocations() {
	info("Loading starting locations.");

	IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/creation/starting_locations.iff");

	if (iffStream == nullptr) {
		info("Couldn't load starting locations.", true);
		return;
	}

	startingLocationList.parseFromIffStream(iffStream);

	delete iffStream;

	info(true) << "Loaded " << startingLocationList.getTotalLocations() << " starting locations.";
}

void PlayerManagerImplementation::loadXpBonusList() {
	IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/xp/species.iff");

	if (iffStream == nullptr) {
		info("Couldn't load species xp bonuses.", true);
		return;
	}

	DataTableIff dtiff;
	dtiff.readObject(iffStream);

	delete iffStream;

	for (int i = 0; i < dtiff.getTotalColumns(); i++) {
		VectorMap<String, int> bonusList;
		String speciesName = dtiff.getColumnNameByIndex(i);

		for (int j = 0; j < dtiff.getTotalRows(); j++) {
			DataTableRow* row = dtiff.getRow(j);
			String columnData = "";
			row->getCell(i)->getValue(columnData);

			if (columnData != "") {
				StringTokenizer callbackString(columnData);
				callbackString.setDelimeter(":");

				String xpType = "";
				int bonusMod = 0;

				callbackString.getStringToken(xpType);
				bonusMod = callbackString.getIntToken();

				if (xpType == "" or bonusMod == 0)
					continue;

				bonusList.put(xpType, bonusMod);
			}
		}

		if (bonusList.size() > 0)
			xpBonusList.put(speciesName, bonusList);
	}

	info(true) << "Loaded xp bonuses for " << xpBonusList.size() << " species.";
}

void PlayerManagerImplementation::loadQuestInfo() {
	TemplateManager* templateManager = TemplateManager::instance();

	IffStream* iffStream = templateManager->openIffFile("datatables/player/quests.iff");

	if (iffStream == nullptr) {
		info("quests.iff could not be found.", true);
		return;
	}

	DataTableIff dtable;
	dtable.readObject(iffStream);

	delete iffStream;

	for (int i = 0; i < dtable.getTotalRows(); ++i) {
		DataTableRow* row = dtable.getRow(i);

		QuestInfo* quest = new QuestInfo();
		quest->parseDataTableRow(row);
		questInfo.add(quest);
	}

	info(true) << "Loaded " << questInfo.size() << " quests.";
}

void PlayerManagerImplementation::loadPermissionLevels() {
	try {
		permissionLevelList = PermissionLevelList::instance();
		permissionLevelList->loadLevels();
	} catch(const Exception& e) {
		error("Couldn't load permission levels.");
		error(e.getMessage());
	}
}

void PlayerManagerImplementation::finalize() {
	stopOnlinePlayerLogTask();

	nameMap = nullptr;

	permissionLevelList->removeAll();
	permissionLevelList = nullptr;

	jukeboxSongs.removeAll();

	questInfo.removeAll();
}

void PlayerManagerImplementation::loadNameMap() {
	info("loading character names");

	try {
		String query = "SELECT character_oid, firstname FROM characters where character_oid > 16777216 and galaxy_id = " + String::valueOf(server->getGalaxyID()) + " order by character_oid asc";

		UniqueReference<ResultSet*> res(ServerDatabase::instance()->executeQuery(query));

		while (res->next()) {
			uint64 oid = res->getUnsignedLong(0);
			String firstName = res->getString(1);

			if (!nameMap->put(firstName.toLowerCase(), oid)) {
				error("error coliding name:" + firstName.toLowerCase());
			}
		}

	} catch (const Exception& e) {
		fatal(e.getMessage());
	}

	info(true) << "loaded " << nameMap->size() << " character names in memory";
}

void PlayerManagerImplementation::writePlayerLogEntry(JSONSerializationType& logEntry) {
	FileWriter* logFile = playerLogger.getFileLogger();
	StringBuffer logLine;

	logLine << logEntry.dump().c_str() << "\n";

	Locker lock(&playerLoggerMutex);

	(*logFile) << logLine;

	logFile->flush();

	// Check for log rotation
	if (--playerLoggerLines <= 0) {
		playerLoggerLines = ConfigManager::instance()->getMaxLogLines();

		Time now;
		StringBuffer archiveFilename;
		archiveFilename << "log/player-" << now.getMiliTime() << ".log";

		// If the rename failed its ok because we open with append below
		int err = std::rename(playerLoggerFilename.toCharArray(), archiveFilename.toString().toCharArray());

		// report failure if any
		if (err != 0)
			error() << "Failed to archive player log to " << archiveFilename.toString() << " err = " << err;

		playerLogger.setFileLogger(playerLoggerFilename, true);
	}
}

JSONSerializationType PlayerManagerImplementation::basePlayerLogEntry(CreatureObject* creature, PlayerObject* ghost) {
	JSONSerializationType logEntry;
	Time time;

	logEntry["@timestamp"] = time.getFormattedTimeFull().toCharArray();
	logEntry["time_msecs"] = time.getMiliTime();

	Thread* currentThread = Thread::getCurrentThread();

	if (currentThread != nullptr)
		logEntry["thread"] = currentThread->getName();

	if (creature != nullptr) {
		Locker locker(creature);
		logEntry["oid"] = creature->getObjectID();
		logEntry["first_name"] = creature->getFirstName();
	} else {
		logEntry["oid"] = ghost != nullptr ? ghost->getObjectID() : 0;
		logEntry["first_name"] = "-ghost-";
	}

	if (ghost != nullptr) {
		Locker locker(ghost);
		logEntry["account_id"] = ghost->getAccountID();
		logEntry["played_ms"] = ghost->getPlayedMiliSecs();
		logEntry["session_ms"] = ghost->getSessionMiliSecs();
	}

	logEntry["uptime_secs"] = playerLogger.getElapsedTime();

	return logEntry;
}

void PlayerManagerImplementation::writePlayerLog(CreatureObject* creature, PlayerObject* ghost, const String& msg, int logLevelType) {
	if (logLevelType > ghost->getLogLevel())
		return;

	JSONSerializationType logEntry = basePlayerLogEntry(creature, ghost);

	logEntry["type"] = "log";
	logEntry["log"] = msg;
	logEntry["log_level"] = playerLogger.getLogType((Logger::LogLevel)logLevelType);
	logEntry["log_tag"] = playerLogger.getLoggingName();

	// Add additional info on error
	if (logLevelType == Logger::LogLevel::ERROR) {
		logEntry["worldPositionX"] = (int)creature->getWorldPositionX();
		logEntry["worldPositionZ"] = (int)creature->getWorldPositionZ();
		logEntry["worldPositionY"] = (int)creature->getWorldPositionY();

		if (creature != nullptr) {
			auto parent = creature->getParent().get();

			if (parent != nullptr) {
				logEntry["parentOID"] = parent->getObjectID();

				if (parent->isCellObject()) {
					logEntry["positionX"] = (int)creature->getPositionX();
					logEntry["positionZ"] = (int)creature->getPositionZ();
					logEntry["positionY"] = (int)creature->getPositionY();
				}
			}

			auto zone = creature->getZone();

			logEntry["zone"] = zone != nullptr ? zone->getZoneName() : "null";
		}
	}

	writePlayerLogEntry(logEntry);
}

void PlayerManagerImplementation::writePlayerLog(PlayerObject* ghost, const String& msg, int logLevelType) {
	if (ghost == nullptr)
		return;

	Reference<CreatureObject*> creature = ghost->getParent().get()->asCreatureObject();

	if (creature == nullptr)
		return;

	PlayerManagerImplementation::writePlayerLog(creature, ghost, msg, logLevelType);
}

void PlayerManagerImplementation::writePlayerLog(CreatureObject* creature, const String& msg, int logLevelType) {
	if (creature == nullptr)
		return;

	Reference<PlayerObject*> ghost = creature->getPlayerObject();

	if (ghost == nullptr)
		return;

	PlayerManagerImplementation::writePlayerLog(creature, ghost, msg, logLevelType);
}

int PlayerManagerImplementation::getPlayerQuestID(const String& name) {
	for (int i = 0; i < questInfo.size(); ++i) {
		QuestInfo* quest = questInfo.get(i);

		if (quest != nullptr && quest->getQuestName().hashCode()== name.hashCode())
			return i;
	}

	return -1;
}

String PlayerManagerImplementation::getPlayerQuestParent(int questID) {
	QuestInfo* quest = questInfo.get(questID);

	if (quest != nullptr)
		return quest->getQuestParent();
	else
		return "";
}

bool PlayerManagerImplementation::existsName(const String& name) {
	bool res = false;

	try {
		res = nameMap->containsKey(name.toLowerCase());
	} catch (DatabaseException& e) {
		error(e.getMessage());
	}

	return res;
}

bool PlayerManagerImplementation::existsPlayerCreatureOID(uint64 oid) {
	return nameMap->containsOID(oid);
}

bool PlayerManagerImplementation::kickUser(const String& name, const String& admin, String& reason, bool doBan) {
	Reference<CreatureObject*> player = getPlayer(name);

	if (player == nullptr)
		return false;

	Reference<CreatureObject*> adminPlayer = getPlayer(admin);

	if (adminPlayer == nullptr)
		return false;

	info(true) << admin << (doBan ? " kickbanned " : " kicked ") << name << " for '" << reason << "'";

	StringBuffer buf;
	buf << "You have been kicked by " << admin << " for '" << reason << "'";
	auto kickMessage = buf.toString();

	player->sendSystemMessage(kickMessage);

	ErrorMessage* errmsg = new ErrorMessage(admin, kickMessage, 0);
	player->sendMessage(errmsg);

	Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

	if (ghost != nullptr)
		ghost->setLoggingOut();

	player->sendMessage(new LogoutMessage());

	Core::getTaskManager()->scheduleTask([
			this, playerCreo = Reference<CreatureObject*>(player), adminCreo = Reference<CreatureObject*>(adminPlayer), reason, doBan] {

		Reference<ZoneClientSession*> session = playerCreo->getClient();

		if (session != nullptr)
			session->disconnect(true);

		Reference<PlayerObject*> ghost = playerCreo->getSlottedObject("ghost").castTo<PlayerObject*>();

		if (ghost == nullptr)
			return;

		Reference<Account*> account = ghost->getAccount();

		if (account == nullptr)
			return;

		if (doBan) {
			Reference<PlayerObject*> adminGhost = adminCreo->getSlottedObject("ghost").castTo<PlayerObject*>();

			if (adminGhost != nullptr) {
				String banMessage = banAccount(adminGhost, account, 60 * 10, reason);

				adminCreo->sendSystemMessage(banMessage);
			}
		} else {
			adminCreo->sendSystemMessage("Kicked " + playerCreo->getFirstName() + " on account " + account->getUsername() + ".");
		}
	}, "kickUserTask", 500);

	return true;
}

Reference<CreatureObject*> PlayerManagerImplementation::getPlayer(const String& name) {
	uint64 oid = 0;

	try {
		oid = nameMap->get(name.toLowerCase());
	} catch (ArrayIndexOutOfBoundsException& ex) {
		// `oid` is initialized with zero so the method will return nullptr after unlocking.
		error("Didn't find player " +  name);
	}

	if (oid == 0)
		return nullptr;

	Reference<SceneObject*> obj = server->getObject(oid);

	if (obj == nullptr || !obj->isPlayerCreature())
		return nullptr;

	return obj.castTo<CreatureObject*>();
}

uint64 PlayerManagerImplementation::getObjectID(const String& name) {
	uint64 oid = 0;

	oid = nameMap->get(name.toLowerCase());

	return oid;
}

String PlayerManagerImplementation::getPlayerName(uint64 oid) {
	return nameMap->get(oid);
}

bool PlayerManagerImplementation::checkExistentNameInDatabase(const String& name) {
	if (name.isEmpty())
		return false;

	try {
		String fname = name.toLowerCase();
		Database::escapeString(fname);
		String query = "SELECT * FROM characters WHERE lower(firstname) = \""
				+ fname + "\"";

		UniqueReference<ResultSet*> res(ServerDatabase::instance()->executeQuery(query));
		bool nameExists = res->next();

		return !nameExists;
	} catch (DatabaseException& e) {
		return false;
	}

	return false;
}

bool PlayerManagerImplementation::checkPlayerName(ClientCreateCharacterCallback* callback) {
	auto client = callback->getClient();

	NameManager* nm = processor->getNameManager();
	BaseMessage* msg = nullptr;

	String firstName;

	UnicodeString unicodeName;
	callback->getCharacterName(unicodeName);

	String name = unicodeName.toString();

	//Get the firstname
	int idx = name.indexOf(" ");
	if (idx != -1)
		firstName = name.subString(0, idx);
	else
		firstName = name;

	//Does this name already exist?
	if (nameMap->containsKey(firstName.toLowerCase())) {
		msg = new ClientCreateCharacterFailed("name_declined_in_use");
		client->sendMessage(msg);

		return false;
	}

	//Check to see if name is valid
	int res = nm->validateName(name, callback->getSpecies());

	if (res != NameManagerResult::ACCEPTED) {
		switch (res) {
		case NameManagerResult::DECLINED_EMPTY:
			msg = new ClientCreateCharacterFailed("name_declined_empty");
			break;
		case NameManagerResult::DECLINED_DEVELOPER:
			msg = new ClientCreateCharacterFailed("name_declined_developer");
			break;
		case NameManagerResult::DECLINED_FICT_RESERVED:
			msg = new ClientCreateCharacterFailed("name_declined_fictionally_reserved");
			break;
		case NameManagerResult::DECLINED_PROFANE:
			msg = new ClientCreateCharacterFailed("name_declined_profane");
			break;
		case NameManagerResult::DECLINED_RACE_INAPP:
			msg = new ClientCreateCharacterFailed("name_declined_racially_inappropriate");
			break;
		case NameManagerResult::DECLINED_SYNTAX:
			msg = new ClientCreateCharacterFailed("name_declined_syntax");
			break;
		case NameManagerResult::DECLINED_RESERVED:
			msg = new ClientCreateCharacterFailed("name_declined_reserved");
			break;
		default:
			msg = new ClientCreateCharacterFailed("name_declined_retry");
			break;
		}

		client->sendMessage(msg); //Name failed filters
		return false;
	}

	return true;
}

String PlayerManagerImplementation::setFirstName(CreatureObject* creature, const String& newFirstName) {
    if (creature == nullptr)
		return "nullptr creature specified";

	if (!creature->isPlayerCreature())
		return "Can only set FirstName on players.";

	auto ghost = creature->getPlayerObject();

	if (ghost == nullptr)
		return "missing ghost";

	if (newFirstName.isEmpty())
		return "Empty first name is now allowed";

	if (existsName(newFirstName))
		return "That name is already in use";

	Locker locker(creature, ghost);

	auto nameManager = processor->getNameManager();

	int result = nameManager->validateName(newFirstName, creature->getSpecies());

	switch (result) {
	case NameManagerResult::ACCEPTED:
		break;
	case NameManagerResult::DECLINED_EMPTY:
		return "First names may not be empty.";
		break;
	case NameManagerResult::DECLINED_RACE_INAPP:
		return "That name is inappropriate for the player's species.";
		break;
	case NameManagerResult::DECLINED_PROFANE:
		return "That name is profane.";
		break;
	case NameManagerResult::DECLINED_DEVELOPER:
		return "That is a developer's name.";
		break;
	case NameManagerResult::DECLINED_FICT_RESERVED:
		return "That name is a reserved fictional name.";
		break;
	case NameManagerResult::DECLINED_RESERVED:
		return "That name is reserved.";
		break;
	case NameManagerResult::DECLINED_SYNTAX:
		return "That name contains invalid syntax.";
		break;
	}

	String oldFirstName = creature->getFirstName();
	String oldLastName = creature->getLastName();
	String newFullName = newFirstName;

	if (!oldLastName.isEmpty())
		newFullName = newFirstName + " " + oldLastName;

	creature->setCustomObjectName(newFullName, true);

	// If staff fix their staff tags
	if (ghost->hasGodMode())
		updatePermissionName(creature, ghost->getAdminLevel());

	auto chatManager = server->getChatManager();
	chatManager->removePlayer(oldFirstName);
	chatManager->addPlayer(creature);

	removePlayer(oldFirstName);
	addPlayer(creature);

	// Remove the old name from other people's friends lists
	ghost->removeAllReverseFriends(oldFirstName);

	// Update mysql characters table
	String characterFirstName = creature->getFirstName();
	Database::escapeString(characterFirstName);

	int galaxyID = server->getGalaxyID();

	StringBuffer charDirtyQuery;
	charDirtyQuery
			<< "UPDATE `characters_dirty` SET `firstname` = '"  << characterFirstName
			<< "' WHERE `character_oid` = '" << creature->getObjectID()
			<< "' AND `galaxy_id` = '" << galaxyID << "'";

	ServerDatabase::instance()->executeStatement(charDirtyQuery);

	StringBuffer charQuery;
	charQuery
			<< "UPDATE `characters` SET `firstname` = '"  << characterFirstName
			<< "' WHERE `character_oid` = '" << creature->getObjectID()
			<< "' AND `galaxy_id` = '" << galaxyID << "'";

	ServerDatabase::instance()->executeStatement(charQuery);

	// Success, return empty string
	return "";
}

String PlayerManagerImplementation::setLastName(CreatureObject* creature, const String& newLastName, bool skipVerify) {
	if (creature == nullptr)
		return "nullptr creature specified";

	if (!creature->isPlayerCreature())
		return "Can only set LastName on players.";

	auto ghost = creature->getPlayerObject();

	if (ghost == nullptr)
		return "missing ghost";

	Locker locker(creature, ghost);

	String oldFirstName = creature->getFirstName();
	String newFullName = oldFirstName;

	if (!newLastName.isEmpty())
		newFullName = oldFirstName + " " + newLastName;

	if (!skipVerify) {
		auto nameManager = processor->getNameManager();

		int result = nameManager->validateName(newFullName, creature->getSpecies());

		switch (result) {
		case NameManagerResult::ACCEPTED:
			break;
		case NameManagerResult::DECLINED_RACE_INAPP:
			return "That name is inappropriate for the player's species.";
			break;
		case NameManagerResult::DECLINED_PROFANE:
			return "That name is profane.";
			break;
		case NameManagerResult::DECLINED_DEVELOPER:
			return "That is a developer's name.";
			break;
		case NameManagerResult::DECLINED_FICT_RESERVED:
			return "That name is a reserved fictional name.";
			break;
		case NameManagerResult::DECLINED_RESERVED:
			return "That name is reserved.";
			break;
		case NameManagerResult::DECLINED_SYNTAX:
			return "That name contains invalid syntax.";
			break;
		}
	}

	creature->setCustomObjectName(newFullName, true);

	// If staff fix their staff tags
	if (ghost->hasGodMode())
		updatePermissionName(creature, ghost->getAdminLevel());

	// Update mysql characters table
	String characterLastName = creature->getLastName();
	Database::escapeString(characterLastName);

	int galaxyID = server->getGalaxyID();

	StringBuffer charDirtyQuery;
	charDirtyQuery
			<< "UPDATE `characters_dirty` SET `surname` = '"  << characterLastName
			<< "' WHERE `character_oid` = '" << creature->getObjectID()
			<< "' AND `galaxy_id` = '" << galaxyID << "'";

	ServerDatabase::instance()->executeStatement(charDirtyQuery);

	StringBuffer charQuery;
	charQuery
			<< "UPDATE `characters` SET `surname` = '"  << characterLastName
			<< "' WHERE `character_oid` = '" << creature->getObjectID()
			<< "' AND `galaxy_id` = '" << galaxyID << "'";

	ServerDatabase::instance()->executeStatement(charQuery);

	// Success, return empty string
	return "";
}

void PlayerManagerImplementation::createTutorialBuilding(CreatureObject* player) {
	Zone* zone = server->getZone("tutorial");

	if (zone == nullptr) {
		error("Character creation failed, tutorial zone disabled.");
		return;
	}

	Reference<TutorialBuildingObject*> tutorial = server->createObject(STRING_HASHCODE("object/building/general/newbie_hall.iff"), 1).castTo<TutorialBuildingObject*>();

	if (tutorial == nullptr) {
		error("Tutorial building creation failed.");
		return;
	}

	Locker locker(tutorial);

	tutorial->createCellObjects();
	tutorial->setPublicStructure(true);
	tutorial->setTutorialOwnerID(player->getObjectID());

	tutorial->initializePosition(System::random(5000), 0, System::random(5000));
	zone->transferObject(tutorial, -1, true);

	locker.release();

	SceneObject* cellTut = tutorial->getCell(11);

	SceneObject* cellTutPlayer = tutorial->getCell(1);

	player->initializePosition(0, 0, -3);

	cellTutPlayer->transferObject(player, -1);
	PlayerObject* ghost = player->getPlayerObject();
	ghost->setSavedTerrainName(zone->getZoneName());
	ghost->setSavedParentID(cellTutPlayer->getObjectID());

	tutorial->updateToDatabase();
}

void PlayerManagerImplementation::createSkippedTutorialBuilding(CreatureObject* player) {
	Zone* zone = server->getZone("tutorial");

	if (zone == nullptr) {
		error("Character creation failed, tutorial zone disabled.");
		return;
	}


	Reference<BuildingObject*> tutorial = server->createObject(STRING_HASHCODE("object/building/general/newbie_hall_skipped.iff"), 1).castTo<BuildingObject*>();

	Locker locker(tutorial);

	tutorial->createCellObjects();
	tutorial->initializePosition(System::random(5000), 0, System::random(5000));
	zone->transferObject(tutorial, -1, true);

	locker.release();

	Reference<SceneObject*> travelTutorialTerminal = server->createObject(STRING_HASHCODE("object/tangible/terminal/terminal_travel_tutorial.iff"), 1);

	SceneObject* cellTut = tutorial->getCell(1);

	Locker locker2(travelTutorialTerminal);

	cellTut->transferObject(travelTutorialTerminal, -1);

	travelTutorialTerminal->initializePosition(27.0f, -3.5f, -168.0f);

	player->initializePosition(27.0f, -3.5f, -165.0f);
	cellTut->transferObject(player, -1);
	PlayerObject* ghost = player->getPlayerObject();
	ghost->setSavedTerrainName(zone->getZoneName());
	ghost->setSavedParentID(cellTut->getObjectID());

	tutorial->updateToDatabase();
}

uint8 PlayerManagerImplementation::calculateIncapacitationTimer(CreatureObject* playerCreature, int condition) {
	// Switch the sign of the value
	int value = -condition;

	if (value < 0)
		return 0;

	int recoveryTime = (value / 5); // In seconds - 3 seconds is recoveryEvent timer

	// Recovery time cannot be higher than 60 seconds.
	recoveryTime = (recoveryTime > 60) ? 60 : recoveryTime;

	//Check for incap recovery food buff - overrides recovery time gate.
	if (playerCreature->hasBuff(STRING_HASHCODE("food.incap_recovery"))) {
		Buff* buff = playerCreature->getBuff(STRING_HASHCODE("food.incap_recovery"));

		if (buff != nullptr) {
			float percent = buff->getSkillModifierValue("incap_recovery");

			recoveryTime = round(recoveryTime * ((100.0f - percent) / 100.0f));

			StringIdChatParameter message("combat_effects", "incap_recovery");
			message.setDI(recoveryTime);
			playerCreature->sendSystemMessage(message); // Incapacitation recovery time reduced by %DI%.

			playerCreature->removeBuff(buff);
		}
	}

	return recoveryTime;
}

int PlayerManagerImplementation::notifyDestruction(TangibleObject* destructor, TangibleObject* destructedObject, int condition, bool isCombatAction) {
	fatal(destructor, "destructor cant be nullptr.");

	if (!destructedObject->isPlayerCreature())
		return 1;

	CreatureObject* playerCreature = cast<CreatureObject*>(destructedObject);

	if ((playerCreature->isIncapacitated() && !(playerCreature->isFeigningDeath())) || playerCreature->isDead())
		return 1;

	if (playerCreature->isRidingMount()) {
		playerCreature->updateCooldownTimer("mount_dismount", 0);
		playerCreature->executeObjectControllerAction(STRING_HASHCODE("dismount"));
	}

	PlayerObject* ghost = playerCreature->getPlayerObject();

	ghost->addIncapacitationTime();

	const DeltaVector<ManagedReference<SceneObject*> >* defenderList = destructor->getDefenderList();

	bool isDefender = false;

	if (defenderList->contains(destructedObject)) {
		isDefender = true;
		destructor->removeDefender(destructedObject);
	}

	if (defenderList->size() == 0) {
		destructor->clearCombatState(false);
	}

	if ((destructor->isKiller() && isDefender) || ghost->getIncapacitationCounter() >= 3) {
		killPlayer(destructor, playerCreature, 0, isCombatAction);
	} else {

		playerCreature->setPosture(CreaturePosture::INCAPACITATED, !isCombatAction, !isCombatAction);
		playerCreature->clearCombatState(false);
		playerCreature->clearState(CreatureState::FEIGNDEATH); // We got incapped for real - Remove the state so we can be DB'd

		uint8 incapTime = calculateIncapacitationTimer(playerCreature, condition);
		playerCreature->setCountdownTimer((uint32) incapTime, true);

		Reference<Task*> oldTask = playerCreature->getPendingTask("incapacitationRecovery");

		if (oldTask != nullptr && oldTask->isScheduled()) {
			oldTask->cancel();
			playerCreature->removePendingTask("incapacitationRecovery");
		}

		Reference<Task*> task = new PlayerIncapacitationRecoverTask(playerCreature, false);
		playerCreature->addPendingTask("incapacitationRecovery", task, incapTime * 1000);

		StringIdChatParameter toVictim;

		toVictim.setStringId("base_player", "prose_victim_incap");
		toVictim.setTT(destructor->getDisplayedName());

		playerCreature->sendSystemMessage(toVictim);

		if (destructor->isPlayerCreature()) {
			ManagedReference<CreatureObject*> destructorCreature = destructor->asCreatureObject();

			PlayerObject* attackerGhost = destructorCreature->getPlayerObject();
			PlayerObject* victimGhost = playerCreature->getPlayerObject();

			if (attackerGhost != nullptr && victimGhost != nullptr) {
				FrsData* attackerData = attackerGhost->getFrsData();
				int attackerCouncil = attackerData->getCouncilType();

				FrsData* victimData = victimGhost->getFrsData();
				int victimCouncil = victimData->getCouncilType();

				ManagedReference<FrsManager*> strongMan = playerCreature->getZoneServer()->getFrsManager();
				ManagedReference<CreatureObject*> strongRef = playerCreature->asCreatureObject();

				if (attackerCouncil == FrsManager::COUNCIL_DARK && victimCouncil == FrsManager::COUNCIL_DARK) {
					Core::getTaskManager()->executeTask([strongRef, destructorCreature, attackerCouncil, victimCouncil, strongMan] () {
						bool isFrsBattle = false;

						if (attackerCouncil == FrsManager::COUNCIL_DARK && victimCouncil == FrsManager::COUNCIL_DARK)
							isFrsBattle = strongMan->handleDarkCouncilIncap(destructorCreature, strongRef);

						if (!isFrsBattle) {
							StringIdChatParameter toKiller;

							toKiller.setStringId("base_player", "prose_target_incap");
							toKiller.setTT(strongRef->getDisplayedName());

							destructorCreature->sendSystemMessage(toKiller);
						}
					}, "PvPFRSIncapTask");
				} else {
					StringIdChatParameter toKiller;

					toKiller.setStringId("base_player", "prose_target_incap");
					toKiller.setTT(playerCreature->getDisplayedName());

					destructorCreature->sendSystemMessage(toKiller);
				}
			}
		}
	}

	if (playerCreature->isWatching()) {
		uint64 watchID = playerCreature->getWatchToID();
		stopWatch(playerCreature, watchID, false);
	} else if (playerCreature->isListening()) {
		uint64 listenID = playerCreature->getListenID();
		stopListen(playerCreature, listenID, false);
	}

	ThreatMap* destructorThreatMap = destructor->getThreatMap();

	if (destructorThreatMap != nullptr) {
		for (int i = 0; i < destructorThreatMap->size(); i++) {
			TangibleObject* destructedTano = destructorThreatMap->elementAt(i).getKey();

			if (destructedTano == destructedObject) {
				destructorThreatMap->remove(i);
			}
		}
	}

	return 0;
}

void PlayerManagerImplementation::killPlayer(TangibleObject* attacker, CreatureObject* player, int typeofdeath, bool isCombatAction) {
	StringIdChatParameter stringId;

	if (player->isRidingMount()) {
		player->updateCooldownTimer("mount_dismount", 0);
		player->executeObjectControllerAction(STRING_HASHCODE("dismount"));
	}

	player->clearCombatState(true);

	player->clearDots();

	player->setPosture(CreaturePosture::DEAD, !isCombatAction, !isCombatAction);

	sendActivateCloneRequest(player, typeofdeath);

	stringId.setStringId("base_player", "prose_victim_dead");
	stringId.setTT(attacker->getDisplayedName());
	player->sendSystemMessage(stringId);

	player->updateTimeOfDeath();
	player->clearBuffs(true, false);

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost != nullptr) {
		ghost->resetIncapacitationTimes();
		if (ghost->hasTef()) {
			ghost->schedulePvpTefRemovalTask(true, true, true);
		}
	}

	ThreatMap* threatMap = player->getThreatMap();

	if (attacker->getFaction() != 0) {
		if (attacker->isPlayerCreature() || attacker->isPet()) {
			CreatureObject* attackerCreature = attacker->asCreatureObject();

			if (attackerCreature->isPet()) {
				CreatureObject* owner = attackerCreature->getLinkedCreature().get();

				if (owner != nullptr && owner->isPlayerCreature()) {
					attackerCreature = owner;
				}
			}

			if (attackerCreature->isPlayerCreature()) {
				if (!CombatManager::instance()->areInDuel(attackerCreature, player)) {
					FactionManager::instance()->awardPvpFactionPoints(attackerCreature, player);
				}
			}

			PlayerObject* attackerGhost = attackerCreature->getPlayerObject();
			PlayerObject* victimGhost = player->getPlayerObject();

			if (attackerGhost != nullptr && victimGhost != nullptr) {
				FrsData* attackerData = attackerGhost->getFrsData();
				int attackerCouncil = attackerData->getCouncilType();

				FrsData* victimData = victimGhost->getFrsData();
				int victimCouncil = victimData->getCouncilType();

				if (attackerCouncil == FrsManager::COUNCIL_DARK && victimCouncil == FrsManager::COUNCIL_DARK) {
					ManagedReference<FrsManager*> strongMan = player->getZoneServer()->getFrsManager();
					ManagedReference<CreatureObject*> attackerStrongRef = attackerCreature->asCreatureObject();
					ManagedReference<CreatureObject*> playerStrongRef = player->asCreatureObject();

					Reference<ThreatMap*> copyThreatMap = new ThreatMap(*threatMap);

					Core::getTaskManager()->executeTask([attackerStrongRef, playerStrongRef, strongMan, copyThreatMap] () {
						if (!strongMan->handleDarkCouncilDeath(attackerStrongRef, playerStrongRef))
							strongMan->handleSuddenDeathLoss(playerStrongRef, copyThreatMap);
					}, "PvPFRSKillTask");
				}
			}
		}
	}

	CombatManager::instance()->freeDuelList(player, false);

	if (attacker->isPlayerCreature()) {
		ManagedReference<CreatureObject*> playerRef = player->asCreatureObject();

		stringId.setStringId("base_player", "prose_target_dead");
		stringId.setTT(player->getDisplayedName());
		playerRef->sendSystemMessage(stringId);

		Reference<ThreatMap*> copyThreatMap = new ThreatMap(*threatMap);

		Core::getTaskManager()->executeTask([=] () {
			if (playerRef != nullptr) {
				Locker locker(playerRef);
				doPvpDeathRatingUpdate(playerRef, copyThreatMap);
			}
		}, "PvpDeathRatingUpdateLambda");
	}

	threatMap->removeAll(true);

	player->dropFromDefenderLists();
	player->setTargetID(0, true);

	player->notifyObjectKillObservers(attacker);
}

void PlayerManagerImplementation::sendActivateCloneRequest(CreatureObject* player, int typeofdeath) {
	Zone* zone = player->getZone();

	if (zone == nullptr)
		return;

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr)
		return;

	ghost->removeSuiBoxType(SuiWindowType::CLONE_REQUEST);

	ManagedReference<SuiListBox*> cloneMenu = new SuiListBox(player, SuiWindowType::CLONE_REQUEST);
	cloneMenu->setCallback(new CloningRequestSuiCallback(player->getZoneServer(), typeofdeath));
	cloneMenu->setPromptTitle("@base_player:revive_title");

	uint64 preDesignatedFacilityOid = ghost->getCloningFacility();
	ManagedReference<SceneObject*> preDesignatedFacility = server->getObject(preDesignatedFacilityOid);
	String predesignatedName = "None";

	//Get the name of the pre-designated facility
	if (preDesignatedFacility != nullptr) {
		ManagedReference<CityRegion*> cr = preDesignatedFacility->getCityRegion().get();

		if (preDesignatedFacility->getZone() != zone) {
			predesignatedName = "off-planet (unavailable)";
		} else if (cr != nullptr) {
			predesignatedName = cr->getRegionDisplayedName();
		} else {
			predesignatedName = preDesignatedFacility->getDisplayedName();
		}
	}

	SortedVector<ManagedReference<SceneObject*> > locations = zone->getPlanetaryObjectList("cloningfacility");

	ManagedReference<SceneObject*> closestCloning = zone->getNearestPlanetaryObject(player, "cloningfacility");
	if (closestCloning == nullptr) {
		warning("nearest cloning facility for player is nullptr");
		return;
	}
	String closestName = "None";

	//Check if player is city banned where the closest facility is or if it's not a valid cloner
	if (!isValidClosestCloner(player, closestCloning)) {
		int distance = 50000;
		for (int j = 0; j < locations.size(); j++) {
			ManagedReference<SceneObject*> location = locations.get(j);

			if (!isValidClosestCloner(player, location))
				continue;

			ManagedReference<CityRegion*> cr = location->getCityRegion().get();

			String name = "";

			if (cr != nullptr) {
				name = cr->getRegionDisplayedName();
			} else {
				name = location->getDisplayedName();
			}

			if (location->getDistanceTo(player) < distance) {
				distance = location->getDistanceTo(player);
				closestName = name;
				closestCloning = location;
			}
		}

	} else {
		ManagedReference<CityRegion*> cr = closestCloning->getCityRegion().get();

		if (cr != nullptr)
			closestName = cr->getRegionDisplayedName();
		else
			closestName = closestCloning->getDisplayedName();
	}

	StringBuffer promptText;
	promptText << "Closest:\t\t " << closestName << "\n"
			<< "Pre-Designated: " << predesignatedName << "\n"
			<< "Cash Balance:\t " << player->getCashCredits() << "\n\n"
			<< "Select the desired option and click OK.";

	cloneMenu->setPromptText(promptText.toString());

	if (closestCloning != nullptr)
		cloneMenu->addMenuItem("@base_player:revive_closest", closestCloning->getObjectID());

	if (preDesignatedFacility != nullptr && preDesignatedFacility->getZone() == zone)
		cloneMenu->addMenuItem("@base_player:revive_bind", preDesignatedFacility->getObjectID());

	for (int i = 0; i < locations.size(); i++) {
		ManagedReference<SceneObject*> loc = locations.get(i);

		if (loc == nullptr)
			continue;

		CloningBuildingObjectTemplate* cbot = cast<CloningBuildingObjectTemplate*>(loc->getObjectTemplate());

		if (cbot == nullptr)
			continue;

		if (cbot->getFacilityType() == CloningBuildingObjectTemplate::JEDI_ONLY && player->hasSkill("force_title_jedi_rank_01")) {
			String name = "Force Shrine (" + String::valueOf((int)loc->getWorldPositionX()) + ", " + String::valueOf((int)loc->getWorldPositionY()) + ")";
			cloneMenu->addMenuItem(name, loc->getObjectID());
		} else if ((cbot->getFacilityType() == CloningBuildingObjectTemplate::LIGHT_JEDI_ONLY && player->hasSkill("force_rank_light_novice")) ||
				(cbot->getFacilityType() == CloningBuildingObjectTemplate::DARK_JEDI_ONLY && player->hasSkill("force_rank_dark_novice"))) {
			FrsManager* frsManager = server->getFrsManager();

			if (frsManager != nullptr && frsManager->isFrsEnabled()) {
				String name = "Jedi Enclave (" + String::valueOf((int)loc->getWorldPositionX()) + ", " + String::valueOf((int)loc->getWorldPositionY()) + ")";
				cloneMenu->addMenuItem(name, loc->getObjectID());
			}
		}
	}

	ghost->addSuiBox(cloneMenu);
	player->sendMessage(cloneMenu->generateMessage());
}

bool PlayerManagerImplementation::isValidClosestCloner(CreatureObject* player, SceneObject* cloner) {
	if (cloner == nullptr)
		return false;

	ManagedReference<CityRegion*> cr = cloner->getCityRegion().get();

	if (cr != nullptr && cr->isBanned(player->getObjectID()))
		return false;

	CloningBuildingObjectTemplate* cbot = cast<CloningBuildingObjectTemplate*>(cloner->getObjectTemplate());

	if (cbot == nullptr)
		return false;

	if (cbot->getFacilityType() == CloningBuildingObjectTemplate::FACTION_IMPERIAL && player->getFaction() != Factions::FACTIONIMPERIAL)
		return false;

	if (cbot->getFacilityType() == CloningBuildingObjectTemplate::FACTION_REBEL && player->getFaction() != Factions::FACTIONREBEL)
		return false;

	if (cbot->isJediCloner())
		return false;

	return true;
}

void PlayerManagerImplementation::sendPlayerToCloner(CreatureObject* player, uint64 clonerID, int typeofdeath) {
	ManagedReference<SceneObject*> cloner = server->getObject(clonerID);

	if (cloner == nullptr) {
		error("Cloning structure is null");
		return;
	}

	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr)	{
		error("The player to be cloned is null");
		return;
	}


	CloningBuildingObjectTemplate* cbot = cast<CloningBuildingObjectTemplate*>(cloner->getObjectTemplate());

	if (cbot == nullptr) {
		error("Not a cloning building template.");
		return;
	}

	CloneSpawnPoint* clonePoint = cbot->getRandomSpawnPoint();

	if (clonePoint == nullptr) {
		error("clone point null");
		return;
	}

	Coordinate* coordinate = clonePoint->getCoordinate();
	Quaternion* direction = clonePoint->getDirection();

	int cellID = clonePoint->getCellID();
	SceneObject* cell = nullptr;

	if (cellID != 0) {
		BuildingObject* cloningBuilding = cloner.castTo<BuildingObject*>();

		if (cloningBuilding == nullptr)  {
			error("Cloning building is null");
			return;
		}

		cell = cloningBuilding->getCell(cellID);

		if (cell == nullptr) {
			StringBuffer msg;
			msg << "null cell for cellID " << cellID << " in building: " << cbot->getFullTemplateString();
			error(msg.toString());
			return;
		}
	}

	Zone* zone = player->getZone();

	ghost->setCloning(true);

	if (cellID == 0)
		player->switchZone(zone->getZoneName(), cloner->getWorldPositionX() + coordinate->getPositionX(), cloner->getWorldPositionZ() + coordinate->getPositionZ(), cloner->getWorldPositionY() + coordinate->getPositionY(), 0);
	else
		player->switchZone(zone->getZoneName(), coordinate->getPositionX(), coordinate->getPositionZ(), coordinate->getPositionY(), cell->getObjectID());

	uint64 preDesignatedFacilityOid = ghost->getCloningFacility();
	ManagedReference<SceneObject*> preDesignatedFacility = server->getObject(preDesignatedFacilityOid);

	if (preDesignatedFacility == nullptr || preDesignatedFacility != cloner) {
		player->addWounds(CreatureAttribute::HEALTH, 100, true, false);
		player->addWounds(CreatureAttribute::ACTION, 100, true, false);
		player->addWounds(CreatureAttribute::MIND, 100, true, false);
		player->addShockWounds(100, true);
	}

	if (player->getFactionStatus() != FactionStatus::ONLEAVE && cbot->getFacilityType() != CloningBuildingObjectTemplate::FACTION_IMPERIAL && cbot->getFacilityType() != CloningBuildingObjectTemplate::FACTION_REBEL && !player->hasSkill("force_title_jedi_rank_03"))
		player->setFactionStatus(FactionStatus::ONLEAVE);

	SortedVector<ManagedReference<SceneObject*> > insurableItems = getInsurableItems(player, false);

	// Decay
	if (typeofdeath == 0 && insurableItems.size() > 0) {

		ManagedReference<SuiListBox*> suiCloneDecayReport = new SuiListBox(player, SuiWindowType::CLONE_REQUEST_DECAY, SuiListBox::HANDLESINGLEBUTTON);
		suiCloneDecayReport->setPromptTitle("DECAY REPORT");
		suiCloneDecayReport->setPromptText("The following report summarizes the status of your items after the decay event.");
		suiCloneDecayReport->addMenuItem("\\#00FF00DECAYED ITEMS");

		for (int i = 0; i < insurableItems.size(); i++) {
			SceneObject* item = insurableItems.get(i);

			if (item != nullptr && item->isTangibleObject()) {
				ManagedReference<TangibleObject*> obj = cast<TangibleObject*>(item);

				Locker clocker(obj, player);

				if (obj->getOptionsBitmask() & OptionBitmask::INSURED) {
					//1% Decay for insured items
					obj->inflictDamage(obj, 0, 0.01 * obj->getMaxCondition(), true, true);
					//Set uninsured
					uint32 bitmask = obj->getOptionsBitmask() - OptionBitmask::INSURED;
					obj->setOptionsBitmask(bitmask);
				} else {
					//5% Decay for uninsured items
					obj->inflictDamage(obj, 0, 0.05 * obj->getMaxCondition(), true, true);
				}

				// Calculate condition percentage for decay report
				int max = obj->getMaxCondition();
				int min = max - obj->getConditionDamage();
				int condPercentage = ( min / (float)max ) * 100.0f;
				String line = " - " + obj->getDisplayedName() + " (@"+String::valueOf(condPercentage)+"%)";

				suiCloneDecayReport->addMenuItem(line, item->getObjectID());
			}
		}

		ghost->addSuiBox(suiCloneDecayReport);
		player->sendMessage(suiCloneDecayReport->generateMessage());

	}



	Reference<Task*> task = new PlayerIncapacitationRecoverTask(player, true);
	task->schedule(3 * 1000);

	player->notifyObservers(ObserverEventType::PLAYERCLONED, player, 0);


	// Jedi experience loss.
	if (ghost->getJediState() >= 2) {
		int jediXpCap = ghost->getXpCap("jedi_general");
		int xpLoss = (int)(jediXpCap * -0.05);
		int curExp = ghost->getExperience("jedi_general");

		int negXpCap = -10000000; // Cap on negative jedi experience

		if ((curExp + xpLoss) < negXpCap)
			xpLoss = negXpCap - curExp;

		awardExperience(player, "jedi_general", xpLoss, true);
		StringIdChatParameter message("base_player","prose_revoke_xp");
		message.setDI(xpLoss * -1);
		message.setTO("exp_n", "jedi_general");
		player->sendSystemMessage(message);
	}
}

void PlayerManagerImplementation::ejectPlayerFromBuilding(CreatureObject* player) {
	Zone* zone = player->getZone();

	if (zone == nullptr)
		return;

	ManagedReference<SceneObject*> parent = player->getParent().get();

	if (parent == nullptr || !parent->isCellObject())
		return;

	ManagedReference<CellObject*> cell = cast<CellObject*>(parent.get());

	if (cell == nullptr)
		return;

	ManagedReference<BuildingObject*> building = cell->getParent().get().castTo<BuildingObject*>();

	if (building == nullptr)
		return;

	if (building->hasTemplateEjectionPoint()) {
		Vector3 ejectionPoint = building->getEjectionPoint();
		player->switchZone(zone->getZoneName(), ejectionPoint.getX(), ejectionPoint.getZ(), ejectionPoint.getY(), 0);
	}
}



void PlayerManagerImplementation::disseminateExperience(TangibleObject* destructedObject, ThreatMap* threatMap,
		SynchronizedVector<ManagedReference<CreatureObject*> >* spawnedCreatures,Zone* lairZone) {
	uint32 totalDamage = threatMap->getTotalDamage();

	if (totalDamage == 0) {
		threatMap->removeAll();
		return;
	}

	VectorMap<ManagedReference<CreatureObject*>, int> slExperience;
	slExperience.setAllowOverwriteInsertPlan();
	slExperience.setNullValue(0);


	float gcwBonus = 1.0f;
	uint32 winningFaction = -1;
	int baseXp = 0;
	Zone* zone = lairZone;
	if (zone==nullptr) {
		zone = destructedObject->getZone();
	}
	if (zone != nullptr) {
		GCWManager* gcwMan = zone->getGCWManager();

		if (gcwMan != nullptr) {
			gcwBonus += (gcwMan->getGCWXPBonus() / 100.0f);
			winningFaction = gcwMan->getWinningFaction();
		}
	}

	if (!destructedObject->isCreatureObject() && spawnedCreatures != nullptr) {
		ManagedReference<AiAgent*> ai = nullptr;

		for (int i = 0; i < spawnedCreatures->size(); i++) {
			ai = cast<AiAgent*>(spawnedCreatures->get(i).get());

			if (ai != nullptr) {
				Creature* creature = cast<Creature*>(ai.get());

				if (creature != nullptr && creature->isBaby())
					continue;
				else
					break;
			}
		}

		if (ai != nullptr)
			baseXp = ai->getBaseXp();

	} else {
		ManagedReference<AiAgent*> ai = cast<AiAgent*>(destructedObject);

		if (ai != nullptr)
			baseXp = ai->getBaseXp();
	}

	for (int i = 0; i < threatMap->size(); ++i) {
		ThreatMapEntry* entry = &threatMap->elementAt(i).getValue();
		TangibleObject* attacker = threatMap->elementAt(i).getKey();

		if (entry == nullptr || attacker == nullptr) {
			continue;
		}

		if (attacker->isPet()) {
			if (!attacker->isCreatureObject()) {
				continue;
			}

			CreatureObject* attackerCreo = attacker->asCreatureObject();

			if (attackerCreo == nullptr) {
				continue;
			}

			PetControlDevice* pcd = attackerCreo->getControlDevice().get().castTo<PetControlDevice*>();

			// only creature pets will award exp, so discard anything else
			if (pcd == nullptr || pcd->getPetType() != PetManager::CREATUREPET) {
				continue;
			}

			CreatureObject* owner = attackerCreo->getLinkedCreature().get();

			if (owner == nullptr || !owner->isPlayerCreature()) {
				continue;
			}

			Locker crossLocker(owner, destructedObject);

			PlayerObject* ownerGhost = owner->getPlayerObject();
			if (ownerGhost == nullptr || !owner->hasSkill("outdoors_creaturehandler_novice") || !destructedObject->isInRange(owner, 80)) {
				continue;
			}

			int totalPets = 1;

			for (int i = 0; i < ownerGhost->getActivePetsSize(); i++) {
				ManagedReference<AiAgent*> object = ownerGhost->getActivePet(i);

				if (object != nullptr && object->isCreature()) {
					if (object == attacker)
						continue;

					PetControlDevice* petControlDevice = object->getControlDevice().get().castTo<PetControlDevice*>();
					if (petControlDevice != nullptr && petControlDevice->getPetType() == PetManager::CREATUREPET)
						totalPets++;
				}
			}

			// TODO: Find a more correct CH xp formula
			float levelRatio = (float)destructedObject->getLevel() / (float)attacker->getLevel();

			float xpAmount = levelRatio * 500.f;

			if (levelRatio <= 0.5) {
				xpAmount = 1;
			} else {
				xpAmount = Math::min(xpAmount, (float)attacker->getLevel() * 50.f);
				xpAmount /= totalPets;

				if (winningFaction == attacker->getFaction())
					xpAmount *= gcwBonus;
			}

			awardExperience(owner, "creaturehandler", xpAmount);
		} else if (attacker->isPlayerCreature()) {
			if (!(attacker->getZone() == zone && destructedObject->isInRangeZoneless(attacker, 80))) {
				continue;
			}

			CreatureObject* attackerCreo = attacker->asCreatureObject();

			if (attackerCreo == nullptr) {
				continue;
			}

			ManagedReference<GroupObject*> group = attackerCreo->getGroup();

			uint32 combatXp = 0;

			Locker crossLocker(attackerCreo, destructedObject);

			for (int j = 0; j < entry->size(); ++j) {
				uint32 damage = entry->elementAt(j).getValue();
				String xpType = entry->elementAt(j).getKey();
				float xpAmount = baseXp;

				xpAmount *= (float) damage / totalDamage;

				//Cap xp based on level
				xpAmount = Math::min(xpAmount, calculatePlayerLevel(attackerCreo, xpType) * 300.f);

				//Apply group bonus if in group
				if (group != nullptr)
					xpAmount *= groupExpMultiplier;

				if (winningFaction == attackerCreo->getFaction())
					xpAmount *= gcwBonus;

				// Jedi experience doesn't count towards combat experience, and is earned at 20% the rate of normal experience
				if (xpType != "jedi_general")
					combatXp += xpAmount;
				else
					xpAmount *= 0.2f;

				if (xpType == "dotDMG") { // Prevents XP generated from DoTs from applying to the equiped weapon, but still counts towards combat XP
					continue;
				}

				//Award individual expType
				awardExperience(attackerCreo, xpType, xpAmount);
			}

			awardExperience(attackerCreo, "combat_general", combatXp, true, 0.1f);


			//Check if the group leader is a squad leader
			if (group == nullptr)
				continue;

			//Calculate squad leader group size experience @ 10% person + combat experience which is 10% of the variable
			float squadXp = (combatXp * 0.1f) + (combatXp * 0.1f * group->getGroupSize());

			Vector3 pos(attacker->getWorldPosition());

			crossLocker.release();

			ManagedReference<CreatureObject*> groupLeader = group->getLeader();

			if (groupLeader == nullptr || !groupLeader->isPlayerCreature())
				continue;

			Locker squadLock(groupLeader, destructedObject);

			//If he is a squad leader, and is in range of this player, then add the combat exp for him to use.
			//Removed distance check to keep current functionality. Attacker was previously comparing its location to itself
			if (groupLeader->hasSkill("outdoors_squadleader_novice")) {
				int v = slExperience.get(groupLeader) + squadXp;
				slExperience.put(groupLeader, v);
			}
		}
	}




	//Send out squad leader experience.
	for (int i = 0; i < slExperience.size(); ++i) {
		VectorMapEntry<ManagedReference<CreatureObject*>, int>* entry = &slExperience.elementAt(i);
		CreatureObject* leader = entry->getKey();

		if (leader == nullptr)
			continue;

		Locker clock(leader, destructedObject);

		awardExperience(leader, "squadleader", entry->getValue() * 2.f);
	}

	threatMap->removeAll();
}

bool PlayerManagerImplementation::checkEncumbrancies(CreatureObject* player, ArmorObject* armor) {
	int strength = player->getHAM(CreatureAttribute::STRENGTH);
	int constitution = player->getHAM(CreatureAttribute::CONSTITUTION);
	int quickness = player->getHAM(CreatureAttribute::QUICKNESS);
	int stamina = player->getHAM(CreatureAttribute::STAMINA);
	int focus = player->getHAM(CreatureAttribute::FOCUS);
	int willpower = player->getHAM(CreatureAttribute::WILLPOWER);

	int healthEncumb = armor->getHealthEncumbrance();
	int actionEncumb = armor->getActionEncumbrance();
	int mindEncumb = armor->getMindEncumbrance();

	if (healthEncumb <= 0 && actionEncumb <= 0 && mindEncumb <= 0)
		return true;

	if (healthEncumb >= strength || healthEncumb >= constitution ||
			actionEncumb >= quickness || actionEncumb >= stamina ||
			mindEncumb >= focus || mindEncumb >= willpower) {
		player->sendSystemMessage("@system_msg:equip_armor_fail"); // You are not healthy enough to wear this armor!

		if (healthEncumb >= strength) {
			int statStr = (healthEncumb - strength) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose"); // You need %DI more %TT to wear this armor.
			params.setDI(statStr);
			params.setTT("@att_n:strength");
			player->sendSystemMessage(params);
		}

		if (healthEncumb >= constitution) {
			int statCon = (healthEncumb - constitution) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose");
			params.setDI(statCon);
			params.setTT("@att_n:constitution");
			player->sendSystemMessage(params);
		}

		if (actionEncumb >= quickness) {
			int statQuick = (actionEncumb - quickness) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose");
			params.setDI(statQuick);
			params.setTT("@att_n:quickness");
			player->sendSystemMessage(params);
		}

		if (actionEncumb >= stamina) {
			int statStam = (actionEncumb - stamina) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose");
			params.setDI(statStam);
			params.setTT("@att_n:stamina");
			player->sendSystemMessage(params);
		}

		if (mindEncumb >= focus) {
			int statFoc = (mindEncumb - focus) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose");
			params.setDI(statFoc);
			params.setTT("@att_n:focus");
			player->sendSystemMessage(params);
		}

		if (mindEncumb >= willpower) {
			int statWill = (mindEncumb - willpower) + 1;
			StringIdChatParameter params("@system_msg:equip_armor_fail_prose");
			params.setDI(statWill);
			params.setTT("@att_n:willpower");
			player->sendSystemMessage(params);
		}

		return false;
	}
	else
		return true;
}

void PlayerManagerImplementation::applyEncumbrancies(CreatureObject* player, ArmorObject* armor) {
	int healthEncumb = Math::max(0, armor->getHealthEncumbrance());
	int actionEncumb = Math::max(0, armor->getActionEncumbrance());
	int mindEncumb = Math::max(0, armor->getMindEncumbrance());

	player->addEncumbrance(CreatureEncumbrance::HEALTH, healthEncumb, true);
	player->addEncumbrance(CreatureEncumbrance::ACTION, actionEncumb, true);
	player->addEncumbrance(CreatureEncumbrance::MIND, mindEncumb, true);

	player->inflictDamage(player, CreatureAttribute::STRENGTH, healthEncumb, true);
	player->addMaxHAM(CreatureAttribute::STRENGTH, -healthEncumb, true);

	player->inflictDamage(player, CreatureAttribute::CONSTITUTION, healthEncumb, true);
	player->addMaxHAM(CreatureAttribute::CONSTITUTION, -healthEncumb, true);

	player->inflictDamage(player, CreatureAttribute::QUICKNESS, actionEncumb, true);
	player->addMaxHAM(CreatureAttribute::QUICKNESS, -actionEncumb, true);

	player->inflictDamage(player, CreatureAttribute::STAMINA, actionEncumb, true);
	player->addMaxHAM(CreatureAttribute::STAMINA, -actionEncumb, true);

	player->inflictDamage(player, CreatureAttribute::FOCUS, mindEncumb, true);
	player->addMaxHAM(CreatureAttribute::FOCUS, -mindEncumb, true);

	player->inflictDamage(player, CreatureAttribute::WILLPOWER, mindEncumb, true);
	player->addMaxHAM(CreatureAttribute::WILLPOWER, -mindEncumb, true);
}

void PlayerManagerImplementation::removeEncumbrancies(CreatureObject* player, ArmorObject* armor) {
	int healthEncumb = Math::max(0, armor->getHealthEncumbrance());
	int actionEncumb = Math::max(0, armor->getActionEncumbrance());
	int mindEncumb = Math::max(0, armor->getMindEncumbrance());

	player->addEncumbrance(CreatureEncumbrance::HEALTH, -healthEncumb, true);
	player->addEncumbrance(CreatureEncumbrance::ACTION, -actionEncumb, true);
	player->addEncumbrance(CreatureEncumbrance::MIND, -mindEncumb, true);

	player->addMaxHAM(CreatureAttribute::STRENGTH, healthEncumb, true);
	player->healDamage(player, CreatureAttribute::STRENGTH, healthEncumb, true);

	player->addMaxHAM(CreatureAttribute::CONSTITUTION, healthEncumb, true);
	player->healDamage(player, CreatureAttribute::CONSTITUTION, healthEncumb, true);

	player->addMaxHAM(CreatureAttribute::QUICKNESS, actionEncumb, true);
	player->healDamage(player, CreatureAttribute::QUICKNESS, actionEncumb, true);

	player->addMaxHAM(CreatureAttribute::STAMINA, actionEncumb, true);
	player->healDamage(player, CreatureAttribute::STAMINA, actionEncumb, true);

	player->addMaxHAM(CreatureAttribute::FOCUS, mindEncumb, true);
	player->healDamage(player, CreatureAttribute::FOCUS, mindEncumb, true);

	player->addMaxHAM(CreatureAttribute::WILLPOWER, mindEncumb, true);
	player->healDamage(player, CreatureAttribute::WILLPOWER, mindEncumb, true);
}

void PlayerManagerImplementation::awardBadge(PlayerObject* ghost, uint32 badgeId) {
	const Badge* badge = BadgeList::instance()->get(badgeId);
	if (badge != nullptr)
		awardBadge(ghost, badge);
}

void PlayerManagerImplementation::awardBadge(PlayerObject* ghost, const Badge* badge) {
	if (badge == nullptr) {
		ghost->error("Failed to award null badge.");
		return;
	}

	StringIdChatParameter stringId("badge_n", "");
	stringId.setTO("badge_n", badge->getKey());

	ManagedReference<CreatureObject*> player = dynamic_cast<CreatureObject*>(ghost->getParent().get().get());
	const unsigned int badgeId = badge->getIndex();
	if (ghost->hasBadge(badgeId)) {
		stringId.setStringId("badge_n", "prose_hasbadge");
		player->sendSystemMessage(stringId);
		return;
	}

	ghost->setBadge(badgeId);
	stringId.setStringId("badge_n", "prose_grant");
	player->sendSystemMessage(stringId);

	if (badge->getHasMusic()) {
		String music = badge->getMusic();
		PlayMusicMessage* musicMessage = new PlayMusicMessage(music);
		player->sendMessage(musicMessage);
	}

	player->notifyObservers(ObserverEventType::BADGEAWARDED, player, badgeId);
	BadgeList* badgeList = BadgeList::instance();
	switch (ghost->getNumBadges()) {
	case 5:
		awardBadge(ghost, badgeList->get("count_5"));
		break;
	case 10:
		awardBadge(ghost, badgeList->get("count_10"));
		break;
	case 25:
		awardBadge(ghost, badgeList->get("count_25"));
		break;
	case 50:
		awardBadge(ghost, badgeList->get("count_50"));
		break;
	case 75:
		awardBadge(ghost, badgeList->get("count_75"));
		break;
	case 100:
		awardBadge(ghost, badgeList->get("count_100"));
		break;
	case 125:
		awardBadge(ghost, badgeList->get("count_125"));
		break;
	default:
		break;
	}

	if (badge->getType() == Badge::EXPLORATION) {
		switch (ghost->getBadgeTypeCount(static_cast<uint8>(Badge::EXPLORATION))) {
		case 10:
			awardBadge(ghost, badgeList->get("bdg_exp_10_badges"));
			break;
		case 20:
			awardBadge(ghost, badgeList->get("bdg_exp_20_badges"));
			break;
		case 30:
			awardBadge(ghost, badgeList->get("bdg_exp_30_badges"));
			break;
		case 40:
			awardBadge(ghost, badgeList->get("bdg_exp_40_badges"));
			break;
		case 45:
			awardBadge(ghost, badgeList->get("bdg_exp_45_badges"));
			break;
		default:
			break;
		}
	}
}

void PlayerManagerImplementation::setExperienceMultiplier(float globalMultiplier) {
	globalExpMultiplier = globalMultiplier;
}

/*
 * Award experience to a player.
 * Ex.
	PlayerManager* playerManager = server->getPlayerManager();
	playerManager->awardExperience(playerCreature, "resource_harvesting_inorganic", 500);
 *
 */
int PlayerManagerImplementation::awardExperience(CreatureObject* player, const String& xpType,
		int amount, bool sendSystemMessage, float localMultiplier, bool applyModifiers) {

	PlayerObject* playerObject = player->getPlayerObject();

	if (playerObject == nullptr)
		return 0;

	float speciesModifier = 1.f;

	if (amount > 0)
		speciesModifier = getSpeciesXpModifier(player->getSpeciesName(), xpType);

	float buffMultiplier = 1.f;

	if (player->hasBuff(BuffCRC::FOOD_XP_INCREASE) && !player->containsActiveSession(SessionFacadeType::CRAFTING))
		buffMultiplier += player->getSkillModFromBuffs("xp_increase") / 100.f;

	int xp = 0;

	if (applyModifiers)
		xp = playerObject->addExperience(xpType, (int) (amount * speciesModifier * buffMultiplier * localMultiplier * globalExpMultiplier));
	else
		xp = playerObject->addExperience(xpType, (int)amount);

	player->notifyObservers(ObserverEventType::XPAWARDED, player, xp);

	if (sendSystemMessage) {
		if (xp > 0) {
			StringIdChatParameter message("base_player","prose_grant_xp");
			message.setDI(xp);
			message.setTO("exp_n", xpType);
			player->sendSystemMessage(message);
		}
		if (xp > 0 && playerObject->hasCappedExperience(xpType)) {
			StringIdChatParameter message("base_player", "prose_hit_xp_cap"); //You have achieved your current limit for %TO experience.
			message.setTO("exp_n", xpType);
			player->sendSystemMessage(message);
		}
	}

	return xp;
}

void PlayerManagerImplementation::sendLoginMessage(CreatureObject* creature) {
	String motd = server->getLoginMessage();

	ChatSystemMessage* csm = new ChatSystemMessage(UnicodeString(motd), ChatSystemMessage::DISPLAY_CHATONLY);
	creature->sendMessage(csm);
}

void PlayerManagerImplementation::resendLoginMessageToAll() {
	ChatManager* chatManager = server->getChatManager();

	if (chatManager != nullptr) {
		String motd = server->getLoginMessage();

		ChatSystemMessage* csm = new ChatSystemMessage(UnicodeString(motd), ChatSystemMessage::DISPLAY_CHATONLY);
		chatManager->broadcastMessage(csm);
	}
}

void PlayerManagerImplementation::handleAbortTradeMessage(CreatureObject* player) {
	Locker _locker(player);

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr) {
		AbortTradeMessage* msg = new AbortTradeMessage();
		player->sendMessage(msg);

		return;
	}

	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	AbortTradeMessage* msg = new AbortTradeMessage();

	if (obj != nullptr && obj->isPlayerCreature()) {
		CreatureObject* receiver = cast<CreatureObject*>( obj.get());

		Locker locker(receiver, player);

		ManagedReference<TradeSession*> receiverContainer = receiver->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

		if (receiverContainer != nullptr && receiverContainer->getTradeTargetPlayer() == player->getObjectID()) {
			receiver->dropActiveSession(SessionFacadeType::TRADE);
			receiver->sendMessage(msg->clone());
		}

		locker.release();
	}

	player->sendMessage(msg->clone());

	delete msg;

	player->dropActiveSession(SessionFacadeType::TRADE);
}

void PlayerManagerImplementation::handleAddItemToTradeWindow(CreatureObject* player, uint64 itemID) {
	Locker _locker(player);

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr)
		return;

	// First Verify Target is Player
	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	if (obj == nullptr || !obj->isPlayerCreature())
		return;

	CreatureObject* receiver = cast<CreatureObject*>( obj.get());

	ManagedReference<SceneObject*> objectToTrade = server->getObject(itemID);

	if (objectToTrade == nullptr || !objectToTrade->isASubChildOf(player) ||
			!objectToTrade->checkContainerPermission(player, ContainerPermissions::MOVECONTAINER)) {
		player->sendSystemMessage("@container_error_message:container26");
		handleAbortTradeMessage(player);
		return;
	}

	if (objectToTrade->isNoTrade()) {
		player->sendSystemMessage("@container_error_message:container26");
		handleAbortTradeMessage(player);
		return;
	}

	// Containers containing notrade items...
	if (objectToTrade->containsNoTradeObjectRecursive()) {
		player->sendSystemMessage("@container_error_message:container26");
		handleAbortTradeMessage(player);
		return;
	}

	if (objectToTrade->isControlDevice()) {
		Reference<ControlDevice*> controlDevice = cast<ControlDevice*>(objectToTrade.get());
		Reference<TangibleObject*> controlledObject = controlDevice->getControlledObject();

		if (controlledObject != nullptr) {
			Locker crossLocker(controlledObject, player);

			controlDevice->storeObject(player, true);
		}
	}

	tradeContainer->addTradeItem(objectToTrade);

	SceneObject* inventory = player->getSlottedObject("inventory");
	inventory->sendWithoutContainerObjectsTo(receiver);
	objectToTrade->sendTo(receiver, true);

	AddItemMessage* msg = new AddItemMessage(itemID);
	receiver->sendMessage(msg);
}

void PlayerManagerImplementation::handleGiveMoneyMessage(CreatureObject* player, uint32 value) {
	Locker _locker(player);

	int currentMoney = player->getCashCredits();

	if (value > currentMoney)
		value = currentMoney;

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr)
		return;

	tradeContainer->setMoneyToTrade(value);

	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	if (obj != nullptr && obj->isPlayerCreature()) {
		CreatureObject* receiver = cast<CreatureObject*>( obj.get());

		GiveMoneyMessage* msg = new GiveMoneyMessage(value);
		receiver->sendMessage(msg);
	}
}

void PlayerManagerImplementation::handleAcceptTransactionMessage(CreatureObject* player) {
	Locker _locker(player);

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr)
		return;

	tradeContainer->setAcceptedTrade(true);

	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	if (obj != nullptr && obj->isPlayerCreature()) {
		CreatureObject* receiver = cast<CreatureObject*>(obj.get());

		AcceptTransactionMessage* msg = new AcceptTransactionMessage();
		receiver->sendMessage(msg);
	}
}

void PlayerManagerImplementation::handleUnAcceptTransactionMessage(CreatureObject* player) {
	Locker _locker(player);

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr)
		return;

	tradeContainer->setAcceptedTrade(false);

	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	if (obj != nullptr && obj->isPlayerCreature()) {
		CreatureObject* receiver = cast<CreatureObject*>(obj.get());

		UnAcceptTransactionMessage* msg = new UnAcceptTransactionMessage();
		receiver->sendMessage(msg);
	}

}

bool PlayerManagerImplementation::checkTradeItems(CreatureObject* player, CreatureObject* receiver) {
	PlayerObject* ghost = player->getPlayerObject();
	PlayerObject* targetGhost = receiver->getPlayerObject();

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();
	ManagedReference<TradeSession*> receiverContainer = receiver->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr || receiverContainer == nullptr)
		return false;

	if (tradeContainer->getTradeTargetPlayer() != receiver->getObjectID())
		return false;

	if (receiverContainer->getTradeTargetPlayer() != player->getObjectID())
		return false;

	SceneObject* playerInventory = player->getSlottedObject("inventory");
	SceneObject* receiverInventory = receiver->getSlottedObject("inventory");

	SceneObject* playerDatapad = player->getSlottedObject("datapad");
	SceneObject* receiverDatapad = receiver->getSlottedObject("datapad");

	int playerTanos = 0;
	int playerItnos = 0;
	int recieverTanos = 0;
	int recieverItnos = 0;
	int playerCreaturePetsTraded = 0;
	int receiverCreaturePetsTraded = 0;
	int playerFactionPetsTraded = 0;
	int receiverFactionPetsTraded = 0;
	int playerDroidsTraded = 0;
	int receiverDroidsTraded = 0;
	int playerVehiclesTraded = 0;
	int receiverVehiclesTraded = 0;
	int playerShipsTraded = 0;
	int receiverShipsTraded = 0;

	for (int i = 0; i < tradeContainer->getTradeSize(); ++i) {
		ManagedReference<SceneObject*> scene = tradeContainer->getTradeItem(i);

		if (scene->isNoTrade())
			return false;

		if (scene->isTangibleObject()) {

			String err;
			if (receiverInventory->canAddObject(scene, -1, err) != 0)
				return false;

			if (!playerInventory->hasObjectInContainer(scene->getObjectID()))
				return false;

			recieverTanos++;

		} else if (scene->isIntangibleObject()) {

			String err;
			if (receiverDatapad->canAddObject(scene, -1, err) != 0)
				return false;

			if (!playerDatapad->hasObjectInContainer(scene->getObjectID()))
				return false;

			if (scene->isPetControlDevice()) {
				PetControlDevice* petControlDevice = cast<PetControlDevice*>(scene.get());

				if (petControlDevice->getPetType() == PetManager::CREATUREPET) {
					if (!petControlDevice->canBeTradedTo(player, receiver, receiverCreaturePetsTraded))
						return false;

					receiverCreaturePetsTraded++;
				} else if (petControlDevice->getPetType() == PetManager::FACTIONPET) {
					if (!petControlDevice->canBeTradedTo(player, receiver, receiverFactionPetsTraded))
						return false;

					receiverFactionPetsTraded++;
				} else if (petControlDevice->getPetType() == PetManager::DROIDPET) {
					if (!petControlDevice->canBeTradedTo(player, receiver, receiverDroidsTraded))
						return false;

					receiverDroidsTraded++;
				}
			} else if (scene->isVehicleControlDevice()) {
				VehicleControlDevice* vehicleControlDevice = cast<VehicleControlDevice*>(scene.get());

				if (!vehicleControlDevice->canBeTradedTo(player, receiver, receiverVehiclesTraded))
					return false;

				receiverVehiclesTraded++;
			} else if (scene->isShipControlDevice()) {
				ShipControlDevice* shipControlDevice = cast<ShipControlDevice*>(scene.get());

				if (!shipControlDevice->canBeTradedTo(player, receiver, receiverShipsTraded))
					return false;

				receiverShipsTraded++;
			}

			recieverItnos++;

		} else {
			return false;
		}
	}

	for (int i = 0; i < receiverContainer->getTradeSize(); ++i) {
		ManagedReference<SceneObject*> scene = receiverContainer->getTradeItem(i);

		if (scene->isNoTrade())
			return false;

		if (scene->isTangibleObject()) {

			String err;
			if (playerInventory->canAddObject(scene, -1, err) != 0)
				return false;

			if (!receiverInventory->hasObjectInContainer(scene->getObjectID()))
				return false;

			playerTanos++;

		} else if (scene->isIntangibleObject()) {

			String err;
			if (playerDatapad->canAddObject(scene, -1, err) != 0)
				return false;

			if (!receiverDatapad->hasObjectInContainer(scene->getObjectID()))
				return false;

			if (scene->isPetControlDevice()) {
				PetControlDevice* petControlDevice = cast<PetControlDevice*>(scene.get());

				if (petControlDevice->getPetType() == PetManager::CREATUREPET) {
					if (!petControlDevice->canBeTradedTo(receiver, player, playerCreaturePetsTraded))
						return false;

					playerCreaturePetsTraded++;
				} else if (petControlDevice->getPetType() == PetManager::FACTIONPET) {
					if (!petControlDevice->canBeTradedTo(receiver, player, playerFactionPetsTraded))
						return false;

					playerFactionPetsTraded++;
				} else if (petControlDevice->getPetType() == PetManager::DROIDPET) {
					if (!petControlDevice->canBeTradedTo(receiver, player, playerDroidsTraded))
						return false;

					playerDroidsTraded++;
				}
			} else if (scene->isVehicleControlDevice()) {
				VehicleControlDevice* vehicleControlDevice = cast<VehicleControlDevice*>(scene.get());

				if (!vehicleControlDevice->canBeTradedTo(receiver, player, playerVehiclesTraded))
					return false;

				playerVehiclesTraded++;
			} else if (scene->isShipControlDevice()) {
				ShipControlDevice* shipControlDevice = cast<ShipControlDevice*>(scene.get());

				if (!shipControlDevice->canBeTradedTo(receiver, player, playerShipsTraded))
					return false;

				playerShipsTraded++;
			}

			playerItnos++;

		} else {
			return false;
		}
	}

	if (recieverTanos != 0 && (receiverInventory->getContainerObjectsSize() + recieverTanos >= receiverInventory->getContainerVolumeLimit())) {
		player->sendSystemMessage("@container_error_message:container19");
		receiver->sendSystemMessage("@container_error_message:container19");
		return false;
	}

	if (playerTanos != 0 && (playerInventory->getContainerObjectsSize() + playerTanos >= playerInventory->getContainerVolumeLimit())) {
		player->sendSystemMessage("@container_error_message:container19");
		receiver->sendSystemMessage("@container_error_message:container19");
		return false;
	}

	if (receiverDatapad->getContainerObjectsSize() + recieverItnos >= receiverDatapad->getContainerVolumeLimit()) {
		player->sendSystemMessage("@container_error_message:container19");
		receiver->sendSystemMessage("@container_error_message:container19");
		return false;
	}

	if (playerDatapad->getContainerObjectsSize() + playerItnos >= playerDatapad->getContainerVolumeLimit()) {
		player->sendSystemMessage("@container_error_message:container19");
		receiver->sendSystemMessage("@container_error_message:container19");
		return false;
	}

	int playerMoneyToTrade = tradeContainer->getMoneyToTrade();

	if (playerMoneyToTrade < 0)
		return false;

	if (playerMoneyToTrade > player->getCashCredits())
		return false;

	int receiverMoneyToTrade = receiverContainer->getMoneyToTrade();

	if (receiverMoneyToTrade < 0)
		return false;

	if (receiverMoneyToTrade > receiver->getCashCredits())
		return false;

	if (player->getDistanceTo(receiver) >= 15.f) {
		player->sendSystemMessage("You are too far to trade");
		receiver->sendSystemMessage("You are too far to trade");
		return false;
	}


	return true;
}

void PlayerManagerImplementation::handleVerifyTradeMessage(CreatureObject* player) {
	ManagedReference<ObjectController*> objectController = server->getObjectController();

	Locker locker(player);

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer == nullptr) {
		return;
	}

	// Get a trx group to trace all trx's in this session
	auto trxGroup = TransactionLog::getNewTrxGroup();

	tradeContainer->setVerifiedTrade(true);

	uint64 targID = tradeContainer->getTradeTargetPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);

	if (obj != nullptr && obj->isPlayerCreature()) {
		CreatureObject* receiver = cast<CreatureObject*>(obj.get());

		Locker clocker(receiver, player);

		ManagedReference<TradeSession*> receiverTradeContainer = receiver->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

		if (receiverTradeContainer == nullptr) {
			tradeContainer->setVerifiedTrade(false);
			return;
		}

		if (!checkTradeItems(player, receiver)) {
			clocker.release();
			handleAbortTradeMessage(player);

			locker.release();
			return;
		}

		if (receiverTradeContainer->hasVerifiedTrade()) {
			SceneObject* receiverInventory = receiver->getSlottedObject("inventory");
			SceneObject* receiverDatapad = receiver->getSlottedObject("datapad");

			for (int i = 0; i < tradeContainer->getTradeSize(); ++i) {
				ManagedReference<SceneObject*> item = tradeContainer->getTradeItem(i);

				TransactionLog trx(player, receiver, item, TrxCode::PLAYERTRADE);
				trx.setTrxGroup(trxGroup);

				if (item->isTangibleObject()) {
					if (objectController->transferObject(item, receiverInventory, -1, true))
						item->sendDestroyTo(player);
				} else {
					if (objectController->transferObject(item, receiverDatapad, -1, true))
						item->sendDestroyTo(player);
				}
			}

			SceneObject* playerInventory = player->getSlottedObject("inventory");
			SceneObject* playerDatapad = player->getSlottedObject("datapad");

			for (int i = 0; i < receiverTradeContainer->getTradeSize(); ++i) {
				ManagedReference<SceneObject*> item = receiverTradeContainer->getTradeItem(i);

				TransactionLog trx(receiver, player, item, TrxCode::PLAYERTRADE);
				trx.setTrxGroup(trxGroup);

				if (item->isTangibleObject()) {
					if (objectController->transferObject(item, playerInventory, -1, true)) {
						item->sendDestroyTo(receiver);
					} else {
						trx.errorMessage() << "transferObject failed";
					}
				} else {
					if (objectController->transferObject(item, playerDatapad, -1, true)) {
						item->sendDestroyTo(receiver);
					} else {
						trx.errorMessage() << "transferObject failed";
					}
				}
			}

			uint32 giveMoney = tradeContainer->getMoneyToTrade();

			if (giveMoney > 0) {
				TransactionLog trx(player, receiver, TrxCode::PLAYERTRADE, giveMoney, true);
				trx.setTrxGroup(trxGroup);
				player->subtractCashCredits(giveMoney);
				receiver->addCashCredits(giveMoney);
			}

			giveMoney = receiverTradeContainer->getMoneyToTrade();

			if (giveMoney > 0) {
				TransactionLog trx(receiver, player, TrxCode::PLAYERTRADE, giveMoney, true);
				trx.setTrxGroup(trxGroup);
				receiver->subtractCashCredits(giveMoney);
				player->addCashCredits(giveMoney);
			}

			receiver->dropActiveSession(SessionFacadeType::TRADE);
			player->dropActiveSession(SessionFacadeType::TRADE);

			TradeCompleteMessage* msg = new TradeCompleteMessage();
			receiver->sendMessage(msg->clone());
			player->sendMessage(msg->clone());

			delete msg;
		}
	}
}

int PlayerManagerImplementation::notifyObserverEvent(uint32 eventType, Observable* observable, ManagedObject* arg1, int64 arg2) {

	if (eventType == ObserverEventType::POSTURECHANGED) {
		CreatureObject* creature = cast<CreatureObject*>(observable);

		if (creature == nullptr)
			return 1;

		if (creature->hasState(CreatureState::ALERT)) { // This can apply to TKA AND Jedi meditate since they share the same sysmsgs / moods.
			creature->sendSystemMessage("@teraskasi:med_end");
			creature->setMood(creature->getMoodID(), true);
			creature->clearState(CreatureState::ALERT, true);

			// Check POSTERCHANGE on Meditate...
			Reference<MeditateTask*> meditateTask = creature->getPendingTask("meditate").castTo<MeditateTask*>();
			if (meditateTask != nullptr) {
				creature->removePendingTask("meditate");

				if (meditateTask->isScheduled())
					meditateTask->cancel();
			}

			// Check POSTERCHANGE on Force Meditate...
			Reference<ForceMeditateTask*> fmeditateTask = creature->getPendingTask("forcemeditate").castTo<ForceMeditateTask*>( );

			if (fmeditateTask != nullptr) {
				creature->removePendingTask("forcemeditate");

				if (fmeditateTask->isScheduled())
					fmeditateTask->cancel();
			}
		}

		// Check POSTURECHANGED disrupting Logout...
		Reference<LogoutTask*> logoutTask = creature->getPendingTask("logout").castTo<LogoutTask*>();
		if (logoutTask != nullptr && !creature->isSitting()) {
			logoutTask->cancelLogout();
		}

		return 1;
	}

	return 0;
}

void PlayerManagerImplementation::sendBattleFatigueMessage(CreatureObject* player, CreatureObject* target) {
	uint32 targetBattleFatigue = target->getShockWounds();

	if (player == target) {
		if (targetBattleFatigue >= 250 && targetBattleFatigue < 500) {
			player->sendSystemMessage("@healing:shock_effect_low");
		} else if (targetBattleFatigue >= 500 && targetBattleFatigue < 750) {
			player->sendSystemMessage("@healing:shock_effect_medium");
		} else if (targetBattleFatigue >= 750) {
			player->sendSystemMessage("@healing:shock_effect_high");
		}
	} else if (target->isPlayerCreature()) {
		if (targetBattleFatigue >= 250 && targetBattleFatigue < 500) {
			target->sendSystemMessage("@healing:shock_effect_low_target");
		} else if (targetBattleFatigue >= 500 && targetBattleFatigue < 750) {
			target->sendSystemMessage("@healing:shock_effect_medium_target");
		} else if (targetBattleFatigue >= 750) {
			target->sendSystemMessage("@healing:shock_effec_high_target");
		}
	}
}

int PlayerManagerImplementation::healEnhance(CreatureObject* enhancer, CreatureObject* patient, byte attribute, int buffvalue, float duration, int absorption) {
	String buffname = "medical_enhance_" + BuffAttribute::getName(attribute);
	uint32 buffcrc = buffname.hashCode();
	uint32 buffdiff = buffvalue;

	//If a stronger buff already exists, then we don't buff the patient.
	if (patient->hasBuff(buffcrc)) {
		Buff* buff = patient->getBuff(buffcrc);

		if (buff != nullptr) {
			int value = buff->getAttributeModifierValue(attribute);

			if (BuffAttribute::isProtection(attribute))
				value = buff->getSkillModifierValue(BuffAttribute::getProtectionString(attribute));

			if (value > buffvalue)
				return 0;

			buffdiff -= value;
		}
	}

	Reference<Buff*> buff = new Buff(patient, buffcrc, duration, BuffType::MEDICAL);

	Locker locker(buff);

	if (BuffAttribute::isProtection(attribute)) {
		buff->setSkillModifier(BuffAttribute::getProtectionString(attribute), buffvalue);

		if (absorption > 0)
			buff->setSkillModifier(BuffAttribute::getAbsorptionString(attribute), absorption);
	} else {
		buff->setAttributeModifier(attribute, buffvalue);
		buff->setFillAttributesOnBuff(true);
	}

	patient->addBuff(buff);

	enhancer->notifyObservers(ObserverEventType::ENHANCINGPERFORMED, patient, buffdiff);

	return buffdiff;
}

void PlayerManagerImplementation::stopListen(CreatureObject* creature, uint64 entertainerID, bool doSendPackets, bool forced, bool doLock, bool outOfRange) {
	Locker locker(creature);

	uint64 listenID = creature->getListenID();

	// If the player selected "Stop listening" by using a radial menu created on a
	// musician other than the one that they are currently listening to then change
	// entertainerID to their listenID so that the player can still stop listening.
	if (entertainerID != listenID && listenID != 0 && creature->isListening()) {
		entertainerID = listenID;
	}

	ManagedReference<SceneObject*> object = server->getObject(entertainerID);

	if (object == nullptr)
		return;

	if (object->isDroidObject()) {
		creature->setMood(creature->getMoodID());
		if (doSendPackets) {
			creature->setListenToID(0, true);
			creature->setMoodString(creature->getMoodString(), true);
		}
		if (creature->isPlayerCreature()) {
			CreatureObject* player = cast<CreatureObject*>( creature);
			StringIdChatParameter stringID;
			if (forced) {
				stringID.setTU(entertainerID);
				stringID.setStringId("performance", "music_stop_other"); // "%TU stops playing."
				player->sendSystemMessage(stringID);
				return;
			} else if (outOfRange) {
				// The correct string id is @performance:music_listen_out_of_range ("You stop listening to %TT because %OT is too far away.")
				// but %OT will get replaced by him/her which gives an incorrect message.
				StringBuffer msg;
				msg << "You stop listening to " << object->getDisplayedName() << " because they are too far away.";
				player->sendSystemMessage(msg.toString());
				return;
			} else {
				player->sendSystemMessage("@performance:music_listen_stop_self"); // "You stop listening."
				return;
			}
		}
		return;
	}

	if (!object->isPlayerCreature()) {
		creature->sendSystemMessage("@performance:music_listen_npc"); // "You cannot /listen to NPCs."
		return;
	}

	CreatureObject* entertainer = cast<CreatureObject*>( object.get());

	if (entertainer == creature)
		return;

	String entName;
	ManagedReference<EntertainingSession*> esession;

	if (entertainer != nullptr) {
		Locker clocker(entertainer, creature);

		entName = entertainer->getFirstName();

		ManagedReference<Facade*> session = entertainer->getActiveSession(SessionFacadeType::ENTERTAINING);

		if (session != nullptr) {
			esession = dynamic_cast<EntertainingSession*>(session.get());

			if (esession != nullptr) {
				esession->activateEntertainerBuff(creature, PerformanceType::MUSIC);

				esession->removePatron(creature);
			}
		}

		clocker.release();
	}

	creature->setMood(creature->getMoodID());

	if (doSendPackets && esession != nullptr)
		esession->sendEntertainmentUpdate(creature, 0, creature->getMoodString());

	if (creature->isPlayerCreature() && entertainer != nullptr) {
		CreatureObject* player = cast<CreatureObject*>( creature);

		StringIdChatParameter stringID;

		if (forced) {
			stringID.setTU(entertainerID);
			stringID.setStringId("performance", "music_stop_other"); // "%TU stops playing."

			player->sendSystemMessage(stringID);
		} else if (outOfRange) {
			// The correct string id is @performance:music_listen_out_of_range ("You stop listening to %TT because %OT is too far away.")
			// but %OT will get replaced by him/her which gives an incorrect message.
			StringBuffer msg;
			msg << "You stop listening to " << entertainer->getFirstName() << " because they are too far away.";
			player->sendSystemMessage(msg.toString());
		} else {
			player->sendSystemMessage("@performance:music_listen_stop_self"); // "You stop listening."
		}

		ManagedReference<PlayerObject*> entPlayer = entertainer->getPlayerObject();
		if (entPlayer != nullptr && entPlayer->getPerformanceBuffTarget() == player->getObjectID())
			entPlayer->setPerformanceBuffTarget(0);
	}
	//esession->setEntertainerBuffDuration(creature, PerformanceType::MUSIC, 0.0f); // reset
	//esession->setEntertainerBuffStrength(creature, PerformanceType::MUSIC, 0.0f);
	creature->info("stopped listening [" + entName + "]");

	creature->setListenToID(0, true);
}


void PlayerManagerImplementation::stopWatch(CreatureObject* creature, uint64 entertainerID, bool doSendPackets, bool forced, bool doLock, bool outOfRange) {
	Locker locker(creature);

	uint64 watchID = creature->getWatchToID();

	// If the player selected "Stop watching" by using a radial menu created on a
	// dancer other than the one that they are currently watching then change
	// entertainerID to their watchID so that the player can still stop watching.
	if (entertainerID != watchID && watchID != 0 && creature->isWatching()) {
		entertainerID = watchID;
	}

	ManagedReference<SceneObject*> object = server->getObject(entertainerID);

	if (object == nullptr)
		return;

	if (!object->isPlayerCreature()) {
		creature->sendSystemMessage("@performance:dance_watch_npc"); // "You cannot /watch NPCs."
		return;
	}

	CreatureObject* entertainer = cast<CreatureObject*>( object.get());

	if (entertainer == creature)
		return;

	ManagedReference<EntertainingSession*> esession = nullptr;

	String entName;
	if (entertainer != nullptr) {
		Locker clocker(entertainer, creature);

		entName = entertainer->getFirstName();

		ManagedReference<Facade*> session = entertainer->getActiveSession(SessionFacadeType::ENTERTAINING);

		if (session != nullptr) {
			esession = dynamic_cast<EntertainingSession*>(session.get());

			if (esession != nullptr) {
				esession->activateEntertainerBuff(creature, PerformanceType::DANCE);

				esession->removePatron(creature);
			}
		}

		clocker.release();
	}

	creature->setMood(creature->getMoodID());

	if (doSendPackets && esession != nullptr)
		esession->sendEntertainmentUpdate(creature, 0, creature->getMoodString());

	//System Message.
	if (creature->isPlayerCreature() && entertainer != nullptr) {
		CreatureObject* player = cast<CreatureObject*>( creature);

		StringIdChatParameter stringID;

		if (forced) {
			stringID.setTU(entertainerID);
			stringID.setStringId("performance", "dance_stop_other"); // %TU stops dancing.

			player->sendSystemMessage(stringID);
		} else if (outOfRange) {
			// The correct string id is @performance:dance_watch_out_of_range ("You stop watching %TT because %OT is too far away.")
			// but %OT will get replaced by him/her which gives an incorrect message.
			StringBuffer msg;
			msg << "You stop watching " << entertainer->getFirstName() << " because they are too far away.";
			player->sendSystemMessage(msg.toString());
		} else {
			player->sendSystemMessage("@performance:dance_watch_stop_self"); //"You stop watching."
		}

		ManagedReference<PlayerObject*> entPlayer = entertainer->getPlayerObject();
		if (entPlayer != nullptr && entPlayer->getPerformanceBuffTarget() == player->getObjectID())
			entPlayer->setPerformanceBuffTarget(0);
	}


	//esession->setEntertainerBuffDuration(creature, PerformanceType::DANCE, 0.0f); // reset
	//esession->setEntertainerBuffStrength(creature, PerformanceType::DANCE, 0.0f);
	creature->info("stopped watching [" + entName + "]");

	creature->setWatchToID(0);
	/*doWatching = false;
	watchID = 0;*/
}

void PlayerManagerImplementation::startWatch(CreatureObject* creature, uint64 entertainerID) {
	Locker locker(creature);

	uint64 watchID = creature->getWatchToID();

	if (watchID == entertainerID)
		return;

	ManagedReference<SceneObject*> object = server->getObject(entertainerID);

	if (object == nullptr)
		return;

	if (!object->isPlayerCreature()) {
		creature->sendSystemMessage("@performance:dance_watch_npc"); // "You can not /watch NPCs."
		return;
	}

	CreatureObject* entertainer = cast<CreatureObject*>( object.get());

	if (creature == entertainer)
		return;

	Locker clocker(entertainer, creature);

	if (creature->isDancing() || creature->isPlayingMusic()) {
		StringIdChatParameter stringID;
		stringID.setStringId("cmd_err", "locomotion_prose"); // "You cannot %TO while %TU."
		stringID.setTO("/watch");
		stringID.setTU("@locomotion_n:skillanimating"); // "Skill Animating"
		creature->sendSystemMessage(stringID);

		return;
	} else if (!entertainer->isDancing()) {
		StringIdChatParameter stringID;
		stringID.setStringId("performance", "dance_watch_not_dancing"); // "%TT is not dancing."
		stringID.setTT(entertainerID);
		creature->sendSystemMessage(stringID);
		return;
	}

	ManagedReference<EntertainingSession*> entertainingSession = entertainer->getActiveSession(SessionFacadeType::ENTERTAINING).castTo<EntertainingSession*>();

	if (entertainingSession == nullptr)
		return;

	if (creature->isWatching()) {
		stopWatch(creature, watchID, false);
	}

	entertainingSession->sendEntertainmentUpdate(creature, entertainerID, "entertained");
	entertainingSession->addPatron(creature);

	entertainer->notifyObservers(ObserverEventType::WASWATCHED, creature);

	StringIdChatParameter stringID;
	stringID.setStringId("performance", "dance_watch_self"); // You start watching %TT.
	stringID.setTT(entertainerID);
	creature->sendSystemMessage(stringID);

	creature->info("started watching [" + entertainer->getCustomObjectName().toString() + "]");

	creature->setWatchToID(entertainer->getObjectID());
	//watchID =  entertainerID;
}

void PlayerManagerImplementation::startListen(CreatureObject* creature, uint64 entertainerID) {
	Locker locker(creature);

	ManagedReference<SceneObject*> object = server->getObject(entertainerID);
	uint64 listenID = creature->getListenID();

	if (listenID == entertainerID)
		return;

	if (object == nullptr)
		return;

	/*if (object->isNonPlayerCreature()) {
		creature->sendSystemMessage("@performance:dance_watch_npc");
		return;
	}*/

	// Droid playback handling
	if (object->isDroidObject()) {
		DroidObject* droid = cast<DroidObject*>( object.get());
		if (droid == nullptr) {
			creature->sendSystemMessage("@performance:music_listen_npc"); // "You cannot /listen to NPCs."
			return;
		}
		auto bmodule = droid->getModule("playback_module");
		if (bmodule != nullptr) {
			DroidPlaybackModuleDataComponent* module = cast<DroidPlaybackModuleDataComponent*>(bmodule.get());
			if (module != nullptr) {
				if (creature->isDancing() || creature->isPlayingMusic()) {
					StringIdChatParameter stringID;
					stringID.setStringId("cmd_err", "locomotion_prose"); // "You cannot %TO while %TU."
					stringID.setTO("/listen");
					stringID.setTU("@locomotion_n:skillanimating"); // "Skill Animating"
					creature->sendSystemMessage(stringID);
					return;
				}

				if (module->isPlayingMusic()) {
					// the droid is playing so we can do something
					if (creature->isListening()) {
						stopListen(creature, listenID, false);
					}

					StringIdChatParameter stringID;
					stringID.setTT(entertainerID);
					stringID.setStringId("performance", "music_listen_self"); // "You start listening to %TT."
					creature->sendSystemMessage(stringID);

					creature->setListenToID(entertainerID, true);
					String str = server->getChatManager()->getMoodAnimation("entertained");
					creature->setMoodString(str, true);
					creature->setListenToID(droid->getObjectID());
					module->addListener(creature->getObjectID());
					return;
				} else {
					StringIdChatParameter stringID;
					stringID.setTT(entertainerID);
					stringID.setStringId("performance", "music_listen_not_playing"); // %TT is not playing a song.
					creature->sendSystemMessage(stringID);
					return;
				}
			} else {
				creature->sendSystemMessage("@performance:music_listen_npc"); // "You cannot /listen to NPCs."
			}
		} else {
			creature->sendSystemMessage("@performance:music_listen_npc"); // "You cannot /listen to NPCs."
		}
		return;
	}

	if (!object->isPlayerCreature()) {
		creature->sendSystemMessage("@performance:music_listen_npc"); // "You cannot /listen to NPCs."
		return;
	}

	CreatureObject* entertainer = cast<CreatureObject*>( object.get());

	if (creature == entertainer)
		return;

	Locker clocker(entertainer, creature);

	if (creature->isDancing() || creature->isPlayingMusic()) {
		StringIdChatParameter stringID;
		stringID.setStringId("cmd_err", "locomotion_prose"); // "You cannot %TO while %TU."
		stringID.setTO("/listen");
		stringID.setTU("@locomotion_n:skillanimating"); // "Skill Animating"
		creature->sendSystemMessage(stringID);
		return;
	} else if (!entertainer->isPlayingMusic()) {
		StringIdChatParameter stringID;
		stringID.setTT(entertainerID);
		stringID.setStringId("performance", "music_listen_not_playing"); // %TT is not playing a song.
		creature->sendSystemMessage(stringID);

		return;
	}

	ManagedReference<Facade*> facade = entertainer->getActiveSession(SessionFacadeType::ENTERTAINING);

	if (facade == nullptr)
		return;

	EntertainingSession* entertainingSession = dynamic_cast<EntertainingSession*>(facade.get());

	if (entertainingSession == nullptr)
		return;

	if (creature->isListening()) {
		stopListen(creature, listenID, false);
	}

	//sendEntertainmentUpdate(entertainerID, "entertained");

	entertainingSession->sendEntertainmentUpdate(creature, entertainerID, "entertained");
	entertainingSession->addPatron(creature);

	entertainer->notifyObservers(ObserverEventType::WASLISTENEDTO, creature);

	//creature->addWatcher(_this);

	StringIdChatParameter stringID;
	stringID.setTT(entertainerID);
	stringID.setStringId("performance", "music_listen_self"); // "You start listening to %TT."
	creature->sendSystemMessage(stringID);

	//setEntertainerBuffDuration(PerformanceType::DANCE, 0.0f);
	//setEntertainerBuffStrength(PerformanceType::DANCE, 0.0f);

	creature->info("started listening to [" + entertainer->getCustomObjectName().toString() + "]");

	creature->setListenToID(entertainer->getObjectID());
	//watchID =  entertainerID;
}


SceneObject* PlayerManagerImplementation::getInRangeStructureWithAdminRights(CreatureObject* creature, uint64 targetID) {
	ZoneServer* zoneServer = server;

	//First we need to check if the target. if it's not right...
	//Check the building they are standing in, if it's not right...
	//Find the nearest installation.

	ManagedReference<SceneObject*> obj = nullptr;

	if (targetID != 0) {
		obj = zoneServer->getObject(targetID);

		if (obj != nullptr && obj->isStructureObject() && (cast<StructureObject*>(obj.get()))->isOnAdminList(creature))
			return obj.get();
	}


	ManagedReference<SceneObject*> rootParent = creature->getRootParent();

	if (rootParent != nullptr && rootParent->isStructureObject() && (cast<StructureObject*>(rootParent.get()))->isOnAdminList(creature)) {
		return rootParent;
	}

	StructureObject* structure = nullptr;
	float distance = 16000;

	Zone* zone = creature->getZone();

	if (zone == nullptr) {
		return nullptr;
	}

	//We need to search nearby for an installation that belongs to the player.
	Locker _locker(zone);

	CloseObjectsVector* closeObjs = (CloseObjectsVector*)creature->getCloseObjects();
	SortedVector<QuadTreeEntry*> closeObjects;
	closeObjs->safeCopyReceiversTo(closeObjects, CloseObjectsVector::STRUCTURETYPE);

	for (int i = 0; i < closeObjects.size(); ++i) {
		ManagedReference<SceneObject*> tObj = cast<SceneObject*>( closeObjects.get(i));

		if (tObj != nullptr) {
			if (tObj->isStructureObject()) {
				float dist = tObj->getDistanceTo(creature);

				StructureObject* structureObject = cast<StructureObject*>( tObj.get());

				if (structureObject->isOnAdminList(creature) && dist < distance) {
					structure = structureObject;
					distance = dist;
				}
			}
		}
	}

	if (distance < 25)
		return structure;

	return nullptr;
}

StructureObject* PlayerManagerImplementation::getInRangeOwnedStructure(CreatureObject* creature, float range) {
	Locker _lock(creature);

	ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();

	if (ghost == nullptr)
		return nullptr;

	ManagedReference<StructureObject*> closestStructure = nullptr;
	float closestDistance = 16000.f;

	for (int i = 0; i < ghost->getTotalOwnedStructureCount(); ++i) {
		uint64 oid = ghost->getOwnedStructure(i);

		ManagedReference<StructureObject*> structure = (ghost->getZoneServer()->getObject(oid)).castTo<StructureObject*>();

		Locker _slock(structure, creature);

		if (creature->getZone() != structure->getZone())
			continue;

		float distance = structure->getDistanceTo(creature);

		if (distance <= range && distance < closestDistance) {
			closestStructure = structure;
			closestDistance = distance;
		}
	}

	return closestStructure;
}

void PlayerManagerImplementation::updatePermissionLevel(CreatureObject* targetPlayer, int permissionLevel) {

	if (targetPlayer == nullptr) {
		return;
	}

	Locker locker(targetPlayer);
	ManagedReference<PlayerObject*> ghost = targetPlayer->getPlayerObject();

	if (ghost == nullptr) {
		return;
	}

	SkillManager* skillManager = server->getSkillManager();

	int currentPermissionLevel = ghost->getAdminLevel();

	/*Temporarily removed so that we can update admin levels immediately
	if (currentPermissionLevel == permissionLevel)
		return;*/

	if (currentPermissionLevel != 0) {
		Vector<String>* skillsToBeRemoved = permissionLevelList->getPermissionSkills(currentPermissionLevel);
		if (skillsToBeRemoved != nullptr) {
			for (int i = 0; i < skillsToBeRemoved->size(); i++) {
				const String& skill = skillsToBeRemoved->get(i);
				targetPlayer->sendSystemMessage("Staff skill revoked: " + skill);
				skillManager->surrenderSkill(skill, targetPlayer, true);
			}
		}
	}

	ghost->setAdminLevel(permissionLevel);

	if (permissionLevel != 0) {
		Vector<String>* skillsToBeAdded = permissionLevelList->getPermissionSkills(permissionLevel);
		if (skillsToBeAdded != nullptr) {
			for (int i = 0; i < skillsToBeAdded->size(); ++i) {
				const String& skill = skillsToBeAdded->get(i);
				targetPlayer->sendSystemMessage("Staff skill granted: " + skill);
				skillManager->awardSkill(skill, targetPlayer, false, true, true);
			}
		}
	}

	updatePermissionName(targetPlayer, permissionLevel);
}

void PlayerManagerImplementation::updatePermissionName(CreatureObject* player, int permissionLevel) {
	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();
	int priviledgeFlag = permissionLevelList->getPriviledgeFlag(permissionLevel);

	ghost->setPriviledgeFlag(priviledgeFlag);
	//Send deltas
	if (player->isOnline()) {
		UnicodeString tag = permissionLevelList->getPermissionTag(permissionLevel);

		TangibleObjectDeltaMessage3* tanod3 = new TangibleObjectDeltaMessage3(player);
		tanod3->updateCustomName(player->getDisplayedName(), tag);
		tanod3->close();
		player->broadcastMessage(tanod3, true);

		ghost->updateInRangeBuildingPermissions();

		/*PlayerObjectDeltaMessage6* playd6 = new PlayerObjectDeltaMessage6(ghost);
			playd6->setAdminLevel(adminLevel);
			playd6->close();
			player->broadcastMessage(playd6, true);*/
	}
}

void PlayerManagerImplementation::updateSwimmingState(CreatureObject* player, float newZ, IntersectionResults* intersections, CloseObjectsVector* closeObjectsVector) {
	player->notifySelfPositionUpdate();
	if (player->getParent() != nullptr) {
		return;
	}

	Zone* zone = player->getZone();

	if (zone == nullptr) {
		player->info("No zone.", true);
		return;
	}

	PlanetManager* planetManager = zone->getPlanetManager();

	if (planetManager == nullptr) {
		player->info("No planet manager.", true);
		return;
	}

	TerrainManager* terrainManager = planetManager->getTerrainManager();

	if (terrainManager == nullptr) {
		player->info("No terrain manager.", true);
		return;
	}

	float landHeight = zone->getHeight(player->getPositionX(), player->getPositionY());
	float waterHeight = landHeight;
	bool waterIsDefined = terrainManager->getWaterHeight(player->getPositionX(), player->getPositionY(), waterHeight);

	if (waterIsDefined && (waterHeight > landHeight) && (landHeight + player->getSwimHeight() - waterHeight < 0.2)) {
		//Water level is higher than the terrain level and is deep enough for the player to be swimming.
		//Check if the player is on a bridge or other terrain above the water level.
		//SortedVector<IntersectionResult> intersections;
		Reference<IntersectionResults*> ref;

		if (intersections == nullptr) {
			ref = intersections = new IntersectionResults();

			CollisionManager::getWorldFloorCollisions(player->getPositionX(), player->getPositionY(), zone, intersections, closeObjectsVector);
		}

		for (int i = 0; i < intersections->size(); i++) {
			if (fabs(16384 - intersections->get(i).getIntersectionDistance() - newZ) < 0.2) {
				//Player is on terrain above the water.
				player->clearState(CreatureState::SWIMMING, true);
				return;
			}
		}

		//Player is in the water.
		player->setState(CreatureState::SWIMMING, true);
		return;
	}

	//Terrain is above water level.
	player->clearState(CreatureState::SWIMMING, true);
}

int PlayerManagerImplementation::checkSpeedHackFirstTest(CreatureObject* player, float parsedSpeed, ValidatedPosition& teleportPosition, float errorMultiplier) {
	float allowedSpeedMod = player->getSpeedMultiplierMod();
	float allowedSpeedBase = player->getRunSpeed();
	ManagedReference<SceneObject*> parent = player->getParent().get();
	SpeedMultiplierModChanges* changeBuffer = player->getSpeedMultiplierModChanges();
	Vector3 teleportPoint = teleportPosition.getPosition();
	uint64 teleportParentID = teleportPosition.getParent();

	if (parent != nullptr && parent->isVehicleObject()) {
		VehicleObject* vehicle = cast<VehicleObject*>( parent.get());

		allowedSpeedMod = vehicle->getSpeedMultiplierMod();
		allowedSpeedBase = vehicle->getRunSpeed();
	} else if (parent != nullptr && parent->isMount()) {
		Creature* mount = cast<Creature*>( parent.get());

		allowedSpeedMod = mount->getSpeedMultiplierMod();

		PetManager* petManager = server->getPetManager();

		if (petManager != nullptr) {
			allowedSpeedBase = petManager->getMountedRunSpeed(mount);
		}

	}

	float maxAllowedSpeed = allowedSpeedMod * allowedSpeedBase;

	if (parsedSpeed > maxAllowedSpeed * errorMultiplier) {
		//float delta = abs(parsedSpeed - maxAllowedSpeed);

		if (changeBuffer->size() == 0) { // no speed changes
			auto msg = player->info();
			msg << "max allowed speed should be " << maxAllowedSpeed * errorMultiplier;
			msg << " parsed " << parsedSpeed;

			msg.flush();

			player->teleport(teleportPoint.getX(), teleportPoint.getZ(), teleportPoint.getY(), teleportParentID);

			return 1;
		}

		SpeedModChange* firstChange = &changeBuffer->get(changeBuffer->size() - 1);
		const Time* timeStamp = &firstChange->getTimeStamp();

		if (timeStamp->miliDifference() > 2000) { // we already should have lowered the speed, 2 seconds lag
			auto msg = player->info();
			msg << "max allowed speed should be " << maxAllowedSpeed * errorMultiplier;
			msg << " parsed " << parsedSpeed;

			msg.flush();

			player->teleport(teleportPoint.getX(), teleportPoint.getZ(), teleportPoint.getY(), teleportParentID);

			return 1;
		}

		for (int i = 0; i < changeBuffer->size() - 1; ++i) {
			SpeedModChange* change = &changeBuffer->get(i);
			//Time timeStamp = change->getTimeStamp();

			float oldSpeedMod = change->getNewSpeed();
			float allowed = allowedSpeedBase * oldSpeedMod * errorMultiplier;

			if (allowed >= parsedSpeed) {
				return 0; // no hack detected
			}

			if (allowed > maxAllowedSpeed)
				maxAllowedSpeed = allowed;
		}

		auto msg = player->info();
		msg << "max allowed speed should be " << maxAllowedSpeed;
		msg << " parsed " << parsedSpeed;
		msg << " changeBufferSize: " << changeBuffer->size();

		msg.flush();

		player->teleport(teleportPoint.getX(), teleportPoint.getZ(), teleportPoint.getY(), teleportParentID);

		return 1;
	}

	return 0;
}

int PlayerManagerImplementation::checkSpeedHackSecondTest(CreatureObject* player, float newX, float newZ, float newY, uint32 newStamp, SceneObject* newParent) {
	PlayerObject* ghost = player->getPlayerObject();

	uint32 deltaTime = ghost->getServerMovementTimeDelta();//newStamp - stamp;

	if (deltaTime < 1000) {
		player->debug() << "deltaTime hasnt passed yet";
		return 0;
	}

	uint32 stamp = ghost->getClientLastMovementStamp();

	if (stamp > newStamp) {
		player->debug() << "older client movement stamp received";
		return 1;
	}

	Vector3 newWorldPosition(newX, newY, newZ);

	player->debug() << "checkSpeedHackSecondTest newWorldPosition x:" << newWorldPosition.getX() << " z:" << newWorldPosition.getZ() << " y:" << newWorldPosition.getY();

	if (newParent != nullptr) {
		ManagedReference<SceneObject*> root = newParent->getRootParent();

		if (!root->isBuildingObject())
			return 1;

		float length = Math::sqrt(newX * newX + newY * newY);
		float angle = root->getDirection()->getRadians() + atan2(newX, newY);

		newWorldPosition.set(root->getPositionX() + (sin(angle) * length), root->getPositionZ() + newZ, root->getPositionY() + (cos(angle) * length));
	}

	player->debug() << "after parent transform newWorldPosition x:" << newWorldPosition.getX() << " z:" << newWorldPosition.getZ() << " y:" << newWorldPosition.getY();

	ValidatedPosition* lastValidatedPosition = ghost->getLastValidatedPosition();

	Vector3 lastValidatedWorldPosition = lastValidatedPosition->getWorldPosition(server);

	//ignoring Z untill we have all heightmaps
	float oldValidZ = lastValidatedWorldPosition.getZ();
	float oldNewPosZ = newWorldPosition.getZ();

	lastValidatedWorldPosition.setZ(0);
	newWorldPosition.setZ(0);

	float dist = newWorldPosition.distanceTo(lastValidatedWorldPosition);

	if (dist < 1) {
		player->debug("speed hack distance too small");
		return 0;
	}

	float speed = dist / (float) deltaTime * 1000.f;

	/*if (oldNewPosZ > oldValidZ) {
		float heightDist = oldNewPosZ - oldValidZ;

		//if (heightDist > speed) {
			StringBuffer msg;
			msg << " heightDist:" << heightDist << " speed:" << speed << " terrain neg:" << player->getSlopeModPercent();
			player->info(msg.toString(), true);
		//}
	}*/

	//lastValidatedPosition->set(newWorldPosition.getX(), oldNewPosZ, newWorldPosition.getY());

	player->debug() << "distancia recorreguda " << dist << " a una velocitat " << speed;

	int ret = checkSpeedHackFirstTest(player, speed, *lastValidatedPosition, 1.5f);

	if (ret == 0) {
		lastValidatedPosition->setPosition(newX, newZ, newY);

		if (newParent != nullptr)
			lastValidatedPosition->setParent(newParent->getObjectID());
		else
			lastValidatedPosition->setParent(0);

		ghost->updateServerLastMovementStamp();

		if (ghost->isOnLoadScreen())
			ghost->setOnLoadScreen(false);

		ghost->incrementSessionMovement(dist);
	}

	return ret;
}

void PlayerManagerImplementation::lootAll(CreatureObject* player, CreatureObject* ai) {
	Locker locker(ai, player);

	if (!ai->isDead() || player->isDead())
		return;

	SceneObject* creatureInventory = ai->getSlottedObject("inventory");

	if (creatureInventory == nullptr)
		return;

	auto trxGroup = TransactionLog::getNewTrxGroup();

	int cashCredits = ai->getCashCredits();

	if (cashCredits > 0) {
		int luck = player->getSkillMod("force_luck");

		if (luck > 0)
			cashCredits += (cashCredits * luck) / 20;

		{
			TransactionLog trx(ai, player, TrxCode::NPCLOOTCLAIM, cashCredits, true);
			trx.setTrxGroup(trxGroup);
			trx.addState("srcDisplayedName", ai->getDisplayedName());
			player->addCashCredits(cashCredits, true);
			ai->clearCashCredits();
		}

		StringIdChatParameter param("base_player", "prose_coin_loot"); //You loot %DI credits from %TT.
		param.setDI(cashCredits);
		param.setTT(ai->getObjectID());

		player->sendSystemMessage(param);
	}

	ai->notifyObservers(ObserverEventType::LOOTCREATURE, player, 0);

	SceneObject* playerInventory = player->getSlottedObject("inventory");

	if (playerInventory == nullptr)
		return;

	int totalItems = creatureInventory->getContainerObjectsSize();

	if (totalItems < 1) {
		rescheduleCorpseDestruction(player, ai);
		return;
	}

	for (int i = totalItems - 1; i >= 0; --i) {
		SceneObject* object = creatureInventory->getContainerObject(i);

		TransactionLog trx(ai, player, object, TrxCode::NPCLOOTCLAIM);
		trx.setTrxGroup(trxGroup);

		TransferItemMiscCommand::doTransferItemMisc(player, object, playerInventory, -1, trx);
	}

	if (creatureInventory->getContainerObjectsSize() <= 0) {
		player->sendSystemMessage("@base_player:corpse_looted"); //You have completely looted the corpse of all items.

		rescheduleCorpseDestruction(player, ai);
	}
}

void PlayerManagerImplementation::sendStartingLocationsTo(CreatureObject* player) {
	StartingLocationListMessage* slm = new StartingLocationListMessage(player);
	startingLocationList.insertToMessage(slm);

	player->sendMessage(slm);
}

StartingLocation* PlayerManagerImplementation::getStartingLocation(const String& city) {
	for (int i = 0; i < startingLocationList.size(); ++i) {
		StartingLocation* loc = &startingLocationList.get(i);

		if (loc == nullptr)
			continue;

		if (loc->getLocation() == city)
			return loc;
	}

	return nullptr;
}

void PlayerManagerImplementation::addInsurableItemsRecursive(SceneObject* obj, SortedVector<ManagedReference<SceneObject*> >* items, bool onlyInsurable) {
	for (int j = 0; j < obj->getContainerObjectsSize(); j++) {
		SceneObject* object = obj->getContainerObject(j);

		if (!object->isTangibleObject())
			continue;

		TangibleObject* item = cast<TangibleObject*>( object);

		if (item == nullptr || item->hasAntiDecayKit() || item->isJediRobe() || item->isUnionRing())
			continue;

		if (!(item->getOptionsBitmask() & OptionBitmask::INSURED) && (item->isArmorObject() || item->isWearableObject())) {
			items->put(item);
		} else if ((item->getOptionsBitmask() & OptionBitmask::INSURED) && (item->isArmorObject() || item->isWearableObject()) && !onlyInsurable) {
			items->put(item);
		}

		if (object->isContainerObject())
			addInsurableItemsRecursive(object, items, onlyInsurable);
	}
}

SortedVector<ManagedReference<SceneObject*> > PlayerManagerImplementation::getInsurableItems(CreatureObject* player, bool onlyInsurable) {
	SortedVector<ManagedReference<SceneObject*> > insurableItems;
	insurableItems.setNoDuplicateInsertPlan();

	if (player == nullptr)
		return insurableItems;

	SceneObject* datapad = player->getSlottedObject("datapad");
	SceneObject* defweapon = player->getSlottedObject("default_weapon");
	SceneObject* bank = player->getSlottedObject("bank");

	for (int i = 0; i < player->getSlottedObjectsSize(); ++i) {
		SceneObject* container = player->getSlottedObject(i);

		if (container == datapad || container == nullptr || container == bank || container == defweapon)
			continue;

		if (container->isTangibleObject()) {
			TangibleObject* item = cast<TangibleObject*>( container);

			if (item == nullptr || item->hasAntiDecayKit() || item->isJediRobe() || item->isUnionRing())
				continue;

			if (!(item->getOptionsBitmask() & OptionBitmask::INSURED) && (item->isArmorObject() || item->isWearableObject())) {
				insurableItems.put(item);
			} else if ((item->getOptionsBitmask() & OptionBitmask::INSURED) && (item->isArmorObject() || item->isWearableObject()) && !onlyInsurable) {
				insurableItems.put(item);
			}
		}

		addInsurableItemsRecursive(container, &insurableItems, onlyInsurable);
	}

	return insurableItems;
}

SortedVector<ManagedReference<SceneObject*> > PlayerManagerImplementation::getInventoryItemsOfType(CreatureObject* player, int mask) {
	SortedVector<ManagedReference<SceneObject*> > matchedItems;
	matchedItems.setNoDuplicateInsertPlan();

	if (player == nullptr)
		return matchedItems;

	SceneObject* inventory = player->getSlottedObject("inventory");

	if ( inventory == nullptr )
		return matchedItems;

	for (int j = 0; j < inventory->getContainerObjectsSize(); j++) {
		SceneObject* item = inventory->getContainerObject(j);

		if (!item->isTangibleObject())
			continue;

		TangibleObject* tano = cast<TangibleObject*>( item);

		if (tano == nullptr )
			continue;

		if ( (tano -> getGameObjectType()) & mask ) {
			matchedItems.put(tano);
		}
	}

	return matchedItems;
}

int PlayerManagerImplementation::calculatePlayerLevel(CreatureObject* player) {

	ManagedReference<WeaponObject*> weapon = player->getWeapon();

	if (weapon == nullptr) {
		player->error("player with nullptr weapon");

		return 0;
	}

	String weaponType = weapon->getWeaponType();
	int skillMod = player->getSkillMod("private_" + weaponType + "_combat_difficulty");

	if (player->getPlayerObject() != nullptr && player->getPlayerObject()->isJedi() && weapon->isJediWeapon())
		skillMod += player->getSkillMod("private_jedi_difficulty");

	int level = Math::min(25, skillMod / 100 + 1);

	return level;
}

int PlayerManagerImplementation::calculatePlayerLevel(CreatureObject* player, String& xpType) {
	if (xpType.isEmpty() || xpType == "jedi_general")
		return calculatePlayerLevel(player);

	String weaponType;
	if (xpType.contains("onehand"))
		weaponType = "onehandmelee";
	else if (xpType.contains("polearm"))
		weaponType = "polearm";
	else if (xpType.contains("twohand"))
		weaponType = "twohandmelee";
	else if (xpType.contains("unarmed"))
		weaponType = "unarmed";
	else if (xpType.contains("carbine"))
		weaponType = "carbine";
	else if (xpType.contains("pistol"))
		weaponType = "pistol";
	else if (xpType.contains("rifle"))
		weaponType = "rifle";
	else
		weaponType = "heavyweapon";

	int level = Math::min(25, player->getSkillMod("private_" + weaponType + "_combat_difficulty") / 100 + 1);

	return level;
}

CraftingStation* PlayerManagerImplementation::getNearbyCraftingStation(CreatureObject* player, int type) {

	ManagedReference<Zone*> zone = player->getZone();

	if (zone == nullptr)
		return nullptr;

	ManagedReference<ZoneServer*> server = zone->getZoneServer();

	if (server == nullptr)
		return nullptr;

	ManagedReference<CraftingStation*> station = nullptr;

	//Locker locker(zone);

	CloseObjectsVector* vec = (CloseObjectsVector*) player->getCloseObjects();
	SortedVector<QuadTreeEntry*> closeObjects(vec->size(), 10);
	vec->safeCopyTo(closeObjects);

	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* scno = static_cast<SceneObject*> (closeObjects.get(i));
		if (scno->isCraftingStation() && (fabs(scno->getPositionZ() - player->getPositionZ()) < 7.0f) && player->isInRange(scno, 7.0f)) {

			station = server->getObject(scno->getObjectID()).castTo<CraftingStation*>();

			if (station == nullptr)
				continue;

			ManagedReference<SceneObject*> parent = station->getParent().get();

			if (parent != nullptr && !parent->isCellObject())
				continue;

			if (type == station->getStationType() || (type
					== CraftingTool::JEDI && station->getStationType()
					== CraftingTool::WEAPON)) {

				return station;
			}
		}
		// dont check Z axis here just check in range call. z axis check for some reason returns a huge number when checking a mob standing on you.
		// in range should be sufficient
		if (scno->isDroidObject() && player->isInRange(scno, 7.0f)) {
			// check the droids around
			DroidObject* droid = cast<DroidObject*>(scno);
			if (droid == nullptr) {
				continue;
			}
			// only the player can benefit from their own droid
			if (droid->getLinkedCreature() != player) {
				continue;
			}
			// check the droid
			station = droid->getCraftingStation(type);
			if (station != nullptr && droid->hasPower()) {
				return station;
			}
		}
	}

	return nullptr;
}

void PlayerManagerImplementation::finishHologrind(CreatureObject* player) {

	ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

	if (ghost->hasSuiBoxWindowType(SuiWindowType::HOLOGRIND_UNLOCK))
		return;

	ManagedReference<SuiMessageBox*> box = new SuiMessageBox(player, SuiWindowType::NONE);
	box->setPromptTitle("@quest/force_sensitive/intro:force_sensitive"); // You feel a tingle in the Force.
	box->setPromptText("Perhaps you should meditate somewhere alone...");

	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());

	SkillManager::instance()->awardSkill("force_title_jedi_novice", player, true, false, true);

	ghost->setJediState(1);

}

String PlayerManagerImplementation::banAccount(PlayerObject* admin, Account* account, uint32 seconds, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	try {
		StringBuffer query;
		query << "INSERT INTO account_bans values (NULL, " << account->getAccountID() << ", " << admin->getAccountID() << ", now(), " << (uint64)time(0) + seconds << ", '" << escapedReason << "');";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception banning account: " + e.getMessage();
	}

	Locker locker(account);

	account->setBanReason(reason);
	account->setBanExpires(time(0) + seconds);
	account->setBanAdmin(admin->getAccountID());

	StringBuffer banResult;

	Time expireTime;

	expireTime.addMiliTime(seconds * 1000);

	banResult << "Account \"" + account->getUsername() + "\" successfully banned until " << expireTime.getFormattedTime() + " server time";

	try {
		Reference<const CharacterList*> characters = account->getCharacterList();

		for (int i = 0; i < characters->size(); ++i) {
			const CharacterListEntry* entry = &characters->get(i);
			if (entry->getGalaxyID() == server->getGalaxyID()) {
				ManagedReference<CreatureObject*> player = getPlayer(entry->getFirstName());

				if (player != nullptr) {
					clearOwnedStructuresPermissions(player);

					if (player->isOnline()) {
						player->sendMessage(new LogoutMessage());

						Reference<ZoneClientSession*> session = player->getClient();

						if (session != nullptr)
							session->disconnect(true);
					}
				}
			}
		}
	} catch(Exception& e) {
		banResult << ", error kicking characters: " + e.getMessage();
	}

	banResult << ".";

	return banResult.toString();
}

String PlayerManagerImplementation::unbanAccount(PlayerObject* admin, Account* account, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	try {
		StringBuffer query;
		query << "UPDATE account_bans SET expires = UNIX_TIMESTAMP(), reason = '" << escapedReason << "'  WHERE account_id = " << account->getAccountID() << " and expires > UNIX_TIMESTAMP();";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception unbanning account: " + e.getMessage();
	}

	Locker locker(account);
	account->setBanExpires(System::getMiliTime());
	account->setBanReason(reason);

	return "Account Successfully Unbanned";
}

String PlayerManagerImplementation::banFromGalaxy(PlayerObject* admin, Account* account, const uint32 galaxy, uint32 seconds, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	try {
		StringBuffer query;
		query << "INSERT INTO galaxy_bans values (NULL, " << account->getAccountID() << ", " << admin->getAccountID() << "," << galaxy << ", now()," << (uint64)time(0) + seconds << ", '" << escapedReason << "');";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception banning from galaxy: " + e.getMessage();
	}

	Locker locker(account);

	Time current;
	Time expires;

	expires.addMiliTime(seconds*10000);

	Reference<GalaxyBanEntry*> ban = new GalaxyBanEntry();

	ban->setAccountID(account->getAccountID());
	ban->setBanAdmin(admin->getAccountID());
	ban->setGalaxyID(galaxy);

	ban->setCreationDate(current);

	ban->setBanExpiration(expires);

	ban->setBanReason(reason);

	account->addGalaxyBan(ban, galaxy);

	try {

		if (server->getGalaxyID() == galaxy) {
			Reference<const CharacterList*> characters = account->getCharacterList();

			for (int i = 0; i < characters->size(); ++i) {
				const CharacterListEntry* entry = &characters->get(i);
				if (entry->getGalaxyID() == galaxy) {

					ManagedReference<CreatureObject*> player = getPlayer(entry->getFirstName());
					if (player != nullptr) {
						clearOwnedStructuresPermissions(player);

						if (player->isOnline()) {
							player->sendMessage(new LogoutMessage());

							ManagedReference<ZoneClientSession*> session = player->getClient();

							if (session != nullptr)
								session->disconnect(true);
						}
					}
				}
			}
		} else {
			return "Successfully Banned from Galaxy, but cannot kick characters because Galaxy is not your current galaxy.";
		}
	} catch(Exception& e) {
		return "Successfully Banned from Galaxy, but error kicking characters. " + e.getMessage();
	}

	return "Successfully Banned from Galaxy";
}

String PlayerManagerImplementation::unbanFromGalaxy(PlayerObject* admin, Account* account, const uint32 galaxy, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	try {
		StringBuffer query;

		query << "UPDATE galaxy_bans SET expires = UNIX_TIMESTAMP(), reason = '" << escapedReason << "' WHERE account_id = " <<  account->getAccountID() << " and galaxy_id = " << galaxy << " and expires > UNIX_TIMESTAMP();";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception unbanning from galaxy: " + e.getMessage();
	}

	Locker locker(account);
	account->removeGalaxyBan(galaxy);

	return "Successfully Unbanned from Galaxy";
}

String PlayerManagerImplementation::banCharacter(PlayerObject* admin, Account* account, const String& name, const uint32 galaxyID, uint32 seconds, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	String escapedName = name;
	Database::escapeString(escapedName);

	try {
		StringBuffer query;
		query << "INSERT INTO character_bans values (NULL, " << account->getAccountID() << ", " << admin->getAccountID() << ", " << galaxyID << ", '" << escapedName << "', " <<  "now(), UNIX_TIMESTAMP() + " << seconds << ", '" << escapedReason << "');";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception banning character: " + e.getMessage();
	}

	Locker locker(account);

	Reference<const CharacterList*> characters = account->getCharacterList();

	for (int i = 0; i<characters->size(); i++) {
		CharacterListEntry& entry = characters->get(i);

		if (entry.getFirstName() == name && entry.getGalaxyID() == galaxyID) {
			Time expires;
			expires.addMiliTime(seconds*1000);

			entry.setBanReason(reason);
			entry.setBanAdmin(admin->getAccountID());
			entry.setBanExpiration(expires);
		}
	}

	locker.release();

	try {
		if (server->getGalaxyID() == galaxyID) {
			ManagedReference<CreatureObject*> player = getPlayer(name);
			if (player != nullptr) {
				clearOwnedStructuresPermissions(player);

				if (player->isOnline()) {
					player->sendMessage(new LogoutMessage());

					ManagedReference<ZoneClientSession*> session = player->getClient();

					if (session != nullptr)
						session->disconnect(true);
				}
			}
		} else {
			return "Character Successfully Banned, but cannot kick character because it's on a different galaxy.";
		}
	} catch(Exception& e) {
		return "Character Successfully Banned, but error kicking Character. " + e.getMessage();
	}

	return "Character Successfully Banned";
}

String PlayerManagerImplementation::unbanCharacter(PlayerObject* admin, Account* account, const String& name, const uint32 galaxyID, const String& reason) {

	if (admin == nullptr || !admin->isPrivileged())
		return "";

	if (account == nullptr)
		return "Account Not Found";

	String escapedReason = reason;
	Database::escapeString(escapedReason);

	String escapedName = name;
	Database::escapeString(escapedName);

	try {
		StringBuffer query;
		query << "UPDATE character_bans SET expires = UNIX_TIMESTAMP(), reason = '" << escapedReason << "' WHERE account_id = " <<  account->getAccountID() << " AND galaxy_id = " << galaxyID << " and name =  '" << escapedName << "' and expires > UNIX_TIMESTAMP();";

		ServerDatabase::instance()->executeStatement(query);
	} catch(Exception& e) {
		return "Exception banning character: " + e.getMessage();
	}

	Locker locker(account);
	CharacterListEntry *entry = account->getCharacterBan(galaxyID, name);

	if (entry != nullptr) {
		Time now;
		entry->setBanExpiration(now);
		entry->setBanReason(reason);
	}

	return "Character Successfully Unbanned";
}

void PlayerManagerImplementation::clearOwnedStructuresPermissions(CreatureObject* player) {
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr) {
		return;
	}

	for (int i = 0; i < ghost->getTotalOwnedStructureCount(); i++) {
		uint64 structureID = ghost->getOwnedStructure(i);

		ManagedReference<StructureObject*> structure = server->getObject(structureID).castTo<StructureObject*>();

		if (structure == nullptr) {
			continue;
		}

		structure->revokeAllPermissions();
	}
}

void PlayerManagerImplementation::fixHAM(CreatureObject* player) {
	Locker locker(player);

	try {
		const BuffList* buffs = player->getBuffList();

		VectorMap<byte, int> attributeValues;
		attributeValues.setNullValue(0);
		attributeValues.setAllowOverwriteInsertPlan();

		ManagedReference<Buff*> powerBoost;
		ManagedReference<Buff*> forceWeaken;

		//check buffs
		for (int i = 0; i < buffs->getBuffListSize(); ++i) {
			ManagedReference<Buff*> buff = buffs->getBuffByIndex(i);

			PowerBoostBuff* power = dynamic_cast<PowerBoostBuff*>(buff.get());

			if (power != nullptr) {
				powerBoost = power;
				continue;
			}

			ForceWeakenDebuff* debuff = dynamic_cast<ForceWeakenDebuff*>(buff.get());

			if (debuff != nullptr) {
				forceWeaken = debuff;
				continue;
			}

			const VectorMap<byte, int>* attributeModifiers = buff->getAttributeModifiers();

			for (int j = 0; j < attributeModifiers->size(); ++j) {
				byte modifier = attributeModifiers->elementAt(j).getKey();
				int val = attributeModifiers->elementAt(j).getValue();

				attributeValues.put(modifier, attributeValues.get(modifier) + val);
			}
		}

		if (powerBoost != nullptr) {
			Locker buffLocker(powerBoost);

			player->removeBuff(powerBoost);
		}

		if (forceWeaken != nullptr) {
			Locker buffLocker(forceWeaken);

			player->removeBuff(forceWeaken);
		}

		int encumbranceType = -1;

		for (int i = 0; i < 9; ++i) {
			int maxModifier = attributeValues.get((byte)i);
			int baseHam = player->getBaseHAM(i);
			int max = player->getMaxHAM(i);

			int calculated = baseHam + maxModifier;

			if (i % 3 == 0) {
				++encumbranceType;
			} else {
				calculated -= player->getEncumbrance(encumbranceType);
			}

			//info("attribute: " + CreatureAttribute::getName(i, true) + " max = " + String::valueOf(max) + " calculatedMax = " + String::valueOf(calculated), true);

			if (calculated != max && calculated > 1) {
				if (player->getHAM(i) > calculated)
					player->setHAM(i, calculated, false);

				player->setMaxHAM(i, calculated, false);
			}
		}
	} catch (const Exception& e) {
		error(e.getMessage());
	}
}

void PlayerManagerImplementation::fixBuffSkillMods(CreatureObject* player) {
	Locker locker(player);

	try {
		GroupObject* grp = player->getGroup();
		if (grp != nullptr)
			GroupManager::instance()->leaveGroup(grp, player);

		Reference<Buff*> buff = player->getBuff(STRING_HASHCODE("squadleader"));
		if (buff != nullptr) {
			Locker locker(buff);
			player->removeBuff(buff);
		}

		if (player->getSkillModList() == nullptr)
			return;

		Locker smodsGuard(player->getSkillModMutex());

		SkillModGroup* smodGroup = player->getSkillModList()->getSkillModGroup(SkillModManager::BUFF);
		smodGroup->removeAll();

		smodsGuard.release();

		const BuffList* buffs = player->getBuffList();

		for (int i = 0; i < buffs->getBuffListSize(); i++) {
			ManagedReference<Buff*> buff = buffs->getBuffByIndex(i);

			Locker clocker(buff, player);

			buff->setModsApplied(true);

			buff->applySkillModifiers();
		}

		if (grp != nullptr && grp->getLeader() != nullptr) {
			player->updateGroupInviterID(grp->getLeader()->getObjectID());
			GroupManager::instance()->joinGroup(player);
		}
	} catch (const Exception& e) {
		error(e.getMessage());
	}
}

bool PlayerManagerImplementation::promptTeachableSkills(CreatureObject* teacher, SceneObject* target) {
	if (target == nullptr || !target->isPlayerCreature()) {
		teacher->sendSystemMessage("@teaching:no_target"); //Whom do you want to teach?
		return false;
	}

	if (target == teacher) {
		teacher->sendSystemMessage("@teaching:no_teach_self"); //You cannot teach yourself.
		return false;
	}

	Locker _lock(target, teacher);

	//We checked if they had the player object in slot with isPlayerCreature
	CreatureObject* student = cast<CreatureObject*>(target);

	if (teacher->getGroup() == nullptr || student->getGroup() != teacher->getGroup()) {
		StringIdChatParameter params("teaching", "not_in_same_group"); //You must be within the same group as %TT in order to teach.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	if (student->isDead()) {
		StringIdChatParameter params("teaching", "student_dead"); //%TT does not feel like being taught right now.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	if (!student->isInRange(teacher, 32.f)) {
		teacher->sendSystemMessage("@teaching:student_too_far"); // Your student must be nearby in order to teach.
		return false;
	}

	ManagedReference<PlayerObject*> studentGhost = student->getPlayerObject();

	//Do they have an outstanding teaching offer?
	if (studentGhost->hasSuiBoxWindowType(SuiWindowType::TEACH_OFFER)) {
		StringIdChatParameter params("teaching", "student_has_offer_to_learn"); //%TT already has an offer to learn.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	SortedVector<String> skills = getTeachableSkills(teacher, student);

	if (skills.size() <= 0) {
		StringIdChatParameter params("teaching", "no_skills_for_student"); //You have no skills that  %TT can currently learn."
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	ManagedReference<SuiListBox*> listbox = new SuiListBox(teacher, SuiWindowType::TEACH_SKILL);
	listbox->setUsingObject(student);
	listbox->setForceCloseDistance(32.f);
	listbox->setPromptTitle("SELECT SKILL");
	listbox->setPromptText("Select a skill to teach.");
	listbox->setCancelButton(true, "@cancel");

	for (int i = 0; i < skills.size(); ++i) {
		const String& skill = skills.get(i);
		listbox->addMenuItem("@skl_n:" + skill, skill.hashCode());
	}

	listbox->setCallback(new PlayerTeachSuiCallback(server));

	if (teacher->isPlayerCreature()) {
		ManagedReference<PlayerObject*> teacherGhost = teacher->getPlayerObject();

		teacherGhost->addSuiBox(listbox);
	}

	teacher->sendMessage(listbox->generateMessage());

	return true;
}

bool PlayerManagerImplementation::offerTeaching(CreatureObject* teacher, CreatureObject* student, Skill* skill) {
	ManagedReference<PlayerObject*> studentGhost = student->getPlayerObject();

	//Do they have an outstanding teaching offer?
	if (studentGhost->hasSuiBoxWindowType(SuiWindowType::TEACH_OFFER)) {
		StringIdChatParameter params("teaching", "student_has_offer_to_learn"); //%TT already has an offer to learn.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	ManagedReference<SuiMessageBox*> suibox = new SuiMessageBox(teacher, SuiWindowType::TEACH_OFFER);
	suibox->setUsingObject(teacher);
	suibox->setForceCloseDistance(32.f);
	suibox->setPromptTitle("@sui:swg"); //Star Wars Galaxies

	StringIdManager* sidman = StringIdManager::instance();

	String sklname = sidman->getStringId(String("@skl_n:" + skill->getSkillName()).hashCode()).toString();
	String expname = sidman->getStringId(String("@exp_n:" + skill->getXpType()).hashCode()).toString();

	StringBuffer prompt;
	prompt << teacher->getDisplayedName()
															<< " has offered to teach you " << sklname << " (" << skill->getXpCost()
															<< " " << expname  << " experience cost).";

	suibox->setPromptText(prompt.toString());
	suibox->setCallback(new PlayerTeachConfirmSuiCallback(server, skill));

	suibox->setOkButton(true, "@yes");
	suibox->setCancelButton(true, "@no");

	studentGhost->addSuiBox(suibox);
	student->sendMessage(suibox->generateMessage());

	StringIdChatParameter params("teaching", "offer_given"); //You offer to teach %TT %TO.
	params.setTT(student->getDisplayedName());
	params.setTO("@skl_n:" + skill->getSkillName());
	teacher->sendSystemMessage(params);

	return true;
}

bool PlayerManagerImplementation::acceptTeachingOffer(CreatureObject* teacher, CreatureObject* student, Skill* skill) {
	if (teacher->getGroup() == nullptr || student->getGroup() != teacher->getGroup()) {
		StringIdChatParameter params("teaching", "not_in_same_group"); //You must be within the same group as %TT in order to teach.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);
		return false;
	}

	//Check to see if the teacher still has the skill and the student can still learn the skill.
	SkillManager* skillManager = SkillManager::instance();

	if (!student->isInRange(teacher, 32.f)) {
		StringIdChatParameter params("teaching", "teacher_too_far_target"); //You are too far away from %TT to learn.
		params.setTT(teacher->getDisplayedName());
		student->sendSystemMessage(params);

		params.setStringId("teaching", "student_too_far_target");
		params.setTT(student->getDisplayedName()); //You are too far away from %TT to teach.
		teacher->sendSystemMessage(params);
		return false;
	}

	if (teacher->hasSkill(skill->getSkillName()) && skillManager->awardSkill(skill->getSkillName(), student, true, false, false)) {
		StringIdChatParameter params("teaching", "student_skill_learned"); //You learn %TO from %TT.
		params.setTO("@skl_n:" + skill->getSkillName());
		params.setTT(teacher->getDisplayedName());
		student->sendSystemMessage(params);

		params.setStringId("teaching", "teacher_skill_learned"); //%TT learns %TO from you.
		params.setTT(student->getDisplayedName());
		teacher->sendSystemMessage(params);

		if (skillManager->isApprenticeshipEnabled() && !skill->getSkillName().endsWith("novice")) {
			int exp = 10 + (skill->getTotalChildren() * 10);

			StringIdChatParameter params("teaching", "experience_received"); //You have received %DI Apprenticeship experience.
			params.setDI(exp);
			teacher->sendSystemMessage(params);

			awardExperience(teacher, "apprenticeship", exp, false);
		}
	} else {
		student->sendSystemMessage("@teaching:learning_failed"); //Learning failed.
		teacher->sendSystemMessage("@teaching:teaching_failed"); //Teaching failed.
		return false;
	}

	return true;
}

SortedVector<String> PlayerManagerImplementation::getTeachableSkills(CreatureObject* teacher, CreatureObject* student) {
	SortedVector<String> skills;
	skills.setNoDuplicateInsertPlan();

	const SkillList* skillList = teacher->getSkillList();

	SkillManager* skillManager = SkillManager::instance();

	for (int i = 0; i < skillList->size(); ++i) {
		const Skill* skill = skillList->get(i);

		const auto& skillName = skill->getSkillName();

		if (!(skillName.contains("novice") || skillName.contains("force_sensitive") || skillName.contains("force_rank") || skillName.contains("force_title") || skillName.contains("admin_")) && skillManager->canLearnSkill(skillName, student, false))
			skills.put(skillName);
	}

	return skills;
}

void PlayerManagerImplementation::decreaseOnlineCharCount(ZoneClientSession* client) {
	Locker locker(&onlineMapMutex);

	auto server = ServerCore::getZoneServer();
	uint32 accountId = client->getAccountID();

	if (!onlineZoneClientMap.containsKey(accountId)) {
		error("decreaseOnlineCharCount missing account " + String::valueOf(accountId) + " in onlineZoneClientMap");
		onlineZoneClientMap.accountLoggedOut(client->getIPAddress(), accountId, server != nullptr ? server->getGalaxyID() : 0);
		return;
	}

	Vector<Reference<ZoneClientSession*> > clients = onlineZoneClientMap.get(accountId);

	for (int i = 0; i < clients.size(); ++i)
		if (clients.get(i) == client) {
			clients.remove(i);

			break;
		}

	if (clients.size() == 0) {
		onlineZoneClientMap.remove(accountId);
		onlineZoneClientMap.accountLoggedOut(client->getIPAddress(), accountId, server != nullptr ? server->getGalaxyID() : 0);
	} else
		onlineZoneClientMap.put(accountId, clients);

	locker.release();
}

void PlayerManagerImplementation::proposeUnity( CreatureObject* askingPlayer, CreatureObject* respondingPlayer, SceneObject* askingPlayerRing) {
	if (!askingPlayer->isPlayerCreature()) {
		return;
	}

	// Check if target is self
	if (askingPlayer == respondingPlayer) {
		askingPlayer->sendSystemMessage("@unity:bad_target"); // "You must have a valid player target to Propose Unity."
		return;
	}

	// Check if target is a player
	if (!respondingPlayer->isPlayerCreature()) {
		askingPlayer->sendSystemMessage("@unity:bad_target"); // "You must have a valid player target to Propose Unity."
		return;
	}

	Reference<PlayerObject*> askingGhost = askingPlayer->getPlayerObject();
	Reference<PlayerObject*> respondingGhost = respondingPlayer->getPlayerObject();
	if (askingGhost == nullptr || respondingGhost == nullptr) {
		return;
	}

	// Check if askingPlayer is married
	if (askingGhost->isMarried()) {
		StringIdChatParameter errAskerMarried;
		errAskerMarried.setStringId("unity", "prose_already_married"); // "You cannot propose unity. You are already united with %TO."
		errAskerMarried.setTO( askingGhost->getSpouseName() );
		askingPlayer->sendSystemMessage(errAskerMarried );
		return;
	}

	// Check if respondingPlayer is married
	if (respondingGhost->isMarried()) {
		askingPlayer->sendSystemMessage("@unity:target_married"); // "You cannot propose unity to someone who is already united."
		return;
	}

	// Check distance
	if (!respondingPlayer->isInRange( askingPlayer, 15.0 )) {
		askingPlayer->sendSystemMessage("@unity:out_of_range"); // "Your target is too far away to properly propose!"
		return;
	}

	if (!askingPlayer->isFacingObject(respondingPlayer)) {
		askingPlayer->sendSystemMessage("@unity:bad_facing"); // "You must be facing your target to properly propose!"
		return;
	}

	// Check if asking player has a proposal outstanding
	if (askingPlayer->getActiveSession(SessionFacadeType::PROPOSEUNITY) != nullptr) {
		askingPlayer->sendSystemMessage("But you already have an outstanding unity proposal");
		return;
	}

	// Check if responding player has a proposal outstanding
	if (respondingPlayer->getActiveSession(SessionFacadeType::PROPOSEUNITY) != nullptr) {
		askingPlayer->sendSystemMessage("@unity:target_proposed"); // "Your proposal target is already engaged in a unity proposal."
		return;
	}

	//
	// All checks passed
	//

	if (askingPlayer->getPosture() != CreaturePosture::CROUCHED) {
		askingPlayer->setPosture(CreaturePosture::CROUCHED);
	}

	Locker rlocker( askingPlayer, respondingPlayer );

	// Initialize session
	ManagedReference<ProposeUnitySession*> askerUnitySession =
			new ProposeUnitySession( askingPlayer->getObjectID(), respondingPlayer->getObjectID(), askingPlayerRing->getObjectID() );
	askingPlayer->addActiveSession(SessionFacadeType::PROPOSEUNITY, askerUnitySession);

	ManagedReference<ProposeUnitySession*> responderUnitySession =
			new ProposeUnitySession( askingPlayer->getObjectID(), respondingPlayer->getObjectID(), askingPlayerRing->getObjectID() );
	respondingPlayer->addActiveSession(SessionFacadeType::PROPOSEUNITY, responderUnitySession);

	// Submit timeout task
	Reference<Task*> askerExpiredTask = new ProposeUnityExpiredTask( askingPlayer );
	askingPlayer->addPendingTask("propose_unity", askerExpiredTask, 60000); // 1 min

	Reference<Task*> responderExpiredTask = new ProposeUnityExpiredTask( respondingPlayer );
	respondingPlayer->addPendingTask("propose_unity", responderExpiredTask, 60000); // 1 min

	// Build and send proposal window
	ManagedReference<SuiMessageBox*> suiBox = new SuiMessageBox(respondingPlayer, SuiWindowType::PROPOSE_UNITY);
	suiBox->setCallback(new ProposeUnitySuiCallback(server));
	suiBox->setPromptTitle("@unity:accept_title"); // "Accept Unity Proposal?"
	suiBox->setPromptText( askingPlayer->getCreatureName().toString() + " is proposing unity to you. Do you wish to accept?" );
	suiBox->setUsingObject( askingPlayer );
	suiBox->setCancelButton(true, "@no");
	suiBox->setOkButton(true, "@yes");

	respondingGhost->addSuiBox(suiBox);
	respondingPlayer->sendMessage(suiBox->generateMessage());

	// Send message to asking player
	StringIdChatParameter proposalSent;
	proposalSent.setStringId("unity", "prose_propose"); // "You propose unity to %TO."
	proposalSent.setTO( respondingPlayer->getFirstName() );
	askingPlayer->sendSystemMessage(proposalSent );

}

void PlayerManagerImplementation::denyUnity( CreatureObject* respondingPlayer) {

	if (respondingPlayer == nullptr )
		return;

	// Check session
	ManagedReference<ProposeUnitySession*> proposeUnitySession = respondingPlayer->getActiveSession(SessionFacadeType::PROPOSEUNITY).castTo<ProposeUnitySession*>();
	if (proposeUnitySession == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:expire_target"); // "The unity proposal extended to you has expired."
		return;
	}

	// Pull asking player
	uint64 targID = proposeUnitySession->getAskingPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);
	if (obj == nullptr || !obj->isPlayerCreature()) {
		respondingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		return;
	}

	CreatureObject* askingPlayer = cast<CreatureObject*>( obj.get());
	Locker alocker( askingPlayer, respondingPlayer );
	askingPlayer->sendSystemMessage("@unity:declined"); // "Your unity proposal has been declined.")
	respondingPlayer->sendSystemMessage("@unity:decline"); // "You decline the unity proposal.")

	// Remove session
	cancelProposeUnitySession(respondingPlayer, askingPlayer);

}

void PlayerManagerImplementation::acceptUnity( CreatureObject* respondingPlayer) {

	if (respondingPlayer == nullptr )
		return;

	// Check session
	ManagedReference<ProposeUnitySession*> proposeUnitySession = respondingPlayer->getActiveSession(SessionFacadeType::PROPOSEUNITY).castTo<ProposeUnitySession*>();
	if (proposeUnitySession == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:expire_target"); // "The unity proposal extended to you has expired."
		return;
	}

	// Pull asking player
	uint64 targID = proposeUnitySession->getAskingPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);
	if (obj == nullptr || !obj->isPlayerCreature()) {
		respondingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		return;
	}

	CreatureObject* askingPlayer = cast<CreatureObject*>( obj.get());
	Locker alocker( askingPlayer, respondingPlayer );

	// Check distance
	if (!respondingPlayer->isInRange( askingPlayer, 15.0 )) {
		askingPlayer->sendSystemMessage("@unity:wed_oor"); // "You must remain within 15 meters during the unity process for it to complete."
		respondingPlayer->sendSystemMessage("@unity:wed_oor"); // "You must remain within 15 meters during the unity process for it to complete."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	}

	// Check for a ring in player's inventory
	ManagedReference<SceneObject*> inventory = respondingPlayer->getSlottedObject("inventory");
	if (inventory == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		askingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	}

	ManagedReference<WearableObject*> wearable = nullptr;
	Vector<WearableObject*> rings;

	// Check proposal target for unequipped applicable rings
	for (int i = 0; i < inventory->getContainerObjectsSize(); i++) {
		wearable = cast<WearableObject*>(inventory->getContainerObject(i).get());

		if (wearable != nullptr && wearable->getGameObjectType() == SceneObjectType::RING && !wearable->isNoTrade()) {
			rings.add(wearable);
		}
	}

	// Check equipped items for applicable rings
	VectorMap<String, ManagedReference<SceneObject* > > slotted;
	respondingPlayer->getSlottedObjects(slotted);

	for (int i = 0; i < slotted.size(); ++i) {
		SceneObject* object = slotted.get(i);

		if (object != nullptr && object->isWearableObject() && object->getGameObjectType() == SceneObjectType::RING && !object->isNoTrade()) {
			wearable = cast<WearableObject*>(object);

			if (wearable != nullptr) {
				rings.add(wearable);
			}
		}
	}

	// No ring found
	if (rings.size() <= 0) {
		askingPlayer->sendSystemMessage("@unity:accept_fail"); // "Your proposal target has no ring to offer in return."
		respondingPlayer->sendSystemMessage("@unity:no_ring"); // "You cannot accept a unity proposal without a ring to offer."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	} else {
		// Build and send list box for ring selection
		ManagedReference<SuiListBox*> box = new SuiListBox(respondingPlayer, SuiWindowType::SELECT_UNITY_RING, SuiListBox::HANDLETWOBUTTON);
		box->setCallback(new SelectUnityRingSuiCallback(server));
		box->setPromptText("@unity:ring_prompt"); // "Select the ring you would like to offer, in return, for your unity."
		box->setPromptTitle("Select Unity Ring");
		box->setOkButton(true, "@ok");
		box->setCancelButton(true, "@cancel");

		for (int i = 0; i < rings.size(); i++) {
			WearableObject* ring = rings.get(i);

			if (ring != nullptr) {
				String itemName = ring->getDisplayedName();
				box->addMenuItem(itemName, ring->getObjectID());
			}
		}

		box->setUsingObject(respondingPlayer);
		respondingPlayer->getPlayerObject()->addSuiBox(box);
		respondingPlayer->sendMessage(box->generateMessage());
	}
}

void PlayerManagerImplementation::completeUnity( CreatureObject* respondingPlayer, unsigned long long respondingPlayerRingId) {
	if (respondingPlayer == nullptr )
		return;

	// Check session
	ManagedReference<ProposeUnitySession*> proposeUnitySession = respondingPlayer->getActiveSession(SessionFacadeType::PROPOSEUNITY).castTo<ProposeUnitySession*>();
	if (proposeUnitySession == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:expire_target"); // "The unity proposal extended to you has expired."
		return;
	}

	// Pull asking player
	uint64 targID = proposeUnitySession->getAskingPlayer();
	ManagedReference<SceneObject*> obj = server->getObject(targID);
	if (obj == nullptr || !obj->isPlayerCreature()) {
		respondingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		return;
	}

	CreatureObject* askingPlayer = cast<CreatureObject*>( obj.get());
	Locker alocker( askingPlayer, respondingPlayer );

	// Check distance
	if (!respondingPlayer->isInRange( askingPlayer, 15.0 )) {
		askingPlayer->sendSystemMessage("@unity:wed_oor"); // "You must remain within 15 meters during the unity process for it to complete."
		respondingPlayer->sendSystemMessage("@unity:wed_oor"); // "You must remain within 15 meters during the unity process for it to complete."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	}

	// Check if Asking and Responding players Inventories are null
	ManagedReference<SceneObject*> respondingPlayerInventory = respondingPlayer->getSlottedObject("inventory");
	ManagedReference<SceneObject*> askingPlayerInventory = askingPlayer->getSlottedObject("inventory");

	if (respondingPlayerInventory == nullptr || askingPlayerInventory == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		askingPlayer->sendSystemMessage("@unity:wed_error"); // "An error has occurred during the unity process."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	}

	ManagedReference<WearableObject*> wearable = nullptr;
	ManagedReference<WearableObject*> responderRing = nullptr;

	// Find responder's ring -- unequipped items
	for (int i = 0; i < respondingPlayerInventory->getContainerObjectsSize(); i++) {
		wearable = cast<WearableObject*>(respondingPlayerInventory->getContainerObject(i).get());
		if (wearable != nullptr && wearable->getObjectID() == respondingPlayerRingId) {
			responderRing = wearable;
			break;
		}
	}

	// If responderRing is null, check equipped items
	if (responderRing == nullptr) {
		VectorMap<String, ManagedReference<SceneObject* > > slotted;
		respondingPlayer->getSlottedObjects(slotted);

		for (int i = 0; i < slotted.size(); ++i) {
			SceneObject* responderObject = slotted.get(i);

			if (responderObject != nullptr && responderObject->isWearableObject() && responderObject->getGameObjectType() == SceneObjectType::RING) {
				wearable = cast<WearableObject*>(responderObject);

				if (wearable != nullptr && wearable->getObjectID() == respondingPlayerRingId) {
					responderRing = wearable;
					break;
				}
			}
		}
	}

	// Find asker's ring -- unequipped items
	wearable = nullptr;
	ManagedReference<WearableObject*> askerRing = nullptr;
	for (int i = 0; i < askingPlayerInventory->getContainerObjectsSize(); i++) {
		wearable = cast<WearableObject*>(askingPlayerInventory->getContainerObject(i).get());
		if (wearable != nullptr && wearable->getObjectID() == proposeUnitySession->getAskingPlayerRing()) {
			askerRing = wearable;
			break;
		}
	}

	// If askerRing is null, check equipped items
	if (askerRing == nullptr) {
		VectorMap<String, ManagedReference<SceneObject* > > slotted;
		askingPlayer->getSlottedObjects(slotted);

		for (int i = 0; i < slotted.size(); ++i) {
			SceneObject* askingObject = slotted.get(i);

			if (askingObject != nullptr && askingObject->isWearableObject() && askingObject->getGameObjectType() == SceneObjectType::RING) {
				wearable = cast<WearableObject*>(askingObject);

				if (wearable != nullptr && wearable->getObjectID() == proposeUnitySession->getAskingPlayerRing()) {
					askerRing = wearable;
					break;
				}
			}
		}
	}


	// Rings not found
	if (responderRing == nullptr) {
		askingPlayer->sendSystemMessage("@unity:accept_fail"); // "Your proposal target has no ring to offer in return."
		respondingPlayer->sendSystemMessage("@unity:no_ring"); // "You cannot accept a unity proposal without a ring to offer."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	} else if (askerRing == nullptr) {
		respondingPlayer->sendSystemMessage("@unity:accept_fail"); // "Your proposal target has no ring to offer in return."
		askingPlayer->sendSystemMessage("@unity:no_ring"); // "You cannot accept a unity proposal without a ring to offer."
		cancelProposeUnitySession(respondingPlayer, askingPlayer);
		return;
	}

	// Exchange rings, set Insured and set as Union Ring
	ManagedReference<ObjectController*> objectController = server->getObjectController();
	if (objectController->transferObject(askerRing, respondingPlayerInventory, -1, true, true)) { // Allow overflow
		Locker aringlock(askerRing);
		askerRing->sendDestroyTo(askingPlayer);

		respondingPlayerInventory->broadcastObject(askerRing, true);

		askerRing->setOptionsBitmask(OptionBitmask::INSURED);
		askerRing->setUnionRing(true);
	}

	if (objectController->transferObject(responderRing, askingPlayerInventory, -1, true, true)) { // Allow overflow
		Locker rringlock(responderRing);
		responderRing->sendDestroyTo(respondingPlayer);

		askingPlayerInventory->broadcastObject(responderRing, true);

		responderRing->setOptionsBitmask(OptionBitmask::INSURED);
		responderRing->setUnionRing(true);
	}

	// Set married
	String respondingPlayerName = respondingPlayer->getFirstName();
	String askingPlayerName = askingPlayer->getFirstName();
	askingPlayer->getPlayerObject()->setSpouseName( respondingPlayerName );
	respondingPlayer->getPlayerObject()->setSpouseName( askingPlayerName );

	// Send obligatory congratulations
	StringIdChatParameter congratsAsker;
	congratsAsker.setStringId("unity", "prose_wed_complete"); //  "Your union with %TT is complete."
	congratsAsker.setTT( respondingPlayer->getFirstName() );
	askingPlayer->sendSystemMessage(congratsAsker );

	StringIdChatParameter congratsResponder;
	congratsResponder.setStringId("unity", "prose_wed_complete"); //  "Your union with %TT is complete."
	congratsResponder.setTT( askingPlayer->getFirstName() );
	respondingPlayer->sendSystemMessage(congratsResponder );

	// Remove session
	cancelProposeUnitySession(respondingPlayer, askingPlayer);

}

void PlayerManagerImplementation::cancelProposeUnitySession(CreatureObject* respondingPlayer, CreatureObject* askingPlayer) {
	askingPlayer->dropActiveSession(SessionFacadeType::PROPOSEUNITY);
	respondingPlayer->dropActiveSession(SessionFacadeType::PROPOSEUNITY);
	askingPlayer->removePendingTask( "propose_unity" );
	respondingPlayer->removePendingTask( "propose_unity" );
}

void PlayerManagerImplementation::promptDivorce(CreatureObject* player) {
	if (player == nullptr || !player->isPlayerCreature())
		return;

	// Check if player is married
	PlayerObject* playerGhost = player->getPlayerObject();

	if (playerGhost == nullptr)
		return;

	if (!playerGhost->isMarried()) {
		player->sendSystemMessage("You are not united with anyone!");
		return;
	}

	// Build and confirmation window
	ManagedReference<SuiMessageBox*> suiBox = new SuiMessageBox(player, SuiWindowType::CONFIRM_DIVORCE);
	suiBox->setCallback(new ConfirmDivorceSuiCallback(server));
	suiBox->setPromptTitle("Confirm Divorce?");
	suiBox->setPromptText("Do you wish to nullify your unity with " + playerGhost->getSpouseName() + "?");
	suiBox->setCancelButton(true, "@no");
	suiBox->setOkButton(true, "@yes");

	playerGhost->addSuiBox(suiBox);
	player->sendMessage(suiBox->generateMessage());
}

void PlayerManagerImplementation::grantDivorce(CreatureObject* player) {
	if (player == nullptr || !player->isPlayerCreature())
		return;

	// Check if player is married
	PlayerObject* playerGhost = player->getPlayerObject();

	if (playerGhost == nullptr || !playerGhost->isMarried())
		return;

	// Get & check players inventory
	ManagedReference<SceneObject*> playerInventory = player->getSlottedObject("inventory");

	if (playerInventory == nullptr) {
		return;
	}

	// Check for unionRing and set false
	ManagedReference<WearableObject*> wearable = nullptr;
	ManagedReference<WearableObject*> ring = nullptr;

	// Unequipped items
	for (int i = 0; i < playerInventory->getContainerObjectsSize(); i++) {
		wearable = cast<WearableObject*>(playerInventory->getContainerObject(i).get());
		if (wearable != nullptr && wearable->isUnionRing()) {
			ring = wearable;
			break;
		}
	}

	// If ring is null, check equipped items
	if (ring == nullptr) {
		VectorMap<String, ManagedReference<SceneObject* > > slotted;
		player->getSlottedObjects(slotted);

		for (int i = 0; i < slotted.size(); ++i) {
			SceneObject* askingObject = slotted.get(i);

			if (askingObject != nullptr && askingObject->isWearableObject() && askingObject->getGameObjectType() == SceneObjectType::RING) {
				wearable = cast<WearableObject*>(askingObject);

				if (wearable != nullptr && wearable->isUnionRing()) {
					ring = wearable;
					break;
				}
			}
		}
	}

	if (ring != nullptr) {
		Locker lock(ring);
		ring->setUnionRing(false);
	}

	// Find spouse
	CreatureObject* spouse = getPlayer(playerGhost->getSpouseName());

	StringIdChatParameter msg;
	msg.setStringId("unity", "prose_end_unity"); // "Your union with %TO has ended."

	// Remove spouse name from both players
	if (spouse != nullptr && spouse->isPlayerCreature()) {
		Locker slocker(spouse, player);

		PlayerObject* spouseGhost = spouse->getPlayerObject();

		if (spouseGhost != nullptr)
			spouseGhost->removeSpouse();

		playerGhost->removeSpouse();

		msg.setTO(player->getFirstName());
		spouse->sendSystemMessage(msg);

		msg.setTO(spouse->getFirstName());
		player->sendSystemMessage(msg);

		// Get spouses inventory
		ManagedReference<SceneObject*> spouseInventory = spouse->getSlottedObject("inventory");

		if (spouseInventory != nullptr) {
			ring = nullptr;
			wearable = nullptr;

			// Check for Union Ring
			for (int i = 0; i < spouse->getContainerObjectsSize(); i++) {
				wearable = cast<WearableObject*>(spouseInventory->getContainerObject(i).get());
				if (wearable != nullptr && wearable->isUnionRing()) {
					ring = wearable;
					break;
				}
			}

			// If ring is null, check equipped items
			if (ring == nullptr) {
				VectorMap<String, ManagedReference<SceneObject* > > slotted;
				spouse->getSlottedObjects(slotted);

				for (int i = 0; i < slotted.size(); ++i) {
					SceneObject* askingObject = slotted.get(i);

					if (askingObject != nullptr && askingObject->isWearableObject() && askingObject->getGameObjectType() == SceneObjectType::RING) {
						wearable = cast<WearableObject*>(askingObject);

						if (wearable != nullptr && wearable->isUnionRing()) {
							ring = wearable;
							break;
						}
					}
				}
			}

			if (ring != nullptr) {
				Locker srlock(ring);
				ring->setUnionRing(false);
			}
		}

	} else {
		// If spouse player is null (perhaps it's been deleted), we can still remove the spouse from the current player
		msg.setTO(playerGhost->getSpouseName());
		player->sendSystemMessage(msg);

		playerGhost->removeSpouse();
	}
}

void PlayerManagerImplementation::claimVeteranRewards(CreatureObject* player) {

	if (player == nullptr || !player->isPlayerCreature() )
		return;

	PlayerObject* playerGhost = player->getPlayerObject();

	// Get account
	ManagedReference<Account*> account = playerGhost->getAccount();
	if (account == nullptr )
		return;

	// Send message with current account age
	StringIdChatParameter timeActiveMsg;
	timeActiveMsg.setStringId("veteran", "self_time_active"); // You have %DI days logged for veteran rewards.
	timeActiveMsg.setDI( account->getAgeInDays() );
	player->sendSystemMessage(timeActiveMsg );

	// Verify player is eligible for a reward
	int milestone = getEligibleMilestone( playerGhost, account );
	if (milestone < 0) {
		player->sendSystemMessage("@veteran:not_eligible"); // You are not currently eligible for a veteran reward.
		return;
	}

	// Verify player is not already choosing a reward
	if (player->getActiveSession(SessionFacadeType::VETERANREWARD) != nullptr) {
		player->sendSystemMessage("You are already attempting to claim a veteran reward." );
		return;
	}

	// Create session
	ManagedReference<VeteranRewardSession*> rewardSession =	new VeteranRewardSession( milestone );
	player->addActiveSession(SessionFacadeType::VETERANREWARD, rewardSession);

	// Build and SUI list box of rewards
	ManagedReference<SuiListBox*> box = new SuiListBox(player, SuiWindowType::SELECT_VETERAN_REWARD, SuiListBox::HANDLETWOBUTTON);
	box->setCallback(new SelectVeteranRewardSuiCallback(server));
	box->setPromptText("@veteran_new:choice_description" ); // You may choose one of the items listed below. This item will be placed in your inventory.
	box->setPromptTitle("@veteran_new:item_grant_box_title"); // Reward
	box->setOkButton(true, "@ok");
	box->setCancelButton(true, "@cancel");

	for ( int i = 0; i < veteranRewards.size(); i++) {

		// Any rewards at or below current milestone are eligible
		VeteranReward reward = veteranRewards.get(i);
		if (reward.getMilestone() <= milestone) {

			// Filter out one-time rewards already claimed
			if (reward.isOneTime() && playerGhost->hasChosenVeteranReward(reward.getTemplateFile())) {
				continue;
			}

			SharedObjectTemplate* rewardTemplate = TemplateManager::instance()->getTemplate(reward.getTemplateFile().hashCode());
			if (rewardTemplate != nullptr) {
				if (reward.getDescription().isEmpty()) {
					box->addMenuItem(rewardTemplate->getDetailedDescription(), i);
				}
				else{
					box->addMenuItem(reward.getDescription(), i);
				}
			}
		}
	}

	box->setUsingObject(nullptr);
	playerGhost->addSuiBox(box);
	player->sendMessage(box->generateMessage());

}

void PlayerManagerImplementation::cancelVeteranRewardSession(CreatureObject* player) {
	player->dropActiveSession(SessionFacadeType::VETERANREWARD);
}

void PlayerManagerImplementation::confirmVeteranReward(CreatureObject* player, int itemIndex) {

	if (player == nullptr || !player->isPlayerCreature()) {
		return;
	}

	if (itemIndex < 0 || itemIndex >= veteranRewards.size()) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	// Get account
	PlayerObject* playerGhost = player->getPlayerObject();
	ManagedReference<Account*> account = playerGhost->getAccount();
	if (account == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	// Check session
	ManagedReference<VeteranRewardSession*> rewardSession = player->getActiveSession(SessionFacadeType::VETERANREWARD).castTo<VeteranRewardSession*>();
	if (rewardSession == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		return;
	}

	VeteranReward reward = veteranRewards.get(itemIndex);

	if (reward.isOneTime() && playerGhost->hasChosenVeteranReward(reward.getTemplateFile())) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	rewardSession->setSelectedRewardIndex(itemIndex);

	// Generate confirmation dialog if item is one-time.  Otherwise, just generate it.
	if (reward.isOneTime()) {
		ManagedReference<SuiMessageBox*> suibox = new SuiMessageBox(player, SuiWindowType::CONFIRM_VETERAN_REWARD);
		suibox->setPromptTitle("@veteran_new:unique_are_you_sure_box_title"); // Reward
		suibox->setPromptText( "@veteran_new:item_unique_are_you_sure"); // The item you are selecting can only be selected as a reward item once for the the lifetime of your account. Are you sure you wish to continue selecting this item?
		suibox->setCallback(new ConfirmVeteranRewardSuiCallback(server));
		suibox->setOkButton(true, "@yes");
		suibox->setCancelButton(true, "@no");

		playerGhost->addSuiBox(suibox);
		player->sendMessage(suibox->generateMessage());
	} else {
		generateVeteranReward(player);
	}

}

void PlayerManagerImplementation::generateVeteranReward(CreatureObject* player) {
	if (player == nullptr || !player->isPlayerCreature()) {
		return;
	}

	// Get account
	PlayerObject* playerGhost = player->getPlayerObject();
	ManagedReference<Account*> account = playerGhost->getAccount();
	if (account == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	// Check session
	ManagedReference<VeteranRewardSession*> rewardSession = player->getActiveSession(SessionFacadeType::VETERANREWARD).castTo<VeteranRewardSession*>();
	if (rewardSession == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		return;
	}

	// Final check to see if milestone has already been claimed on any of the player's characters
	// (prevent claiming while multi-logged)

	bool milestoneClaimed = false;
	if (!playerGhost->getChosenVeteranReward(rewardSession->getMilestone() ).isEmpty() )
		milestoneClaimed = true;

	if (milestoneClaimed) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	// Generate item
	SceneObject* inventory = player->getSlottedObject("inventory");
	if (inventory == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	VeteranReward reward = veteranRewards.get(rewardSession->getSelectedRewardIndex());
	Reference<SceneObject*> rewardSceno = server->createObject(reward.getTemplateFile().hashCode(), 1);
	if (rewardSceno == nullptr) {
		player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
		cancelVeteranRewardSession(player);
		return;
	}

	{
		TransactionLog trx(TrxCode::VETERANREWARD, player, rewardSceno);

		// Transfer to player
		if (!inventory->transferObject(rewardSceno, -1, false, true)) { // Allow overflow
			trx.abort() << "Failed to transfer to player inventory";
			player->sendSystemMessage("@veteran:reward_error"); //	The reward could not be granted.
			rewardSceno->destroyObjectFromDatabase(true);
			cancelVeteranRewardSession(player);
			return;
		}
	}

	inventory->broadcastObject(rewardSceno, true);
	player->sendSystemMessage("@veteran:reward_given");  // Your reward has been placed in your inventory.

	// Record reward in all characters registered to the account
	GalaxyAccountInfo* accountInfo = account->getGalaxyAccountInfo(player->getZoneServer()->getGalaxyName());

	accountInfo->addChosenVeteranReward(rewardSession->getMilestone(), reward.getTemplateFile());

	cancelVeteranRewardSession(player);

	// If player is eligible for another reward, kick off selection
	if (getEligibleMilestone(playerGhost, account ) >= 0) {
		player->enqueueCommand(STRING_HASHCODE("claimveteranreward"), 0, 0, "");
	}
}

int PlayerManagerImplementation::getEligibleMilestone(PlayerObject *playerGhost, Account* account) {

	if (account == nullptr || playerGhost == nullptr )
		return -1;

	int accountAge = account->getAgeInDays();
	int milestone = -1;

	// Return -1 if account age is less than the first milestone
	if (accountAge < veteranRewardMilestones.get(0)) {
		return -1;
	}

	// Return the first milestone for which the player is eligible and has not already claimed
	for (int i = 0; i < veteranRewardMilestones.size(); i++) {
		milestone = veteranRewardMilestones.get(i);
		if (accountAge >= milestone && playerGhost->getChosenVeteranReward(milestone).isEmpty()) {
			return milestone;
		}
	}

	// They've claimed all of the established milestones, see if they're eligible for an additional one
	milestone += veteranRewardAdditionalMilestones;

	while (accountAge >= milestone) {
		if (playerGhost->getChosenVeteranReward(milestone).isEmpty()) {
			return milestone;
		}

		milestone += veteranRewardAdditionalMilestones;
	}

	// Not eligible for any milestones
	return -1;
}

int PlayerManagerImplementation::getFirstIneligibleMilestone(PlayerObject *playerGhost, Account* account) {

	if (account == nullptr || playerGhost == nullptr )
		return -1;

	int accountAge = account->getAgeInDays();
	int milestone = -1;

	// Return the first milestone the player has not already claimed
	for (int i = 0; i < veteranRewardMilestones.size(); i++) {
		milestone = veteranRewardMilestones.get(i);
		if (accountAge < milestone) {
			return milestone;
		}
	}

	// Check additional milestones if all established ones have been claimed
	while (accountAge >= milestone) {
		milestone += veteranRewardAdditionalMilestones;
	}

	return milestone;
}

bool PlayerManagerImplementation::increaseOnlineCharCountIfPossible(ZoneClientSession* client) {
	Locker locker(&onlineMapMutex);

	auto server = ServerCore::getZoneServer();
	uint32 accountId = client->getAccountID();

	if (!onlineZoneClientMap.containsKey(accountId)) {
		Vector<Reference<ZoneClientSession*> > clients;
		clients.add(client);

		onlineZoneClientMap.put(accountId, clients);

		locker.release();

		onlineZoneClientMap.accountLoggedIn(client->getIPAddress(), accountId, server != nullptr ? server->getGalaxyID() : 0);

		return true;
	}

	Vector<Reference<ZoneClientSession*> > clients = onlineZoneClientMap.get(accountId);

	int onlineCount = 0;

	for (int i = 0; i < clients.size(); ++i) {
		ZoneClientSession* session = clients.get(i);

		if (session == nullptr)
			continue;

		ManagedReference<CreatureObject*> player = session->getPlayer();

		if (player != nullptr) {
			Reference<PlayerObject*> ghost = player->getPlayerObject();

			if (ghost != nullptr && ghost->getAdminLevel() > 0)
				continue;
			else if (player->getClient() == session)
				++onlineCount;
		}
	}

	if (onlineCount >= onlineCharactersPerAccount)
		return false;

	clients.add(client);

	onlineZoneClientMap.put(accountId, clients);

	locker.release();

	return true;
}

/*
int PlayerManagerImplementation::getOnlineCharCount(unsigned int accountId) {
	//onlineMapMutex.rlock()
	return 0;
}
 */

void PlayerManagerImplementation::disconnectAllPlayers() {
	Locker locker(&onlineMapMutex);

	HashTableIterator<uint32, Vector<Reference<ZoneClientSession*> > > iter = onlineZoneClientMap.iterator();

	while (iter.hasNext()) {
		Vector<Reference<ZoneClientSession*> > clients = iter.next();

		for (int i = 0; i < clients.size(); i++) {
			ZoneClientSession* session = clients.get(i);

			if (session != nullptr) {
				CreatureObject* player = session->getPlayer();

				if (player != nullptr) {
					PlayerObject* ghost = player->getPlayerObject();

					if (ghost != nullptr) {
						Locker plocker(player);
						ghost->setLinkDead(true);
						ghost->disconnect(true, true);
					}
				}
			}
		}
	}
}

bool PlayerManagerImplementation::shouldRescheduleCorpseDestruction(CreatureObject* player, CreatureObject* ai) {
	if (player == nullptr || ai == nullptr)
		return false;

	if (!player->isPlayerCreature()) {
		return true;
	}

	Locker locker(player, ai);

	if (ai->isNonPlayerCreatureObject()) {
		NonPlayerCreatureObject *npc = dynamic_cast<NonPlayerCreatureObject*>(ai);

		if (!npc->hasLoot() && npc->getCashCredits() < 1 && npc->getBankCredits() < 1) {
			return true;
		}
	} else if (ai->isCreature()) {
		Creature * creature = dynamic_cast<Creature*>(ai);

		if (creature->hasLoot() || creature->getCashCredits() > 0 || creature->getBankCredits() > 0)
			return false;

		return !(creature->hasSkillToHarvestMe(player) || canGroupMemberHarvestCorpse(player, creature));

	}

	return false;
}

bool PlayerManagerImplementation::canGroupMemberHarvestCorpse(CreatureObject* player, Creature* creature) {

	if (!player->isGrouped())
		return false;

	ManagedReference<GroupObject*> group = player->getGroup();
	int groupSize = group->getGroupSize();

	for (int i = 0; i < groupSize; i++) {
		ManagedReference<CreatureObject*> groupMember = group->getGroupMember(i);

		if (player->getObjectID() == groupMember->getObjectID())
			continue;

		if (creature->isInRange(groupMember, 256.0f) && creature->hasSkillToHarvestMe(groupMember)) {
			return true;
		}
	}

	return false;
}

void PlayerManagerImplementation::rescheduleCorpseDestruction(CreatureObject* player, CreatureObject* ai) {

	//If the looting player or no group members in the area can harvest then despawn immediately
	if (shouldRescheduleCorpseDestruction(player, ai)) {
		Reference<DespawnCreatureTask*> despawn = ai->getPendingTask("despawn").castTo<DespawnCreatureTask*>();
		if (despawn != nullptr) {
			despawn->cancel();
			despawn->reschedule(1000);
		}

	}
}


void PlayerManagerImplementation::getCleanupCharacterCount() {
	info("**** GETTING CHARACTER CLEANUP INFORMATION ***",true);

	ObjectDatabase* thisDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true, 0xFFFF, false);

	if (thisDatabase == nullptr)
		return;

	ObjectInputStream objectData(2000);
	ObjectDatabaseIterator iterator(thisDatabase);

	uint64 objectID;

	String className;
	uint64 deletedCount = 0;
	uint64 playerCount = 0;

	ZoneServer* server = ServerCore::getZoneServer();

	if (server == nullptr) {
		error("nullptr ZoneServer in character cleanup");
		return;
	}

	int galaxyID = server->getGalaxyID();

	while(iterator.getNextKeyAndValue(objectID, &objectData)) {
		if (Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {
			if (className == "CreatureObject") {
				playerCount++;

				if (shouldDeleteCharacter(objectID, galaxyID)) {
					deletedCount++;
					info("DELETE CHARACTER " + String::valueOf(objectID),true);
				}

			}
		}
		objectData.reset();
	}

	StringBuffer deletedMessage;
	deletedMessage << "TOTAL CHARACTERS " << " TO BE DELETED " << String::valueOf(deletedCount);
	info("TOTAL CHARACTERS IN OBJECT DB: " + String::valueOf(playerCount),true);
	info(deletedMessage.toString(),true);
}

void PlayerManagerImplementation::cleanupCharacters() {

	info("**** PERFORMING CHARACTER CLEANUP ***",true);

	ObjectDatabase* thisDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true, 0xFFFF, false);

	if (thisDatabase == nullptr)
		return;

	ObjectInputStream objectData(2000);
	ObjectDatabaseIterator iterator(thisDatabase);

	uint64 objectID;

	String className;
	uint64 deletedCount = 0;
	uint64 playerCount = 0;

	ZoneServer* server = ServerCore::getZoneServer();

	if (server == nullptr) {
		error("nullptr ZoneServer in character cleanup");
		return;
	}

	int galaxyID = server->getGalaxyID();

	while(iterator.getNextKeyAndValue(objectID, &objectData) && deletedCount < 400) {
		if (Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {
			if (className == "CreatureObject") {
				playerCount++;

				if (shouldDeleteCharacter(objectID, galaxyID)) {

					ManagedReference<CreatureObject*> object = Core::getObjectBroker()->lookUp(objectID).castTo<CreatureObject*>();

					if (object == nullptr) {
						info("OBJECT nullptr when getting object " + String::valueOf(objectID),true);
					}else if (object->isPlayerCreature()) {

						deletedCount++;
						info("DELETING CHARACTER: " + String::valueOf(objectID)+ " NAME: " +  object->getFirstName() + " " + object->getLastName() ,true);
						Locker _lock(object);

						ManagedReference<ZoneClientSession*> client = object->getClient();

						if (client != nullptr)
							client->disconnect();

						object->destroyObjectFromWorld(false); //Don't need to send destroy to the player - they are being disconnected.
						object->destroyPlayerCreatureFromDatabase(true);

					}

				}

			}

		}

		objectData.reset();
	}

	StringBuffer deletedMessage;
	deletedMessage << "TOTAL CHARACTERS";
	deletedMessage << " DELETED FROM OBJECTDB: " << String::valueOf(deletedCount);
	info(deletedMessage.toString(),true);

}

bool PlayerManagerImplementation::shouldDeleteCharacter(uint64 characterID, int galaxyID) {
	const String query = "SELECT * FROM characters WHERE character_oid = " + String::valueOf(characterID) + " AND galaxy_id = " + String::valueOf(galaxyID);

	try {
		UniqueReference<ResultSet*> result(ServerDatabase::instance()->executeQuery(query));

		if (result == nullptr) {
			error("ERROR WHILE LOOKING UP CHARACTER IN SQL TABLE");
		} else if (result->getRowsAffected() > 1) {

			error("More than one character with oid = " + String::valueOf(characterID) + " in galaxy " + String::valueOf(galaxyID));
			return false;

		} else if (result->getRowsAffected() == 0) {
			return true;
		}

		return false;
	} catch (const DatabaseException& err) {
		error() << "database error " << err.getMessage();
		return false;
	}
}

bool PlayerManagerImplementation::doBurstRun(CreatureObject* player, float hamModifier, float cooldownModifier) {
	if (player == nullptr)
		return false;

	if (player->isRidingMount()) {
		player->sendSystemMessage("@cbt_spam:no_burst"); // You cannot burst-run while mounted on a creature or vehicle.
		return false;
	}

	if (player->hasBuff(STRING_HASHCODE("gallop")) || player->hasBuff(STRING_HASHCODE("burstrun")) || player->hasBuff(STRING_HASHCODE("retreat"))) {
		player->sendSystemMessage("@combat_effects:burst_run_no"); // You cannot burst run right now.
		return false;
	}

	uint32 forceRun1CRC = BuffCRC::JEDI_FORCE_RUN_1;
	uint32 forceRun2CRC = BuffCRC::JEDI_FORCE_RUN_2;
	uint32 forceRun3CRC = BuffCRC::JEDI_FORCE_RUN_3;

	if (player->hasBuff(forceRun1CRC) || player->hasBuff(forceRun2CRC) || player->hasBuff(forceRun3CRC)) {
		player->sendSystemMessage("@combat_effects:burst_run_no"); // You cannot burst run right now.
		return false;
	}

	Zone* zone = player->getZone();

	if (zone == nullptr) {
		return false;
	}

	if (zone->getZoneName() == "dungeon1") {
		player->sendSystemMessage("@combat_effects:burst_run_space_dungeon"); // The artificial gravity makes burst running impossible here.
		return false;
	}

	if (!player->checkCooldownRecovery("burstrun")) {
		player->sendSystemMessage("@combat_effects:burst_run_wait"); //You are too tired to Burst Run.
		return false;
	}

	uint32 crc = STRING_HASHCODE("burstrun");
	float hamCost = 100.0f;
	float duration = 30;
	float cooldown = 300;

	float burstRunMod = (float) player->getSkillMod("burst_run");
	hamModifier += (burstRunMod / 100.f);

	if (hamModifier > 1.0f) {
		hamModifier = 1.0f;
	}

	float hamReduction = 1.f - hamModifier;

	int healthCost = (int) (player->calculateCostAdjustment(CreatureAttribute::STRENGTH, hamCost) * hamReduction);
	int actionCost = (int) (player->calculateCostAdjustment(CreatureAttribute::QUICKNESS, hamCost) * hamReduction);
	int mindCost = (int) (player->calculateCostAdjustment(CreatureAttribute::FOCUS, hamCost) * hamReduction);

	if (player->getHAM(CreatureAttribute::HEALTH) <= healthCost || player->getHAM(CreatureAttribute::ACTION) <= actionCost || player->getHAM(CreatureAttribute::MIND) <= mindCost) {
		player->sendSystemMessage("@combat_effects:burst_run_wait"); // You are too tired to Burst Run.
		return false;
	}

	player->inflictDamage(player, CreatureAttribute::HEALTH, healthCost, true);
	player->inflictDamage(player, CreatureAttribute::ACTION, actionCost, true);
	player->inflictDamage(player, CreatureAttribute::MIND, mindCost, true);

	if (cooldownModifier > 1.0f) {
		cooldownModifier = 1.0f;
	}

	float coodownReduction = 1.f - cooldownModifier;
	cooldown *= coodownReduction;
	int newCooldown = (int) cooldown;

	StringIdChatParameter startStringId("cbt_spam", "burstrun_start_single");
	StringIdChatParameter modifiedStartStringId("combat_effects", "instant_burst_run");
	StringIdChatParameter endStringId("cbt_spam", "burstrun_stop_single");

	ManagedReference<Buff*> buff = new Buff(player, crc, duration, BuffType::SKILL);

	Locker locker(buff);

	buff->setSpeedMultiplierMod(1.822f);
	buff->setAccelerationMultiplierMod(1.822f);

	if (cooldownModifier == 0.f)
		buff->setStartMessage(startStringId);
	else
		buff->setStartMessage(modifiedStartStringId);

	buff->setEndMessage(endStringId);

	StringIdChatParameter startSpam("cbt_spam", "burstrun_start");
	StringIdChatParameter endSpam("cbt_spam", "burstrun_stop");
	buff->setStartSpam(startSpam);
	buff->setEndSpam(endSpam);
	buff->setBroadcastSpam(true);

	player->addBuff(buff);

	player->updateCooldownTimer("burstrun", (newCooldown + duration) * 1000);

	Reference<BurstRunNotifyAvailableEvent*> task = new BurstRunNotifyAvailableEvent(player);
	player->addPendingTask("burst_run_notify", task, (newCooldown + duration) * 1000);

	return true;
}

bool PlayerManagerImplementation::doEnhanceCharacter(uint32 crc, CreatureObject* player, int amount, int duration, int buffType, uint8 attribute) {
	if (player == nullptr)
		return false;

	if (player->hasBuff(crc))
		return false;

	ManagedReference<Buff*> buff = new Buff(player, crc, duration, buffType);

	Locker locker(buff);

	buff->setAttributeModifier(attribute, amount);
	player->addBuff(buff);

	return true;
}

void PlayerManagerImplementation::enhanceCharacter(CreatureObject* player) {
	if (player == nullptr)
		return;

	bool message = true;

	message = message && doEnhanceCharacter(0x98321369, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 0); // medical_enhance_health
	message = message && doEnhanceCharacter(0x815D85C5, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 1); // medical_enhance_strength
	message = message && doEnhanceCharacter(0x7F86D2C6, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 2); // medical_enhance_constitution
	message = message && doEnhanceCharacter(0x4BF616E2, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 3); // medical_enhance_action
	message = message && doEnhanceCharacter(0x71B5C842, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 4); // medical_enhance_quickness
	message = message && doEnhanceCharacter(0xED0040D9, player, medicalBuff, medicalDuration, BuffType::MEDICAL, 5); // medical_enhance_stamina

	message = message && doEnhanceCharacter(0x11C1772E, player, performanceBuff, performanceDuration, BuffType::PERFORMANCE, 6); // performance_enhance_dance_mind
	message = message && doEnhanceCharacter(0x2E77F586, player, performanceBuff, performanceDuration, BuffType::PERFORMANCE, 7); // performance_enhance_music_focus
	message = message && doEnhanceCharacter(0x3EC6FCB6, player, performanceBuff, performanceDuration, BuffType::PERFORMANCE, 8); // performance_enhance_music_willpower

	if (message && player->isPlayerCreature())
		player->sendSystemMessage("An unknown force strengthens you for battles yet to come.");
}

void PlayerManagerImplementation::sendAdminJediList(CreatureObject* player) {
	Reference<ObjectManager*> objectManager = player->getZoneServer()->getObjectManager();

	HashTable<String, uint64> names = nameMap->getNames();
	HashTableIterator<String, uint64> iter = names.iterator();

	VectorMap<UnicodeString, int> players;
	uint32 a = STRING_HASHCODE("SceneObject.slottedObjects");
	uint32 b = STRING_HASHCODE("SceneObject.customName");
	uint32 c = STRING_HASHCODE("PlayerObject.jediState");

	while (iter.hasNext()) {
		uint64 creoId = iter.next();
		VectorMap<String, uint64> slottedObjects;
		UnicodeString playerName;
		int state = -1;

		objectManager->getPersistentObjectsSerializedVariable<VectorMap<String, uint64> >(a, &slottedObjects, creoId);
		objectManager->getPersistentObjectsSerializedVariable<UnicodeString>(b, &playerName, creoId);

		uint64 ghostId = slottedObjects.get("ghost");

		if (ghostId == 0) {
			continue;
		}

		objectManager->getPersistentObjectsSerializedVariable<int>(c, &state, ghostId);

		if (state > 1) {
			players.put(playerName, state);
		}
	}

	ManagedReference<SuiListBox*> listBox = new SuiListBox(player, SuiWindowType::ADMIN_JEDILIST);
	listBox->setPromptTitle("Jedi List");
	listBox->setPromptText("This is a list of all characters with a jedi state of 2 or greater (Name - State).");
	listBox->setCancelButton(true, "@cancel");

	for (int i = 0; i < players.size(); i++) {
		listBox->addMenuItem(players.elementAt(i).getKey().toString() + " - " + String::valueOf(players.get(i)));
	}

	Locker locker(player);

	player->getPlayerObject()->closeSuiWindowType(SuiWindowType::ADMIN_JEDILIST);

	player->getPlayerObject()->addSuiBox(listBox);
	player->sendMessage(listBox->generateMessage());
}

// FRS List
void PlayerManagerImplementation::sendAdminFRSList(CreatureObject* player) {
	Reference<ObjectManager*> objectManager = player->getZoneServer()->getObjectManager();

	HashTable<String, uint64> names = nameMap->getNames();
	HashTableIterator<String, uint64> iter = names.iterator();

	VectorMap<UnicodeString, int> players;
	uint32 a = STRING_HASHCODE("SceneObject.slottedObjects");
	uint32 b = STRING_HASHCODE("SceneObject.customName");
	uint32 c = STRING_HASHCODE("PlayerObject.jediState");

	while (iter.hasNext()) {
		uint64 creoId = iter.next();
		VectorMap<String, uint64> slottedObjects;
		UnicodeString playerName;
		int state = -1;

		objectManager->getPersistentObjectsSerializedVariable<VectorMap<String, uint64> >(a, &slottedObjects, creoId);
		objectManager->getPersistentObjectsSerializedVariable<UnicodeString>(b, &playerName, creoId);

		uint64 ghostId = slottedObjects.get("ghost");

		if (ghostId == 0) {
			continue;
		}

		objectManager->getPersistentObjectsSerializedVariable<int>(c, &state, ghostId);

		if (state >= 4) {
			players.put(playerName, state);
		}
	}

	ManagedReference<SuiListBox*> listBox = new SuiListBox(player, SuiWindowType::ADMIN_FRSLIST);
	listBox->setPromptTitle("Force Ranking System List");
	listBox->setPromptText("This is a list of all characters within the Force Ranking System (Name - State).");
	listBox->setCancelButton(true, "@cancel");

	for (int i = 0; i < players.size(); i++) {
		listBox->addMenuItem(players.elementAt(i).getKey().toString() + " - " + String::valueOf(players.get(i)));
	}

	Locker locker(player);

	player->getPlayerObject()->closeSuiWindowType(SuiWindowType::ADMIN_FRSLIST);

	player->getPlayerObject()->addSuiBox(listBox);
	player->sendMessage(listBox->generateMessage());
}

VectorMap<String, int> PlayerManagerImplementation::generateAdminList() {
	static Mutex guard; //only one thread cann run this

	Locker locker(&guard);

	VectorMap<String, int> players;

	HashTable<String, uint64> names = nameMap->getNames();
	HashTableIterator<String, uint64> iter = names.iterator();

	auto objectManager = server->getObjectManager();
	auto taskManager = Core::getTaskManager();

	constexpr const int objectsPerTask = 500;

	static SynchronizedVectorMap<String, int> sharedMap;
	static AtomicInteger totalObjects;

	totalObjects = names.size();

	Vector<VectorMapEntry<String, uint64>> currentObjects;

	int count = 0;
	static TaskQueue* customQueue = [taskManager] () { return taskManager->initializeCustomQueue("AdminListThreads", 10); } (); //only once

	while (iter.hasNext()) {
		uint64 oid;
		String playerName;
		iter.getNextKeyAndValue(playerName, oid);
		++count;

		currentObjects.emplace(std::move(playerName), std::move(oid));

		if (currentObjects.size() >= objectsPerTask || count >= names.size()) {
			taskManager->executeTask([currentObjects, objectManager]() {
				for (const auto& obj : currentObjects) {
					try {
						VectorMap<String, uint64> slottedObjects;
						int state = 0;

						objectManager->getPersistentObjectsSerializedVariable<VectorMap<String, uint64> >(STRING_HASHCODE("SceneObject.slottedObjects"), &slottedObjects, obj.getValue());

						uint64 ghostId = slottedObjects.get("ghost");

						if (ghostId == 0) {
							totalObjects.decrement();
							continue;
						}

						objectManager->getPersistentObjectsSerializedVariable<int>(STRING_HASHCODE("PlayerObject.adminLevel"), &state, ghostId);

						if (state != 0) {
							sharedMap.put(obj.getKey(), state);
						}
					} catch (...) {
						Logger::console.error("unreported exception caught in GetAdminPlayerObjectTask");
					}

					totalObjects.decrement();
				}

			}, "GetAdminPlayerObjectTask", "AdminListThreads");

			currentObjects.removeAll(objectsPerTask, 50);
		}
	}

	taskManager->waitForQueueToFinish("AdminListThreads");

	players = sharedMap.getMapUnsafe();

	sharedMap.removeAll();
	totalObjects = 0;

	return players;
}

void PlayerManagerImplementation::sendAdminList(CreatureObject* player) {
	VectorMap<String, int> players = generateAdminList();

	ManagedReference<SuiListBox*> listBox = new SuiListBox(player, SuiWindowType::ADMIN_LIST);
	listBox->setPromptTitle("Admin List");
	listBox->setPromptText("This is a list of all characters with a admin level of 1 or greater (Name - State).");
	listBox->setCancelButton(true, "@cancel");

	for (int i = 0; i < players.size(); i++) {
		listBox->addMenuItem(players.elementAt(i).getKey() + " - " + String::valueOf(players.get(i)));
	}

	Locker locker(player);

	player->getPlayerObject()->closeSuiWindowType(SuiWindowType::ADMIN_LIST);

	player->getPlayerObject()->addSuiBox(listBox);
	player->sendMessage(listBox->generateMessage());
}

void PlayerManagerImplementation::doPvpDeathRatingUpdate(CreatureObject* player, ThreatMap* threatMap) {
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr)
		return;

	uint32 totalDamage = threatMap->getTotalDamage();

	if (totalDamage == 0)
		return;

	int defenderPvpRating = ghost->getPvpRating();
	int victimRatingTotalDelta = 0;
	ManagedReference<CreatureObject*> highDamageAttacker = nullptr;
	uint32 highDamageAmount = 0;
	FrsManager* frsManager = server->getFrsManager();
	int frsXpAdjustment = 0;
	bool throttleOnly = true;

	for (int i = 0; i < threatMap->size(); ++i) {
		ThreatMapEntry* entry = &threatMap->elementAt(i).getValue();
		TangibleObject* attacker = threatMap->elementAt(i).getKey();

		if (entry == nullptr || attacker == nullptr || attacker == player || !attacker->isPlayerCreature())
			continue;

		CreatureObject* attackerCreo = attacker->asCreatureObject();

		if (attackerCreo == nullptr) {
			continue;
		}

		PlayerObject* attackerGhost = attackerCreo->getPlayerObject();

		if (attackerGhost == nullptr)
			continue;

		Locker crossLock(attackerCreo, player);

		if (!allowSameAccountPvpRatingCredit && ghost->getAccountID() == attackerGhost->getAccountID())
			continue;

		if (entry->getTotalDamage() <= 0)
			continue;

		if (player->getDistanceTo(attacker) > 80.f)
			continue;

		int curAttackerRating = attackerGhost->getPvpRating();

		if (highDamageAmount == 0 || entry->getTotalDamage() > highDamageAmount) {
			highDamageAmount = entry->getTotalDamage();
			highDamageAttacker = attackerCreo;
		}

		if (attackerGhost->hasOnVictimList(player->getObjectID())) {
			String stringFile;

			if (attackerCreo->getSpecies() == CreatureObject::TRANDOSHAN)
				stringFile = "rating_throttle_trandoshan_winner";
			else
				stringFile = "rating_throttle_winner";

			StringIdChatParameter toAttacker;
			toAttacker.setStringId("pvp_rating", stringFile);
			toAttacker.setTT(player->getFirstName());
			toAttacker.setTU(attackerCreo->getObjectID());
			toAttacker.setDI(curAttackerRating);

			attackerCreo->sendSystemMessage(toAttacker);
			continue;
		}

		if (defenderPvpRating <= PlayerObject::PVP_RATING_FLOOR) {
			String stringFile;
			if (attackerCreo->getSpecies() == CreatureObject::TRANDOSHAN)
				stringFile = "rating_floor_trandoshan_winner";
			else
				stringFile = "rating_floor_winner";

			StringIdChatParameter toAttacker;
			toAttacker.setStringId("pvp_rating", stringFile);
			toAttacker.setTT(player->getFirstName());
			toAttacker.setDI(curAttackerRating);

			attackerCreo->sendSystemMessage(toAttacker);
		}

		float damageContribution = (float) entry->getTotalDamage() / totalDamage;

		if (frsManager != nullptr && frsManager->isFrsEnabled() && frsManager->isValidFrsBattle(attackerCreo, player)) {
			int attackerFrsXp = frsManager->calculatePvpExperienceChange(attackerCreo, player, damageContribution, false);
			int victimFrsXp = frsManager->calculatePvpExperienceChange(attackerCreo, player, damageContribution, true);
			frsXpAdjustment += victimFrsXp;

			ManagedReference<CreatureObject*> attackerRef = attackerCreo;
			if (attackerFrsXp > 0) {
				Core::getTaskManager()->executeTask([attackerRef, frsManager, attackerFrsXp] () {
					Locker locker(attackerRef);
					Locker clocker(frsManager, attackerRef);
					frsManager->adjustFrsExperience(attackerRef, attackerFrsXp);
				}, "FrsExperienceAdjustLambda");
			}
		}

		attackerGhost->addToVictimList(player->getObjectID());
		throttleOnly = false;

		if (defenderPvpRating > PlayerObject::PVP_RATING_FLOOR) {
			int attackerRatingDelta = 20 + ((defenderPvpRating - curAttackerRating) / 25);
			int victimRatingDelta = -20 + ((curAttackerRating - defenderPvpRating) / 25);

			if (attackerRatingDelta > 40)
				attackerRatingDelta = 40;
			else if (attackerRatingDelta < 0)
				attackerRatingDelta = 0;

			if (victimRatingDelta < -40)
				victimRatingDelta = -40;
			else if (victimRatingDelta > 0)
				victimRatingDelta = 0;

			attackerRatingDelta *= damageContribution;
			victimRatingDelta *= damageContribution;

			victimRatingTotalDelta += victimRatingDelta;
			int newRating = curAttackerRating + attackerRatingDelta;

			attackerGhost->setPvpRating(newRating);

			crossLock.release();

			String stringFile;

			int randNum = System::random(2) + 1;
			if (attackerCreo->getSpecies() == CreatureObject::TRANDOSHAN)
				stringFile = "trandoshan_win" + String::valueOf(randNum);
			else
				stringFile = "win" + String::valueOf(randNum);

			StringIdChatParameter toAttacker;
			toAttacker.setStringId("pvp_rating", stringFile);
			toAttacker.setTT(player->getFirstName());
			toAttacker.setDI(newRating);

			attackerCreo->sendSystemMessage(toAttacker);
		}
	}

	if (highDamageAttacker == nullptr)
		return;

	if (frsManager != nullptr && frsManager->isFrsEnabled() && frsXpAdjustment < 0) {
		Locker crossLock(frsManager, player);

		frsManager->adjustFrsExperience(player, frsXpAdjustment);
	}

	if (defenderPvpRating <= PlayerObject::PVP_RATING_FLOOR) {
		String stringFile;
		if (player->getSpecies() == CreatureObject::TRANDOSHAN)
			stringFile = "rating_floor_trandoshan_loser";
		else
			stringFile = "rating_floor_victim";

		StringIdChatParameter toVictim;
		toVictim.setStringId("pvp_rating", stringFile);
		toVictim.setTT(highDamageAttacker->getFirstName());
		toVictim.setDI(defenderPvpRating);

		player->sendSystemMessage(toVictim);
	} else if (victimRatingTotalDelta != 0) {
		int newDefenderRating = defenderPvpRating + victimRatingTotalDelta;

		if (newDefenderRating < PlayerObject::PVP_RATING_FLOOR)
			newDefenderRating = PlayerObject::PVP_RATING_FLOOR;

		ghost->setPvpRating(newDefenderRating);

		String stringFile;

		int randNum = System::random(2) + 1;
		if (player->getSpecies() == CreatureObject::TRANDOSHAN)
			stringFile = "trandoshan_killed" + String::valueOf(randNum);
		else
			stringFile = "killed" + String::valueOf(randNum);

		StringIdChatParameter toVictim;
		toVictim.setStringId("pvp_rating", stringFile);
		toVictim.setTT(highDamageAttacker->getFirstName());
		toVictim.setDI(newDefenderRating);

		player->sendSystemMessage(toVictim);
	} else if (throttleOnly) {
		String stringFile;

		if (player->getSpecies() == CreatureObject::TRANDOSHAN)
			stringFile = "rating_throttle_trandoshan_loser";
		else
			stringFile = "rating_throttle_loser";

		StringIdChatParameter toVictim;
		toVictim.setStringId("pvp_rating", stringFile);
		toVictim.setTT(highDamageAttacker->getFirstName());
		toVictim.setDI(defenderPvpRating);

		player->sendSystemMessage(toVictim);
	}
}

float PlayerManagerImplementation::getSpeciesXpModifier(const String& species, const String& xpType) {
	int bonus = xpBonusList.get(species).get(xpType);

	if (bonus == -1)
		bonus = xpBonusList.get(species).get("all");

	if (bonus == -1)
		return 1.f;

	return (100.f + bonus) / 100.f;
}

void PlayerManagerImplementation::unlockFRSForTesting(CreatureObject* player, int councilType) {
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr)
		return;

	if (player->hasSkill("force_rank_light_novice") || player->hasSkill("force_rank_dark_novice")) {
		player->sendSystemMessage("You already have FRS skills. You must drop them before using this feature again.");
		return;
	}

	SkillManager* skillManager = SkillManager::instance();

	int glowyBadgeIds[] = { 12, 14, 15, 16, 17, 19, 20, 21, 23, 30, 38, 39, 71, 105, 106, 107 };

	for (int i = 0; i < 16; i++) {
		ghost->awardBadge(glowyBadgeIds[i]);
	}

	SkillManager::instance()->surrenderAllSkills(player, true, false);

	Lua* lua = DirectorManager::instance()->getLuaInstance();

	Reference<LuaFunction*> luaFrsTesting = lua->createFunction("FsIntro", "completeVillageIntroFrog", 0);
	*luaFrsTesting << player;

	luaFrsTesting->callFunction();

	String branches[] = {
			"force_sensitive_combat_prowess_ranged_accuracy",
			"force_sensitive_combat_prowess_ranged_speed",
			"force_sensitive_combat_prowess_melee_accuracy",
			"force_sensitive_combat_prowess_melee_speed",
			"force_sensitive_enhanced_reflexes_ranged_defense",
			"force_sensitive_enhanced_reflexes_melee_defense"
		};

	for (int i = 0; i < 6; i++) {
		String branch = branches[i];
		player->setScreenPlayState("VillageUnlockScreenPlay:" + branch, 2);
		skillManager->awardSkill(branch + "_04", player, true, true, true);
	}

	luaFrsTesting = lua->createFunction("FsOutro", "completeVillageOutroFrog", 0);
	*luaFrsTesting << player;

	luaFrsTesting->callFunction();

	luaFrsTesting = lua->createFunction("JediTrials", "completePadawanForTesting", 0);
	*luaFrsTesting << player;

	luaFrsTesting->callFunction();

	skillManager->awardSkill("force_discipline_light_saber_master", player, true, true, true);
	skillManager->awardSkill("force_discipline_enhancements_master", player, true, true, true);
	skillManager->awardSkill("force_discipline_healing_damage_04", player, true, true, true);
	skillManager->awardSkill("force_discipline_healing_states_04", player, true, true, true);

	luaFrsTesting = lua->createFunction("JediTrials", "completeKnightForTesting", 0);
	*luaFrsTesting << player;
	*luaFrsTesting << councilType;

	luaFrsTesting->callFunction();
}

Vector<uint64> PlayerManagerImplementation::getOnlinePlayerList() {
	Vector<uint64> playerList;

	Locker locker(&onlineMapMutex);

	HashTableIterator<uint32, Vector<Reference<ZoneClientSession*> > > iter = onlineZoneClientMap.iterator();

	while (iter.hasNext()) {
		Vector<Reference<ZoneClientSession*> > clients = iter.next();

		for (int i = 0; i < clients.size(); i++) {
			ZoneClientSession* session = clients.get(i);

			if (session != nullptr) {
				CreatureObject* player = session->getPlayer();

				if (player != nullptr) {
					playerList.add(player->getObjectID());
				}
			}
		}
	}

	return playerList;
}

void PlayerManagerImplementation::logOnlinePlayers(bool onlyWho) {
	int countOnline = 0;
	int countAccounts = 0;
	int countPlayers = 0;
	int countnullptrClient = 0;
	int countnullptrCreature = 0;
	int countnullptrGhost = 0;

	JSONSerializationType logClients;

	Locker locker(&onlineMapMutex);

	int countDistinctIPs = onlineZoneClientMap.getDistinctIps();

	auto iter = onlineZoneClientMap.iterator();

	while (iter.hasNext()) {
		countAccounts++;

		auto clients = iter.next();

		for (int i = 0;i < clients.size();i++) {
			auto client = clients.get(i);

			if (client == nullptr) {
				countnullptrClient++;
				continue;
			}

			JSONSerializationType logClient;

			logClient["accountID"] = client->getAccountID();
			logClient["ip"] = client->getIPAddress();

			Reference<CreatureObject*> creature = client->getPlayer();

			if (creature != nullptr) {
				countPlayers++;

				logClient["oid"] = creature->getObjectID();
				logClient["firstName"] = creature->getFirstName();

				if (creature->isInvisible())
					logClient["invisible"] = true;

				Reference<PlayerObject*> ghost = creature->getPlayerObject();

				if (ghost != nullptr) {
					Locker lock(ghost);

					logClient["playedSeconds"] = (int)(ghost->getPlayedMiliSecs() / 1000);
					logClient["sessionSeconds"] = (int)(ghost->getSessionMiliSecs() / 1000);
					logClient["totalMovement"] = ghost->getSessionTotalMovement();

					auto admin_level = ghost->getAdminLevel();

					if (admin_level > 0 && ghost->hasAbility("admin"))
						logClient["admin_level"] = admin_level;

					if (ghost->isOnline()) {
						countOnline++;

						auto zone = creature->getZone();

						if (zone != nullptr) {
							auto worldPosition = creature->getWorldPosition();

							logClient["worldPositionX"] = (int)worldPosition.getX();
							logClient["worldPositionZ"] = (int)worldPosition.getZ();
							logClient["worldPositionY"] = (int)worldPosition.getY();
							logClient["zone"] = zone->getZoneName();
						}
					} else {
						logClient["isOnline"] = false;
					}

					if (ghost->isAFK())
						logClient["isAFK"] = true;
				} else {
					countnullptrGhost++;
				}
			} else {
				countnullptrCreature++;
				logClient["isNullCreature"] = true;
				logClient["isNullSession"] = client->getSession() == nullptr;
				// TODO - Periodic cleanup of these?
			}

			logClients.push_back(logClient);
		}
	}

	locker.release();

	JSONSerializationType logEntry;
	Time now;

	logEntry["@timestamp"] = now.getFormattedTimeFull().toCharArray();
	logEntry["timeMSecs"] = now.getMiliTime();
	logEntry["clients"] = logClients.size() > 0 ? logClients : nlohmann::json::array();
	logEntry["uptime_secs"] = Logger::getElapsedTime();

	auto server = ServerCore::getZoneServer();

	if (server != nullptr) {
		logEntry["uptime"] = (int)(server->getStartTimestamp()->miliDifference(now) / 1000);

		if (server->isServerLocked())
			logEntry["isServerLocked"] = true;

		if (server->isServerLoading())
			logEntry["isServerLoading"] = true;

		if (server->isServerShuttingDown())
			logEntry["isServerShuttingDown"] = true;
	}

	logEntry["countAccounts"] = countAccounts;
	logEntry["countPlayers"] = countPlayers;
	logEntry["countDistinctIPs"] = countDistinctIPs;

	if (countOnline != countPlayers)
		logEntry["countOnline"] = countOnline;

	if (countnullptrClient > 0)
		logEntry["countnullptrClient"] = countnullptrClient;

	if (countnullptrCreature > 0)
		logEntry["countnullptrCreature"] = countnullptrCreature;

	if (countnullptrGhost > 0)
		logEntry["countnullptrGhost"] = countnullptrGhost;

	StringBuffer logLine;

	logLine << logEntry.dump().c_str() << "\n";

	// Write who file
	try {
		// Write a new "current status" file
		File file("log/who.json.next");
		FileWriter logFile(&file, false);

		logFile << logLine;

		logFile.close();

		// Update current status file
#ifdef PLATFORM_WIN
		std::remove("log/who.json");
#endif
		int err = std::rename("log/who.json.next", "log/who.json");

		if (err != 0) {
			error() << "Failed to rename log/who.json.next to log/who.json err = " << err;
		}
	} catch (const Exception& e) {
		error() << "logOnlinePlayers failed to write log/who.json: " << e.getMessage();
	}

	if (onlyWho)
		return;

	Locker logfileLock(&onlinePlayerLogMutext);

	String fileName = "log/online-players.log";

	struct stat st_log;

	// Check for rollover
	if (stat(fileName.toCharArray(), &st_log) == 0) {
		if (st_log.st_size >= ConfigManager::instance()->getOnlineLogSize()) {
			StringBuffer archiveFilename;
			archiveFilename << "log/online-players-" << now.getMiliTime() << ".log";

			// If the rename failed its ok because we open with append below
			int err = std::rename(fileName.toCharArray(), archiveFilename.toString().toCharArray());

			if (err != 0)
				error() << "Failed to archive online-players to " << archiveFilename.toString() << " err = " << err;
		}
	}

	try {
		// Append log file with this entry
		File file(fileName);
		FileWriter logFile(&file, true);

		logFile << logLine;

		logFile.close();

		logfileLock.release();

		int LogSum = countOnline + countAccounts + countPlayers + countnullptrClient + countnullptrCreature + countnullptrGhost + countDistinctIPs;

		// Throttle to no more often than once per 5s and only if something to report
		if (lastOnlinePlayerLogMsg.miliDifference() >= 5000 && LogSum != onlinePlayerLogSum) {
			StringBuffer logMsg;

			logMsg << "Logged " << countOnline << " players (" << countAccounts << " accounts, " << countDistinctIPs << " distinct IPs) to " << fileName;

			if (countnullptrClient > 0)
				logMsg << "; " << countnullptrClient << " null clients";

			if (countnullptrCreature > 0)
				logMsg << "; " << countnullptrCreature << " clients without a creature";

			if (countnullptrGhost > 0)
				logMsg << "; " << countnullptrGhost << " creatures without a player object";

			logMsg << ".";

			info(logMsg.toString(), true);
			lastOnlinePlayerLogMsg.updateToCurrentTime();
			onlinePlayerLogSum = LogSum;
		}
	} catch (const Exception& e) {
		error() << "logOnlinePlayers failed to write " << fileName << ": " << e.getMessage();
	}
}

void PlayerManagerImplementation::iteratePlayerNames(const PlayerNameIterator& iterator) {
	auto names = nameMap->getNames();
	auto iter = names.iterator();

	while (iter.hasNext()) {
		String name;
		uint64 oid;
		iter.getNextKeyAndValue(name, oid);
		iterator(name, oid);
	}
}
