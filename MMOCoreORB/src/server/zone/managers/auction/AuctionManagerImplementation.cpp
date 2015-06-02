/*
 * AuctionManagerImplementation.cpp
 *
 *  Created on: 13/03/2010
 *      Author: victor
 */

#include "server/zone/managers/auction/AuctionManager.h"
#include "server/zone/managers/auction/AuctionsMap.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/auction/AuctionItem.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/zone/packets/auction/ItemSoldMessage.h"
#include "server/zone/packets/auction/CancelLiveAuctionResponseMessage.h"
#include "server/zone/packets/auction/AuctionQueryHeadersResponseMessage.h"
#include "server/zone/packets/auction/RetrieveAuctionItemResponseMessage.h"
#include "server/zone/packets/auction/BidAuctionResponseMessage.h"
#include "server/zone/packets/auction/IsVendorOwnerMessageCallback.h"
#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/chat/StringIdChatParameter.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/region/CityRegion.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/waypoint/WaypointObject.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "server/chat/ChatManager.h"
#include "CheckAuctionsTask.h"
#include "ExpireAuctionTask.h"
#include "server/zone/managers/vendor/VendorManager.h"
#include "server/zone/objects/tangible/components/vendor/VendorDataComponent.h"
#include "server/zone/objects/tangible/components/vendor/AuctionTerminalDataComponent.h"
#include "server/zone/objects/player/sessions/TradeSession.h"

void AuctionManagerImplementation::initialize() {

	Locker locker(_this.getReferenceUnsafeStaticCast());

	auctionMap = new AuctionsMap();

	ObjectDatabase* auctionDatabase = ObjectDatabaseManager::instance()->loadObjectDatabase("auctionitems", true);
	ObjectDatabaseManager::instance()->commitLocalTransaction();

	ObjectDatabaseIterator iterator(auctionDatabase);

	uint64 objectID = 0;

	Vector<ManagedReference<AuctionItem*> > orphanedBazaarItems;
	ManagedReference<SceneObject*> defaultBazaar = NULL;

	while (iterator.getNextKey(objectID)) {
		Reference<AuctionItem*> auctionItem = Core::getObjectBroker()->lookUp(objectID).castTo<AuctionItem*>();
		ObjectDatabaseManager::instance()->commitLocalTransaction();

		if(auctionItem == NULL) {
			error("unable to load auction item: " + String::valueOf(objectID));
			continue;
		}

		ManagedReference<SceneObject*> vendor = zoneServer->getObject(auctionItem->getVendorID());

		if(vendor == NULL || vendor->getZone() == NULL) {
			if(auctionItem->isOnBazaar()) {
				orphanedBazaarItems.add(auctionItem);
				continue;
			}

			if(vendor != NULL) {
				vendor->destroyObjectFromWorld(true);
				vendor->destroyObjectFromDatabase();
			}

			ObjectManager::instance()->destroyObjectFromDatabase(auctionItem->_getObjectID());
			warning("Auction Item's vendor is gone, deleting auction item: " + String::valueOf(auctionItem->_getObjectID()));
			continue;
		}

		if(vendor->isBazaarTerminal() && defaultBazaar == NULL)
			defaultBazaar = vendor;

		auctionMap->addItem(NULL, vendor, auctionItem);

		if(auctionItem->isOnBazaar() || auctionItem->getStatus() == AuctionItem::OFFERED)
			auctionMap->addToCommodityLimit(auctionItem);

		if(auctionItem->isAuction()) {
			Reference<Task*> newTask = new ExpireAuctionTask(_this.getReferenceUnsafeStaticCast(), auctionItem);
			newTask->schedule((auctionItem->getExpireTime() - time(0)) * 1000);

			Locker locker(&auctionEvents);
			auctionEvents.put(auctionItem->getAuctionedItemObjectID(), newTask);
		}
	}

	/// This is in case a bazaar is removed, it could move and item
	/// to a difference city, but at least it doesn't poof
	if(defaultBazaar != NULL) {

		for(int i = 0; i < orphanedBazaarItems.size(); ++i) {
			ManagedReference<AuctionItem*> auctionItem = orphanedBazaarItems.get(i);

			String vuid = getVendorUID(defaultBazaar);
			auctionMap->addItem(NULL, defaultBazaar, auctionItem);
			auctionItem->setVendorID(defaultBazaar->getObjectID());

			if(auctionItem->isAuction()) {
				Reference<Task*> newTask = new ExpireAuctionTask(_this.getReferenceUnsafeStaticCast(), auctionItem);
				newTask->schedule((auctionItem->getExpireTime() - time(0)) * 1000);

				Locker locker(&auctionEvents);
				auctionEvents.put(auctionItem->getAuctionedItemObjectID(), newTask);
			}
		}
	}

	for(int i = 0; i < pendingUIDUpdates.size(); ++i) {
		ManagedReference<SceneObject*> vendor = pendingUIDUpdates.elementAt(i).getKey();
		String uid = pendingUIDUpdates.get(vendor);
		String oldUID = pendingOldUIDUpdates.get(vendor);

		auctionMap->updateUID(vendor, oldUID, uid);
	}

	locker.release();

	checkAuctions();
	checkVendorItems();

	info("loaded auctionsMap of size: " + String::valueOf(auctionMap->getTotalItemCount()), true);

	marketEnabled = true;
}

void AuctionManagerImplementation::checkVendorItems() {

	TerminalListVector items = auctionMap->getVendorTerminalData("", "", 0);

	info("Checking " + String::valueOf(items.size()) + " vendor terminals", true);

	doAuctionMaint(&items);
}

void AuctionManagerImplementation::checkAuctions() {

	Reference<CheckAuctionsTask*> task = new CheckAuctionsTask(_this.getReferenceUnsafeStaticCast());
	task->schedule(CHECKEVERY * 60 * 1000);

	TerminalListVector items = auctionMap->getBazaarTerminalData("", "", 0);

	info("Checking " + String::valueOf(items.size()) + " bazaar terminals", true);

	doAuctionMaint(&items);
}

void AuctionManagerImplementation::doAuctionMaint(TerminalListVector* items) {

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;

	for (int i = 0; i < items->size(); ++i) {
		Reference<TerminalItemList*> list = items->get(i);
		if (list == NULL)
			continue;

		for (int j = 0; j < list->size(); ++j) {
			ManagedReference<AuctionItem*> item = list->get(j);
			if (item == NULL)
				continue;

			ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());
			uint64 objectId = item->getAuctionedItemObjectID();

			if(vendor == NULL || vendor->getZone() == NULL) {
				auctionMap->deleteItem(vendor, item);
				continue;
			}

			String vuid = getVendorUID(vendor);

			Locker locker(item);

			if (item->getExpireTime() <= currentTime) {
				if (item->getStatus() == AuctionItem::EXPIRED) {
					expireSale(item);
				}
			}

			if (item->getStatus() == AuctionItem::RETRIEVED) {

				auctionMap->deleteItem(vendor, item);

			}
		}
	}
}

