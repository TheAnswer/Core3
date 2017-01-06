/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef ISVENDOROWNERRESPONSEMESSAGE_H_
#define ISVENDOROWNERRESPONSEMESSAGE_H_

#include "engine/service/proto/BaseMessage.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/tangible/components/vendor/VendorDataComponent.h"

class IsVendorOwnerResponseMessage : public BaseMessage {

public:
    IsVendorOwnerResponseMessage(SceneObject* vendor, CreatureObject* player, const String& planet, const String& region) {

    	insertShort(3);
		insertInt(0xCE04173E);

		// Make sure sceno is a valid Vendor Object.
		if (!vendor->isVendor() && !vendor->isBazaarTerminal()) {
			//returning half built packets on errors is bad, mmkay
			insertInt(1);
			insertInt(2); // 2 == invalid object
			insertLong(vendor->getObjectID());
			insertAscii("");
			insertShort(0x64);
			return;
		}

		Reference<AuctionManager*> auctionManager = player->getZoneServer()->getAuctionManager();

		int rights = 2;
		uint32 errorCode = auctionManager->isMarketEnabled() == false; // 1 == auctioneer issue


		if(vendor->isVendor()) {


			DataObjectComponentReference* data = vendor->getDataObjectComponent();
			if(data == NULL || data->get() == NULL || !data->get()->isVendorData()) {
				return;
			}

			VendorDataComponent* vendorData = cast<VendorDataComponent*>(data->get());
			if(vendorData == NULL) {
				return;
			}

			rights = vendorData->getOwnershipRightsOf(player);

			if(vendorData->isOnStrike()) {
				errorCode = 1;
				player->sendSystemMessage("@ui_auc:err_vendor_deactivated");
			}
		}

		uint64 objectID = vendor->getObjectID();

		// 0: own vendor, 1: someone else owns the vendor, 2: the galaxy owns the vendor (bazaar)
		insertInt(rights);

		insertInt(errorCode);
		
		insertLong(objectID);

		StringBuffer title;
		title << planet << ".";
		
		int x = vendor->getWorldPositionX();
		int y = vendor->getWorldPositionY();

		String uuid = auctionManager->getVendorUID(vendor);

		insertAscii(uuid);
		
		insertShort(0x64); // ?? 64
	}

};

#endif /*ISVENDOROWNERRESPONSEMESSAGE_H_*/
