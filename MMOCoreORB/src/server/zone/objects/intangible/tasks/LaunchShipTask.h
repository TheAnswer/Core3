/*
 * LaunchShipTask.h
 */

#ifndef LAUNCHSHIPTASK_H_
#define LAUNCHSHIPTASK_H_

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/intangible/ShipControlDevice.h"
#include "server/zone/objects/ship/ShipObject.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/objects/ship/events/InsertPilotIntoShipTask.h"
#include "server/zone/objects/ship/events/InsertGroupMemberIntoShipTask.h"
#include "templates/params/creature/PlayerArrangement.h"

class LaunchShipTask : public Task, public Logger {
	ManagedWeakReference<CreatureObject*> play;
	ManagedWeakReference<ShipControlDevice*> shipDev;
	Vector<uint64> groupMembers;

public:
	LaunchShipTask(CreatureObject* creo, ShipControlDevice* controlDevice, Vector<uint64> group) : play(creo), shipDev(controlDevice), groupMembers(group) {
		setLoggingName("LaunchShipTask");
	}

	void run() {
		ManagedReference<CreatureObject*> player = play.get();
		ManagedReference<ShipControlDevice*> shipControlDevice = shipDev.get();

		if (player == nullptr || shipControlDevice == nullptr)
			return;

		Zone* departureZone = player->getZone();

		if (departureZone == nullptr) {
			return;
		}

		PlanetManager* planetManager = departureZone->getPlanetManager();

		if (planetManager == nullptr) {
			return;
		}

		String jtlZoneName = planetManager->getJtlZoneName();

		if (jtlZoneName == "") {
			return;
		}

		Vector3 launchPosition = planetManager->getJtlLaunchLocations();

		float randomX = Math::clamp(-7680.f, ((System::random(1500.f) - 750.f) + launchPosition.getX()), 7680.f);
		float randomY = Math::clamp(-7680.f, ((System::random(1500.f) - 750.f) + launchPosition.getY()), 7680.f);
		float randomZ = Math::clamp(-7680.f, ((System::random(1500.f) - 750.f) + launchPosition.getZ()), 7680.f);

		launchPosition = Vector3(randomX, randomY, randomZ);

		// Lock the control device
		Locker deviceLock(shipControlDevice);

		// Ship is cross locked to device in the launchShip function
		if (!shipControlDevice->launchShip(player, jtlZoneName, launchPosition)) {
			error() << "Ship failed to launch - Device ID: " << shipControlDevice->getObjectID();
			return;
		}

		auto controlledDevice = shipControlDevice->getControlledObject();

		if (controlledDevice == nullptr || !controlledDevice->isShipObject()) {
			return;
		}

		auto ship = controlledDevice->asShipObject();

		if (ship == nullptr || ship->getLocalZone() == nullptr) {
			return;
		}

		// Lock player to store launch point
		Locker pilotLock(player, shipControlDevice);

		shipControlDevice->setStoredLocationData(player);

		pilotLock.release();

		InsertPilotIntoShipTask* pilotTask = new InsertPilotIntoShipTask(player, ship);

		if (pilotTask != nullptr)
			pilotTask->schedule(100);

		if (groupMembers.size() > 0 && (ship->isPobShip() || ship->isMultiPassengerShip())) {
			for (int j = 0; j < groupMembers.size(); ++j) {
				auto memberID = groupMembers.get(j);

				// info(true) << "LaunchShipTask - Scheduling Group Member for Insertion - ID: " << memberID;

				// POB Ships will send group members into cells directly with a -1 arrangement. MultiPassenger ships will send one group member into the gunner positon.
				InsertGroupMembertIntoShipTask* memberTask = new InsertGroupMembertIntoShipTask(ship, memberID, (ship->isMultiPassengerShip() ? PlayerArrangement::SHIP_GUNNER1 : -1));

				if (memberTask != nullptr) {
					memberTask->schedule(200 + (j * 50));
				}

				// Pre-CU Multipassenger ships only have one gunner slot
				if (ship->isMultiPassengerShip())
					break;
			}
		}
	}
};

#endif /* LAUNCHSHIPTASK_H_ */
