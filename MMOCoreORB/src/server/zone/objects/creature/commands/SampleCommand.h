/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SAMPLECOMMAND_H_
#define SAMPLECOMMAND_H_

class SampleCommand : public QueueCommand {
public:

	SampleCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature)) {
			if(creature->isPlayerCreature() && creature->isInCombat()) {
				creature->sendSystemMessage("@survey:sample_cancel_attack"); //You can't take samples while under attack!
			}

			return INVALIDSTATE;
		}

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		return SUCCESS;
	}

};

#endif //SAMPLECOMMAND_H_
