/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef UNARMEDHIT3COMMAND_H_
#define UNARMEDHIT3COMMAND_H_

#include "CombatQueueCommand.h"

class UnarmedHit3Command : public CombatQueueCommand {
public:

	UnarmedHit3Command(const String& name, ZoneProcessServer* server)
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

#endif //UNARMEDHIT3COMMAND_H_
