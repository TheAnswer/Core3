/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef REMOVEFRIENDCOMMAND_H_
#define REMOVEFRIENDCOMMAND_H_

class RemoveFriendCommand : public QueueCommand {
public:

	RemoveFriendCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		Reference<PlayerObject*> ghost = creature->getSlottedObject("ghost").castTo<PlayerObject*>();
		ghost->removeFriend(arguments.toString());

		return SUCCESS;
	}

};

#endif //REMOVEFRIENDCOMMAND_H_
