/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef TOGGLECOMBATTAUNTSCOMMAND_H_
#define TOGGLECOMBATTAUNTSCOMMAND_H_

class ToggleCombatTauntsCommand : public QueueCommand {
public:

	ToggleCombatTauntsCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		return SUCCESS;
	}

};

#endif //TOGGLECOMBATTAUNTSCOMMAND_H_
