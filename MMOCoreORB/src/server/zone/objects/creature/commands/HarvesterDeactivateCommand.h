/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef HARVESTERDEACTIVATECOMMAND_H_
#define HARVESTERDEACTIVATECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class HarvesterDeactivateCommand : public QueueCommand {
public:

	HarvesterDeactivateCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		CreatureObject* player = cast<CreatureObject*>(creature);

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object == NULL || !object->isInstallationObject())
			return GENERALERROR;

		InstallationObject* inso = cast<InstallationObject*>( object.get());

		try {
			Locker clocker(inso, player);

			if (inso->isOnAdminList(player) && inso->isInRange(player, 20))
				inso->setOperating(false);
			else
				player->sendSystemMessage("You are too far.");

		} catch (Exception& e) {
		}

		return SUCCESS;
	}

};

#endif //HARVESTERDEACTIVATECOMMAND_H_