void AuctionManagerImplementation::addSaleItem(CreatureObject* player, uint64 objectid, SceneObject* vendor, const UnicodeString& description, int price, uint32 duration, bool auction, bool premium) {

	if (vendor == NULL || (!vendor->isVendor() && !vendor->isBazaarTerminal())) {
		error("terminal is not a valid vendor object");
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::VENDORNOTWORKING);
		player->sendMessage(soldMessage);
		return;
	}

	if (player->isDead() || player->isIncapacitated()) {
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::UNKNOWNERROR);
		player->sendMessage(soldMessage);
		return;
	}

	ManagedReference<TradeSession*> tradeContainer = player->getActiveSession(SessionFacadeType::TRADE).castTo<TradeSession*>();

	if (tradeContainer != NULL) {
		zoneServer->getPlayerManager()->handleAbortTradeMessage(player);
	}

	ManagedReference<AuctionItem*> oldItem = auctionMap->getItem(objectid);
	ManagedReference<SceneObject*> objectToSell = zoneServer->getObject(objectid);
	String vendorUID = getVendorUID(vendor);
	bool stockroomSale = false;

	if (objectToSell == NULL) {
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::INVALIDITEM);
		player->sendMessage(soldMessage);
		return;
	}

	if(oldItem == NULL) {
		if (objectToSell == NULL || !objectToSell->isASubChildOf(player) || objectToSell->isNoTrade()) {
			if(objectToSell != NULL && !objectToSell->isNoTrade())
				error("trying to add invalid object");
			ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::INVALIDITEM);
			player->sendMessage(soldMessage);
			return;
		}
	} else {

		if(oldItem->getStatus() == AuctionItem::FORSALE) {
			ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::ALREADYFORSALE);
			player->sendMessage(soldMessage);
			return;
		}

		/// Is it being sold from the stockroom
		if (oldItem->getOwnerID() != player->getObjectID()) {
			error("trying to add invalid object");
			ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::INVALIDITEM);
			player->sendMessage(soldMessage);
			return;
		}

		stockroomSale = true;

		ManagedReference<SceneObject*> oldVendor = zoneServer->getObject(oldItem->getVendorID());

		if (oldVendor != NULL && oldVendor->isVendor())
			vendor = oldVendor;
	}

	ManagedReference<Zone*> zone = vendor->getZone();
	if (zone == NULL) {
		error("null vendor zone");
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::UNKNOWNERROR);
		player->sendMessage(soldMessage);
		return;
	}


	int res = checkSaleItem(player, objectToSell, vendor, price, premium, stockroomSale);

	if (res != 0) {
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, res);
		player->sendMessage(soldMessage);
		return;
	}

	if (oldItem != NULL)
		auctionMap->deleteItem(vendor, oldItem);

	if(auctionMap->containsItem(objectToSell->getObjectID())) {
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::ALREADYFORSALE);
		player->sendMessage(soldMessage);
		return;
	}

	// add city tax to the price
	if(vendor->getCityRegion() != NULL) {
		price *= (1.0f + (vendor->getCityRegion().get()->getSalesTax() / 100.0f));
	}

	ManagedReference<AuctionItem*> item = createVendorItem(player, objectToSell.get(), vendor, description, price, duration, auction, premium);

	if(item == NULL) {
		error("Unable to create vendor item");
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, ItemSoldMessage::UNKNOWNERROR);
		player->sendMessage(soldMessage);
		return;
	}

	Locker locker(item);

	int result = auctionMap->addItem(player, vendor, item);

	if(result != ItemSoldMessage::SUCCESS) {
		ItemSoldMessage* soldMessage = new ItemSoldMessage(objectid, result);
		player->sendMessage(soldMessage);
		auctionMap->removeFromCommodityLimit(item);
		return;
	}

	Locker objectToSellLocker(objectToSell);

	objectToSell->destroyObjectFromWorld(true);

	objectToSellLocker.release();

	if (vendor->isBazaarTerminal()) {
		StringIdChatParameter str("@base_player:sale_fee"); // The fee for your listing is %DI credits.

		float costReduction = 1;
		if(player->hasSkill("crafting_merchant_sales_01"))
				costReduction = .80f;
		if(player->hasSkill("crafting_merchant_sales_03"))
				costReduction = .60f;

		if (item->isPremiumAuction()) {
			player->subtractBankCredits(costReduction * (SALESFEE * 5));
			str.setDI(costReduction * (SALESFEE * 5));

		} else {
			player->subtractBankCredits(costReduction * SALESFEE);
			str.setDI(costReduction * SALESFEE);
		}

		player->sendSystemMessage(str);
	}

	if (item->getStatus() == AuctionItem::OFFERED) {

		VendorDataComponent* vendorData = NULL;
		DataObjectComponentReference* data = vendor->getDataObjectComponent();
		if(data != NULL && data->get() != NULL && data->get()->isVendorData())
			vendorData = cast<VendorDataComponent*>(data->get());

		if(vendorData != NULL) {
			ManagedReference<SceneObject*> strongRef = zoneServer->getObject(vendorData->getOwnerId());

			if (strongRef != NULL && strongRef->isPlayerCreature()) {
				ManagedReference<CreatureObject*> strongOwnerRef = cast<CreatureObject*>(strongRef.get());

				if(strongOwnerRef->isOnline()) {
					strongOwnerRef->sendSystemMessage(player->getFirstName() + " has offered an item to " + vendor->getDisplayedName());
				}
			}
		}
	}

	item->setPersistent(1);

	if(item->isAuction()) {
		Reference<Task*> newTask = new ExpireAuctionTask(_this.getReferenceUnsafeStaticCast(), item);
		newTask->schedule((item->getExpireTime() - time(0)) * 1000);

		Locker locker(&auctionEvents);
		auctionEvents.put(item->getAuctionedItemObjectID(), newTask);
	}

	BaseMessage* msg = new ItemSoldMessage(objectid, 0);
	player->sendMessage(msg);

}

String AuctionManagerImplementation::getVendorUID(SceneObject* vendor) {

	//planet.region.vendorname.oid#x,y

	if(vendor->getZone() == NULL) {
		error("no zone for our poor vendor");
		return "nozone.nozone.sadpandavendor." + String::valueOf(vendor->getObjectID()) + "#0,0";
	}

	String uid = "error.error.errorvendor." + String::valueOf(vendor->getObjectID()) + "#0,0";

	AuctionTerminalDataComponent* terminalData = NULL;
	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data != NULL && data->get() != NULL && data->get()->isAuctionTerminalData())
		terminalData = cast<AuctionTerminalDataComponent*>(data->get());

	if(terminalData != NULL)
		uid = terminalData->getUID();

	return uid;
}

int AuctionManagerImplementation::checkSaleItem(CreatureObject* player, SceneObject* object, SceneObject* vendor, int price, bool premium, bool stockroomSale) {

	if(vendor == NULL) {
		error("NULL Vendor");
		return ItemSoldMessage::UNKNOWNERROR;
	}

	if (price < 1)
		return ItemSoldMessage::INVALIDSALEPRICE;

	if (player->getPlayerObject()->getVendorCount() > player->getSkillMod("manage_vendor"))
		return ItemSoldMessage::TOOMANYITEMS;

	if(vendor->isVendor()) {

		VendorDataComponent* vendorData = NULL;
		DataObjectComponentReference* data = vendor->getDataObjectComponent();
		if(data != NULL && data->get() != NULL && data->get()->isVendorData())
			vendorData = cast<VendorDataComponent*>(data->get());

		if (player->getObjectID() == vendorData->getOwnerId()) {
			if (stockroomSale) {
				if (auctionMap->getPlayerItemCount(player) > player->getSkillMod("vendor_item_limit"))
					return ItemSoldMessage::TOOMANYITEMS;
			} else {
				if ((auctionMap->getPlayerItemCount(player) + object->getSizeOnVendorRecursive()) > player->getSkillMod("vendor_item_limit"))
					return ItemSoldMessage::TOOMANYITEMS;
			}
		} else {
			if (auctionMap->getCommodityCount(player) >= MAXSALES)
				return ItemSoldMessage::TOOMANYITEMS;
		}

		if (price > MAXVENDORPRICE)
			return ItemSoldMessage::INVALIDSALEPRICE;
	}

	if(vendor->isBazaarTerminal()) {
		if (auctionMap->getCommodityCount(player) >= MAXSALES)
			return ItemSoldMessage::TOOMANYITEMS;

		if (price > MAXBAZAARPRICE)
			return ItemSoldMessage::INVALIDSALEPRICE;

		if (player->getBankCredits() < SALESFEE)
			return ItemSoldMessage::NOTENOUGHCREDITS;

		if (premium && player->getBankCredits() < SALESFEE * 5)
			return ItemSoldMessage::NOTENOUGHCREDITS;

	}

	if (object->isIntangibleObject() && !object->isManufactureSchematic())
		return ItemSoldMessage::INVALIDITEM;

	for (int i = 0; i < object->getArrangementDescriptorSize(); ++i) {
		const Vector<String>* descriptors = object->getArrangementDescriptor(i);

		for (int j = 0; j < descriptors->size(); ++j) {
			const String& descriptor = descriptors->get(j);

			if (descriptor == "inventory" || descriptor == "datapad" || descriptor == "default_weapon"
					|| descriptor == "mission_bag" || descriptor == "ghost" || descriptor == "bank" || descriptor == "hair")
				return ItemSoldMessage::INVALIDITEM;
		}
	}

	return 0;
}

