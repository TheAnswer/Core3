/*
 * ServerStatisticsCommand.h
 *
 *  Created on: Jan 21, 2013
 *      Author: swgemu
 */

#ifndef SERVERSTATISTICSCOMMAND_H_
#define SERVERSTATISTICSCOMMAND_H_

#include "engine/engine.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/statistics/StatisticsManager.h"

class ServerStatisticsCommand {
public:
	static int executeCommand(CreatureObject* creature, uint64 target, const UnicodeString& arguments) {
		PlayerObject* ghost = creature->getPlayerObject();

		if (ghost == NULL || !ghost->isPrivileged())
			return 1;

		StringTokenizer args(arguments.toString());

		if (args.hasMoreTokens()) {
			String command;
			args.getStringToken(command);

			if (command.toLowerCase() == "reset") {
				StatisticsManager::instance()->reset();
				creature->sendSystemMessage("Statistics have been reset.");
			} else if ( command.toLowerCase() == "character") {
				creature->sendSystemMessage(StatisticsManager::instance()->getCharacterStats());
			}


		} else {
			creature->sendSystemMessage(StatisticsManager::instance()->getStatistics());
		}

		return 0;
	}

	static void sendSyntax(CreatureObject* player) {
		if (player != NULL)
			player->sendSystemMessage("Syntax: /server statistics");
	}
};

#endif /* SERVERSTATISTICSCOMMAND_H_ */
