/*
 * StructureTerminalMenuComponent.cpp
 *
 *  Created on: Feb 26, 2012
 *      Author: swgemu
 */

#include "StructureTerminalMenuComponent.h"
#include "server/zone/Zone.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/objects/tangible/terminal/Terminal.h"
#include "server/zone/managers/structure/StructureManager.h"
#include "server/zone/objects/player/sessions/StructureSetAccessFeeSession.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/creature/DroidObject.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/managers/creature/PetManager.h"


void StructureTerminalMenuComponent::fillObjectMenuResponse(SceneObject* sceneObject, ObjectMenuResponse* menuResponse, CreatureObject* creature) {

	if(!sceneObject->isTerminal())
		return;

	if(!creature->isPlayerCreature())
		return;

	ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
	if( ghost == NULL )
		return;

	ManagedReference<Terminal*> terminal = cast<Terminal*>(sceneObject);
	if(terminal == NULL)
		return;

	ManagedReference<StructureObject*> structureObject = cast<StructureObject*>(terminal->getControlledObject());

	if (structureObject == NULL)
		return;

	if (structureObject->isCivicStructure()) {
		if (structureObject->isOnAdminList(creature)) {
			menuResponse->addRadialMenuItem(118, 3, "@player_structure:management"); //Structure Management
			menuResponse->addRadialMenuItemToRadialID(118, 128, 3, "@player_structure:permission_destroy"); //Destroy Structure
			menuResponse->addRadialMenuItemToRadialID(118, 124, 3, "@player_structure:management_status"); //Status

			if (structureObject->isBuildingObject()) {
				menuResponse->addRadialMenuItemToRadialID(118, 50, 3, "@player_structure:management_name_structure"); //Name Structure
				menuResponse->addRadialMenuItemToRadialID(118, 201, 3, "@player_structure:delete_all_items"); //Delete all items
				menuResponse->addRadialMenuItemToRadialID(118, 202, 3, "@player_structure:move_first_item"); //Find Lost Items

				// Not all civic buildings have signs.  Check to see if build already has one before allowing a change
				BuildingObject* building = cast<BuildingObject*>(structureObject.get());
				if( building != NULL && building->getSignObject() != NULL )
					menuResponse->addRadialMenuItemToRadialID(118, 69, 3, "@player_structure:management_change_sign"); //Change Sign
			}
		}

		return;
	}

	if (structureObject->isOnAdminList(creature)) {
		menuResponse->addRadialMenuItem(118, 3, "@player_structure:management"); //Structure Management
		menuResponse->addRadialMenuItemToRadialID(118, 128, 3, "@player_structure:permission_destroy"); //Destroy Structure
		menuResponse->addRadialMenuItemToRadialID(118, 124, 3, "@player_structure:management_status"); //Status
		menuResponse->addRadialMenuItemToRadialID(118, 129, 3, "@player_structure:management_pay"); //Pay Maintenance

		if (structureObject->isGuildHall()) {
			menuResponse->addRadialMenuItemToRadialID(118, 70, 3, "@player_structure:take_maintenance"); // Withdraw Maintenance
		}

		menuResponse->addRadialMenuItemToRadialID(118, 50, 3, "@player_structure:management_name_structure"); //Name Structure

		ManagedReference<SceneObject*> datapad = creature->getSlottedObject("datapad");
		if(datapad != NULL) {
			for (int i = 0; i < datapad->getContainerObjectsSize(); ++i) {
				ManagedReference<SceneObject*> object = datapad->getContainerObject(i);

				if (object != NULL && object->isPetControlDevice()) {
					PetControlDevice* device = cast<PetControlDevice*>( object.get());

					if (device->getPetType() == PetManager::DROIDPET) {
						menuResponse->addRadialMenuItemToRadialID(118, 131, 3, "@player_structure:assign_droid"); //Assign Droid
						break;
					}
				}
			}
		}

		if (structureObject->isBuildingObject()) {
			menuResponse->addRadialMenuItemToRadialID(118, 127, 3, "@player_structure:management_residence"); //Declare Residence
			menuResponse->addRadialMenuItemToRadialID(118, 125, 3, "@player_structure:management_privacy"); //Privacy

			if (creature->hasSkill("crafting_artisan_business_01") && structureObject->isOnAdminList(creature)) {
				BuildingObject* building = cast<BuildingObject*>(structureObject.get());
				if(!building->hasAccessFee())
					menuResponse->addRadialMenuItemToRadialID(118, 68, 3, "@player_structure:management_add_turnstile"); //Set Access Fee
				else
					menuResponse->addRadialMenuItemToRadialID(118, 68, 3, "@player_structure:management_remove_turnstile"); //Remove Access Fee

			}

			if (creature->hasSkill("crafting_artisan_business_03"))
				menuResponse->addRadialMenuItemToRadialID(118, 130, 3, "@player_structure:create_vendor"); //Create Vendor


			menuResponse->addRadialMenuItemToRadialID(118, 69, 3, "@player_structure:management_change_sign"); //Change Sign
			menuResponse->addRadialMenuItemToRadialID(118, 201, 3, "@player_structure:delete_all_items"); //Delete all items
			menuResponse->addRadialMenuItemToRadialID(118, 202, 3, "@player_structure:move_first_item"); //Find Lost Items
		}

		menuResponse->addRadialMenuItem(117, 3, "@player_structure:permissions"); //Structure Permissions
		menuResponse->addRadialMenuItemToRadialID(117, 121, 3, "@player_structure:permission_admin"); //Administrator List

		if (structureObject->isBuildingObject()) {
			menuResponse->addRadialMenuItemToRadialID(117, 119, 3, "@player_structure:permission_enter"); //Entry List
			menuResponse->addRadialMenuItemToRadialID(117, 120, 3, "@player_structure:permission_banned"); //Ban List
			menuResponse->addRadialMenuItemToRadialID(117, 122, 3, "@player_structure:permission_vendor"); //Vendor List
		}
	} else if(structureObject->isOnPermissionList("VENDOR", creature)) {
		if (creature->hasSkill("crafting_artisan_business_03")) {
			menuResponse->addRadialMenuItem(118, 3, "@player_structure:management"); //Structure Management
			menuResponse->addRadialMenuItemToRadialID(118, 130, 3, "@player_structure:create_vendor"); //Create Vendor
		}
	}
}

