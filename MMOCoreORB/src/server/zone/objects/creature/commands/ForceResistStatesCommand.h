/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCERESISTSTATESCOMMAND_H_
#define FORCERESISTSTATESCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"

class ForceResistStatesCommand : public JediQueueCommand {
public:

	ForceResistStatesCommand(const String& name, ZoneProcessServer* server)
		: JediQueueCommand(name, server) {

		buffCRC = BuffCRC::JEDI_RESIST_STATES;

		skillMods.put("resistance_states", 25);

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (creature->hasBuff(BuffCRC::JEDI_RESIST_STATES)) {
			creature->removeBuff(BuffCRC::JEDI_RESIST_STATES);
			return SUCCESS;
		} else
			return doJediSelfBuffCommand(creature);
	}

};

#endif //FORCERESISTSTATESCOMMAND_H_
