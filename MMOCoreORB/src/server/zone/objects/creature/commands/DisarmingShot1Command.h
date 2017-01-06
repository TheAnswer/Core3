/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef DISARMINGSHOT1COMMAND_H_
#define DISARMINGSHOT1COMMAND_H_

#include "CombatQueueCommand.h"

class DisarmingShot1Command : public CombatQueueCommand {
public:

	DisarmingShot1Command(const String& name, ZoneProcessServer* server)
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

#endif //DISARMINGSHOT1COMMAND_H_
