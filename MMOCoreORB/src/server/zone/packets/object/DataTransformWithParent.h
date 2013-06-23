/*
Copyright (C) 2007 <SWGEmu>

This File is part of Core3.

This program is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software
Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Linking Engine3 statically or dynamically with other modules
is making a combined work based on Engine3.
Thus, the terms and conditions of the GNU Lesser General Public License
cover the whole combination.

In addition, as a special exception, the copyright holders of Engine3
give you permission to combine Engine3 program with free software
programs or libraries that are released under the GNU LGPL and with
code included in the standard release of Core3 under the GNU LGPL
license (or modified versions of such code, with unchanged license).
You may copy and distribute such a system following the terms of the
GNU LGPL for Engine3 and the licenses of the other code concerned,
provided that you include the source code of that other code when
and as the GNU LGPL requires distribution of source code.

Note that people who make modified versions of Engine3 are not obligated
to grant this special exception for their modified versions;
it is their choice whether to do so. The GNU Lesser General Public License
gives permission to release a modified version without this exception;
this exception also makes it possible to release a modified version
which carries forward this exception.
*/

#ifndef DATATRANSFORMWITHPARENT_H_
#define DATATRANSFORMWITHPARENT_H_

#include "ObjectControllerMessage.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/creature/CreatureState.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "ObjectControllerMessageCallback.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/Zone.h"

class DataTransformWithParent : public ObjectControllerMessage {
public:
	DataTransformWithParent(SceneObject* creo)
			: ObjectControllerMessage(creo->getObjectID(), 0x1B, 0xF1) {

		insertInt(creo->getMovementCounter());

		insertLong(creo->getParentID());

		insertFloat(creo->getDirectionX());
		insertFloat(creo->getDirectionY());
		insertFloat(creo->getDirectionZ());
		insertFloat(creo->getDirectionW());

		insertFloat(creo->getPositionX());
		insertFloat(creo->getPositionZ());
		insertFloat(creo->getPositionY());

		insertInt(0);
	}

};

class DataTransformWithParentCallback : public MessageCallback {
	uint32 movementStamp;
	uint32 movementCounter;
	uint64 parent;

	float directionX, directionY, directionZ, directionW;
	float positionX, positionZ, positionY;
	float parsedSpeed;

	ObjectControllerMessageCallback* objectControllerMain;
	
//	taskqueue = 3;
public:
	DataTransformWithParentCallback(ObjectControllerMessageCallback* objectControllerCallback) :
		MessageCallback(objectControllerCallback->getClient(), objectControllerCallback->getServer()) {

		objectControllerMain = objectControllerCallback;
		
		
		taskqueue = 3;
		
		 ManagedReference<SceneObject*> player = client->getPlayer();
		 
                 if (player != NULL) {
                        Zone* zone = player->getLocalZone();
                                    
                         if (zone != NULL) {
                             String zoneName = zone->getZoneName();
                                                                           
                              if (zoneName == "corellia")
                                   taskqueue = 4;
                              else if (zoneName == "tatooine")
                                    taskqueue = 5;
                               else if (zoneName == "naboo")
                                        taskqueue = 6;
                                                                                                                                                                                                      }
                                                                                                                                                                                                                                                                                                                             }
		                                                                                                                                                                                                                                                                                                                                                                 
	}

	void parse(Message* message) {
		movementStamp = message->parseInt();
		movementCounter = message->parseInt();

		parent = message->parseLong();

		directionX = message->parseFloat();
		directionY = message->parseFloat();
		directionZ = message->parseFloat();
		directionW = message->parseFloat();

		positionX = message->parseFloat();
		positionZ = message->parseFloat();
		positionY = message->parseFloat();

		parsedSpeed = message->parseFloat();

		//info("datatransform with parent", true);
	}

	void bounceBack(SceneObject* object, ValidatedPosition& pos) {
		Vector3 teleportPoint = pos.getPosition();
		uint64 teleportParentID = pos.getParent();

		object->teleport(teleportPoint.getX(), teleportPoint.getZ(), teleportPoint.getY(), teleportParentID);
	}

