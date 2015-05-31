/*
 * InstrumentImplementation.cpp
 *
 *  Created on: 31/10/2010
 *      Author: victor
 */

#include "server/zone/objects/tangible/Instrument.h"
#include "server/zone/objects/tangible/InstrumentObserver.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/objects/structure/StructureObject.h"
#include "server/zone/Zone.h"

bool InstrumentImplementation::canDropInstrument() {
	ManagedReference<SceneObject*> parent = getParent().get();

	if (isInQuadTree() || (parent != NULL && parent->isCellObject()))
		return false;

	return true;
}

int InstrumentImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	if (instrumentType != OMNIBOX && instrumentType != NALARGON) {
		return 1;
	}

	if (selectedID == 69) {
		if (!canDropInstrument())
			return 1;

		SortedVector<ManagedReference<Observer* > > observers = player->getObservers(ObserverEventType::POSITIONCHANGED);

		for (int i = 0; i < observers.size(); ++i) {
			Observer* observer = observers.get(i);

			if (dynamic_cast<InstrumentObserver*>(observer) != NULL) {
				//couldnt find stringi
				player->sendSystemMessage("You already dropped an instrument");
				return 1;
			}
		}

		ManagedReference<SceneObject*> playerParent = player->getParent();

		if (playerParent != NULL) {
			if (!playerParent->isCellObject()) {
				return 1;
			} else {
				CellObject* cell = cast<CellObject*>( playerParent.get());

				StructureObject* structureObject = dynamic_cast<StructureObject*>(cell->getParent().get().get());

				if (structureObject == NULL)
					return 1;

				if (!structureObject->isOnAdminList(player))
					spawnInForeignCell(player);
				else {
					spawnInAdminCell(player);
				}
			}
		} else {
			spawnOutside(player);
		}

	} else if (selectedID == 20) {
		if (getZone() == NULL)
			return 1;

		Reference<Instrument*> instrument = player->getSlottedObject("hold_r").castTo<Instrument*>();

		if (instrument != NULL) {
			player->sendSystemMessage("@performance:music_must_unequip");
			return 1;
		}

		Reference<PlayerObject*> ghost = player->getSlottedObject("ghost").castTo<PlayerObject*>();

		if (ghost == NULL)
			return 1;

		if (!ghost->hasAbility("startmusic")) {
			player->sendSystemMessage("@performance:music_lack_skill_instrument");
			return 1;
		}

		if (player->getDistanceTo(_this.getReferenceUnsafeStaticCast()) >= 5) {
			player->sendSystemMessage("@elevator_text:too_far");
		} else
			player->executeObjectControllerAction(STRING_HASHCODE("startmusic"), getObjectID(), "");
	}

	return 0;
}

void InstrumentImplementation::spawnInForeignCell(CreatureObject* player) {
	if (spawnedObject == NULL) {
		spawnedObject = ObjectManager::instance()->createObject(serverObjectCRC, 0, "sceneobjects");
		spawnedObject->setParent(NULL);
		spawnedObject->setZone(NULL);
	}

	if (spawnedObject->getZone() == NULL) {
		Instrument* instrument = cast<Instrument*>( spawnedObject.get());

		Locker locker(instrument);

		instrument->initializePosition(player->getPositionX(), player->getPositionZ(), player->getPositionY());
		instrument->setSpawnerPlayer(player);

		player->getParent().get()->transferObject(instrument, -1);
		//instrument->insertToZone(player->getZone());
		//player->getZone()->transferObject(instrument, -1, true);

		ManagedReference<InstrumentObserver*> posObserver = new InstrumentObserver(instrument);
		player->registerObserver(ObserverEventType::POSITIONCHANGED, posObserver);
		player->registerObserver(ObserverEventType::OBJECTREMOVEDFROMZONE, posObserver);
	} else {
		spawnedObject->teleport(player->getPositionX(), player->getPositionZ(), player->getPositionY(), player->getParentID());
	}

	spawnerPlayer = player;
}

void InstrumentImplementation::spawnOutside(CreatureObject* player) {
	if (spawnedObject == NULL) {
		spawnedObject = ObjectManager::instance()->createObject(serverObjectCRC, 0, "sceneobjects");
		spawnedObject->setParent(NULL);
		spawnedObject->setZone(NULL);
	}

	if (spawnedObject->getZone() == NULL) {
		Instrument* instrument = cast<Instrument*>( spawnedObject.get());

		Locker locker(instrument);

		instrument->initializePosition(player->getPositionX(), player->getPositionZ(), player->getPositionY());
		instrument->setSpawnerPlayer(player);
		//instrument->insertToZone(player->getZone());
		player->getZone()->transferObject(instrument, -1, true);

		ManagedReference<InstrumentObserver*> posObserver = new InstrumentObserver(instrument);
		player->registerObserver(ObserverEventType::POSITIONCHANGED, posObserver);
		player->registerObserver(ObserverEventType::OBJECTREMOVEDFROMZONE, posObserver);
	} else {
		spawnedObject->teleport(player->getPositionX(), player->getPositionZ(), player->getPositionY(), player->getParentID());
	}

	spawnerPlayer = player;
}

void InstrumentImplementation::spawnInAdminCell(CreatureObject* player) {
	StringBuffer arguments;
	arguments << player->getParent().get()->getObjectID() << " -1 " << player->getPositionX() << " " << player->getPositionZ() << " " << player->getPositionY();

	UnicodeString uni = arguments.toString();

	player->executeObjectControllerAction(STRING_HASHCODE("transferitemmisc"), _this.getReferenceUnsafeStaticCast()->getObjectID(), uni);

	spawnerPlayer = NULL;

	if (spawnedObject != NULL) {
		spawnedObject->destroyObjectFromWorld(true);
		spawnedObject = NULL;
	}
}
