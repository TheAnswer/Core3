/*
 * MinefieldAttackTask.h
 *
 *  Created on: 20/12/2012
 *      Author: victor
 */

#ifndef MINEFIELDATTACKTASK_H_
#define MINEFIELDATTACKTASK_H_

#include "engine/engine.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/tangible/UpdatePVPStatusMessage.h"
class MinefieldAttackTask : public Task {
	ManagedReference<SceneObject*> sceneObject;
	ManagedReference<CreatureObject*> player;
	//ManagedReference<WeaponObject*> mine;

public:
	MinefieldAttackTask(SceneObject* scene, CreatureObject* player) {
		sceneObject = scene;
		this->player = player;
	}

	void run() {
		if(sceneObject == NULL)
			return;

		Locker locker(sceneObject);

		if(sceneObject->getContainerObjectsSize() <= 0)
			return;

		DataObjectComponentReference* ref = sceneObject->getDataObjectComponent();
		if(ref == NULL){
			return;
		}

		MinefieldDataComponent* mineData = cast<MinefieldDataComponent*>(ref->get());

		if(mineData == NULL || !mineData->canExplode()){
			return;
		}

		//Locker clocker(player, sceneObject);

		ManagedReference<WeaponObject*> weapon = sceneObject->getContainerObject(0).castTo<WeaponObject*>();

		if (weapon == NULL || sceneObject->getZone() == NULL)
			return;

		PlayClientEffectLoc* explodeLoc = new PlayClientEffectLoc("clienteffect/lair_damage_heavy.cef", sceneObject->getZone()->getZoneName(), sceneObject->getPositionX(), sceneObject->getPositionZ(), sceneObject->getPositionY());
		sceneObject->broadcastMessage(explodeLoc, false);

		sceneObject->removeObject(weapon,NULL,true);

		if(sceneObject->getZoneServer() != NULL){
			ManagedReference<ObjectController*> objectController = sceneObject->getZoneServer()->getObjectController();
			QueueCommand* command = objectController->getQueueCommand(STRING_HASHCODE("minefieldattack"));

			if(command != NULL){
				CombatQueueCommand* combatCommand = cast<CombatQueueCommand*>(command);
				if(combatCommand != NULL){
					CombatManager::instance()->doCombatAction(sceneObject.castTo<TangibleObject*>(), weapon, player, combatCommand);
				}
			}

		}

		mineData->updateCooldown(weapon->getAttackSpeed());
		mineData->setMaxRange(weapon->getMaxRange());

		if(sceneObject->getContainerObjectsSize() <= 0){
			TangibleObject* tano = sceneObject.castTo<TangibleObject*>();
			if(tano == NULL)
				return;

			tano->setPvpStatusBitmask(tano->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE);
		}

		//clocker.release();
		locker.release();

		Locker lockerw(weapon);

		weapon->destroyObjectFromWorld(true);
		weapon->destroyObjectFromDatabase(true);

	}

};


#endif /* MINEFIELDATTACKTASK_H_ */
