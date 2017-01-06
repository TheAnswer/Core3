/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef STOPCITYSPAWNERCOMMAND_H_
#define STOPCITYSPAWNERCOMMAND_H_

class StopCitySpawnerCommand : public QueueCommand {
public:

	StopCitySpawnerCommand(const String& name, ZoneProcessServer* server)
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

#endif //STOPCITYSPAWNERCOMMAND_H_
