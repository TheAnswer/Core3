/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef DESTROYTARGET_H_
#define DESTROYTARGET_H_

#include "CombatQueueCommand.h"

class DestroyTargetCommand : public CombatQueueCommand {
public:

	DestroyTargetCommand(const String& name, ZoneProcessServer* server)
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

#endif //DESTROYTARGET_H_
