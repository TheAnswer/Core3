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

#ifndef SNIPERSHOTCOMMAND_H_
#define SNIPERSHOTCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "CombatQueueCommand.h"

class SniperShotCommand : public CombatQueueCommand {
public:

	SniperShotCommand(const String& name, ZoneProcessServer* server)
		: CombatQueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;
			
		if (!creature->isPlayerCreature())
			return GENERALERROR;			

		ManagedReference<WeaponObject*> weapon = creature->getWeapon();

		if (!weapon->isRifleWeapon()) {
			return INVALIDWEAPON;
		}
		
		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);
		
		if (creature == targetObject || targetObject == NULL || !targetObject->isPlayerCreature())
			return INVALIDTARGET;

		CreatureObject* player = cast<CreatureObject*>( targetObject.get());

		Locker clocker(player, creature);

		PlayerManager* playerManager = server->getPlayerManager();

		if (!CollisionManager::checkLineOfSight(creature, player)) {
			creature->sendSystemMessage("@container_error_message:container18"); //You can't see that object.  You may have to move closer to it.
			return GENERALERROR;
		}


		int maxRange = weapon->getMaxRange();

		if (!player->isInRange(creature, maxRange + player->getTemplateRadius() + creature->getTemplateRadius()))
			return TOOFAR;
				
		if (player->isAttackableBy(creature) && player->isIncapacitated()) {
			playerManager->killPlayer(creature, player, 1);			
		}  else if (!player->isIncapacitated()) {
			creature->sendSystemMessage("@error_message:target_not_incapacitated");  //You cannot perform the death blow. Your target is not incapacitated.
		}  else if (player->isDead()) {
			return GENERALERROR;
		}

 		return doCombatAction(creature, target);

	}

};

#endif //SNIPERSHOTCOMMAND_H_