AuctionItem* AuctionManagerImplementation::createVendorItem(CreatureObject* player, SceneObject* objectToSell, SceneObject* vendor, const UnicodeString& description, int price, unsigned int duration, bool auction, bool premium) {

	Zone* zone = vendor->getZone();

	if (zone == NULL)
		return NULL;

	uint64 vendorExpire = time(0) + AuctionManager::VENDOREXPIREPERIOD;
	uint64 commodityExpire = time(0) + AuctionManager::COMMODITYEXPIREPERIOD;

	String playername = player->getFirstName().toLowerCase();
	String descr = description.toString();
	String planetStr = zone->getZoneName();

	AuctionItem* item  = new AuctionItem(objectToSell->getObjectID());

	ManagedReference<CityRegion*> cityRegion = vendor->getCityRegion();
	String region = "@planet_n:" + planetStr;

	if (cityRegion != NULL)
		region = cityRegion->getRegionName();

	String name = objectToSell->getDisplayedName();

	Locker locker(item);

	item->setVendorUID(getVendorUID(vendor));
	item->setOnBazaar(vendor->isBazaarTerminal());

	if (premium)
		item->setAuctionPremium();

	item->setVendorID(vendor->getObjectID());
	item->setItemName(name);
	item->setItemDescription(description.toString());
	item->setItemType(objectToSell->getClientGameObjectType());
	item->setPrice(price);
	item->setAuction(auction);
	item->setStatus(AuctionItem::FORSALE);
	item->setBuyerID(0);
	item->setBidderName("");
	item->setSize(objectToSell->getSizeOnVendorRecursive());

	VendorDataComponent* vendorData = NULL;
	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data != NULL && data->get() != NULL && data->get()->isVendorData())
		vendorData = cast<VendorDataComponent*>(data->get());

	// Someone elses Vendor (making this sell item an offer)
	if (!vendor->isBazaarTerminal()) {
		if(vendorData == NULL)
			return NULL;

		if(vendorData->getOwnershipRightsOf(player) == 1) {
			item->setStatus(AuctionItem::OFFERED);
			item->setOfferToID(vendorData->getOwnerId());
			item->setExpireTime(commodityExpire);
		} else {
			item->setExpireTime(vendorExpire);

			if(auctionMap->getVendorItemCount(vendor, true) == 0)
				sendVendorUpdateMail(vendor, false);
		}
	} else {
		item->setExpireTime(commodityExpire);
	}

	updateAuctionOwner(item, player);
	ObjectManager::instance()->persistObject(item, 0, "auctionitems");


	return item;
}

int AuctionManagerImplementation::checkBidAuction(CreatureObject* player, AuctionItem* item, int price1, int price2) {
	if ((price1 > MAXBAZAARPRICE || price2 > MAXBAZAARPRICE) && item->isOnBazaar()) {
		return BidAuctionResponseMessage::PRICEOVERFLOW;
	}

	if (price1 < 1 || price2 < 1 || price1 < item->getPrice()) {
		return BidAuctionResponseMessage::INVALIDPRICE;
	}

	if (player->getBankCredits() < price1) { // Credit Check
		return BidAuctionResponseMessage::NOTENOUGHCREDITS;
	}

	return 0;
}

