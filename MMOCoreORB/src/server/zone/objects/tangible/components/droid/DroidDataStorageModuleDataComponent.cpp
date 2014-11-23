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

#include "DroidDataStorageModuleDataComponent.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/tangible/component/droid/DroidComponent.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"

DroidDataStorageModuleDataComponent::DroidDataStorageModuleDataComponent() {
	setLoggingName("DroidDataStorageModule");
	rating = 0;
}
DroidDataStorageModuleDataComponent::~DroidDataStorageModuleDataComponent() {

}
String DroidDataStorageModuleDataComponent::getModuleName() {
	return String("datapad_storage_module");
}
void DroidDataStorageModuleDataComponent::initializeTransientMembers() {
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent == NULL) {
		info("droidComponent was null");
		return;
	}
	if(droidComponent->hasKey( "data_module")) {
		rating = droidComponent->getAttributeValue( "data_module");
	}
}
void DroidDataStorageModuleDataComponent::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	rating = values->getCurrentValue("data_module");
}
int DroidDataStorageModuleDataComponent::getStorageRating() {
	switch(rating) {
		case 1:
		case 2:
			return 1;
		case 3:
		case 4:
			return 2;
		case 5:
		case 6:
			return 3;
		case 7:
		case 8:
			return 4;
		case 9:
		case 10:
			return 5;
	}
	return 6;
}
void DroidDataStorageModuleDataComponent::fillAttributeList(AttributeListMessage* alm, CreatureObject* droid) {
	// convert module rating to actual rating
	alm->insertAttribute( "data_module", rating > 10 ? 10 : rating);
}
String DroidDataStorageModuleDataComponent::toString(){
	return BaseDroidModuleComponent::toString();
}
void DroidDataStorageModuleDataComponent::addToStack(BaseDroidModuleComponent* other) {
	DroidDataStorageModuleDataComponent* otherModule = cast<DroidDataStorageModuleDataComponent*>(other);
	if(otherModule == NULL)
		return;
	rating = rating + otherModule->rating;
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent != NULL)
		droidComponent->changeAttributeValue("data_module",(float)rating);
}
void DroidDataStorageModuleDataComponent::copy(BaseDroidModuleComponent* other) {
	DroidDataStorageModuleDataComponent* otherModule = cast<DroidDataStorageModuleDataComponent*>(other);
	if(otherModule == NULL)
		return;
	rating = otherModule->rating;
	DroidComponent* droidComponent = cast<DroidComponent*>(getParent());
	if (droidComponent != NULL)
		droidComponent->addProperty("data_module",(float)rating,0,"exp_effectiveness");
}
void DroidDataStorageModuleDataComponent::onCall() {
	// no op
}
void DroidDataStorageModuleDataComponent::onStore() {
	// no op on store
}
void DroidDataStorageModuleDataComponent::fillObjectMenuResponse(SceneObject* droidObject, ObjectMenuResponse* menuResponse, CreatureObject* player) {
	// Add to Droid Options subradial from PetMenuComponent
	menuResponse->addRadialMenuItemToRadialID(132, DATA_STORAGE_MODULE_OPEN, 3, "@pet/pet_menu:menu_dpad" );
}
void DroidDataStorageModuleDataComponent::initialize(CreatureObject* droid) {
	StringBuffer path;
	path.append("object/tangible/datapad/droid_datapad_");
	path.append(getStorageRating());
	path.append(".iff");
	ManagedReference<SceneObject*> inventory = droid->getZoneServer()->createObject(path.toString().hashCode(),4);
	ManagedReference<SceneObject*> droidInvorty = droid->getSlottedObject("datapad");
	if (droidInvorty)
		droid->removeObject(droidInvorty,NULL,true);
	droid->transferObject(inventory,4,true);
}
int DroidDataStorageModuleDataComponent::handleObjectMenuSelect(CreatureObject* player, byte selectedID, PetControlDevice* controller) {

	// Handle open droid storage
	if( selectedID == DATA_STORAGE_MODULE_OPEN ){

		ManagedReference<DroidObject*> droid = getDroidObject();
		if( droid == NULL ){
			info( "Droid is null");
			return 0;
		}

		Locker dlock( droid, player );
		// open the inventory slot of the droid
		ManagedReference<SceneObject*> inventory = droid->getSlottedObject("datapad");

		if (inventory != NULL) {
			inventory->openContainerTo(player);
		}
	}
	return 0;
}

