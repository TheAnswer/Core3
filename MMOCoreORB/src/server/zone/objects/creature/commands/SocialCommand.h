/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SOCIALCOMMAND_H_
#define SOCIALCOMMAND_H_

class SocialCommand : public QueueCommand {
public:

	SocialCommand(const String& name, ZoneProcessServer* server)
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

#endif //SOCIALCOMMAND_H_
