/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef WAYPOINTCOMMAND_H_
#define WAYPOINTCOMMAND_H_


#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/waypoint/WaypointObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/Zone.h"

class WaypointCommand : public QueueCommand {
public:

	WaypointCommand(const String& name, ZoneProcessServer* server)
		: QueueCommand(name, server) {

	}

	int doQueueCommand(CreatureObject* creature, const uint64& target, const UnicodeString& arguments) const {

		if (!checkStateMask(creature))
			return INVALIDSTATE;

		if (!checkInvalidLocomotions(creature))
			return INVALIDLOCOMOTION;

		Zone* zone = creature->getZone();

		if (zone == nullptr)
			return GENERALERROR;

		int counter = 0;
		String dummy;

		String usageError = "Usage: /waypoint X Y <name> or /waypoint <name>";

		String waypointData = arguments.toString();

		String waypointName = "@ui:datapad_new_waypoint"; // New Waypoint
		String planet = zone->getZoneName();
		float x = creature->getPositionX();
		float y = creature->getPositionY();
		float z = 0.0f;

		ManagedReference<SceneObject*> parentObject = creature->getParent().get();

		if (parentObject != nullptr) {
			if (parentObject->isCellObject()) {
				ManagedReference<SceneObject*> grandParentObject = parentObject->getParent().get();

				if (grandParentObject != nullptr) {
					x = grandParentObject->getPositionX();
					y = grandParentObject->getPositionY();
				}
			}
		}

		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target).get();

		StringTokenizer tokenizer(waypointData);
		tokenizer.setDelimeter(" ");

		if (tokenizer.hasMoreTokens()) {

			String arg1;
			tokenizer.getStringToken(arg1);

			if (tokenizer.hasMoreTokens()) {
				if (isalpha(arg1[0]) == 0) {
					//A waypoint in the form of /waypoint X Y <name>
					x = atof(arg1.toCharArray());

					if (tokenizer.hasMoreTokens()) {
						String temp;
						tokenizer.getStringToken(temp);
						if (isalpha(temp[0]) == 0) {
							y = atof(temp.toCharArray());
						} else {
							creature->sendSystemMessage(usageError);
							return GENERALERROR;
						}
					}

					StringBuffer newWaypointName;

					while (tokenizer.hasMoreTokens()) {
						String name;
						tokenizer.getStringToken(name);
						newWaypointName << name << " ";
					}

					if (newWaypointName.length() > 0)
						waypointName = newWaypointName.toString();

				} else {
					//A waypoint in the form of /waypoint planet X Z Y - Planetary Map
					planet = arg1;

					if (server->getZoneServer()->getZone(planet) == nullptr) { //Not a valid planet name - malformed command
						creature->sendSystemMessage(usageError);
						return GENERALERROR;
					}

					if (tokenizer.hasMoreTokens()) {
						String temp;
						tokenizer.getStringToken(temp);

						if (!Character::isLetterOrDigit(temp.charAt(0))) {
							x = Float::valueOf(temp);
						} else {
							creature->sendSystemMessage(usageError);
							return GENERALERROR;
						}
					}

					if (tokenizer.hasMoreTokens()) {
						String temp;
						tokenizer.getStringToken(temp);

						if (!Character::isLetterOrDigit(temp.charAt(0))) {
							z = Float::valueOf(temp);
						} else {
							creature->sendSystemMessage(usageError);
							return GENERALERROR;
						}
					}

					if (tokenizer.hasMoreTokens()) {
						String temp;
						tokenizer.getStringToken(temp);

						if (!Character::isLetterOrDigit(temp.charAt(0))) {
							y = Float::valueOf(temp);
						} else {
							creature->sendSystemMessage(usageError);
							return GENERALERROR;
						}
					}
				}
			} else {
				//A waypoint in the form of /waypoint <name>
				waypointName = arg1;
			}
		} else if (targetObject != nullptr) {
			Locker crosslocker(targetObject, creature);

			x = targetObject->getWorldPositionX();
			y = targetObject->getWorldPositionY();
			waypointName = targetObject->getDisplayedName();
		}

		x = (x < -8192) ? -8192 : x;
		x = (x > 8192) ? 8192 : x;

		y = (y < -8192) ? -8192 : y;
		y = (y > 8192) ? 8192 : y;

		Reference<PlayerObject*> playerObject =  creature->getSlottedObject("ghost").castTo<PlayerObject*>();

		ManagedReference<WaypointObject*> obj = server->getZoneServer()->createObject(0xc456e788, 1).castTo<WaypointObject*>();

		Locker locker(obj);

		obj->setPlanetCRC(planet.hashCode());
		obj->setPosition(x, z, y);
		obj->setCustomObjectName(waypointName, false);
		obj->setActive(true);

		playerObject->addWaypoint(obj, false, true); // Should second argument be true, and waypoints with the same name thus remove their old version?

		return SUCCESS;
	}

};

#endif //WAYPOINTCOMMAND_H_


