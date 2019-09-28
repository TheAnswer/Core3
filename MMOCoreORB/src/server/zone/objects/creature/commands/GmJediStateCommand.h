/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef GMJEDISTATECOMMAND_H_
#define GMJEDISTATECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class GmJediStateCommand : public QueueCommand {
public:

	GmJediStateCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<SceneObject*> object = server->getZoneServer()->getObject(target);

		if (object == nullptr || !object->isCreatureObject())
			return INVALIDTARGET;

		CreatureObject* targetCreature = cast<CreatureObject*>( object.get());

		Locker clocker(targetCreature, creature);

		ManagedReference<PlayerObject*> targetGhost = targetCreature->getPlayerObject();
		if(targetGhost == nullptr)
			return INVALIDTARGET;

		StringTokenizer args(arguments.toString());

		int jediState = args.getIntToken();


		targetGhost->setJediState(jediState);

		return SUCCESS;
	}

};

#endif //GMJEDISTATECOMMAND_H_
