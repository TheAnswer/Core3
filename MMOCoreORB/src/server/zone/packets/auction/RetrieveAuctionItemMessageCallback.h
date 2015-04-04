/*
 * RetrieveAuctionItemMessageCallback.h
 *
 *  Created on: 14/03/2010
 *      Author: victor
 */

#ifndef RETRIEVEAUCTIONITEMMESSAGECALLBACK_H_
#define RETRIEVEAUCTIONITEMMESSAGECALLBACK_H_


#include "../MessageCallback.h"
#include "server/zone/managers/auction/AuctionManager.h"


class RetrieveAuctionItemMessageCallback : public MessageCallback {
	uint64 objectid;
	uint64 bazaarid;

public:
	RetrieveAuctionItemMessageCallback(ZoneClientSession* client, ZoneProcessServer* server) :
			MessageCallback(client, server), objectid(0), bazaarid(0) {

	}

	void parse(Message* message) {
		objectid = message->parseLong(); // object for sale
		bazaarid = message->parseLong(); // bazaar
	}

	void run() {
		ManagedReference<CreatureObject*> player = cast<CreatureObject*>( client->getPlayer().get().get());

		if (player == NULL)
			return;

		Locker locker(player);

		AuctionManager* auctionManager = server->getZoneServer()->getAuctionManager();

		auctionManager->retrieveItem(player, objectid, bazaarid);
	}

};


#endif /* RETRIEVEAUCTIONITEMMESSAGECALLBACK_H_ */