int StructureTerminalMenuComponent::handleObjectMenuSelect(SceneObject* sceneObject, CreatureObject* creature, byte selectedID) {
	ManagedReference<Terminal*> terminal = cast<Terminal*>(sceneObject);
	if(terminal == NULL)
		return 1;

	if(!creature->isPlayerCreature())
		return 1;

	ManagedReference<PlayerObject*> ghost = creature->getPlayerObject();
	if( ghost == NULL )
		return 1;

	ManagedReference<StructureObject*> structureObject = cast<StructureObject*>(terminal->getControlledObject());

	if (structureObject == NULL)
		return 1;

	ManagedReference<Zone*> zone = structureObject->getZone();

	if (zone == NULL)
		return 1;

	if (structureObject->isCivicStructure()) {
		if (structureObject->isOnAdminList(creature)) {
			StructureManager* structureManager = StructureManager::instance();

			Locker structureLocker(structureObject, creature);

			switch (selectedID) {
			case 201:
				structureManager->promptDeleteAllItems(creature, structureObject);
				break;
			case 202:
				structureManager->promptFindLostItems(creature, structureObject);
				break;
			case 128:
				creature->executeObjectControllerAction(0x18FC1726, structureObject->getObjectID(), ""); //destroyStructure
				break;
			case 50:
				structureManager->promptNameStructure(creature, structureObject, NULL);
				break;
			case 124:
				creature->executeObjectControllerAction(0x13F7E585, structureObject->getObjectID(), ""); //structureStatus
				break;
			case 69:
				structureManager->promptSelectSign(structureObject, creature);
				break;
			}
		}

		return 0;
	}

	if (structureObject->isOnAdminList(creature)) {

		StructureManager* structureManager = StructureManager::instance();

		Locker structureLocker(structureObject, creature);

		switch (selectedID) {
		case 201:
			structureManager->promptDeleteAllItems(creature, structureObject);
			break;
		case 202:
			structureManager->promptFindLostItems(creature, structureObject);
			break;
		case 121:
			structureObject->sendPermissionListTo(creature, "ADMIN");
			break;
		case 119:
			structureObject->sendPermissionListTo(creature, "ENTRY");
			break;
		case 120:
			structureObject->sendPermissionListTo(creature, "BAN");
			break;
		case 122:
			structureObject->sendPermissionListTo(creature, "VENDOR");
			break;
		case 128:
			creature->executeObjectControllerAction(0x18FC1726, structureObject->getObjectID(), ""); //destroyStructure
			break;
		case 129:
			creature->executeObjectControllerAction(0xE7E35B30, structureObject->getObjectID(), ""); //payMaintenance
			break;
		case 70:
			structureManager->promptWithdrawMaintenance(structureObject, creature);
			break;
		case 124:
			creature->executeObjectControllerAction(0x13F7E585, structureObject->getObjectID(), ""); //structureStatus
			break;
		case 127:
			creature->executeObjectControllerAction(0xF59E3CE1, structureObject->getObjectID(), ""); //declareResidence
			break;
		case 125:
			creature->executeObjectControllerAction(0x786CC38E, structureObject->getObjectID(), ""); //setPrivacy
			break;
		case 68:
			if(structureObject->isBuildingObject()) {
				BuildingObject* building = cast<BuildingObject*>(structureObject.get());
				if(!building->hasAccessFee()) {

					if(!building->canChangeAccessFee()) {
						StringIdChatParameter param("@player_structure:turnstile_wait");
						param.setDI(building->getAccessFeeDelay());
						creature->sendSystemMessage(param);
						return 0;
					}

					ManagedReference<StructureSetAccessFeeSession*> session = new StructureSetAccessFeeSession(creature, building);
					if(session->initializeSession() == 1) {
						creature->addActiveSession(SessionFacadeType::SETSTRUCTUREACCESSFEE, session);
						session->promptSetAccessFee();
					}
				} else {
					building->removeAccessFee();
					creature->sendSystemMessage("Access fee removed");
				}
			}
			break;
		case 50:
			structureManager->promptNameStructure(creature, structureObject, NULL);
			//creature->executeObjectControllerAction(0xC367B461, structureObject->getObjectID(), ""); //nameStructure
			break;
		case 69:
			structureManager->promptSelectSign(structureObject, creature);
			break;
		case 131: // Assign Droid
			structureManager->promptMaintenanceDroid(structureObject,creature);
			break;
		}

	}

	if(selectedID == 130 && (structureObject->isOnAdminList(creature) || structureObject->isOnPermissionList("VENDOR", creature))) {
		if (creature->hasSkill("crafting_artisan_business_03")) {
			creature->executeObjectControllerAction(STRING_HASHCODE("createvendor")); // Create Vendor
		}
	}


	return 0;
}
