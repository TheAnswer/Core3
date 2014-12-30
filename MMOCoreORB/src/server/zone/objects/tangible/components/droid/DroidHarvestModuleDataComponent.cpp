/*
 * Copyright (C) 2014 <SWGEmu>
 * This File is part of Core3.
 * This program is free software; you can redistribute
 * it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Linking Engine3 statically or dynamically with other modules
 * is making a combined work based on Engine3.
 * Thus, the terms and conditions of the GNU Lesser General Public License
 * cover the whole combination.
 *
 * In addition, as a special exception, the copyright holders of Engine3
 * give you permission to combine Engine3 program with free software
 * programs or libraries that are released under the GNU LGPL and with
 * code included in the standard release of Core3 under the GNU LGPL
 * license (or modified versions of such code, with unchanged license).
 * You may copy and distribute such a system following the terms of the
 * GNU LGPL for Engine3 and the licenses of the other code concerned,
 * provided that you include the source code of that other code when
 * and as the GNU LGPL requires distribution of source code.
 *
 * Note that people who make modified versions of Engine3 are not obligated
 * to grant this special exception for their modified versions;
 * it is their choice whether to do so. The GNU Lesser General Public License
 * gives permission to release a modified version without this exception;
 * this exception also makes it possible to release a modified version
 * which carries forward this exception.
 */

#include "DroidHarvestModuleDataComponent.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/tangible/component/droid/DroidComponent.h"
#include "server/zone/objects/creature/events/DroidHarvestTask.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/intangible/tasks/EnqueuePetCommand.h"
#include "server/zone/objects/player/sui/listbox/SuiListBox.h"
#include "server/zone/objects/creature/sui/SelectHarvestTypeSuiCallback.h"
#include "server/zone/objects/player/PlayerObject.h"

DroidHarvestModuleDataComponent::DroidHarvestModuleDataComponent() {
	harvestBonus = 0;
	interest = 0; // random
	active = false;
	setLoggingName("DroidHarvestModule");
}
DroidHarvestModuleDataComponent::~DroidHarvestModuleDataComponent() {

}
String DroidHarvestModuleDataComponent::getModuleName() {
	return String("harvest_module");
}
void DroidHarvestModuleDataComponent::initializeTransientMembers() {

	// Pull module stat from parent sceno
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL) {
		info("droidComponent was null");
		return;
	}

	if( droidComponent->hasKey( "harvest_power") ){
		harvestBonus = droidComponent->getAttributeValue( "harvest_power");
	}
	else{
		info( "harvest_power attribute not found" );
	}
}

void DroidHarvestModuleDataComponent::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	harvestBonus = values->getCurrentValue("harvest_power");
}

void DroidHarvestModuleDataComponent::fillAttributeList(AttributeListMessage* alm, CreatureObject* droid) {
	alm->insertAttribute( "harvest_power", harvestBonus );
	if (interest == INTREST_BONE)
		alm->insertAttribute("pet_command_21","@pet/droid_modules:interest_set_bone");
	if (interest == INTREST_MEAT)
		alm->insertAttribute("pet_command_21","@pet/droid_modules:interest_set_meat");
	if (interest == INTREST_HIDE)
		alm->insertAttribute("pet_command_21","@pet/droid_modules:interest_set_hide");
	if (interest == INTREST_RANDOM)
		alm->insertAttribute("pet_command_21","@pet/droid_modules:interest_set_random");
}

