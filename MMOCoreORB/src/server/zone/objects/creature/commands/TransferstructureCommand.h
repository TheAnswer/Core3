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

#ifndef TRANSFERSTRUCTURECOMMAND_H_
#define TRANSFERSTRUCTURECOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/templates/tangible/SharedStructureObjectTemplate.h"

class TransferstructureCommand : public QueueCommand {
public:

	TransferstructureCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}



	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		ManagedReference<PlayerManager*> playerManager = server->getPlayerManager();

		ManagedReference<SceneObject*> obj = playerManager->getInRangeStructureWithAdminRights(creature);

		if (obj == NULL || !obj->isStructureObject()) {
			creature->sendSystemMessage("@player_structure:command_no_building"); //You must be in a building or near an installation to use that command.
			return INVALIDPARAMETERS;
		}

		StructureObject* structure = cast<StructureObject*>( obj.get());

		if (!structure->isOwnerOf(creature)) {
			creature->sendSystemMessage("@player_structure:not_owner"); //You are not the owner of this structure.
			return GENERALERROR;
		}

		if (structure->isBuildingObject() && creature->getRootParent() != structure) {
			creature->sendSystemMessage("@player_structure:not_in_building"); //You must be inside your building to transfer it.
			return GENERALERROR;
		}

		if (structure->isBuildingObject() && creature->getRootParent() != structure) {

			BuildingObject* building = cast<BuildingObject*>(structure);

			for (int i = 1; i < building->getTotalCellNumber(); ++i) {
				ManagedReference<CellObject*> cell = building->getCell(i);

				if(cell == NULL)
					continue;

				for(int j = 0; j < cell->getContainerObjectsSize(); ++j) {
					ManagedReference<SceneObject*> obj = cell->getContainerObject(j);

					if(obj == NULL)
						continue;

					if(obj->isNoTrade()) {
						StringIdChatParameter param("@player_structure:building_has_notrade");
						param.setTT(obj->getDisplayedName());
						creature->sendSystemMessage(param);
						return GENERALERROR;
					}
				}
			}
		}

		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);

		if (targetObject == NULL || !targetObject->isCreatureObject() || !targetObject->isPlayerCreature()) {
			creature->sendSystemMessage("@player_structure:no_transfer_target"); //You must specify a player with whom to transfer ownership.
			return INVALIDTARGET;
		}

		if (targetObject == creature) {
			creature->sendSystemMessage("@player_structure:already_owner"); //You are already the owner.
			return GENERALERROR;
		}

		CreatureObject* targetCreature = cast<CreatureObject*>( targetObject.get());

		if (structure->isOnBanList(targetCreature)) {
			creature->sendSystemMessage("@player_structure:no_banned_player"); //You cannot transfer ownership to a banend player.
			return GENERALERROR;
		}

		Reference<SharedStructureObjectTemplate*> tmpl = cast<SharedStructureObjectTemplate*>(obj->getObjectTemplate());

		PlayerObject* ghost = targetCreature->getPlayerObject();

		String& abilityRequired = tmpl->getAbilityRequired();

		if (abilityRequired != "" && !ghost->hasAbility(abilityRequired)) {
			StringIdChatParameter params("@player_structure:not_able_to_own"); //%NT is not able to own this structure.
			params.setTT(targetCreature);
			creature->sendSystemMessage(params);
			return GENERALERROR;
		}


		Locker _lock(targetCreature, creature);

		return doTransferStructure(creature, targetCreature, structure);

	}

	// pre: creature locked and targetCreature are locked
	// structure not locked
	// bForceTransfer = whether or not to force the transfer.  meaning do the trasnfer even if the owner is offline or out of range
	static int doTransferStructure(CreatureObject* creature, CreatureObject* targetCreature, StructureObject* structure, bool bForceTransfer = false){
		Locker _cclock(creature);

		Locker _crlock(targetCreature, creature);


		ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
		ManagedReference<PlayerObject*> targetGhost = targetCreature->getPlayerObject();

		if (targetGhost == NULL || ghost == NULL) {
			return GENERALERROR; //Target or creature is not a player and cannot own this structure!
		}

		Locker _slock(structure);

		//Ensure that they are within at least 16m of the transferrer.
		if ((!targetCreature->isInRange(creature, 16.f) || !targetGhost->isOnline()) && !bForceTransfer ) {
			StringIdChatParameter params("@cmd_err:target_range_prose"); //Your target is too far away to %TO.
			params.setTO("Transfer Structure");
			creature->sendSystemMessage(params);
			return TOOFAR;
		}

		int lotSize = structure->getLotSize();

		if (!targetGhost->hasLotsRemaining(lotSize)) {
			if ( !bForceTransfer) {
				System::out << "lotsize: " << lotSize << endl;
				StringIdChatParameter params("@player_structure:not_able_to_own"); //%NT is not able to own this structure.
				params.setTT(targetCreature);
				creature->sendSystemMessage(params);
			} else {

				// send message to the person trying to do the guild transfer
			}
			return GENERALERROR;
		}

		//TODO:
		//@player_structure:trail_no_transfer Trial accounts may not be involved in a property ownership transfer.
		//@player_structure:building_has_no_trade The object %TT may not be traded and must be put in your inventory or destroyed before the building can be transferred.
		//@player_structure:faction_base You cannot transfer your factional base access and allotment responsibility to anyone else.
		ManagedReference<CityRegion*> region = structure->getCityRegion();

		if (region != NULL) {
			Locker locker(region, creature);

			if (region->isBanned(targetCreature->getObjectID())) {
				creature->sendSystemMessage("@city/city:cant_transfer_to_city_banned"); //You cannot transfer ownership of a structure to someone who is banned from the city in which the structure resides.
				return GENERALERROR;
			}

			if (region->isZoningEnabled() && !region->hasZoningRights(targetCreature->getObjectID())) {
				targetCreature->sendSystemMessage("@player_structure:no_rights"); //You don't have the right to place that structure in this city. The mayor or one of the city milita must grant you zoning rights first.

				return GENERALERROR;
			}

			if (ghost->getDeclaredResidence() == structure->getObjectID()) {
				region->removeCitizen(creature->getObjectID());
			}
		}

		if (ghost->getDeclaredResidence() == structure->getObjectID()) {
			ghost->setDeclaredResidence(NULL);
		}

		//Transfer ownership
		ghost->removeOwnedStructure(structure);
		targetGhost->addOwnedStructure(structure);

		//Setup permissions.
		structure->revokeAllPermissions(targetCreature->getObjectID());
		structure->grantPermission("ADMIN", targetCreature->getObjectID());

		structure->setOwner(targetCreature->getObjectID());
		structure->revokePermission("ADMIN", creature->getObjectID());

		//Update the cell permissions if the structure is private and a building.
		if (!structure->isPublicStructure() && structure->isBuildingObject()) {
			BuildingObject* buildingObject = cast<BuildingObject*>( structure);

			buildingObject->updateCellPermissionsTo(targetCreature);
			buildingObject->updateCellPermissionsTo(creature);
		}

		StringIdChatParameter params("@player_structure:ownership_transferred_in"); //%TT has transfered ownership of the structure to you
		params.setTT(creature->getFirstName());
		targetCreature->sendSystemMessage(params);


		params.setStringId("@player_structure:ownership_transferred_out"); //Ownership of the structure has been transferred to %NT.
		params.setTT(targetCreature->getFirstName());
		creature->sendSystemMessage(params);

		return SUCCESS;
	}

};

#endif //TRANSFERSTRUCTURECOMMAND_H_
