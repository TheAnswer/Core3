/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef WATCHCOMMAND_H_
#define WATCHCOMMAND_H_

#include "server/zone/managers/player/PlayerManager.h"

class WatchCommand : public QueueCommand {
public:

	WatchCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (creature->isWatching())
			return GENERALERROR;

		ManagedReference<PlayerManager*> playerManager = server->getPlayerManager();

		if (playerManager != NULL)
			playerManager->startWatch(creature, target);

		return SUCCESS;
	}

};

#endif //WATCHCOMMAND_H_
