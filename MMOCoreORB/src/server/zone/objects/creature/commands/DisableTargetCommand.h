/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef DISABLETARGET_H_
#define DISABLETARGET_H_

#include "CombatQueueCommand.h"

class DisableTargetCommand : public CombatQueueCommand {
public:

	DisableTargetCommand(const String& name, ZoneProcessServer* server)
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

#endif //DISABLETARGET_H_
