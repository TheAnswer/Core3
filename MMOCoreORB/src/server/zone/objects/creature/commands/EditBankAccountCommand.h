/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef EDITBANKACCOUNTCOMMAND_H_
#define EDITBANKACCOUNTCOMMAND_H_

class EditBankAccountCommand : public QueueCommand {
public:

	EditBankAccountCommand(const String& name, ZoneProcessServer* server)
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

#endif //EDITBANKACCOUNTCOMMAND_H_
