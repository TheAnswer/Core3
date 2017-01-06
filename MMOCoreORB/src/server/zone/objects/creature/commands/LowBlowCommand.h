/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef LOWBLOWCOMMAND_H_
#define LOWBLOWCOMMAND_H_

#include "CombatQueueCommand.h"

class LowBlowCommand : public CombatQueueCommand {
public:

	LowBlowCommand(const String& name, ZoneProcessServer* server)
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

#endif //LOWBLOWCOMMAND_H_
