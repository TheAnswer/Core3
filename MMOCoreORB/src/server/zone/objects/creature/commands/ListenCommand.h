/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef LISTENCOMMAND_H_
#define LISTENCOMMAND_H_

#include "server/zone/managers/player/PlayerManager.h"

class ListenCommand : public QueueCommand {
public:

	ListenCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (creature->isListening())
			return GENERALERROR;

		ManagedReference<PlayerManager*> playerManager = server->getPlayerManager();

		if (playerManager != NULL)
			playerManager->startListen(creature, target);

		return SUCCESS;
	}

};

#endif //LISTENCOMMAND_H_
