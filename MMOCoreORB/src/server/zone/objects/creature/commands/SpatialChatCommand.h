/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SPATIALCHATCOMMAND_H_
#define SPATIALCHATCOMMAND_H_

class SpatialChatCommand : public QueueCommand {
public:

	SpatialChatCommand(const String& name, ZoneProcessServer* server)
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

#endif //SPATIALCHATCOMMAND_H_
