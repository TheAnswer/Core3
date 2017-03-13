/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef PURCHASETICKETCOMMAND_H_
#define PURCHASETICKETCOMMAND_H_

#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/player/sui/messagebox/SuiMessageBox.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/objects/region/CityRegion.h"

class PurchaseTicketCommand : public QueueCommand {
public:

	PurchaseTicketCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		SortedVector<QuadTreeEntry*> closeObjects;
		CloseObjectsVector* vec = (CloseObjectsVector*) creature->getCloseObjects();
		vec->safeCopyTo(closeObjects);

		bool nearTravelTerminal = false;

		for (int i = 0; i < closeObjects.size(); i++) {
			SceneObject* object = cast<SceneObject*>( closeObjects.get(i));
			if (object != NULL && object->getGameObjectType() == SceneObjectType::TRAVELTERMINAL && checkDistance(creature, object, 8)) {
				nearTravelTerminal = true;
				break;
			}

		}

		if (!nearTravelTerminal) {
			creature->sendSystemMessage("@travel:too_far"); // You are too far from the terminal to purchase a ticket.
			return GENERALERROR;
		}

		ManagedReference<CityRegion*> currentCity = creature->getCityRegion().get();

		int departureTax = 0;
		if (currentCity != NULL){
			if (currentCity->isBanned(creature->getObjectID())) {
				creature->sendSystemMessage("@city/city:city_cant_purchase_ticket"); //You are banned from using the services of this city. You cannot purchase a ticket.
				return GENERALERROR;
			}
			if(!currentCity->isClientRegion()){
				departureTax = currentCity->getTravelTax();
			}
		}
		ManagedReference<SceneObject*> inventory = creature->getSlottedObject("inventory");

		if (inventory == NULL)
			return GENERALERROR;

		String departurePlanet, departurePoint, arrivalPlanet, arrivalPoint, type;
		bool roundTrip = true;


		try {
			UnicodeTokenizer tokenizer(arguments);
			tokenizer.getStringToken(departurePlanet);
			tokenizer.getStringToken(departurePoint);
			tokenizer.getStringToken(arrivalPlanet);
			tokenizer.getStringToken(arrivalPoint);
			if(tokenizer.hasMoreTokens()) {
				tokenizer.getStringToken(type);
				if (type == "single" || type == "0")
					roundTrip = false;
			}

		} catch(Exception& e) {
			return INVALIDPARAMETERS;
		}

		departurePlanet = departurePlanet.replaceAll("_", " ");
		departurePoint = departurePoint.replaceAll("_", " ");
		arrivalPlanet = arrivalPlanet.replaceAll("_", " ");
		arrivalPoint = arrivalPoint.replaceAll("_", " ");

		ManagedReference<Zone*> departureZone = server->getZoneServer()->getZone(departurePlanet);
		ManagedReference<Zone*> arrivalZone = server->getZoneServer()->getZone(arrivalPlanet);

		if (departureZone == NULL)
			return GENERALERROR;

		if (arrivalZone == NULL)
			return GENERALERROR;

		ManagedReference<PlanetManager*> pmDeparture = departureZone->getPlanetManager();
		ManagedReference<PlanetManager*> pmArrival = arrivalZone->getPlanetManager();

		if (!pmArrival->isExistingPlanetTravelPoint(arrivalPoint)) {
			creature->sendSystemMessage("@travel:no_location_found"); //No location was found for your destination.
			return INVALIDPARAMETERS;
		}

		if (!pmDeparture->isExistingPlanetTravelPoint(departurePoint)) {
			creature->sendSystemMessage("The given departure point was not found.");
			return INVALIDPARAMETERS;
		}

		Reference<PlanetTravelPoint*>  destPoint = pmArrival->getPlanetTravelPoint(arrivalPoint);

		if (destPoint == NULL)
			return GENERALERROR;

		ManagedReference<CreatureObject*> arrivalShuttle = destPoint->getShuttle();

		if (arrivalShuttle == NULL)
			return GENERALERROR;


		ManagedReference<CityRegion*> destCity = arrivalShuttle->getCityRegion().get();

		if (destCity != NULL){
			if (destCity.get()->isBanned(creature->getObjectID())) {
				creature->sendSystemMessage("@city/city:banned_from_that_city");  // You have been banned from traveling to that city by the city militia
				return GENERALERROR;
			}
		}

