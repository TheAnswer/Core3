/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#ifndef FORCESPEED2COMMAND_H_
#define FORCESPEED2COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class ForceSpeed2Command : public JediQueueCommand {
public:

	ForceSpeed2Command(const String& name, ZoneProcessServer* server)
	: JediQueueCommand(name, server) {

		buffCRC = BuffCRC::JEDI_FORCE_SPEED_2;

		blockingCRCs.add(BuffCRC::JEDI_FORCE_SPEED_1);
		blockingCRCs.add(BuffCRC::JEDI_FORCE_SPEED_2);
		skillMods.put("combat_haste", 25);
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		return doJediSelfBuffCommand(creature);
	}

};

#endif //FORCESPEED2COMMAND_H_
