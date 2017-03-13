/*
 * VendorManager.cpp
 *
 *  Created on: Mar 23, 2011
 *      Author: polonel
 */

#include "VendorManager.h"
#include "server/zone/managers/vendor/sui/DestroyVendorSuiCallback.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/auction/AuctionItem.h"
#include "server/zone/objects/player/sui/inputbox/SuiInputBox.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/managers/vendor/sui/RenameVendorSuiCallback.h"
#include "server/zone/managers/vendor/sui/RegisterVendorSuiCallback.h"
#include "server/zone/managers/auction/AuctionManager.h"
#include "server/zone/managers/auction/AuctionsMap.h"
#include "server/zone/objects/tangible/components/vendor/VendorDataComponent.h"
#include "server/zone/ZoneProcessServer.h"

VendorManager::VendorManager() {
	setLoggingName("VendorManager");
	server = NULL;
	nameManager = NULL;
}

void VendorManager::initialize(ZoneProcessServer* zserv) {
	server = zserv;

	loadLuaVendors();
	loadVendorOutfits();
}

void VendorManager::loadLuaVendors() {
	info("Loading Vendor Options...");

	Lua* lua = new Lua();
	lua->init();

	lua->runFile("scripts/managers/vendor_manager.lua");

	LuaObject menu = lua->getGlobalObject("VendorMenu");

	rootNode = new VendorSelectionNode();

	rootNode->parseFromLua(menu);

	menu.pop();

	delete lua;
	lua = NULL;
}

bool VendorManager::isValidVendorName(const String& name) {

	if(nameManager == NULL)
		nameManager = server->getNameManager();

	if(nameManager == NULL) {
		error("Name manager is NULL");
		return false;
	}

	return nameManager->validateVendorName(name) == NameManagerResult::ACCEPTED;
}

