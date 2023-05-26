/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef TELEPORTCOMMAND_H_
#define TELEPORTCOMMAND_H_

#include "server/zone/objects/ship/ShipObject.h"
#include "server/zone/objects/intangible/ShipControlDevice.h"

class TeleportCommand : public QueueCommand {
public:

	TeleportCommand(const String& name, ZoneProcessServer* server) : QueueCommand(name, server) {
	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {
		if (creature == nullptr)
			return GENERALERROR;

		ZoneServer* zoneServer = creature->getZoneServer();

		if (zoneServer == nullptr)
			return GENERALERROR;

		float x, y;
		float z = 0;

		String zoneName;
		uint64 parentID = 0;

		try {
			UnicodeTokenizer tokenizer(arguments);

			x = tokenizer.getFloatToken();
			y = tokenizer.getFloatToken();

			if (tokenizer.hasMoreTokens())
				tokenizer.getStringToken(zoneName);

			if (tokenizer.hasMoreTokens()) {
				z = tokenizer.getFloatToken();
				parentID = tokenizer.getLongToken();
			}

			if (zoneName == "") {
				auto zone = creature->getZone();

				if (zone != nullptr)
					zoneName = zone->getZoneName();
			}

			bool newZoneIsSpace = zoneName.contains("space");
			ManagedReference<ShipObject*> ship = nullptr;

			// Handle a player that is piloting a ship
			if (creature->isPilotingShip()) {
				SceneObject* datapad = creature->getSlottedObject("datapad");
				ShipControlDevice* shipControlDevice = nullptr;

				if (datapad != nullptr) {
					for (int i = 0; i < datapad->getContainerObjectsSize(); ++i) {
						SceneObject* datapadObject = datapad->getContainerObject(i);

						if (datapadObject == nullptr || !datapadObject->isShipControlDevice())
							continue;

						shipControlDevice = cast<ShipControlDevice*>(datapadObject);

						if (shipControlDevice == nullptr)
							continue;

						if (shipControlDevice->isShipLaunched()) {
							break;
						}

						shipControlDevice = nullptr;
					}
				}

				if (shipControlDevice != nullptr && !shipControlDevice->storeShip(nullptr)) {
					creature->sendSystemMessage("Failed to store ship for teleport.");
					return GENERALERROR;
				}
			}

			// Get the world floor and set players direction in the ground zone
			if (!newZoneIsSpace) {
				auto newZone = zoneServer->getGroundZone(zoneName);

				if (newZone != nullptr) {
					z = CollisionManager::getWorldFloorCollision(x, y, newZone, false);

					creature->setDirection(0);
				}
			}

			creature->switchZone(zoneName, x, z, y, parentID);
		} catch (Exception& e) {
			creature->sendSystemMessage("SYNTAX: /teleport <x> <y> [<planet>] [<z> <parentID>]");
		}

		return SUCCESS;
	}

};

#endif //TELEPORTCOMMAND_H_
