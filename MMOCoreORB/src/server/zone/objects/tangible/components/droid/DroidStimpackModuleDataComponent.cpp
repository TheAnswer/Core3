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

#include "DroidStimpackModuleDataComponent.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/tangible/component/droid/DroidComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/tangible/pharmaceutical/StimPack.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "server/zone/objects/creature/events/DroidStimpackTask.h"

DroidStimpackModuleDataComponent::DroidStimpackModuleDataComponent() {
	setLoggingName("DroidStimpackModule");
	speed = 0;
	capacity = 0;
	rate = 0;
}
DroidStimpackModuleDataComponent::~DroidStimpackModuleDataComponent() {

}
String DroidStimpackModuleDataComponent::getModuleName() {
	return String("stimpack_module");
}
void DroidStimpackModuleDataComponent::initializeTransientMembers() {
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL) {
		info("droidComponent was null");
		return;
	}
	if(droidComponent->hasKey( "stimpack_capacity")) {
		capacity = droidComponent->getAttributeValue( "stimpack_capacity");
	}
	if (droidComponent->hasKey("stimpack_speed")) {
		speed = droidComponent->getAttributeValue("stimpack_speed");
	}
}
void DroidStimpackModuleDataComponent::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	capacity = values->getCurrentValue("stimpack_capacity");
	speed = values->getCurrentValue("stimpack_speed");
}
void DroidStimpackModuleDataComponent::fillAttributeList(AttributeListMessage* alm, CreatureObject* droid) {
	// convert module rating to actual rating
	alm->insertAttribute( "stimpack_capacity", capacity);
	alm->insertAttribute( "stimpack_speed", speed);
}
String DroidStimpackModuleDataComponent::toString(){
	return BaseDroidModuleComponent::toString();
}
void DroidStimpackModuleDataComponent::addToStack(BaseDroidModuleComponent* other) {
	DroidStimpackModuleDataComponent* otherModule = cast<DroidStimpackModuleDataComponent*>(other);
	if(otherModule == NULL)
		return;
	speed = speed + otherModule->speed;
	capacity = capacity + otherModule->capacity;
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent != NULL){
		droidComponent->changeAttributeValue("stimpack_capacity",(float)capacity);
		droidComponent->changeAttributeValue("stimpack_speed",(float)speed);
	}
}
void DroidStimpackModuleDataComponent::copy(BaseDroidModuleComponent* other) {
	DroidStimpackModuleDataComponent* otherModule = cast<DroidStimpackModuleDataComponent*>(other);
	if(otherModule == NULL)
		return;
	speed = speed + otherModule->speed;
	capacity = capacity + otherModule->capacity;
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent != NULL){
		droidComponent->addProperty("stimpack_speed",(float)speed,0,"exp_effectiveness");
		droidComponent->addProperty("stimpack_capacity",(float)capacity,0,"exp_durability");
	}
}
void DroidStimpackModuleDataComponent::onCall() {
	// no op
	if (speed == 0) {
		rate = 60000;
	} else {
		rate = round((float)60 * ((float)1 / (float)speed)) * 1000;
	}
}
void DroidStimpackModuleDataComponent::onStore() {
	// no op on store
}
void DroidStimpackModuleDataComponent::fillObjectMenuResponse(SceneObject* droidObject, ObjectMenuResponse* menuResponse, CreatureObject* player) {
	// Add to Droid Options subradial from PetMenuComponent
	menuResponse->addRadialMenuItemToRadialID(132, REQUEST_STIMPACK, 3, "@pet/droid_modules:request_stimpack" );
	if (player != NULL && player->hasSkill("science_medic_ability_04"))
		menuResponse->addRadialMenuItemToRadialID(132, LOAD_STIMPACK, 3, "@pet/droid_modules:load_stimpack" );
}
void DroidStimpackModuleDataComponent::initialize(CreatureObject* droid) {
	// grab the crafted components in this module and remove then
	// then change capacity to the new capacity so we store the stims directly in the component.
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL) {
		info("droidComponent was null");
		return;
	}
	droidComponent->dropSlottedObject("crafted_components");
	ManagedReference<SceneObject*> craftingComponents = droidComponent->getSlottedObject("crafted_components");
	if(craftingComponents == NULL) {
		// create the satchel and container as it would not be present as this object doesnt use components.
		ManagedReference<SceneObject*> craftingComponentsSatchel = NULL;
		String craftingComponentsPath = "object/tangible/crafting/crafting_components_container.iff";
		craftingComponents = droidComponent->getZoneServer()->createObject(craftingComponentsPath.hashCode(), 1);
		craftingComponents->removeAllContainerObjects();
		craftingComponents->setSendToClient(false);
		droidComponent->transferObject(craftingComponents, 4, false);
		craftingComponents->setContainerDefaultDenyPermission(ContainerPermissions::OPEN + ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponents->setContainerDefaultAllowPermission(0);
		craftingComponents->setContainerDenyPermission("owner", ContainerPermissions::OPEN + ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponents->setContainerDenyPermission("admin", ContainerPermissions::OPEN + ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponents->setContainerAllowPermission("owner", 0);
		craftingComponents->setContainerAllowPermission("admin", 0);
		craftingComponents->setContainerInheritPermissionsFromParent(false);

		String craftingComponentsSatchelPath = "object/tangible/hopper/crafting_station_hopper/crafting_station_ingredient_hopper_large.iff";
		craftingComponentsSatchel = droidComponent->getZoneServer()->createObject(craftingComponentsSatchelPath.hashCode(), 1);
		craftingComponentsSatchel->setContainerVolumeLimit(capacity);
		craftingComponentsSatchel->setContainerInheritPermissionsFromParent(false);
		craftingComponentsSatchel->setContainerDefaultDenyPermission(ContainerPermissions::OPEN + ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponentsSatchel->setContainerDefaultAllowPermission(0);
		craftingComponentsSatchel->setContainerAllowPermission("admin", ContainerPermissions::OPEN);
		craftingComponentsSatchel->setContainerDenyPermission("admin", ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponentsSatchel->setContainerAllowPermission("owner", 0);
		craftingComponentsSatchel->setContainerDenyPermission("owner", ContainerPermissions::OPEN + ContainerPermissions::MOVEIN + ContainerPermissions::MOVEOUT + ContainerPermissions::MOVECONTAINER);
		craftingComponentsSatchel->sendTo(droid, true);
		craftingComponents->transferObject(craftingComponentsSatchel, -1, false);
	}
}
int DroidStimpackModuleDataComponent::handleObjectMenuSelect(CreatureObject* player, byte selectedID, PetControlDevice* controller) {

	ManagedReference<DroidObject*> droid = getDroidObject();
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if( droid == NULL || droidComponent == NULL){
		player->sendSystemMessage("@pet/droid_modules:stimpack_error");
		return 0;
	}
	if( selectedID == LOAD_STIMPACK ){
		Locker dLock(droid);
		Locker locker(player);
		ManagedReference<SceneObject*> craftingComponents = droidComponent->getSlottedObject("crafted_components");

		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");
		if (inventory == NULL) {
			player->sendSystemMessage("@pet/droid_modules:no_stimpacks");
			return 0;
		}
		StimPack* stim = NULL;
		for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
			SceneObject* item = inventory->getContainerObject(i);

			if (!item->isTangibleObject())
				continue;

			TangibleObject* tano = cast<TangibleObject*>( item);
			// check it they have atleast 1 stim pack
			if (tano->isPharmaceuticalObject()) {
				PharmaceuticalObject* pharma = cast<PharmaceuticalObject*>( tano);
				if (pharma->isStimPack()) {
					stim = cast<StimPack*>(pharma);
					if (stim->isRangedStimPack() || stim->isStatePack() || stim->isArea() || !stim->isClassA()) {
						stim = NULL;
					}
				}
			}
		}
		if (stim == NULL) {
			player->sendSystemMessage("@pet/droid_modules:no_stimpacks");
			return 0;
		} else {
			// we need to send the invlid stimpack message just wher eis a good question
			if (craftingComponents != NULL) {
				//craftingComponents->sendTo(player,true);
				SceneObject* satchel = craftingComponents->getContainerObject(0);
				if (satchel != NULL){
					if(satchel->getContainerObjectsSize() +1 < satchel->getContainerVolumeLimit()) {
						stim->destroyObjectFromWorld(false);
						satchel->transferObject(stim,-1,true);
						player->sendSystemMessage("@pet/droid_modules:stimpack_loaded");
						inventory->closeContainerTo(player);
					} else {
						player->sendSystemMessage("@pet/droid_modules:stimpack_capacity_full");
					}
				}
			}
			return 0;
		}
	}
	if( selectedID == REQUEST_STIMPACK ){
		// we need to check cooldown on droid
		if( droid->isDead() || droid->isIncapacitated() || player->isDead()) {
			return 0;
		}

		// Droid must have power
		if( !droid->hasPower() ){
			droid->showFlyText("npc_reaction/flytext","low_power", 204, 0, 0);  // "*Low Power*"
			return 0;
		}
		if(droid->getPendingTask("droid_request_stimpack") != NULL) {
			player->sendSystemMessage("@pet/droid_modules:stimpack_not_ready");
			return 0;
		}
		// Submit repair task
		Reference<Task*> task = new DroidStimpackTask( droid,player,controller->getCustomObjectName().toString() );
		droid->addPendingTask("droid_request_stimpack", task, 1); // rte determines when it will fire it
	}
	return 0;
}
int DroidStimpackModuleDataComponent::getBatteryDrain() {
	return 2;
}
