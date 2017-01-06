/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef AUCTIONACCEPTCOMMAND_H_
#define AUCTIONACCEPTCOMMAND_H_

class AuctionAcceptCommand : public QueueCommand {
public:

	AuctionAcceptCommand(const String& name, ZoneProcessServer* server)
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

#endif //AUCTIONACCEPTCOMMAND_H_
