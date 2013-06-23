/*
Copyright (C) 2007 <SWGEmu>

This File is part of Core3.

This program is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software
Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Linking Engine3 statically or dynamically with other modules
is making a combined work based on Engine3.
Thus, the terms and conditions of the GNU Lesser General Public License
cover the whole combination.

In addition, as a special exception, the copyright holders of Engine3
give you permission to combine Engine3 program with free software
programs or libraries that are released under the GNU LGPL and with
code included in the standard release of Core3 under the GNU LGPL
license (or modified versions of such code, with unchanged license).
You may copy and distribute such a system following the terms of the
GNU LGPL for Engine3 and the licenses of the other code concerned,
provided that you include the source code of that other code when
and as the GNU LGPL requires distribution of source code.

Note that people who make modified versions of Engine3 are not obligated
to grant this special exception for their modified versions;
it is their choice whether to do so. The GNU Lesser General Public License
gives permission to release a modified version without this exception;
this exception also makes it possible to release a modified version
which carries forward this exception.
*/

#ifndef HEALMINDWOUNDSELF2COMMAND_H
#define HEALMINDWOUNDSELF2COMMAND_H

#include "server/zone/objects/scene/SceneObject.h"

class HealMindWoundSelf2Command : public QueueCommand {
public:

	HealMindWoundSelf2Command(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	bool canPerformSkill(CreatureObject* creature) {
		if (!creature->getWounds(CreatureAttribute::MIND) && !creature->getWounds(CreatureAttribute::FOCUS) && !creature->getWounds(CreatureAttribute::WILLPOWER)) {
			creature->sendSystemMessage("@healing_response:healing_response_67"); // You have no wounds of that type.
			return false;
		}			
		
		return true;
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (isWearingArmor(creature)) {
			return NOJEDIARMOR;
		}

		// At this point, the player has enough Force... Can they perform skill?

		if (!canPerformSkill(creature))
				return GENERALERROR;



		// Lets see how much healing they are doing.

		int mindHealed = creature->healWound(creature, CreatureAttribute::MIND, 350);
		int focusHealed = creature->healWound(creature, CreatureAttribute::FOCUS, 350);
		int willpowerHealed = creature->healWound(creature, CreatureAttribute::WILLPOWER, 350);

			
		// Send system message(s).

		if (mindHealed > 0){
			StringIdChatParameter message1("jedi_spam", "heal_self");
			message1.setDI(mindHealed);
			message1.setTO("@jedi_spam:mind_wounds");
			creature->sendSystemMessage(message1);
		}

			
		if (focusHealed > 0){
			StringIdChatParameter message2("jedi_spam", "heal_self");
			message2.setDI(focusHealed);
			message2.setTO("@jedi_spam:focus_wounds");
			creature->sendSystemMessage(message2);
		}

			
		if (willpowerHealed > 0){
			StringIdChatParameter message3("jedi_spam", "heal_self");
			message3.setDI(willpowerHealed);
			message3.setTO("@jedi_spam:willpower_wounds");
			creature->sendSystemMessage(message3);
		}
		
		int forceCost = 0;
		
		ManagedReference<PlayerObject*> playerObject = creature->getPlayerObject();


		if (playerObject != NULL) {
			if (playerObject->getForcePower() <= 100) {
				creature->sendSystemMessage("@jedi_spam:no_force_power");
				return GENERALERROR;
			}

		forceCost = MIN(((mindHealed + focusHealed + willpowerHealed) / 15), 100);

			
		// Play client effect, and deduct Force Power.


		creature->playEffect("clienteffect/pl_force_heal_self.cef", "");
		playerObject->setForcePower(playerObject->getForcePower() - forceCost);
			
		return SUCCESS;
		}

		return GENERALERROR;
	}


	float getCommandDuration(CreatureObject* object, const UnicodeString& arguments) {
		return defaultTime * 3.0;
	}

};

#endif //HEALMINDWOUNDSELF2COMMAND_H
