/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef BODYSHOT3COMMAND_H_
#define BODYSHOT3COMMAND_H_

#include "CombatQueueCommand.h"

class BodyShot3Command : public CombatQueueCommand {
public:

	BodyShot3Command(const String& name, ZoneProcessServer* server)
		: CombatQueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		return doCombatAction(creature, target);
	}

};

#endif //BODYSHOT3COMMAND_H_
