/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef LFGCOMMAND_H_
#define LFGCOMMAND_H_

#include "server/zone/objects/player/PlayerObject.h"

class LfgCommand : public QueueCommand {
public:

	LfgCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();

		if (ghost != NULL)
			ghost->toggleCharacterBit(PlayerObject::LFG);

		return SUCCESS;
	}

};

#endif //LFGCOMMAND_H_