void AuctionManagerImplementation::doInstantBuy(CreatureObject* player, AuctionItem* item) {
	ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());

	if (vendor == NULL)
		return;

	int tax = 0;
	ManagedReference<CityRegion*> city = NULL;
	String vendorPlanetName("@planet_n:" + vendor->getZone()->getZoneName());
	String vendorRegionName = vendorPlanetName;

	if( vendor->getCityRegion() != NULL){
		city = vendor->getCityRegion().get();
		tax = item->getPrice() - ( item->getPrice() / ( 1.0f + (city->getSalesTax() / 100.f)));
		vendorRegionName = city->getRegionName();
	}
	String playername = player->getFirstName().toLowerCase();

	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();
	ManagedReference<PlayerManager*> pman = zoneServer->getPlayerManager();
	ManagedReference<CreatureObject*> seller = pman->getPlayer(item->getOwnerName());

	String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";
	String sellerName = item->getOwnerName();

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;
	uint64 availableTime = 0;


	Locker locker(item);

	if(item->isOnBazaar() || item->getStatus() == AuctionItem::OFFERED)
		availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;
	else
		availableTime = currentTime + AuctionManager::VENDOREXPIREPERIOD;

	updateAuctionOwner(item, player);

	item->setStatus(AuctionItem::SOLD);
	item->setExpireTime(availableTime);
	item->setBuyerID(player->getObjectID());
	item->setBidderName(playername);
	item->clearAuctionWithdraw();

	player->subtractBankCredits(item->getPrice());

	BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), 0);
	player->sendMessage(msg);

	// Waypoint to Vendor / bazaar
	float waypointX = vendor->getWorldPositionX();
	float waypointY = vendor->getWorldPositionY();

	WaypointChatParameter waypointParam;
	waypointParam.set(vendor->getDisplayedName(), waypointX, 0, waypointY, vendor->getPlanetCRC());

	String itemName = removeColorCodes(item->getItemName());

	if (!item->isOnBazaar()) {
		//Setup the mail to the vendor owner

		PlayerManager* pman = zoneServer->getPlayerManager();
		/*ManagedReference<CreatureObject*> seller = pman->getPlayer(item->getOwnerName());

		Locker _locker(seller);*/

		StringIdChatParameterVector sellerBodyVector;
		WaypointChatParameterVector sellerWaypointVector;

		UnicodeString sellerSubject("@auction:subject_vendor_seller"); // Vendor Sale Complete
		StringIdChatParameter sellerBodySale("@auction:seller_success_vendor"); // %TU has sold %TO to %TT for %DI credits.
		sellerBodySale.setTU(vendor->getDisplayedName());
		sellerBodySale.setTO(itemName);
		sellerBodySale.setTT(item->getBidderName());
		sellerBodySale.setDI(item->getPrice());

		StringIdChatParameter sellerBodyLoc("@auction:seller_success_location"); // The sale took place at %TT, on %TO.
		sellerBodyLoc.setTO(vendorPlanetName);
		sellerBodyLoc.setTT(vendorRegionName);

		sellerBodyVector.add(sellerBodySale);
		sellerBodyVector.add(sellerBodyLoc);

		sellerWaypointVector.add(waypointParam);

		//Setup the mail to the buyer
		/*ManagedReference<CreatureObject*> buyer = pman->getPlayer(item->getBidderName());

		Locker _locker2(buyer);*/

		StringIdChatParameterVector buyerBodyVector;
		WaypointChatParameterVector buyerWaypointVector;

		UnicodeString buyerSubject("@auction:subject_vendor_buyer"); // Vendor Item Purchased
		StringIdChatParameter buyerBodySale("@auction:buyer_success"); // You have won the auction of "%TO" from "%TT" for %DI credits. See the attached waypoint for location.
		buyerBodySale.setTO(itemName);
		buyerBodySale.setTT(sellerName);
		buyerBodySale.setDI(item->getPrice());

		StringIdChatParameter buyerBodyLoc("@auction:buyer_success_location"); // The sale took place at %TT, on %TO.
		buyerBodyLoc.setTO(vendorPlanetName);
		buyerBodyLoc.setTT(vendorRegionName);

		buyerBodyVector.add(buyerBodySale);
		buyerBodyVector.add(buyerBodyLoc);

		buyerWaypointVector.add(waypointParam);

		//Send the Mail
		locker.release();
		UnicodeString blankBody;
		cman->sendMail(sender, sellerSubject, blankBody, sellerName, &sellerBodyVector, &sellerWaypointVector);
		cman->sendMail(sender, buyerSubject, blankBody, item->getBidderName(), &buyerBodyVector, &buyerWaypointVector);

		if(auctionMap->getVendorItemCount(vendor, true) == 0)
			sendVendorUpdateMail(vendor, true);

	} else {

		StringIdChatParameterVector sellerBodyVector;
		WaypointChatParameterVector sellerWaypointVector;

		// Setup the mail to the seller
		UnicodeString sellerSubject("@auction:subject_instant_seller"); // Instant Sale Complete
		StringIdChatParameter sellerBodySale("@auction:seller_success"); // Your auction of %TO has been sold to %TT for %DI credits
		sellerBodySale.setTO(itemName);
		sellerBodySale.setTT(item->getBidderName());
		sellerBodySale.setDI(item->getPrice());

		StringIdChatParameter sellerBodyLoc("@auction:seller_success_location"); // The sale took place at %TT, on %TO.
		sellerBodyLoc.setTO(vendorPlanetName);
		sellerBodyLoc.setTT(vendorRegionName);

		sellerBodyVector.add(sellerBodySale);
		sellerBodyVector.add(sellerBodyLoc);

		sellerWaypointVector.add(waypointParam);

		// Setup the mail to the buyer
		StringIdChatParameterVector buyerBodyVector;
		WaypointChatParameterVector buyerWaypointVector;

		UnicodeString buyerSubject("@auction:subject_instant_buyer"); // Instant Sale Item Purchased
		StringIdChatParameter buyerBodySale("@auction:buyer_success"); // You have won the auction of "%TO" from "%TT" for %DI credits. See the attached waypoint for location.
		buyerBodySale.setTO(itemName);
		buyerBodySale.setTT(sellerName);
		buyerBodySale.setDI(item->getPrice());

		StringIdChatParameter buyerBodyLoc("@auction:buyer_success_location"); // The sale took place at %TT, on %TO.
		buyerBodyLoc.setTO(vendorPlanetName);
		buyerBodyLoc.setTT(vendorRegionName);

		buyerBodyVector.add(buyerBodySale);
		buyerBodyVector.add(buyerBodyLoc);

		locker.release();

		buyerWaypointVector.add(waypointParam);

		//Send the Mail
		UnicodeString blankBody;
		cman->sendMail(sender, sellerSubject, blankBody, sellerName, &sellerBodyVector, &sellerWaypointVector);
		cman->sendMail(sender, buyerSubject, blankBody, item->getBidderName(), &buyerBodyVector, &buyerWaypointVector);

	}

	if (seller == NULL) {
		error("seller null for name " + item->getOwnerName());
		return;
	}

	locker.release();

	Locker slocker(seller);
	seller->addBankCredits(item->getPrice() - tax);
	slocker.release();


	if(city != NULL && !city->isClientRegion() && tax){
		Locker clock(city);
		city->addToCityTreasury(tax);
	}

}

void AuctionManagerImplementation::doAuctionBid(CreatureObject* player, AuctionItem* item, int price1, int proxyBid) {

	if(price1 <= item->getPrice() || proxyBid <= price1) {
		BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), 1);
		player->sendMessage(msg);
		return;
	}

	String playername = player->getFirstName().toLowerCase();

	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();
	ManagedReference<PlayerManager*> pman = zoneServer->getPlayerManager();
	// don't allow owner bid on the item.  don't allow old auction info
	// send auctioner invalid message
	if (playername == item->getOwnerName() || price1 <= item->getPrice() || proxyBid <= price1) {
		BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), 1);
		player->sendMessage(msg);
		return;
	}

	ManagedReference<CreatureObject*> priorBidder = pman->getPlayer(item->getBidderName());

	/// Use previous proxy
	if(priorBidder != NULL && proxyBid < item->getProxy()) {
		Locker locker(item);
		Locker plocker(priorBidder);

		int increase = price1 - item->getPrice();

		int fullPrice = proxyBid + increase - item->getPrice();

		//TODO: prior didnt have enough money -> assert.. fix properly
		if (priorBidder->getBankCredits() < fullPrice) {
			BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), BidAuctionResponseMessage::NOTENOUGHCREDITS);
			player->sendMessage(msg);

			return;
		}

		priorBidder->subtractBankCredits(fullPrice);
		item->setPrice(proxyBid + increase);
		BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), BidAuctionResponseMessage::SUCCEDED);
		player->sendMessage(msg);

		if(priorBidder != player) {
			StringIdChatParameter body("@auction:bidder_outbid");
			body.setTO(item->getItemName());
			priorBidder->sendSystemMessage(body);
		}

		return;
	}

	Locker locker(item);
	Locker plocker(player);

	if (player->getBankCredits() < price1) {
		BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), BidAuctionResponseMessage::NOTENOUGHCREDITS);
		player->sendMessage(msg);

		return;
	}

	item->setProxy(proxyBid);

	// send prior bidder their money back
	if (item->getBidderName().length() > 0) {

		String itemName = removeColorCodes(item->getItemName());

		StringIdChatParameter bidderBody("@auction:bidder_outbid"); // You have been outbid on the "%TO" that you were bidding on.
		bidderBody.setTO(itemName);

		if (priorBidder != NULL) {
			Locker clocker(priorBidder, player);

			if(priorBidder != player)
				priorBidder->sendSystemMessage(bidderBody);
			priorBidder->addBankCredits(item->getPrice());
		}

		// mail prior bidder with outcome
		UnicodeString bidderSubject("@auction:subject_auction_outbid"); // Auction Outbid


		item->setPrice(price1);
		item->setBuyerID(player->getObjectID());
		item->setBidderName(playername);

		// take money from high bidder
		player->subtractBankCredits(item->getPrice());

		plocker.release();
		locker.release();

		String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";
		cman->sendMail(sender, bidderSubject, bidderBody, item->getBidderName());

		// no prior bidder, just take the money
	} else {
		item->setPrice(price1);
		item->setBuyerID(player->getObjectID());
		item->setBidderName(playername);

		player->subtractBankCredits(item->getPrice());
	}

	BaseMessage* msg = new BidAuctionResponseMessage(item->getAuctionedItemObjectID(), 0);
	player->sendMessage(msg);
}

