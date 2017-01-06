/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef ESHIELDS_H_
#define ESHIELDS_H_

#include "CombatQueueCommand.h"

class EshieldsCommand : public CombatQueueCommand {
public:

	EshieldsCommand(const String& name, ZoneProcessServer* server)
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

#endif //ESHIELDS_H_
