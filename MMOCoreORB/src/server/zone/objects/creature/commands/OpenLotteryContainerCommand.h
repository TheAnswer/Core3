/*
 * OpenLotteryContainerCommand.h
 *
 *  Created on: Aug 26, 2011
 *      Author: da
 */

#ifndef OPENLOTTERYCONTAINERCOMMAND_H_
#define OPENLOTTERYCONTAINERCOMMAND_H_

class OpenLotteryContainerCommand : public QueueCommand {
public:

	OpenLotteryContainerCommand(const String& name, ZoneProcessServer* server)
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

#endif /* OPENLOTTERYCONTAINERCOMMAND_H_ */