void AuctionManagerImplementation::buyItem(CreatureObject* player, uint64 objectid, int price1, int price2) {
	ManagedReference<AuctionItem*> item = auctionMap->getItem(objectid);

	if (item == NULL) {
		BaseMessage* msg = new BidAuctionResponseMessage(objectid, BidAuctionResponseMessage::INVALIDITEM);
		player->sendMessage(msg);
		return;
	}

	ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());

	if (vendor == NULL || item->getStatus() == AuctionItem::SOLD) {
		BaseMessage* msg = new BidAuctionResponseMessage(objectid, BidAuctionResponseMessage::INVALIDITEM);
		player->sendMessage(msg);
		return;
	}

	if (item->getOwnerID() == player->getObjectID()) {
		BaseMessage* msg = new BidAuctionResponseMessage(objectid, BidAuctionResponseMessage::PURCHASEFAILED);
		player->sendMessage(msg);
		return;
	}

	ManagedReference<CityRegion*> city = vendor->getCityRegion();

	int totalPrice = item->getPrice();

	int res = checkBidAuction(player, item, totalPrice , price2);

	if (res != 0) {
		BaseMessage* msg = new BidAuctionResponseMessage(objectid, res);
		player->sendMessage(msg);
		return;
	}


	if (!item->isAuction()) { // Instant buy
		doInstantBuy(player, item);
	} else { // For Auction Bids
		doAuctionBid(player, item, price1, price2);
	}

}


int AuctionManagerImplementation::checkRetrieve(CreatureObject* player, uint64 objectIdToRetrieve, SceneObject* vendor) {
    // Check both Bazaar and Vendors

	if (!auctionMap->containsItem(objectIdToRetrieve))
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;

	ManagedReference<SceneObject*> saleItem = zoneServer->getObject(objectIdToRetrieve);
	
	if (saleItem == NULL) {
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;
	}

	ManagedReference<AuctionItem*> item = auctionMap->getItem(objectIdToRetrieve);
	if (item == NULL || item->getStatus() == AuctionItem::RETRIEVED) {
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;
	}

	if(item->isAuction() && item->getStatus() == AuctionItem::FORSALE)
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;

	int size = item->getSize();
	if(saleItem->isIntangibleObject()) {
		ManagedReference<SceneObject*> datapad = player->getSlottedObject("datapad");
		if (datapad->getCountableObjectsRecursive() + size >= datapad->getContainerVolumeLimit())
			return RetrieveAuctionItemResponseMessage::FULLINVENTORY;
	} else {
		ManagedReference<SceneObject*> inventory = player->getSlottedObject("inventory");

		if (inventory->getCountableObjectsRecursive() + size >= inventory->getContainerVolumeLimit())
			return RetrieveAuctionItemResponseMessage::FULLINVENTORY;
	}

	/*
	if(item->getStatus() == AuctionItem::SOLD && player->getObjectID() == item->getOwnerID()) {
		item->setStatus(AuctionItem::EXPIRED);
		return RetrieveAuctionItemResponseMessage::DONTRETRIEVE;
	}
	*/

	String playername = player->getFirstName();

	// only the owner can yank his own auction off the vendor
	if (item->getStatus() != AuctionItem::SOLD && (player->getObjectID() != item->getOwnerID()))
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;

	// the bidder is the only one who can get his auction after expiration
	if (item->getStatus() == AuctionItem::SOLD && item->getBuyerID() != player->getObjectID()) {
		player->sendSystemMessage(item->getBidderName() + " bought this, " + player->getFirstName() + " trying to retrieve");
		error(item->getBidderName() + " bought this, " + player->getFirstName() + " trying to retrieve");
		return RetrieveAuctionItemResponseMessage::NOTALLOWED;
	}

	if(vendor->isVendor() && !vendor->isInRange(player, 8.0f))
		return RetrieveAuctionItemResponseMessage::TOOFAR;


	if (vendor->isBazaarTerminal()) {
		ManagedReference<CityRegion*> region = vendor->getCityRegion();

		String location = vendor->getZone()->getZoneName() + ".";

		if (region != NULL) {
			location += region->getRegionName();
			//String region = terminal->getBazaarRegion();

			if (!item->getVendorUID().beginsWith(location)) {
				return RetrieveAuctionItemResponseMessage::TOOFAR;
			}
		} else {
			StringBuffer msg;
			msg << "null area for bazaar terminal at" << vendor->getPositionX() << " " << vendor->getPositionY() << " zone " << vendor->getZone()->getZoneName();
			error(msg);
		}

		if (!item->getVendorUID().beginsWith(location)) {
			return RetrieveAuctionItemResponseMessage::TOOFAR;
		}
	}

	return 0;
}

void AuctionManagerImplementation::refundAuction(AuctionItem* item) {
	ManagedReference<PlayerManager*> pman = zoneServer->getPlayerManager();
	ManagedReference<CreatureObject*> bidder = pman->getPlayer(item->getBidderName());
	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();

	String itemName = removeColorCodes(item->getItemName());

	// send the player a mail and system message
	UnicodeString buyerSubject("@auction:subject_auction_cancelled"); // Auction Cancelled
	StringIdChatParameter buyerBody("auction", "buyer_canceled"); // The auction of "%TO" that you were bidding on has been canceled by %TT.
	buyerBody.setTO(itemName);
	buyerBody.setTT(item->getOwnerName());

	if (bidder != NULL) {
		int bankCredits = bidder->getBankCredits();
		bidder->setBankCredits(bankCredits + item->getPrice());
		bidder->sendSystemMessage(buyerBody);
	}
	String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";
	cman->sendMail(sender, buyerSubject, buyerBody, item->getBidderName());
}

void AuctionManagerImplementation::retrieveItem(CreatureObject* player, uint64 objectid, uint64 vendorID) {

	ManagedReference<SceneObject*> vendor = zoneServer->getObject(vendorID);
	RetrieveAuctionItemResponseMessage* msg = NULL;

	// check for valid vendor terminal
	if ((vendor == NULL || !vendor->isVendor()) && !vendor->isBazaarTerminal()) {
		msg = new RetrieveAuctionItemResponseMessage(objectid, RetrieveAuctionItemResponseMessage::NOTALLOWED);
		player->sendMessage(msg);
		return;
	}

	int res = checkRetrieve(player, objectid, vendor);

	if (res != 0) {
		if(res != RetrieveAuctionItemResponseMessage::TOOFAR) {
			if(res == RetrieveAuctionItemResponseMessage::DONTRETRIEVE)
				res = 0;
			msg = new RetrieveAuctionItemResponseMessage(objectid, res);
			player->sendMessage(msg);
		}
		return;
	}

	ManagedReference<AuctionItem*> item = auctionMap->getItem(objectid);
	if (item == NULL) {
		error("NULL item in retrieveItem()");
		return;
	}

	ManagedReference<SceneObject*> objectToRetrieve = zoneServer->getObject(objectid);

	if (objectToRetrieve == NULL) {
		error("null objectToRetrieve in retrieveItem()");
		msg = new RetrieveAuctionItemResponseMessage(objectid, 0);
		player->sendMessage(msg);
		return;
	}

	Locker locker(item);
	Locker plocker(player);

	ManagedReference<SceneObject*> destination = NULL;

	if(objectToRetrieve->isIntangibleObject())
		destination = player->getSlottedObject("datapad");
	else
		destination = player->getSlottedObject("inventory");

	if(destination->transferObject(objectToRetrieve, -1, false)) {
		destination->broadcastObject(objectToRetrieve, true);

		item->setStatus(AuctionItem::RETRIEVED);

		String vuid = getVendorUID(vendor);

		auctionMap->deleteItem(vendor, item);

		msg = new RetrieveAuctionItemResponseMessage(objectid, 0);
		player->sendMessage(msg);
	} else {
		msg = new RetrieveAuctionItemResponseMessage(objectid, RetrieveAuctionItemResponseMessage::NOTALLOWED);
		player->sendMessage(msg);
	}
}