		//Check to see if this point can be reached from this location.
		if (!pmDeparture->isTravelToLocationPermitted(departurePoint, arrivalPlanet, arrivalPoint))
			return GENERALERROR;

		if (roundTrip && !pmArrival->isTravelToLocationPermitted(arrivalPoint, departurePlanet, departurePoint))
			return GENERALERROR; //If they are doing a round trip, make sure they can travel back.

		int baseFare = pmDeparture->getTravelFare(departurePlanet, arrivalPlanet);

		if (baseFare == 0) { // Make sure that the travel route is valid
			creature->sendSystemMessage("Invalid travel route specified.");
			return GENERALERROR;
		}

		int fare = baseFare + departureTax;

		if (roundTrip)
			fare *= 2;

		//Make sure they have space in the inventory for the tickets before purchasing them.
		Locker _lock(inventory, creature);

		if (inventory->getContainerObjectsSize() + ((roundTrip) ? 2 : 1) > inventory->getContainerVolumeLimit()) {
			creature->sendSystemMessage("@error_message:inv_full"); //Your inventory is full.
			return GENERALERROR;
		}

		//Check if they have funds.
		int bank = creature->getBankCredits();
		int cash = creature->getCashCredits();

		if (bank < fare) {
			int diff = fare - bank;

			if (diff > cash) {
				ManagedReference<SuiMessageBox*> suiBox = new SuiMessageBox(creature, 0);
				suiBox->setPromptTitle("");
				suiBox->setPromptText("You do not have sufficient funds for that.");

				creature->sendMessage(suiBox->generateMessage());
				creature->sendSystemMessage("@travel:short_funds"); //You do not have enough money to complete the ticket purchase.
				return GENERALERROR;
			}

			creature->subtractBankCredits(bank); //Take all from the bank, since they didn't have enough to cover.
			creature->subtractCashCredits(diff); //Take the rest from the cash.

		} else {
			creature->subtractBankCredits(fare); //Take all of the fare from the bank.
		}


		StringIdChatParameter params("@base_player:prose_pay_acct_success"); //You successfully make a payment of %DI credits to %TO.
		params.setDI(baseFare + (roundTrip * baseFare));
		params.setTO("@money/acct_n:travelsystem"); //the Galactic Travel Commission

		creature->sendSystemMessage(params);

		ManagedReference<SceneObject*> ticket1 = pmDeparture->createTicket(departurePoint, arrivalPlanet, arrivalPoint);
		if (ticket1 == NULL) {
			creature->sendSystemMessage("Error creating travel ticket.");
			return GENERALERROR;
		}

		if (inventory->transferObject(ticket1, -1, true)) {
			ticket1->sendTo(creature, true);
		} else {
			ticket1->destroyObjectFromDatabase(true);
			creature->sendSystemMessage("Error transferring travel ticket to inventory.");
			return GENERALERROR;
		}

		if (roundTrip) {
			ManagedReference<SceneObject*> ticket2 = pmArrival->createTicket(arrivalPoint, departurePlanet, departurePoint);

			if (inventory->transferObject(ticket2, -1, true)) {
				ticket2->sendTo(creature, true);
			} else {
				ticket2->destroyObjectFromDatabase(true);
				creature->sendSystemMessage("Error transferring round-trip travel ticket to inventory.");
				return GENERALERROR;
			}
		}
		_lock.release();

		if(currentCity != NULL && !currentCity->isClientRegion() && departureTax > 0) {
			Locker clocker(currentCity, creature);
			int taxPaid = departureTax + (roundTrip * departureTax);
			currentCity->addToCityTreasury(taxPaid);

			StringIdChatParameter param("@city/city:city_ticket_pay"); // You pay a tax of %DI credits to the local City Travel Authority.
			param.setDI(taxPaid);

			creature->sendSystemMessage(param);
		}

		ManagedReference<SuiMessageBox*> suiBox = new SuiMessageBox(creature, 0);
		suiBox->setPromptTitle("");
		suiBox->setPromptText("@travel:ticket_purchase_complete"); //Ticket purchase complete

		creature->sendMessage(suiBox->generateMessage());

		return SUCCESS;
	}

};

#endif //PURCHASETICKETCOMMAND_H_
