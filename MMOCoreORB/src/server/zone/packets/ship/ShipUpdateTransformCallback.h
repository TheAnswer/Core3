/*
 * ShipUpdateTransformCallback.h
 *
 *  Created on: 01/09/2011
 *      Author: TheAnswer
 */

#ifndef SHIPUPDATETRANSFORMCALLBACK_H_
#define SHIPUPDATETRANSFORMCALLBACK_H_

#include "../MessageCallback.h"
#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "ShipUpdateTransformCollisionMessage.h"
#include "ShipUpdateTransformMessage.h"
#include "server/zone/packets/object/DataTransform.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/packets/object/PlayClientEffectObjectMessage.h"

#include "PackedVelocity.h"
#include "PackedRotationRate.h"

class ShipUpdateTransformCallback : public MessageCallback {
	uint16 shipId;

	//PackedTransform dir is multiplied by 127, positions by 4.0958748
	uint8 dirX, dirY, dirZ, dirW;
	int16 posX, posZ, posY;

	//PackedVelocity
	//int16 velocitySpeed, velocityDirection;
	PackedVelocity velocity;

	//3 PackedRotationRate
	PackedRotationRate yawRate, pitchRate, rollRate;

	uint32 counter;

public:
	ShipUpdateTransformCallback(ZoneClientSession* client, ZoneProcessServer* server) :
		MessageCallback(client, server), shipId(0), dirX(0), dirY(0), dirZ(0), dirW(0),
		posX(0), posZ(0), posY(0), counter(0) {
	}

	void parse(Message* message) {
		//info(message->toStringData(), true);
		shipId = message->parseShort();

		dirX = message->parseByte();
		dirY = message->parseByte();
		dirZ = message->parseByte();
		dirW = message->parseByte();

		posX = message->parseSignedShort();
		posZ = message->parseSignedShort();
		posY = message->parseSignedShort();

		velocity.parse(message);

		yawRate.parse(message);
		pitchRate.parse(message);
		rollRate.parse(message);

		counter = message->parseInt();
	}

	void run() {
		float positionMultiplier = 4.0958748f;
		float positionX = posX / positionMultiplier;
		float positionY = posY / positionMultiplier;
		float positionZ = posZ / positionMultiplier;

		float directionX = (float)dirX / 127.f;
		float directionY = (float)dirY / 127.f;
		float directionZ = (float)dirZ / 127.f;
		float directionW = (float)dirW / 127.f;

		/*StringBuffer msg;
		msg << "short: 0x" << hex << unknownShort;
		info(msg.toString(), true);

		StringBuffer msg;
		msg << "positionX: " << posX << " positionZ:" << posZ << " positionY:" << posY;
		info(msg.toString(), true);

		StringBuffer msg;
		msg << "velA:" << velA << " velB:" << velB;
		info(msg.toString(), true);*/

		CreatureObject* object = cast<CreatureObject*>( client->getPlayer().get().get());

		if (object == NULL)
			return;

		Locker _locker(object);

		PlayerObject* ghost = object->getPlayerObject();

		if (ghost == NULL)
			return;

		if (isnan(positionX) || isnan(positionY) || isnan(positionZ))
			return;

		if (isinf(positionX) || isinf(positionY) || isinf(positionZ))
			return;

		if (ghost->isTeleporting())
			return;

		if (object->getZone() == NULL)
			return;

		if (positionX > 8000.0f || positionX < -8000.0f || positionY > 8000.0f || positionY < -8000.0f) {
			StringBuffer msg;
			msg << "position out of bounds";
			object->error(msg.toString());

			return;
		}

		ManagedReference<ShipObject*> ship = dynamic_cast<ShipObject*>(object->getParent().get().get());

		if (ship == NULL)
			return;

		Locker clocker(ship, object);

		Vector3 collisionPoint, targetPosition(positionX, positionY, positionZ);

		if (CollisionManager::checkShipCollision(ship, targetPosition, collisionPoint)) {
			//ship->teleport(ship->getPositionX(), ship->getPositionZ(), ship->getPositionY());
			//ship->info("colliding with terrain", true);
			//ship->setDirection(directionW, directionX, directionY, directionZ);
			ship->setMovementCounter(counter);

			ShipUpdateTransformCollisionMessage* msg = new ShipUpdateTransformCollisionMessage(ship);
			object->broadcastMessage(msg, true);

			PlayClientEffectObjectMessage* effect = new PlayClientEffectObjectMessage(ship, "clienteffect/space_collision.cef", "");
			object->broadcastMessage(effect, true);

			return;
		}

		ship->setMovementCounter(counter);
		ship->setPosition(positionX, positionZ, positionY);
		ship->setDirection(directionW, directionX, directionY, directionZ);
		ship->updateZone(false, false);

		object->setPosition(positionX, positionZ, positionY);
		//object->setDirection(directionW, directionX, directionY, directionZ);

		object->updateZone(false, false);

		ShipUpdateTransformMessage* msga = new ShipUpdateTransformMessage(ship, dirX, dirY, dirZ, dirW, posX, posZ, posY,
				velocity, yawRate, pitchRate, rollRate);
		object->broadcastMessage(msga, false);

		ValidatedPosition* last = ghost->getLastValidatedPosition();
		last->setPosition(positionX, positionZ, positionY);
		ghost->updateServerLastMovementStamp();
	}
};


#endif /* SHIPUPDATETRANSFORMCALLBACK_H_ */
