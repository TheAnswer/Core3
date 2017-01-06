/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef UNARMEDLEGHIT1COMMAND_H_
#define UNARMEDLEGHIT1COMMAND_H_

#include "CombatQueueCommand.h"

class UnarmedLegHit1Command : public CombatQueueCommand {
public:

	UnarmedLegHit1Command(const String& name, ZoneProcessServer* server)
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

#endif //UNARMEDLEGHIT1COMMAND_H_
