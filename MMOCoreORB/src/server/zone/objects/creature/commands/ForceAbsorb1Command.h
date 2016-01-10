/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef FORCEABSORB1COMMAND_H_
#define FORCEABSORB1COMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/buffs/SingleUseBuff.h"
#include "JediQueueCommand.h"

class ForceAbsorb1Command : public JediQueueCommand {
public:

	ForceAbsorb1Command(const String& name, ZoneProcessServer* server)
		: JediQueueCommand(name, server) {
            buffCRCs.add(BuffCRC::JEDI_FORCE_ABSORB_1);
            
            blockingCRCs.add(BuffCRC::JEDI_FORCE_ABSORB_2);
            
            overrideableCRCs.add(BuffCRC::JEDI_FORCE_ABSORB_1);

            // Skill mods.
            skillMods.put("force_absorb", 1);
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
        
        return doJediSelfBuffCommand(creature);
	}

};

#endif //FORCEABSORB1COMMAND_H_
