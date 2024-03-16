/*
 * PlayerZoneComponent.cpp
 *
 *  Created on: 26/05/2011
 *      Author: victor
 */

#include "PlayerZoneComponent.h"

#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/Zone.h"
#include "server/zone/SpaceZone.h"
#include "server/zone/TreeEntry.h"
#include "server/zone/objects/creature/buffs/ConcealBuff.h"

void PlayerZoneComponent::notifyInsertToZone(SceneObject* sceneObject, Zone* newZone) const {
	String zoneName = newZone->getZoneName();

	if (sceneObject->isPlayerCreature() && newZone != nullptr) {
		CreatureObject* player = sceneObject->asCreatureObject();

		if (player != nullptr) {
			PlayerObject* ghost = player->getPlayerObject();
			String zoneName = newZone->getZoneName();

			if (ghost != nullptr)
				ghost->setSavedTerrainName(zoneName);

			// Remove MaskScent state from concealed players when their buff is for a different zone
			uint32 concealCrc = STRING_HASHCODE("skill_buff_mask_scent");
			Reference<CreatureObject*> playerRef = player;

			if (player->hasBuff(concealCrc)) {
				ConcealBuff* concealBuff = cast<ConcealBuff*>(player->getBuff(concealCrc));

				if (concealBuff != nullptr) {
					Reference<ConcealBuff*> buffRef = concealBuff;
					Reference<Zone*> zoneRef = newZone;

					Core::getTaskManager()->executeTask([buffRef, playerRef, zoneRef] () {
						if (buffRef == nullptr || playerRef == nullptr || zoneRef == nullptr)
							return;

						Locker lock(playerRef);

						if (buffRef->getPlanetName() != zoneRef->getZoneName()) {
							playerRef->clearState(CreatureState::MASKSCENT, true);
						} else {
							playerRef->setState(CreatureState::MASKSCENT, true);
						}
					}, "ClearMaskStateLambda");
				}
			}

			// Ensure no space states are on players in ground zones
			Core::getTaskManager()->executeTask([playerRef] () {
				if (playerRef == nullptr)
					return;

				Locker lock(playerRef);

				playerRef->clearSpaceStates();
			}, "ClearSpaceStatesLambda");
		}
	}

	GroundZoneComponent::notifyInsertToZone(sceneObject, newZone);
}

void PlayerZoneComponent::notifyInsert(SceneObject* sceneObject, TreeEntry* entry) const {
	SceneObject* scno = static_cast<SceneObject*>( entry);

	if (scno == sceneObject)
		return;

	if (scno->isTangibleObject()) {
		TangibleObject* tano = scno->asTangibleObject();

		if (tano->isInvisible())
			return;
	}

	ManagedReference<SceneObject*> parent = scno->getParent().get();

	if (parent != nullptr) {
		return;
	}

	scno->sendTo(sceneObject, true, false);
}

void PlayerZoneComponent::notifyDissapear(SceneObject* sceneObject, TreeEntry* entry) const {
	SceneObject* scno = static_cast<SceneObject*>( entry);

	if (scno == sceneObject)
		return;

	scno->sendDestroyTo(sceneObject);
}

void PlayerZoneComponent::switchZone(SceneObject* sceneObject, const String& newTerrainName, float newPostionX, float newPositionZ, float newPositionY, uint64 parentID, bool toggleInvisibility, int playerArrangement) const {
	if (sceneObject->isPlayerCreature()) {
		CreatureObject* player = sceneObject->asCreatureObject();
		PlayerObject* ghost = player->getPlayerObject();

		ManagedReference<SceneObject*> par = sceneObject->getParent().get();

		if (par != nullptr && (par->isVehicleObject() || par->isMount())) {
			player->executeObjectControllerAction(STRING_HASHCODE("dismount"));
		}

		player->clearSpaceStates();

		if (ghost != nullptr) {
			ghost->setSavedParentID(0);

			ghost->setTeleporting(true);
			ghost->setOnLoadScreen(true);
			ghost->updateLastValidatedPosition();
			ghost->setClientLastMovementStamp(0);

			ghost->unloadSpawnedChildren();
		}

		player->notifyObservers(ObserverEventType::ZONESWITCHED, nullptr, newTerrainName.hashCode());
	}

	GroundZoneComponent::switchZone(sceneObject, newTerrainName, newPostionX, newPositionZ, newPositionY, parentID, toggleInvisibility, playerArrangement);
}

void PlayerZoneComponent::teleport(SceneObject* sceneObject, float newPositionX, float newPositionZ, float newPositionY, uint64 parentID) const {
	CreatureObject* player = nullptr;

	if (sceneObject->isPlayerCreature()) {
		player = sceneObject->asCreatureObject();
	}

	ManagedReference<SceneObject*> par = sceneObject->getParent().get();

	if (player != nullptr && par != nullptr && parentID != 0) {

		if (par->isVehicleObject() || par->isMount()) {
			player->executeObjectControllerAction(STRING_HASHCODE("dismount"));
		}
	}

	GroundZoneComponent::teleport(sceneObject, newPositionX, newPositionZ, newPositionY, parentID);

	if (player != nullptr) {
		PlayerObject* ghost = player->getPlayerObject();

		if (ghost != nullptr) {
			ghost->setTeleporting(true);
			ghost->updateLastValidatedPosition();
			ghost->setClientLastMovementStamp(0);
		}
	}
}

/**
 * Updates position of this object to the rest of in range objects
 * @pre { this object is locked}
 * @post { this object is locked, in range objects are updated with the new position }
 * @param lightUpdate if true a standalone message is sent to the in range objects
 */
void PlayerZoneComponent::updateZone(SceneObject* sceneObject, bool lightUpdate, bool sendPackets) const {
	GroundZoneComponent::updateZone(sceneObject, lightUpdate, sendPackets);

	if (sceneObject->isPlayerCreature()) {
		CreatureObject* player = sceneObject->asCreatureObject();
		PlayerObject* ghost = player->getPlayerObject();

		if (ghost != nullptr)
			ghost->setSavedParentID(0);
	}
}

void PlayerZoneComponent::updateZoneWithParent(SceneObject* sceneObject, SceneObject* newParent, bool lightUpdate, bool sendPackets) const {
	GroundZoneComponent::updateZoneWithParent(sceneObject, newParent, lightUpdate, sendPackets);

	if (sceneObject->getParent() != nullptr && sceneObject->isPlayerCreature()) {
		CreatureObject* player = sceneObject->asCreatureObject();
		PlayerObject* ghost = player->getPlayerObject();

		if (ghost != nullptr)
			ghost->setSavedParentID(sceneObject->getParentID());
	}
}