AuctionQueryHeadersResponseMessage* AuctionManagerImplementation::fillAuctionQueryHeadersResponseMessage(CreatureObject* player, SceneObject* vendor, TerminalListVector* terminalList, int screen, uint32 category, int clientcounter, int offset) {
	AuctionQueryHeadersResponseMessage* reply = new AuctionQueryHeadersResponseMessage(screen, clientcounter, player);

	String pname = player->getFirstName().toLowerCase();
	uint32 now = time(0);
	int displaying = 0;
	/*System::out << "Screen =" + String::valueOf(screen) << endl;
	System::out << "Category =" + String::valueOf(category) << endl;
	System::out << "VendorItemSize =" + String::valueOf(auctionMap->getVendorItemCount()) << endl;
	System::out << "AuctionItemSize =" + String::valueOf(auctionMap->getAuctionCount()) << endl;
	System::out << "______________________________" << endl;*/

	for (int j = 0; (j < terminalList->size()) && (displaying < (offset + 100)); ++j) {

		Reference<TerminalItemList*> items = terminalList->get(j);

		if(items == NULL)
			continue;

		/// Exclude non-searchable vendor Items
		if(vendor->isBazaarTerminal() && screen == 7 && !items->isSearchable())
			continue;

		try {
			items->rlock();

			for (int i = 0; (i < items->size()) && (displaying < (offset + 100)); i++) {
				ManagedReference<AuctionItem*> item = items->get(i);

				if(item == NULL)
					continue;

				if(!item->isAuction() && item->getExpireTime() <= now) {
					expireSale(item);
					continue;
				}

				switch(screen) {
				case 7: // Vendor search Bazaar && Vendor

					if(vendor->isVendor() && item->getVendorID() != vendor->getObjectID()) {
						if(item->getOwnerID() != player->getObjectID())
							continue;
					}
				case 2: // All Auctions (Bazaar)
					if (item->getStatus() == AuctionItem::FORSALE) {
						if (category & 255) { // Searching a sub category
							if (item->getItemType() == category) {
								// copy the item

								if (displaying >= offset)
									reply->addItemToList(items->get(i));

								displaying++;
							}
						} else if (item->getItemType() & category) {
							if (displaying >= offset)
								reply->addItemToList(items->get(i));

							displaying++;

						} else if ((category == 8192) && (item->getItemType() < 256)) {
							if (displaying >= offset)
								reply->addItemToList(items->get(i));

							displaying++;
						} else if (category == 0 && vendor->isVendor()) { // Searching all items
							if (displaying >= offset)
								reply->addItemToList(item);

							displaying++;
						}
					}
					break;
				case 3: // My auctions/sales
					if (item->getStatus() == AuctionItem::FORSALE && (item->getOwnerID() == player->getObjectID())) {
						reply->addItemToList(item);
					}
					break;
				case 4: // My Bids
					if (item->isAuction() && item->getStatus() == AuctionItem::FORSALE && (item->getBidderName() == pname)) {
						reply->addItemToList(item);
					}
					break;
				case 5: // Retrieve items screen
					if ((item->getStatus() == AuctionItem::SOLD && item->getBuyerID() == player->getObjectID()) ||
							(item->getStatus() == AuctionItem::EXPIRED && item->getOwnerID() == player->getObjectID())) {
						reply->addItemToList(item);
					}
					break;
				case 6: // Offers to Vendor (vendor owner)
					if (item->getStatus() == AuctionItem::OFFERED && item->getOfferToID() == player->getObjectID()) {
						reply->addItemToList(item);
					}
					break;
				case 8: // Stockroom
					if ((item->getStatus() == AuctionItem::EXPIRED && item->getOwnerID() == player->getObjectID()) ||
							(item->getStatus() == AuctionItem::SOLD && item->getBuyerID() == player->getObjectID()))
						reply->addItemToList(item);
					break;
				case 9: // Offers to vendor (browsing player)
					if (item->getStatus() == AuctionItem::OFFERED && item->getOwnerID() == player->getObjectID()) {
						reply->addItemToList(item);
					}
					break;
				}
			}
			items->runlock();
		} catch(Exception& e) {
			error(e.getMessage());
			items->runlock();
		}
	}

	if (displaying == (offset + 100))
		reply->createMessage(offset, true);
	else
		reply->createMessage(offset);

	return reply;
}

void AuctionManagerImplementation::getData(CreatureObject* player, int extent, uint64 vendorObjectID, int screen, unsigned int category, int clientcounter, int offset) {
	ManagedReference<TangibleObject*> vendorInUse = (zoneServer->getObject(vendorObjectID)).castTo<TangibleObject*>();

	if (vendorInUse == NULL || (!vendorInUse->isVendor() && !vendorInUse->isBazaarTerminal())) {
		error("null vendor in getData()");
		return;
	}

	ManagedReference<SceneObject*> parent = vendorInUse->getRootParent();

	if (parent != NULL && parent != player->getRootParent().get())
		return;

	if(player->getZone() == NULL) {
		error("player not in a zone");
		return;
	}

	ManagedReference<BuildingObject*> rootParent = cast<BuildingObject*>(parent.get());
	if(rootParent != NULL && !rootParent->isAllowedEntry(player))
		return;

	//Handle Merchant XP for players using other players vendors...
	if (!vendorInUse->isBazaarTerminal()) {

		DataObjectComponentReference* data = vendorInUse->getDataObjectComponent();
		if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
			error("Vendor has no data component in getData");
			return;
		}

		VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
		if(vendorData == NULL) {
			error("Vendor has wrong data component in getData");
			return;
		}

		vendorData->awardUseXP();
	}

	String planet = "";
	String region = "";
	ManagedReference<SceneObject*> vendor = NULL;

	switch (extent) {
	case 3:
		vendor = vendorInUse;
	case 2:
		if(player->getCityRegion() != NULL)
			region = player->getCityRegion().get()->getRegionName();
		else {
			region = "@planet_n:" + player->getZone()->getZoneName();
			vendor = vendorInUse;
		}
	case 1:
		planet = player->getZone()->getZoneName();
	default:
		break;
	}

	getAuctionData(player, vendorInUse, planet, region, vendor, screen, category, clientcounter, offset);

}

void AuctionManagerImplementation::getAuctionData(CreatureObject* player, SceneObject* usedVendor, const String& planet, const String& region, SceneObject* vendor, int screen, uint32 category, int clientcounter, int offset) {

	TerminalListVector terminalList;

	if (usedVendor->isBazaarTerminal() && screen != 7) { // This is to prevent bazaar items from showing on Vendor Search
		terminalList = auctionMap->getBazaarTerminalData(planet, region, vendor);
	} else {
		terminalList = auctionMap->getVendorTerminalData(planet, region, vendor);
	}

	AuctionQueryHeadersResponseMessage* msg = fillAuctionQueryHeadersResponseMessage(player, usedVendor, &terminalList, screen, category, clientcounter, offset);
	player->sendMessage(msg);
}

void AuctionManagerImplementation::getItemAttributes(CreatureObject* player, uint64 objectid) {

	ManagedReference<AuctionItem*> auctionItem = auctionMap->getItem(objectid);
	if(auctionItem == NULL)
		return;

	ManagedReference<SceneObject*> object = zoneServer->getObject(auctionItem->getAuctionedItemObjectID());

	if (object == NULL) {
		error("not a valid object in getItemAttributes");
		return;
	}

	UnicodeString description(auctionItem->getItemDescription());
	AttributeListMessage* msg = new AttributeListMessage(objectid, description);

	// For objects that don't fill the attribute list normally...
	if (object->getAttributeListComponent() != NULL) {
		object->getAttributeListComponent()->fillAttributeList(msg, player, object);
	} else
		object->fillAttributeList(msg, player);

	//msg->insertInt(0);
	String templateFile = TemplateManager::instance()->getTemplateFile(object->getClientObjectCRC());
	msg->insertAscii(templateFile);

	String cust = "";
	if(object->isTangibleObject()) {
		ManagedReference<TangibleObject*> tano = cast<TangibleObject*>(object.get());
		if(tano != NULL)
			tano->getCustomizationString(cust);
	}
	msg->insertAscii(cust);

	player->sendMessage(msg);

}

