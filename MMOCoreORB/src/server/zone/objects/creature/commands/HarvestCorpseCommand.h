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

#ifndef HARVESTCORPSECOMMAND_H_
#define HARVESTCORPSECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/Creature.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/Zone.h"

class HarvestCorpseCommand : public QueueCommand {
public:

	HarvestCorpseCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		if (!creature->isPlayerCreature())
			return INVALIDTARGET;

		ManagedReference<SceneObject* > object = server->getZoneServer()->getObject(target);

		ManagedReference<CreatureObject*> player = cast<CreatureObject*>(creature);

		StringTokenizer args(arguments.toString());

		if (object == NULL || !object->isCreatureObject() || player == NULL)
			return INVALIDTARGET;

		CreatureObject* creo = cast<CreatureObject*>( object.get());

		if (!creo->isCreature())
			return INVALIDTARGET;

		Creature* cr = cast<Creature*>( creo);

		Locker clocker(cr, player);

		String harvesttype = "";

		if (args.hasMoreTokens())
			args.getStringToken(harvesttype);

		harvesttype = harvesttype.toLowerCase();
		byte type = 0;

		if(harvesttype == "meat" && !cr->getMeatType().isEmpty())
			type = 234;
		else if(harvesttype == "hide" && !cr->getHideType().isEmpty())
			type = 235;
		else if(harvesttype == "bone" && !cr->getBoneType().isEmpty())
			type = 236;
		else {
			if (harvesttype != "") {
				player->sendSystemMessage("@error_message:no_resource");
				return GENERALERROR;
			}
			Vector<int> types;
			if(!cr->getMeatType().isEmpty()) {
				types.add(234);
			}

			if(!cr->getHideType().isEmpty()) {
				types.add(235);
			}

			if(!cr->getBoneType().isEmpty()) {
				types.add(236);
			}
			if(types.size() > 0)
				type = types.get(System::random(types.size() -1));
		}

		if(type == 0) {
			player->sendSystemMessage("This creature has no resources");
			return GENERALERROR;
		}

		if (cr->canHarvestMe(player)) {
			if (!cr->isDead())
				return GENERALERROR;

			if (cr->getZone() == NULL)
				return GENERALERROR;

			if (cr->getDnaState() == CreatureManager::DNADEATH) {
				player->sendSystemMessage("@skl_use:nothing_to_harvest");
				return GENERALERROR;
			}

			ManagedReference<CreatureManager*> manager = cr->getZone()->getCreatureManager();
			manager->harvest(cr, player, type);
		}


		return SUCCESS;
	}

};

#endif //HARVESTCORPSECOMMAND_H_
