/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef PILOTSHIP_H_
#define PILOTSHIP_H_

#include "CombatQueueCommand.h"

class PilotShipCommand : public CombatQueueCommand {
public:

	PilotShipCommand(const String& name, ZoneProcessServer* server)
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

#endif //PILOTSHIP_H_
