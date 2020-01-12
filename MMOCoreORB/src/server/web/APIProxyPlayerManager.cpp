/*
                Copyright <SWGEmu>
        See file COPYING for copying conditions.*/

/**
 * @author      : lordkator (lordkator@swgemu.com)
 * @file        : APIProxyPlayerManager.cpp
 * @created     : Sun Jan  5 11:26:18 UTC 2020
 */

#ifdef WITH_REST_API

#include "engine/engine.h"
#include "server/ServerCore.h"
#include "server/zone/ZoneServer.h"
#include "server/login/account/AccountManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"

#include "APIProxyPlayerManager.h"
#include "APIRequest.h"

namespace server {
 namespace web3 {

server::zone::managers::player::PlayerManager* APIProxyPlayerManager::getPlayerManager() {
	auto server = getZoneServer();

	if (server == nullptr) {
		return nullptr;
	}

	return server->getPlayerManager();
}

void APIProxyPlayerManager::handle(APIRequest& apiRequest) {
	if (!apiRequest.isMethodPOST()) {
		apiRequest.fail("Only supports POST");
		return;
	}

	if (!apiRequest.parseRequestJSON())
		return;

	auto command = apiRequest.getRequestFieldString("command", true);

	if (command.isEmpty()) {
		apiRequest.fail("Invalid request, command is empty.");
		return;
	}

	if (command != "kick" && command != "ban") {
		apiRequest.fail("Invalid command: [" + command + "]");
		return;
	}

	uint64_t accountID = apiRequest.getPathFieldUnsignedLong("accountID", true);

	uint32_t galaxyID = apiRequest.getPathFieldUnsignedLong("galaxyID", false, 0ull);

	uint64_t characterID = apiRequest.getPathFieldUnsignedLong("characterID", false, 0ull);

	uint64_t adminID = apiRequest.getRequestFieldUnsignedLong("admin", true);

	if (adminID == 0) {
		apiRequest.fail("Invalid request, admin is 0");
		return;
	}

	auto reason = apiRequest.getRequestFieldString("reason", true);

	if (reason.isEmpty()) {
		apiRequest.fail("Invalid request, reason is empty.");
		return;
	}

	int expires = apiRequest.getRequestFieldUnsignedLong("expires", false, 0);

	auto playerManager = getPlayerManager();

	if (playerManager == nullptr) {
		apiRequest.fail("Unable to get playerManager");
		return;
	}

	auto adminName = playerManager->getPlayerName(adminID);

	if (adminName.isEmpty()) {
		apiRequest.fail("admin [" + String::valueOf(adminID) + "] not found");
		return;
	}

	JSONSerializationType result;

	result["request"] = apiRequest.getRequestJSON();
	result["admin_name"] = adminName;

	if (command == "kick") {
		if (galaxyID == 0 || characterID == 0) {
			apiRequest.fail("Currently account kick is not implemented");
			return;
		}

		auto characterName = playerManager->getPlayerName(characterID);

		if (characterName.isEmpty()) {
			apiRequest.fail("character [" + String::valueOf(characterID) + "] not found");
			return;
		}

		result["action"] = expires > 0 ? "kickbanned player" : "kicked player";
		result["target"] = characterName;

		if (!playerManager->kickUser(characterName, adminName, reason, expires > 0 ? true : false)) {
			apiRequest.fail("kickUser failed");
			return;
		}

		result["action_result"] = "SUCCESS";
	} else if (command == "ban") {
		Reference<CreatureObject*> adminCreo = playerManager->getPlayer(adminName);

		if (adminCreo == nullptr) {
			apiRequest.fail("failed to get admin creature object");
			return;
		}

		Reference<PlayerObject*> adminGhost = adminCreo->getPlayerObject();

		if (adminGhost == nullptr) {
			apiRequest.fail("failed to get admin player object");
			return;
		}

		Reference<Account*> account = AccountManager::getAccount(accountID);

		if (account == nullptr) {
			apiRequest.fail("Account not found (accountID: " + String::valueOf(accountID) + ")");
			return;
		}

		result["username"] = account->getUsername();

		String banResult;

		if (galaxyID == 0 || characterID == 0) {
			result["action"] = "banned account";
			result["target"] = account->getUsername();
			result["action_result"] = playerManager->banAccount(adminGhost, account, expires, reason);
		} else {
			auto characterName = playerManager->getPlayerName(characterID);

			if (characterName.isEmpty()) {
				apiRequest.fail("character [" + String::valueOf(characterID) + "] not found");
				return;
			}

			result["action"] = "banned player";
			result["target"] = characterName;
			result["action_result"] = playerManager->banCharacter(adminGhost, account, characterName, galaxyID, expires, reason);
		}

		adminCreo->sendSystemMessage("API command ban: " + String(result["action_result"]));
	}

	apiRequest.info(true)
		<< adminName << " "
		<< result["action"].get<std::string>() << " "
		<< result["target"].get<std::string>() << " "
		<< "for '" << reason << "' "
		<< "expires = " << expires << ", "
		<< "result = " << result["action_result"].get<std::string>();

	apiRequest.success(result);
}
}
}

#endif // WITH_REST_API