void DroidHarvestModuleDataComponent::fillObjectMenuResponse(SceneObject* droidObject, ObjectMenuResponse* menuResponse, CreatureObject* player) {
	// Add to Droid Options subradial from PetMenuComponent
	// add top level optins
	// then the sub menus
	// multiple levels
	if (player->hasSkill("outdoors_scout_novice")){
		menuResponse->addRadialMenuItem(HARVEST_MENU,3,"@pet/droid_modules:harvest_options");
		menuResponse->addRadialMenuItemToRadialID(HARVEST_MENU,HARVEST_PROGRAM_COMMAND,3, "@pet/droid_modules:program_target_harvest");
		menuResponse->addRadialMenuItemToRadialID(HARVEST_MENU,HARVEST_TOGGLE,3,"@pet/droid_modules:toggle_auto_harvest");
		menuResponse->addRadialMenuItemToRadialID(HARVEST_MENU,HARVEST_SET_INTEREST,3,"@pet/droid_modules:set_harvest_interest");
	}
	//menuResponse->addRadialMenuItemToRadialID(132, AUTO_REPAIR_MODULE_TOGGLE, 3, "@pet/droid_modules:harvest_options" );
}
void DroidHarvestModuleDataComponent::setHarvestInterest(CreatureObject* player, int option) {
	interest = option;
	if (option == INTREST_BONE) {
		player->sendSystemMessage("@pet/droid_modules:interest_set_bone");
	}
	if (option == INTREST_MEAT) {
		player->sendSystemMessage("@pet/droid_modules:interest_set_meat");
	}
	if (option == INTREST_HIDE) {
		player->sendSystemMessage("@pet/droid_modules:interest_set_hide");
	}
	if (option == INTREST_RANDOM) {
		player->sendSystemMessage("@pet/droid_modules:interest_set_random");
	}

}
int DroidHarvestModuleDataComponent::handleObjectMenuSelect(CreatureObject* player, byte selectedID, PetControlDevice* controller) {

	if (selectedID == HARVEST_SET_INTEREST) {
		ManagedReference<DroidObject*> droid = getDroidObject();
		if( droid == NULL ){
			info( "Droid is null");
			return 0;
		}

		if( !droid->hasPower() ){
			player->sendSystemMessage("@pet/droid_modules:playback_msg_play_out_of_power");
			return 0;
		}

		// popup a ui to choose the correct interest.
		ManagedReference<SuiListBox*> box = new SuiListBox(player, SuiWindowType::DROID_SET_INTEREST, SuiListBox::HANDLETWOBUTTON);
		box->setCallback(new SelectHarvestSuiCallback(player->getZoneServer()));
		box->setPromptText("@pet/droid_modules:set_interest_d");
		box->setPromptTitle("@pet/droid_modules:set_interest_d");
		box->setOkButton(true,"@ok");
		box->setCancelButton(true, "@cancel");
		// Add tracks
		box->addMenuItem("@pet/droid_modules:set_interest_random",INTREST_RANDOM);
		box->addMenuItem("@pet/droid_modules:set_interest_bone",INTREST_BONE);
		box->addMenuItem("@pet/droid_modules:set_interest_meat",INTREST_MEAT);
		box->addMenuItem("@pet/droid_modules:set_interest_hide",INTREST_HIDE);
		box->setUsingObject(droid);
		player->getPlayerObject()->addSuiBox(box);
		player->sendMessage(box->generateMessage());
		return 0;
	}
	// Handle toggle on/off
	if (selectedID == HARVEST_PROGRAM_COMMAND) {
		if( controller == NULL )
			return 0;
		controller->setTrainingCommand( PetManager::HARVEST );
		return 0;
	}
	if( selectedID == HARVEST_TOGGLE ){

		ManagedReference<DroidObject*> droid = getDroidObject();
		if( droid == NULL ){
			info( "Droid is null");
			return 0;
		}

		Locker dlock( droid, player );

		// Toggle off
		if (active){
			deactivate();
			player->sendSystemMessage("@pet/droid_modules:auto_harvest_on");  // You turn off auto-repair
		}
		else{ // Toggle on

			// Check droid states
			if( droid->isDead() || droid->isIncapacitated())
				return 0;

			// Droid must have power
			if( !droid->hasPower() ){
				droid->showFlyText("npc_reaction/flytext","low_power", 204, 0, 0);  // "*Low Power*"
				return 0;
			}

			// Ensure we don't accidentally have another task outstanding
			deactivate();
			player->sendSystemMessage("@pet/droid_modules:auto_harvest_off");  // You turn on auto-repair
			if (observer == NULL) {
				observer = new DroidHarvestObserver(this);
				observer->deploy();
			}
			Locker plock(player);
			player->registerObserver(ObserverEventType::KILLEDCREATURE, observer);
			active = true;
			// set observer here for creature death to go harvest it if in range
		}

	}
	return 0;
}