	void run() {
		ManagedReference<CreatureObject*> object = cast<CreatureObject*>(client->getPlayer().get().get());

		if (object == NULL)
			return;

		PlayerObject* ghost = object->getPlayerObject();

		if (isnan(positionX) || isnan(positionY) || isnan(positionZ))
			return;

		if (isinf(positionX) || isinf(positionY) || isinf(positionZ))
			return;

		if (ghost->isTeleporting())
			return;

		/*if (!object->isInQuadTree())
			return;*/

		if (positionX > 1024.0f || positionX < -1024.0f || positionY > 1024.0f || positionY < -1024.0f) {
			StringBuffer msg;
			msg << "position out of bounds cell:[" << parent << "]";
			object->error(msg.toString());

			return;
		}

		if (object->getZone() == NULL)
			return;

		if (object->isRidingMount()) {
			ZoneServer* zoneServer = server->getZoneServer();

			ObjectController* objectController = zoneServer->getObjectController();
			objectController->activateCommand(object, String("dismount").hashCode(), 0, 0, "");
		}

		uint32 objectMovementCounter = object->getMovementCounter();

		/*if (objectMovementCounter > movementCounter) { // we already parsed a more updated movement counter
			StringBuffer msg;
			msg << "trying to parse movement update: 0x" << hex << movementCounter << " but we already parsed 0x" << hex << objectMovementCounter;
			object->info(msg.toString(), true);
			return;
		}*/


		ManagedReference<SceneObject*> newParent = server->getZoneServer()->getObject(parent, true);

		if (newParent == NULL)
			return;

		if (!newParent->isCellObject() || newParent->getParent() == NULL)
			return;

		ManagedReference<BuildingObject*> building = newParent->getParent().castTo<BuildingObject*>();

		if (building == NULL)
			return;

		ManagedReference<SceneObject*> par = object->getParent();

		if (par != NULL && par->isShipObject())
			return;

		/*StringBuffer posMsg;
		posMsg << "posX: " << positionX << " posZ: " << positionZ << " posY:" << positionY;
		object->info(posMsg.toString(), true);*/

		ManagedReference<PlayerManager*> playerManager = server->getPlayerManager();

		if (playerManager == NULL)
			return;

		ValidatedPosition pos;
		pos.update(object);

		if (object->isFrozen() || !building->isAllowedEntry(object)) {
			bounceBack(object, pos);
			return;
		}

		Reference<Vector<float>* > collisionPoints = CollisionManager::getCellFloorCollision(positionX, positionY, cast<CellObject*>(newParent.get()));

		if (collisionPoints == NULL) {
			bounceBack(object, pos);
			return;
		}

		float minErr = 16384;

		for (int i = 0; i < collisionPoints->size(); ++i) {
			float val = collisionPoints->get(i);

			float err = fabs(val - positionZ);

			//printf("collision error %f\n value %f", err, val);

			if (err < minErr) {
				minErr = err;
			}
		}

		if (minErr > 0.25) {
			bounceBack(object, pos);

			return;
		}

		ContainerPermissions* perms = newParent->getContainerPermissions();

		if (!perms->hasInheritPermissionsFromParent()) {
			if (!newParent->checkContainerPermission(object, ContainerPermissions::WALKIN)) {
				bounceBack(object, pos);

				return;
			}
		}

		if (playerManager->checkSpeedHackFirstTest(object, parsedSpeed, pos, 1.1f) != 0)
			return;

		if (playerManager->checkSpeedHackSecondTest(object, positionX, positionZ, positionY, movementStamp, newParent) != 0)
			return;

		object->setMovementCounter(movementCounter);
		object->setDirection(directionW, directionX, directionY, directionZ);
		object->setPosition(positionX, positionZ, positionY);
		ghost->setClientLastMovementStamp(movementStamp);


		if (objectControllerMain->getPriority() == 0x23)
			object->updateZoneWithParent(newParent, false);
		else
			object->updateZoneWithParent(newParent, true);

	}
};


#endif /*DATATRANSFORMWITHPARENT_H_*/
