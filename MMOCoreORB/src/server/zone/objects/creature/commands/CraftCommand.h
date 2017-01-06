/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef CRAFTCOMMAND_H_
#define CRAFTCOMMAND_H_

class CraftCommand : public QueueCommand {
public:

	CraftCommand(const String& name, ZoneProcessServer* server)
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

#endif //CRAFTCOMMAND_H_