void AuctionManagerImplementation::cancelItem(CreatureObject* player, uint64 objectID) {

	ManagedReference<AuctionItem*> item = auctionMap->getItem(objectID);

	// Item wasnt found. it doesn't exist
	if (item == NULL) {
		error("null item in cancelItem()");
		BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, CancelLiveAuctionResponseMessage::INVALIDITEM);
		player->sendMessage(msg);
		return;
	}

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;
	uint64 availableTime = 0;
	bool forSaleOnVendor = false;

	if (item->getStatus() == AuctionItem::FORSALE) {
		if(item->getOwnerID() != player->getObjectID()) {
			error("not the owner of the item in cancelItem()");
			BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, CancelLiveAuctionResponseMessage::NOTOWNER);
			player->sendMessage(msg);
			return;
		}

		if(item->isOnBazaar())
			availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;
		else {
			availableTime = currentTime + AuctionManager::VENDOREXPIREPERIOD;
			forSaleOnVendor = true;
		}

	} else if (item->getStatus() == AuctionItem::OFFERED) {
		if(item->getOfferToID() != player->getObjectID() &&
				item->getOwnerID() != player->getObjectID()) {
			error("not the target of offered item in cancelItem()");
			BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, CancelLiveAuctionResponseMessage::INVALIDITEM);
			player->sendMessage(msg);
			return;
		}
		/// 7 Days
		availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;

	} else {
		BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, CancelLiveAuctionResponseMessage::ALREADYCOMPLETED);
		player->sendMessage(msg);
		return;
	}

	if (item->getExpireTime() <= 0) {
		BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, CancelLiveAuctionResponseMessage::ALREADYCOMPLETED);
		player->sendMessage(msg);
		return;
	}

	// refund auction money
	if (item->isAuction()) {
		refundAuction(item);
		Locker locker(&auctionEvents);

		if(auctionEvents.contains(item->getAuctionedItemObjectID())) {
			Reference<Task*> newTask = auctionEvents.get(item->getAuctionedItemObjectID());
			
			if(newTask != NULL)
				newTask->cancel();
				
			auctionEvents.drop(item->getAuctionedItemObjectID());
		}
	}

	/// If the offeree cancels
	if(item->getStatus() == AuctionItem::OFFERED && item->getOfferToID() == player->getObjectID()) {

		ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());

		if(vendor != NULL) {
			ManagedReference<ChatManager*> cman = zoneServer->getChatManager();
			String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";

			// Waypoint to Vendor / bazaar
			float waypointX = vendor->getWorldPositionX();
			float waypointY = vendor->getWorldPositionY();

			ManagedReference<WaypointObject*> waypoint = zoneServer->createObject(0xc456e788, 0).castTo<WaypointObject*>();
			waypoint->setPlanetCRC(vendor->getPlanetCRC());
			waypoint->setPosition(waypointX, 0, waypointY);

			waypoint->setCustomObjectName(vendor->getDisplayedName(), false);

			String itemName = removeColorCodes(item->getItemName());

			UnicodeString sellerSubject("@auction:subject_auction_unsuccessful"); // Auction Unsuccessful
			StringIdChatParameter sellerBody("@auction:seller_fail"); // Your auction of %TO has been completed and has not been purchased.
			sellerBody.setTO(itemName);

			//Send the Mail
			cman->sendMail(sender, sellerSubject, sellerBody, item->getOwnerName(), waypoint);
		}
	}

	Locker locker(item);

	item->setStatus(AuctionItem::EXPIRED);
	item->setExpireTime(availableTime);
	item->clearAuctionWithdraw();

	BaseMessage* msg = new CancelLiveAuctionResponseMessage(objectID, 0);
	player->sendMessage(msg);

	if(forSaleOnVendor) {
		ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());

		if(vendor != NULL && auctionMap->getVendorItemCount(vendor, true) == 0)
			sendVendorUpdateMail(vendor, true);
	}
}

void AuctionManagerImplementation::expireSale(AuctionItem* item) {
	Locker locker(item);
		
	if(item->getStatus() == AuctionItem::EXPIRED) {
		deleteExpiredSale(item);
		return;
	}

	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();

	String itemName = removeColorCodes(item->getItemName());

	//Send the mail to the vendor owner
	String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";

	UnicodeString sellerSubject("@auction:subject_auction_unsuccessful"); // Auction Unsuccessful

	StringIdChatParameter sellerBody("@auction:seller_fail"); // Your auction of %TO has been completed and has not been purchased.
	sellerBody.setTO(itemName);

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;
	uint64 availableTime = 0;

	if(item->isOnBazaar())
		availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;
	else
		availableTime = currentTime + AuctionManager::VENDOREXPIREPERIOD;

	item->setStatus(AuctionItem::EXPIRED);
	item->setExpireTime(availableTime);
	item->clearAuctionWithdraw();

	locker.release();

	cman->sendMail(sender, sellerSubject, sellerBody, item->getOwnerName());

	if (!item->isOnBazaar()) {
		ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());

		if(vendor != NULL && auctionMap->getVendorItemCount(vendor, true) == 0)
			sendVendorUpdateMail(vendor, true);
	}
}

void AuctionManagerImplementation::expireBidAuction(AuctionItem* item) {

	Locker locker(item);

	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();

	String itemName = removeColorCodes(item->getItemName());

	//Send the mail to the vendor owner
	String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";

	UnicodeString sellerSubject("@auction:subject_auction_item_expired"); // Auction Item Expired

	StringIdChatParameter sellerBody("@auction:seller_fail"); // Your auction of %TO has been completed and has not been purchased.
	sellerBody.setTO(itemName);

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;
	uint64 availableTime = 0;

	if(item->isOnBazaar())
		availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;
	else
		availableTime = currentTime + AuctionManager::VENDOREXPIREPERIOD;

	item->setStatus(AuctionItem::EXPIRED);
	item->setExpireTime(availableTime);
	item->clearAuctionWithdraw();
	
	locker.release();
	
	cman->sendMail(sender, sellerSubject, sellerBody, item->getOwnerName());
}