void VendorManager::handleDisplayStatus(CreatureObject* player, TangibleObject* vendor) {

	if(vendor->getZone() == NULL) {
		error("NULL zone in VendorManager::handleDisplayStatus");
		return;
	}

	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if(vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	ManagedReference<SuiListBox*> statusBox = new SuiListBox(player, SuiWindowType::STRUCTURE_VENDOR_STATUS);
	statusBox->setUsingObject(vendor);
	statusBox->setPromptTitle("@player_structure:vendor_status");
	statusBox->setPromptText("Vendor Status");

	ManagedReference<CreatureObject*> owner = server->getZoneServer()->getObject(vendorData->getOwnerId()).castTo<CreatureObject*>();
	String ownerName;

	if (owner == NULL)
		ownerName = "NULL";
	else
		ownerName = owner->getFirstName();

	statusBox->addMenuItem("Owner: " + ownerName);

	int condition = (((float)vendor->getMaxCondition() - (float)vendor->getConditionDamage()) / (float)vendor->getMaxCondition()) * 100;
	statusBox->addMenuItem("Condition: " + String::valueOf(condition) + "%");

	float secsRemaining = 0.f;
	if( vendorData->getMaint() > 0 ){
		secsRemaining = (vendorData->getMaint() / vendorData->getMaintenanceRate())*3600;
	}

	statusBox->addMenuItem("Maintenance Pool: " +
			               String::valueOf(vendorData->getMaint()) +
			               "cr " + getTimeString( (uint32)secsRemaining ) );
	statusBox->addMenuItem("Maintenance Rate: " + String::valueOf((int)vendorData->getMaintenanceRate()) + " cr/hr");

	ManagedReference<AuctionManager*> auctionManager = server->getZoneServer()->getAuctionManager();
	if(auctionManager == NULL) {
		error("null auction manager");
		return;
	}
	ManagedReference<AuctionsMap*> auctionsMap = auctionManager->getAuctionMap();
	if(auctionsMap == NULL) {
		error("null auctionsMap");
		return;
	}

	String planet = vendor->getZone()->getZoneName();
	String region = "@planet_n:" + vendor->getZone()->getZoneName();


	ManagedReference<CityRegion*> regionObject = vendor->getCityRegion().get();
	if(regionObject != NULL)
		region = regionObject->getRegionName();

	TerminalListVector vendorList = auctionsMap->getVendorTerminalData(planet, region, vendor);

	uint32 itemsForSaleCount = 0;

	if(vendorList.size() > 0) {

		Reference<TerminalItemList*> list = vendorList.get(0);
		if (list != NULL) {
			ReadLocker rlocker(list);

			for (int j = 0; j < list->size(); ++j) {
				ManagedReference<AuctionItem*> item = list->get(j);
				if (item == NULL)
					continue;

				int itemSize = item->getSize();

				if (itemSize > 50)
					itemsForSaleCount += 50;
				else if (itemSize > 0)
					itemsForSaleCount += itemSize;
				else
					itemsForSaleCount++;
			}
		}
	}


	statusBox->addMenuItem("Number of Items For Sale: " + String::valueOf(itemsForSaleCount));

	if (vendorData->isVendorSearchEnabled())
		statusBox->addMenuItem("@player_structure:vendor_search_enabled");
	else
		statusBox->addMenuItem("@player_structure:vendor_search_disabled");

	if (!vendorData->isOnStrike() && !vendorData->isEmpty())
		statusBox->addMenuItem("\\#32CD32Vendor Operating Normally\\#.");

	player->getPlayerObject()->addSuiBox(statusBox);
	player->sendMessage(statusBox->generateMessage());

}

String VendorManager::getTimeString(uint32 timestamp) {

	if( timestamp == 0 ){
		return "";
	}

	String abbrvs[3] = { "minutes", "hours", "days" };

	int intervals[3] = { 60, 3600, 86400 };
	int values[3] = { 0, 0, 0 };

	StringBuffer str;

	for (int i = 2; i > -1; --i) {
		values[i] = floor((float) timestamp / intervals[i]);
		timestamp -= values[i] * intervals[i];

		if (values[i] > 0) {
			if (str.length() > 0){
				str << ", ";
			}

			str << values[i] << " " << abbrvs[i];
		}
	}

	return "(" + str.toString() + ")";
}

void VendorManager::promptDestroyVendor(CreatureObject* player, TangibleObject* vendor) {

	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if(vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	if (vendorData->getOwnerId() != player->getObjectID())
		return;

	SuiMessageBox* destroyBox = new SuiMessageBox(player, SuiWindowType::STRUCTURE_DESTROY_VENDOR_CONFIRM);
	destroyBox->setCallback(new DestroyVendorSuiCallback(player->getZoneServer()));
	destroyBox->setUsingObject(vendor);
	destroyBox->setPromptTitle("@player_structure:destroy_vendor_t"); //DestroyVendor?
	destroyBox->setPromptText("@player_structure:destroy_vendor_d");
	destroyBox->setOkButton(true, "@yes");
	destroyBox->setCancelButton(true, "@no");

	player->getPlayerObject()->addSuiBox(destroyBox);
	player->sendMessage(destroyBox->generateMessage());

}

void VendorManager::promptRenameVendorTo(CreatureObject* player, TangibleObject* vendor) {
	SuiInputBox* input = new SuiInputBox(player, SuiWindowType::STRUCTURE_NAME_VENDOR);
	input->setCallback(new RenameVendorSuiCallback(player->getZoneServer()));
	input->setUsingObject(vendor);
	input->setCancelButton(true, "@cancel");
	input->setPromptTitle("@player_structure:name_t");
	input->setPromptText("@player_structure:name_d");

	player->sendMessage(input->generateMessage());
	player->getPlayerObject()->addSuiBox(input);
}

void VendorManager::destroyVendor(TangibleObject* vendor) {
	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if (data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if (vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	ManagedReference<AuctionManager*> auctionManager = server->getZoneServer()->getAuctionManager();
	if (auctionManager == NULL) {
		error("null auctionManager when deleting vendor");
		return;
	}

	ManagedReference<AuctionsMap*> auctionsMap = auctionManager->getAuctionMap();
	if (auctionsMap == NULL) {
		error("null auctionsMap");
		return;
	}

	if (vendorData->isRegistered() && vendor->getZone() != NULL) {
		vendor->getZone()->unregisterObjectWithPlanetaryMap(vendor);
	}

	Locker locker(vendor);

	vendorData->cancelVendorCheckTask();

	vendor->destroyObjectFromWorld(true);
	vendor->destroyObjectFromDatabase(true);

	locker.release();

	auctionsMap->deleteTerminalItems(vendor);
}

void VendorManager::sendRegisterVendorTo(CreatureObject* player, TangibleObject* vendor) {

	SuiListBox* registerBox = new SuiListBox(player, SuiWindowType::STRUCTURE_VENDOR_REGISTER);
	registerBox->setCallback(new RegisterVendorSuiCallback(player->getZoneServer()));
	registerBox->setPromptTitle("@player_structure:vendor_mapcat_t");
	registerBox->setPromptText("@player_structure:vendor_mapcat_d");
	registerBox->setUsingObject(vendor);
	registerBox->setCancelButton(true, "@cancel");

	registerBox->addMenuItem("@player_structure:subcat_armor");
	registerBox->addMenuItem("@player_structure:subcat_clothing");
	registerBox->addMenuItem("@player_structure:subcat_components");
	registerBox->addMenuItem("@player_structure:subcat_droids");
	registerBox->addMenuItem("@player_structure:subcat_equipment");
	registerBox->addMenuItem("@player_structure:subcat_food");
	registerBox->addMenuItem("@player_structure:subcat_housing");
	registerBox->addMenuItem("@player_structure:subcat_medical");
	registerBox->addMenuItem("@player_structure:subcat_pets");
	registerBox->addMenuItem("@player_structure:subcat_resources");
	registerBox->addMenuItem("@player_structure:subcat_ships");
	registerBox->addMenuItem("@player_structure:subcat_tools");
	registerBox->addMenuItem("@player_structure:subcat_weapons");

	player->sendMessage(registerBox->generateMessage());
	player->getPlayerObject()->addSuiBox(registerBox);

}

void VendorManager::handleRegisterVendorCallback(CreatureObject* player, TangibleObject* vendor, const String& planetMapCategoryName) {

	Zone* zone = vendor->getZone();

	if (zone == NULL)
		return;

	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if(vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	if (vendorData->isEmpty()) {
		player->sendSystemMessage("You cannot register an empty vendor.");
		return;
	}

	Reference<PlanetMapCategory*> planetMapCategory = TemplateManager::instance()->getPlanetMapCategoryByName("vendor");
	Reference<PlanetMapCategory*> planetMapSubCategory = TemplateManager::instance()->getPlanetMapCategoryByName("vendor_" + planetMapCategoryName);

	if (planetMapCategory == NULL || planetMapSubCategory == NULL)
		return;

	Locker locker(vendor);

	vendor->setPlanetMapCategory(planetMapCategory);
	vendor->setPlanetMapSubCategory(planetMapSubCategory);
	vendorData->setRegistered(true);

	zone->registerObjectWithPlanetaryMap(vendor);

	player->sendSystemMessage("@player_structure:register_vendor_not");

}

void VendorManager::handleUnregisterVendor(CreatureObject* player, TangibleObject* vendor) {
	if (vendor == NULL)
		return;

	Zone* zone = vendor->getZone();

	if (zone == NULL)
		return;

	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if(vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	Locker locker(vendor);

	zone->unregisterObjectWithPlanetaryMap(vendor);

	vendorData->setRegistered(false);

	player->sendSystemMessage("@player_structure:unregister_vendor_not");
}

void VendorManager::handleRenameVendor(CreatureObject* player, TangibleObject* vendor, String& name) {
	if (vendor == NULL)
		return;

	Zone* zone = vendor->getZone();

	if (zone == NULL)
		return;

	if (!isValidVendorName(name)) {
		player->sendSystemMessage("@player_structure:obscene");
		promptRenameVendorTo(player, vendor);
		return;
	}


	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
		error("Vendor has no data component");
		return;
	}

	VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
	if(vendorData == NULL) {
		error("Vendor has wrong data component");
		return;
	}

	Locker _locker(vendor);
	vendor->setCustomObjectName("Vendor: " + name, true);


	if (vendorData->isRegistered()) {

		vendorData->setRegistered(false);
		zone->unregisterObjectWithPlanetaryMap(vendor);

		player->sendSystemMessage("@player_structure:vendor_rename_unreg");
	} else
		player->sendSystemMessage("@player_structure:vendor_rename");

}
