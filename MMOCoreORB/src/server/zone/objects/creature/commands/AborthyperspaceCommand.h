/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef ABORTHYPERSPACE_H_
#define ABORTHYPERSPACE_H_

#include "CombatQueueCommand.h"

class AborthyperspaceCommand : public CombatQueueCommand {
public:

	AborthyperspaceCommand(const String& name, ZoneProcessServer* server)
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

#endif //ABORTHYPERSPACE_H_
