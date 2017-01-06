/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SETTEFCOMMAND_H_
#define SETTEFCOMMAND_H_

class SetTEFCommand : public QueueCommand {
public:

	SetTEFCommand(const String& name, ZoneProcessServer* server)
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

#endif //SETTEFCOMMAND_H_