int DroidHarvestModuleDataComponent::getBatteryDrain() {

	if( active ){
		return 4;
	}

	return 1;
}

void DroidHarvestModuleDataComponent::deactivate() {

	active = false;

	ManagedReference<DroidObject*> droid = getDroidObject();
	if( droid == NULL ){
		info( "Droid is null" );
		return;
	}

	Locker dlock( droid );

	// remove observer
	ManagedReference<CreatureObject*> player = droid->getLinkedCreature().get();

	if (player != NULL) {
		Locker clock(player, droid);
		player->dropObserver(ObserverEventType::KILLEDCREATURE, observer);
	}
}

String DroidHarvestModuleDataComponent::toString(){
	return BaseDroidModuleComponent::toString();
}

void DroidHarvestModuleDataComponent::onCall(){
	deactivate();
}

void DroidHarvestModuleDataComponent::onStore(){
	deactivate();
}

void DroidHarvestModuleDataComponent::addToStack(BaseDroidModuleComponent* other){

	DroidHarvestModuleDataComponent* otherModule = cast<DroidHarvestModuleDataComponent*>(other);
	if( otherModule == NULL )
		return;

	harvestBonus = harvestBonus + otherModule->harvestBonus;

	// Save stat in parent sceno
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL)
		return;
	droidComponent->changeAttributeValue( "harvest_power", harvestBonus);

}

void DroidHarvestModuleDataComponent::copy(BaseDroidModuleComponent* other){

	DroidHarvestModuleDataComponent* otherModule = cast<DroidHarvestModuleDataComponent*>(other);
	if( otherModule == NULL )
		return;

	harvestBonus = otherModule->harvestBonus;

	// Save stat in parent sceno
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL)
		return;
	droidComponent->addProperty("harvest_power", harvestBonus, 0, "exp_effectiveness");
}
void DroidHarvestModuleDataComponent::handlePetCommand(String cmd, CreatureObject* speaker){

	ManagedReference<DroidObject*> droid = getDroidObject();
	if( droid == NULL ){
		return;
	}

	ManagedReference<PetControlDevice*> pcd = droid->getControlDevice().get().castTo<PetControlDevice*>();
	if( pcd == NULL ) {
		return;
	}

	PetManager* petManager = droid->getZoneServer()->getPetManager();
	if( petManager == NULL ) {
		return;
	}

	// Owner-only command
	if( droid->getLinkedCreature().get() != speaker ) {
		return;
	}

	if( petManager->isTrainedCommand( pcd, PetManager::HARVEST, cmd ) ){
		Locker dlock(droid);
		// tell droid to goto target
		petManager->enqueuePetCommand(speaker, droid, String("petHarvest").toLowerCase().hashCode(), "");
	}
}
void DroidHarvestModuleDataComponent::creatureHarvestCheck(CreatureObject* target) {
	// check to see if we have loot rights
	ManagedReference<DroidObject*> droid = getDroidObject();
	if( droid == NULL ){
		return;
	}
	Locker dlock(droid);
	// harvest task check values
	// tell droid togo target
	droid->setTargetObject(target);
	droid->activateMovementEvent();
	EnqueuePetCommand* enqueueCommand = new EnqueuePetCommand(droid, String("petHarvest").toLowerCase().hashCode(), "", target->getObjectID(), 1);
	// give a second delay for the command so that all updates can occur for inv and such
	droid->addPendingTask("droid_harvest_command_reschedule",enqueueCommand,1000);
}
