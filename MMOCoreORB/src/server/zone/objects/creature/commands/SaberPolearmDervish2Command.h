/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SABERPOLEARMDERVISH2COMMAND_H_
#define SABERPOLEARMDERVISH2COMMAND_H_

#include "JediCombatQueueCommand.h"

class SaberPolearmDervish2Command : public  JediCombatQueueCommand {
public:

	SaberPolearmDervish2Command(const String& name, ZoneProcessServer* server)
		:  JediCombatQueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		return doCombatAction(creature, target);
	}

};

#endif //SABERPOLEARMDERVISH2COMMAND_H_
