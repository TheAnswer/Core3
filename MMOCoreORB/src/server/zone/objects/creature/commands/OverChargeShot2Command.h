/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef OVERCHARGESHOT2COMMAND_H_
#define OVERCHARGESHOT2COMMAND_H_

#include "CombatQueueCommand.h"

class OverChargeShot2Command : public CombatQueueCommand {
public:

	OverChargeShot2Command(const String& name, ZoneProcessServer* server)
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

#endif //OVERCHARGESHOT2COMMAND_H_
