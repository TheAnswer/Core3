/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.
*/

#ifndef SHOWPVPRATINGCOMMAND_H_
#define SHOWPVPRATINGCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class ShowPvPRatingCommand : public QueueCommand {
public:

	ShowPvPRatingCommand(const String& name, ZoneProcessServer* server)
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

#endif //SHOWPVPRATINGCOMMAND_H_
