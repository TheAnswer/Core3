/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef RESTARTCONVERSIONCOMMAND_H_
#define RESTARTCONVERSIONCOMMAND_H_

class RestartConversionCommand : public QueueCommand {
public:

	RestartConversionCommand(const String& name, ZoneProcessServer* server)
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

#endif //RESTARTCONVERSIONCOMMAND_H_
