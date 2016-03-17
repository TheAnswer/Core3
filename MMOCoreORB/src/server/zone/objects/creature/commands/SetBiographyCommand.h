/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef SETBIOGRAPHYCOMMAND_H_
#define SETBIOGRAPHYCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class SetBiographyCommand : public QueueCommand {
public:

	SetBiographyCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return GENERALERROR;

		CreatureObject* player = cast<CreatureObject*>(creature);

		PlayerObject* ghost = player->getPlayerObject();

		ghost->setBiography(arguments);
		
		ghost->sendDestroyTo(player);
		ghost->sendTo(player, true);

		return SUCCESS;
	}

};

#endif //SETBIOGRAPHYCOMMAND_H_