void AuctionManagerImplementation::expireAuction(AuctionItem* item) {
	ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());
	if (vendor == NULL)
		return;

	String playername = item->getBidderName();
	String sellerName = item->getOwnerName();
	ManagedReference<ChatManager*> cman = zoneServer->getChatManager();
	ManagedReference<PlayerManager*> pman = zoneServer->getPlayerManager();

	ManagedReference<CityRegion*> city = NULL;
	String vendorPlanetName("@planet_n:" + vendor->getZone()->getZoneName());
	String vendorRegionName = vendorPlanetName;

	if( vendor->getCityRegion() != NULL){
		city = vendor->getCityRegion().get();
		vendorRegionName = city->getRegionName();
	}

	Time expireTime;
	uint64 currentTime = expireTime.getMiliTime() / 1000;
	uint64 availableTime = currentTime + AuctionManager::COMMODITYEXPIREPERIOD;

	Locker locker(item);
	item->setExpireTime(availableTime);
	item->clearAuctionWithdraw();

	if(playername.isEmpty()) {
		locker.release();
		expireBidAuction(item);

	} else {
		// Someone won the auction
		ManagedReference<CreatureObject*> buyer = pman->getPlayer(item->getBidderName());
		
		if (buyer == NULL) {
			locker.release();
			expireBidAuction(item);
			return;
		}

		item->setStatus(AuctionItem::SOLD);
		updateAuctionOwner(item, buyer);

		// Waypoint to Vendor / bazaar
		float waypointX = vendor->getWorldPositionX();
		float waypointY = vendor->getWorldPositionY();

		WaypointChatParameter waypointParam;
		waypointParam.set(vendor->getDisplayedName(), waypointX, 0, waypointY, vendor->getPlanetCRC());

		String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";

		StringIdChatParameterVector sellerBodyVector;
		WaypointChatParameterVector sellerWaypointVector;

		StringIdChatParameter sellerBodySale;
		UnicodeString sellerSubject;

		String itemName = removeColorCodes(item->getItemName());

		if (!item->isOnBazaar()) {
			sellerSubject = "@auction:subject_vendor_seller"; // Vendor Sale Complete
			sellerBodySale.setStringId("@auction:seller_success_vendor"); // %TU has sold %TO to %TT for %DI credits.
			sellerBodySale.setTU(vendor->getDisplayedName());
		} else {
			sellerSubject = "@auction:subject_auction_seller"; // Auction Complete
			sellerBodySale.setStringId("@auction:seller_success"); // Your auction of %TO has been sold to %TT for %DI credits
		}
		sellerBodySale.setTO(itemName);
		sellerBodySale.setTT(item->getBidderName());
		sellerBodySale.setDI(item->getPrice());

		StringIdChatParameter sellerBodyLoc("@auction:seller_success_location"); // The sale took place at %TT, on %TO.
		sellerBodyLoc.setTO(vendorPlanetName);
		sellerBodyLoc.setTT(vendorRegionName);

		sellerBodyVector.add(sellerBodySale);
		sellerBodyVector.add(sellerBodyLoc);

		sellerWaypointVector.add(waypointParam);

		StringIdChatParameterVector buyerBodyVector;
		WaypointChatParameterVector buyerWaypointVector;

		UnicodeString buyerSubject;
		if (!item->isOnBazaar()) {
			buyerSubject = "@auction:subject_vendor_buyer"; // Vendor Item Purchased
		} else {
			buyerSubject = "@auction:subject_auction_buyer"; // Auction Won
		}
		StringIdChatParameter buyerBodySale("@auction:buyer_success"); // You have won the auction of "%TO" from "%TT" for %DI credits. See the attached waypoint for location.
		buyerBodySale.setTO(itemName);
		buyerBodySale.setTT(sellerName);
		buyerBodySale.setDI(item->getPrice());

		StringIdChatParameter buyerBodyLoc("@auction:buyer_success_location"); // The sale took place at %TT, on %TO.
		buyerBodyLoc.setTO(vendorPlanetName);
		buyerBodyLoc.setTT(vendorRegionName);

		buyerBodyVector.add(buyerBodySale);
		buyerBodyVector.add(buyerBodyLoc);

		buyerWaypointVector.add(waypointParam);

		//Send the Mail
		locker.release();
		
		UnicodeString blankBody;
		cman->sendMail(sender, sellerSubject, blankBody, sellerName, &sellerBodyVector, &sellerWaypointVector);
		cman->sendMail(sender, buyerSubject, blankBody, item->getBidderName(), &buyerBodyVector, &buyerWaypointVector);
	}
}

void AuctionManagerImplementation::deleteExpiredSale(AuctionItem* item) {
	Locker locker(item);
		
	ManagedReference<SceneObject*> vendor = zoneServer->getObject(item->getVendorID());
	if (vendor != NULL) {

		ManagedReference<ChatManager*> cman = zoneServer->getChatManager();
		String sender = "SWG." + zoneServer->getGalaxyName() + ".auctioner";

		// Waypoint to Vendor / bazaar
		float waypointX = vendor->getWorldPositionX();
		float waypointY = vendor->getWorldPositionY();

		ManagedReference<WaypointObject*> waypoint = zoneServer->createObject(0xc456e788, 0).castTo<WaypointObject*>();

		Locker lockerWaypoint(waypoint);

		waypoint->setPlanetCRC(vendor->getPlanetCRC());
		waypoint->setPosition(waypointX, 0, waypointY);

		waypoint->setCustomObjectName(vendor->getDisplayedName(), false);

		lockerWaypoint.release();

		String itemName = removeColorCodes(item->getItemName());

		UnicodeString sellerSubject("@auction:subject_auction_unsuccessful"); // Auction Unsuccessful
		StringIdChatParameter sellerBody("@auction:item_expired"); // Because you failed to pick it up in time, the "%TO" that you were auctioning has expired.
		sellerBody.setTO(itemName);

		//Send the Mail
		locker.release();
		
		cman->sendMail(sender, sellerSubject, sellerBody, item->getOwnerName(), waypoint);
	}

	auctionMap->deleteItem(vendor, item);
}

void AuctionManagerImplementation::displayInfo(CreatureObject* player) {
	ManagedReference<SuiListBox*> list = new SuiListBox(player, SuiWindowType::MARKET_INFO, 0x00);
	list->setPromptTitle("Market Info");
	list->setPromptText("Information about the commodities market");

	list->addMenuItem("Terminals with items");
	list->addMenuItem("\tBazaars: " + String::valueOf(auctionMap->getBazaarCount()));
	list->addMenuItem("\tVendors: " + String::valueOf(auctionMap->getVendorCount()));
	list->addMenuItem("Total Items: " + String::valueOf(auctionMap->getTotalItemCount()));

	player->sendMessage(list->generateMessage());
}

void AuctionManagerImplementation::updateAuctionOwner(AuctionItem* item, CreatureObject* player) {

	Locker locker(item);

	ManagedReference<PlayerManager*> pman = zoneServer->getPlayerManager();
	ManagedReference<CreatureObject*> previousOwner = pman->getPlayer(item->getOwnerName());

	if(player == previousOwner)
		return;

	auctionMap->removeFromCommodityLimit(item);

	item->setOwnerID(player->getObjectID());
	item->setOwnerName(player->getFirstName());

	if(!item->isOnBazaar() && item->getStatus() != AuctionItem::OFFERED)
		return;

	auctionMap->addToCommodityLimit(item);
}

void AuctionManagerImplementation::sendVendorUpdateMail(SceneObject* vendor, bool isEmpty) {
	//Send the mail to the vendor owner
	if (vendor == NULL || !vendor->isVendor())
		return;

	VendorDataComponent* vendorData = NULL;
	DataObjectComponentReference* data = vendor->getDataObjectComponent();
	if(data != NULL && data->get() != NULL && data->get()->isVendorData())
		vendorData = cast<VendorDataComponent*>(data->get());

	if(vendorData == NULL)
		return;

	ManagedReference<ChatManager*> cman = vendor->getZoneServer()->getChatManager();
	ManagedReference<CreatureObject*> owner = vendor->getZoneServer()->getObject(vendorData->getOwnerId()).castTo<CreatureObject*>();

	String sender = vendor->getDisplayedName();
	UnicodeString subject("@auction:vendor_status_subject");

	if (cman == NULL || owner == NULL)
		return;

	if (isEmpty) {
		StringIdChatParameter body("@auction:vendor_status_empty");
		body.setTO(vendor->getDisplayedName());
		cman->sendMail(sender, subject, body, owner->getFirstName());
		vendorData->setEmpty();
		VendorManager::instance()->handleUnregisterVendor(owner, cast<TangibleObject*>(vendor));
	} else {
		StringIdChatParameter body("@auction:vendor_status_normal");
		body.setTO(vendor->getDisplayedName());
		cman->sendMail(sender, subject, body, owner->getFirstName());
	}

}

String AuctionManagerImplementation::removeColorCodes(const String& name) {
	String itemName = name;

	while (itemName.contains("\\#")) {
		int index = itemName.indexOf("\\#");
		String sub = "\\" + itemName.subString(index, index + 2);
		itemName = itemName.replaceFirst(sub,"");
	}

	return itemName;
}
