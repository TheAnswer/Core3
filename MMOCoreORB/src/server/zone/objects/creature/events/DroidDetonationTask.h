/*
Copyright (C) 2014 <SWGEmu>

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

#ifndef DROIDEFFECTSTASK_H_
#define DROIDEFFECTSTASK_H_

#include "server/zone/objects/creature/DroidObject.h"
#include "server/zone/objects/tangible/components/droid/DroidDetonationModuleDataComponent.h"
#include "server/zone/objects/group/GroupObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/combat/CombatManager.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
namespace events {

class DroidDetonationTask : public Task, public Logger {

private:
	ManagedReference<DroidDetonationModuleDataComponent*> module;
	ManagedReference<CreatureObject*> player;
	int detonationStep;
public:
	DroidDetonationTask(DroidDetonationModuleDataComponent* module, CreatureObject* player) : Task() {
		this->module = module;
		this->player = player;
		detonationStep = 0;
	}

	void run() {

		if( module == NULL || module->getDroidObject() == NULL ){
			return;
		}

		DroidObject* droid = module->getDroidObject();

		Locker locker(droid);
		Locker crossLocker(droid,player);
		// Check if droid is spawned
		if( droid->getLocalZone() == NULL ){  // Not outdoors

			ManagedWeakReference<SceneObject*> parent = droid->getParent();
			if( parent == NULL || !parent.get()->isCellObject() ){ // Not indoors either
				droid->removePendingTask("droid_detonation");
				return;
			}
		}
		// Droid must have power
		if( !droid->hasPower() ){
			droid->showFlyText("npc_reaction/flytext","low_power", 204, 0, 0);  // "*Low Power*"
			droid->removePendingTask("droid_detonation");
			return;
		}
		// check your the owner of the droid
		if(droid->getLinkedCreature() != player) {
			player->sendSystemMessage("@pet/droid_modules:must_be_owner_droid_bomb");
			droid->removePendingTask("droid_detonation");
			return;
		}
		if(droid->isDead()) {
			droid->removePendingTask("droid_detonation");
			return ;
		}
		if(droid->isIncapacitated() && detonationStep > 0) {
			module->stopCountDown();
			droid->showFlyText("pet/droid_modules","detonation_disabled",204,0,0);
			module->deactivate();
			droid->removePendingTask("droid_detonation");
			return;
		}
		// if droid gets incapped while started it will disable but be able to be restarted
		switch(detonationStep) {
			case 0: {
				// inital phase
				// are we already started or initializing?
				if(module->readyForDetonation()) {
					if(droid->getCooldownTimerMap()->isPast("detonation_init")) {
						if (module->countdownInProgress()) {
							player->sendSystemMessage("@pet/droid_modules:countdown_already_started");
						} else {
							player->sendSystemMessage("@pet/droid_modules:countdown_started");
							detonationStep = 1;
							module->startCountDown();
							droid->addPendingTask("droid_detonation",this,1000);
						}
					}
				} else {
					player->sendSystemMessage("@pet/droid_modules:detonation_warmup");
					droid->getCooldownTimerMap()->updateToCurrentAndAddMili("detonation_init",10000);
					module->setReadyForDetonation();
					droid->addPendingTask("droid_detonation",this,11000);
				}
				break;
			}
			case 1:{
				// 3
				droid->showFlyText("pet/droid_modules","countdown_1",204,0,0);
				detonationStep = 2;
				droid->addPendingTask("droid_detonation",this,1000);
				break;
			}
			case 2: {
				// 2
				droid->showFlyText("pet/droid_modules","countdown_2",204,0,0);
				detonationStep = 3;
				droid->addPendingTask("droid_detonation",this,1000);
				break;
			}
			case 3:{
				// 1
				droid->showFlyText("pet/droid_modules","countdown_3",204,0,0);
				detonationStep = 4;
				droid->addPendingTask("droid_detonation",this,1000);
				break;
			}
			case 4: {
				// BOOM
				int areaDamage = module->calculateDamage(droid);
				// find all valid targets in 17 m range and hit them with the damage
				CloseObjectsVector* vec = (CloseObjectsVector*) droid->getCloseObjects();
				SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;
				if (vec != NULL) {
					closeObjects.removeAll(vec->size(), 10);
					vec->safeCopyTo(closeObjects);
				} else {
					droid->info("Null closeobjects vector in CombatManager::doAreaCombatAction", true);
					droid->getZone()->getInRangeObjects(droid->getWorldPositionX(), droid->getWorldPositionY(), 40, &closeObjects, true);
				}
				for (int i = 0; i < closeObjects.size(); ++i) {
					ManagedReference<SceneObject*> object = cast<SceneObject*>(closeObjects.get(i).get());
					SceneObject* s;
					if(!object->isCreatureObject()) {
						continue;
					}
					TangibleObject* tano = cast<TangibleObject*>(object.get());
					if (object == droid) {
						continue;
					}
					if (!tano->isAttackableBy(droid) || !droid->isInRange(object,17)) {
						continue;
					}
					if (tano->isCreatureObject() && (cast<CreatureObject*>(tano))->isIncapacitated()) {
						continue;
					}
					try {
						// apply the damage to the target and send themessage
						if(CollisionManager::checkLineOfSight(object,droid)) {
							// apply the damage
							float amount = CombatManager::instance()->doDroidDetonation(droid, cast<CreatureObject*>(tano),areaDamage);
							StringIdChatParameter stringId;
							stringId.setStringId("@pet/droid_modules:hit_by_detonation");
							stringId.setDI((int)amount);
							(cast<CreatureObject*>(tano))->sendSystemMessage(stringId);
						}
					} catch (Exception& e) {
						error(e.getMessage());
					} catch (...) {
						throw;
					}
				}
				droid->removePendingTask("droid_detonation");
				// nuke the droid from the world
				ManagedReference<PetControlDevice*> petControlDevice = droid->getControlDevice().get().castTo<PetControlDevice*>();
				if (petControlDevice != NULL) {
					ManagedReference<CreatureObject*> owner = droid->getZoneServer()->getObject(droid->getCreatureLinkID()).castTo<CreatureObject*>();
					if (owner != NULL)
						petControlDevice->storeObject(owner, true);

					petControlDevice->destroyObjectFromWorld(true);
					petControlDevice->destroyObjectFromDatabase(true);
				}
				break;
			}
		}

	}

};

} // events
} // creature
} // objects
} // zone
} // server

using namespace server::zone::objects::creature::events;

#endif /*DROIDEFFECTSTASK_H_*/
