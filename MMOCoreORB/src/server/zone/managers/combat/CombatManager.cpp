/*
 * CombatManager.cpp
 *
 *  Created on: 24/02/2010
 *      Author: victor
 */

#include "CombatManager.h"
#include "CreatureAttackData.h"
#include "DefenderHitList.h"
#include "server/zone/objects/scene/variables/DeltaVector.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "templates/params/creature/CreatureState.h"
#include "server/zone/objects/creature/commands/CombatQueueCommand.h"
#include "templates/params/creature/CreatureAttribute.h"
#include "server/zone/packets/object/CombatSpam.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/packets/tangible/UpdatePVPStatusMessage.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/visibility/VisibilityManager.h"
#include "server/zone/managers/creature/LairObserver.h"
#include "server/zone/managers/reaction/ReactionManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/installation/components/TurretDataComponent.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/installation/InstallationObject.h"
#include "server/zone/packets/object/ShowFlyText.h"
#include "server/zone/managers/frs/FrsManager.h"
#include "server/zone/objects/intangible/PetControlDevice.h"

#define COMBAT_SPAM_RANGE 85 // Range at which players will see Combat Log Info

/*
* Notes:
* Every player that uses an attack ability, including peace, is considered the attacker in a CombatManager instance.
*
* Each Attacker has their Defender List stored on their Object.
* Peace will clear a Object's defender list, but they will not be able to exit Combat if checks are not passed.
*
*/

// Sets attackers mainDefender and puts both in combat
bool CombatManager::startCombat(CreatureObject* attacker, TangibleObject* defender, bool lockDefender, bool allowIncapTarget) const {
	if (attacker == defender) {
		return false;
	}

	if (attacker->getZone() == nullptr || defender->getZone() == nullptr) {
		return false;
	}

	if (attacker->isRidingMount()) {
		ManagedReference<CreatureObject*> parent = attacker->getParent().get().castTo<CreatureObject*>();

		if (parent == nullptr || !parent->isMount()) {
			return false;
		}

		if (parent->hasBuff(STRING_HASHCODE("gallop"))) {
			return false;
		}
	}

	if (attacker->hasRidingCreature()) {
		return false;
	}

	if (!defender->isAttackableBy(attacker)) {
		return false;
	}

	if (attacker->isPlayerCreature() && attacker->getPlayerObject()->isAFK()) {
		return false;
	}

	CreatureObject* creo = defender->asCreatureObject();
	if (creo != nullptr && creo->isIncapacitated() && creo->isFeigningDeath() == false) {
		if (allowIncapTarget) {
			attacker->clearState(CreatureState::PEACE);
			return true;
		}

		return false;
	}

	attacker->clearState(CreatureState::PEACE);

	if (attacker->isPlayerCreature() && !attacker->hasDefender(defender)) {
		ManagedReference<WeaponObject*> weapon = attacker->getWeapon();

		if (weapon != nullptr && weapon->isJediWeapon()) {
			VisibilityManager::instance()->increaseVisibility(attacker, 25);
		}
	}

	Locker clocker(defender, attacker);

	if (creo != nullptr && creo->isPlayerCreature() && !creo->hasDefender(attacker)) {
		ManagedReference<WeaponObject*> weapon = creo->getWeapon();

		if (weapon != nullptr && weapon->isJediWeapon()) {
			VisibilityManager::instance()->increaseVisibility(creo, 25);
		}
	}

	attacker->setCombatState();
	defender->setCombatState();

	attacker->setDefender(defender);

	if (defender->isCreatureObject()) {
		ThreatMap* defenderThreatMap = defender->getThreatMap();

		if (defenderThreatMap != nullptr) {
			defenderThreatMap->addDamage(attacker, 0);
		}
	}

	return true;
}

// Called when creature attempts to peace out of combat -- Defender List is cleared
bool CombatManager::attemptPeace(CreatureObject* attacker) const {
	attacker->removeDefenders();
	attacker->setState(CreatureState::PEACE);

	ThreatMap* threatMap = attacker->getThreatMap();

	if (threatMap != nullptr) {
		if (threatMap->size() == 0) {
			attacker->clearCombatState(false);
			return true;
		}

		for (int i = 0; i < threatMap->size(); i++) {
			TangibleObject* threatTano = threatMap->elementAt(i).getKey();

			if (threatTano == nullptr || threatTano == attacker) {
				continue;
			}

			if (attacker->isInRange(threatTano, 128.f) && threatTano->getMainDefender() == attacker) {
				return true;
			}
		}

		attacker->clearCombatState(false);
	}
	return true;
}

// Called for AiAgents to break their combat state
void CombatManager::forcePeace(CreatureObject* attacker) const {
	fatal(attacker->isLockedByCurrentThread(), "attacker must be locked");

	const DeltaVector<ManagedReference<SceneObject*>>* defenderList = attacker->getDefenderList();

	for (int i = 0; i < defenderList->size(); ++i) {
		ManagedReference<SceneObject*> object = defenderList->getSafe(i);

		if (object == nullptr || !object->isTangibleObject())
			continue;

		TangibleObject* defender = cast<TangibleObject*>(object.get());

		Locker clocker(defender, attacker);

		if (defender->hasDefender(attacker)) {
			attacker->removeDefender(defender);
			defender->removeDefender(attacker);
		} else {
			attacker->removeDefender(defender);
		}

		--i;

		clocker.release();
	}

	attacker->clearCombatState(false);
	attacker->setState(CreatureState::PEACE);
}

/*
*
*	Start Combat Action
*
*/

/*
	CreO attacker -- doCombatAction

*/

int CombatManager::doCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, const CreatureAttackData& data) const {
	debug("entering doCombat action with data");

	if (data.getCommand() == nullptr)
		return -3;

	if (!startCombat(attacker, defenderObject, true, data.getHitIncapTarget()))
		return -1;

	debug("past start combat");

	if (!applySpecialAttackCost(attacker, weapon, data)) {
		return -2;
	}

	debug("past special attack cost");

	SortedVector<DefenderHitList*> targetDefenders;

	int damage = 0;
	bool shouldGcwCrackdownTef = false, shouldGcwTef = false, shouldBhTef = false;

	damage = doTargetCombatAction(attacker, weapon, defenderObject, &targetDefenders, data, &shouldGcwCrackdownTef, &shouldGcwTef, &shouldBhTef);

	if (data.getCommand()->isAreaAction() || data.getCommand()->isConeAction()) {
		Reference<SortedVector<ManagedReference<TangibleObject*>>*> areaDefenders = getAreaTargets(attacker, weapon, defenderObject, data);

		while (areaDefenders->size() > 0) {
			int areaDam = 0;

			for (int i = areaDefenders->size() - 1; i >= 0; i--) {
				TangibleObject* tano = areaDefenders->get(i);
				if (tano == attacker || tano == nullptr) {
					areaDefenders->remove(i);
					continue;
				}

				if (!tano->tryWLock()) {
					continue;
				}

				areaDam += doTargetCombatAction(attacker, weapon, areaDefenders->get(i), &targetDefenders, data, &shouldGcwCrackdownTef, &shouldGcwTef, &shouldBhTef);
				areaDefenders->remove(i);

				tano->unlock();
			}

			if (areaDam > 0)
				damage += areaDam;

			attacker->unlock();
			Thread::yield();
			attacker->wlock(true);
		}
	}

	if (damage > 0) {
		attacker->updateLastSuccessfulCombatAction();

		if (attacker->isPlayerCreature() && data.getCommandCRC() != STRING_HASHCODE("attack")) {
			weapon->decay(attacker);
		}

		// Decreases the powerup once per successful attack
		if (!data.isForceAttack()) {
			weapon->decreasePowerupUses(attacker);
		}
	}

	// Broadcast CombatSpam and CombatAction packets now that the attack is complete
	if (damage >= 0) {
		finalCombatSpam(attacker, weapon, targetDefenders, data);
		broadcastCombatAction(attacker, weapon, targetDefenders, data);
	}

	// Update PvP TEF Duration
	if (shouldGcwCrackdownTef || shouldGcwTef || shouldBhTef) {
		ManagedReference<CreatureObject*> attackingCreature = nullptr;

		if (attacker->isPet()) {
			ManagedReference<PetControlDevice*> controlDevice = attacker->getControlDevice().get().castTo<PetControlDevice*>();

			if (controlDevice != nullptr) {
				ManagedReference<SceneObject*> lastCommander = controlDevice->getLastCommander().get();

				if (lastCommander != nullptr && lastCommander->isCreatureObject()) {
					attackingCreature = lastCommander->asCreatureObject();
				} else {
					attackingCreature = attacker->getLinkedCreature();
				}
			}
		} else {
			attackingCreature = attacker;
		}

		if (attackingCreature != nullptr) {
			PlayerObject* ghost = attackingCreature->getPlayerObject();

			if (ghost != nullptr) {
				Locker olocker(attackingCreature, attacker);
				ghost->updateLastCombatActionTimestamp(shouldGcwCrackdownTef, shouldGcwTef, shouldBhTef);
			}
		}
	}

	return damage;
}

/*

	CreO attacker -- doTargetCombatAction

*/

int CombatManager::doTargetCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* tano, SortedVector<DefenderHitList*>* targetDefenders, const CreatureAttackData& data, bool* shouldGcwCrackdownTef, bool* shouldGcwTef, bool* shouldBhTef) const {
	int damage = 0;

	Locker clocker(tano, attacker);

	if (!tano->isAttackableBy(attacker)) {
		return -1;
	}

	if (targetDefenders == nullptr) {
		return -1;
	}

	DefenderHitList* hitList = new DefenderHitList();

	if (hitList == nullptr) {
		return -1;
	}

	// Add DefenderHitList to the targetDefenders Vector and set the defender to that list
	hitList->setDefender(tano);
	targetDefenders->add(hitList);

	if (tano->isCreatureObject()) {
		CreatureObject* defender = tano->asCreatureObject();

		if (defender->getWeapon() == nullptr) {
			return -1;
		}

		damage = creoTargetCombatAction(attacker, weapon, defender, hitList, data, shouldGcwCrackdownTef, shouldGcwTef, shouldBhTef);
	} else {
		int poolsToDamage = calculatePoolsToDamage(data.getPoolsToDamage());

		damage = applyDamage(attacker, weapon, tano, hitList, poolsToDamage, data);

		// No Accuracy / Defense Calculation for TanO defender. setHit to HIT value.
		hitList->setHit(HIT);
	}

	if (damage > 0 && tano->isAiAgent()) {
		AiAgent* aiAgent = cast<AiAgent*>(tano);
		bool help = false;

		for (int i = 0; i < 9; i += 3) {
			if (aiAgent->getHAM(i) < (aiAgent->getMaxHAM(i) / 2)) {
				help = true;
				break;
			}
		}

		if (help)
			aiAgent->sendReactionChat(ReactionManager::HELP);
		else
			aiAgent->sendReactionChat(ReactionManager::HIT);
	}

	if (damage > -1 && attacker->isAiAgent()) {
		AiAgent* aiAgent = cast<AiAgent*>(attacker);
		aiAgent->sendReactionChat(ReactionManager::HITTARGET);
	}

	return damage;
}

/*

	CreO attacker && CreO Defender -- doTargetCombatAction

*/

int CombatManager::creoTargetCombatAction(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, DefenderHitList* targetHitList, const CreatureAttackData& data, bool* shouldGcwCrackdownTef, bool* shouldGcwTef, bool* shouldBhTef) const {
	if (targetHitList == nullptr) {
		return 0;
	}

	if (defender->isEntertaining()) {
		defender->stopEntertaining();
	}

	int hitVal = HIT;
	uint8 hitLocation = 0;

	float damageMultiplier = data.getDamageMultiplier();

	int damage = 0;

	if (damageMultiplier != 0) {
		damage = calculateDamage(attacker, weapon, defender, data) * damageMultiplier;
		targetHitList->setInitialDamage(damage);
	}

	damageMultiplier = 1.0f;

	if (!data.isStateOnlyAttack()) {
		hitVal = getHitChance(attacker, defender, weapon, data, damage, data.getAccuracyBonus() + attacker->getSkillMod(data.getCommand()->getAccuracySkillMod()));
	}

	switch (hitVal) {
	case MISS:
		doMiss(attacker, weapon, defender, damage);
		damageMultiplier = 0.0f;
		break;
	case BLOCK:
		doBlock(attacker, weapon, defender, damage);
		damageMultiplier = 0.5f;
		break;
	case DODGE:
		doDodge(attacker, weapon, defender, damage);
		damageMultiplier = 0.0f;
		break;
	case COUNTER: {
		doCounterAttack(attacker, weapon, defender, damage);
		if (!defender->hasState(CreatureState::PEACE))
			defender->executeObjectControllerAction(STRING_HASHCODE("attack"), attacker->getObjectID(), "");
		damageMultiplier = 0.0f;
		break;
	}
	case RICOCHET:
		damageMultiplier = 0.0f;
		break;
	default:
		break;
	}

	// If it's a state only attack (intimidate, warcry, wookiee roar) don't apply dots or break combat delays
	if (!data.isStateOnlyAttack() && hitVal != MISS) {
		if (defender->hasAttackDelay()) {
			defender->removeAttackDelay();
		}

		if (damageMultiplier != 0 && damage != 0) {
			int poolsToDamage = calculatePoolsToDamage(data.getPoolsToDamage());
			// TODO: animations are probably determined by which pools are damaged (high, mid, low, combos, etc)
			int unmitDamage = damage;

			damage = applyDamage(attacker, weapon, defender, targetHitList, damage, damageMultiplier, poolsToDamage, hitLocation, data);

			if (defender->isDead() || defender->isIncapacitated()) {
				// This broadcasts the dead or incapacitated postures immediately
				defender->updatePostures(true);
			} else {
				applyDots(attacker, defender, data, damage, unmitDamage, poolsToDamage);

				// Prevents weapon dots from applying with offensive Force powers
				if (!data.isForceAttack()) {
					applyWeaponDots(attacker, defender, weapon);
				}
			}
		}
	}

	if (hitVal != MISS) {
		checkForTefs(attacker, defender, shouldGcwCrackdownTef, shouldGcwTef, shouldBhTef);
	}

	// Set DefenderHitList values to be used for CombatSpam and CombatAction broadcasts
	targetHitList->setHit(hitVal);
	targetHitList->setDamageMultiplier(damageMultiplier);
	targetHitList->setHitLocation(hitLocation);

	return damage;
}

/*
	TanO attacker -- doCombatAction

*/

int CombatManager::doCombatAction(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* defender, const CombatQueueCommand* command) const {
	if (command == nullptr)
		return -3;

	const CreatureAttackData data = CreatureAttackData("", command, defender->getObjectID());
	int damage = 0;

	SortedVector<DefenderHitList*> targetDefenders;

	if (weapon != nullptr) {
		damage = doTargetCombatAction(attacker, weapon, defender, &targetDefenders, data);

		if (data.getCommand()->isAreaAction() || data.getCommand()->isConeAction()) {
			Reference<SortedVector<ManagedReference<TangibleObject*>>*> areaDefenders = getAreaTargets(attacker, weapon, defender, data);

			for (int i = 0; i < areaDefenders->size(); i++) {
				damage += doTargetCombatAction(attacker, weapon, areaDefenders->get(i), &targetDefenders, data);
			}
		}
	}

	// Send out CombatSpam broadcast now that attack is complete. TanO attackers CombatAction packets are sent out in tanoTargetCombatAction
	finalCombatSpam(attacker, weapon, targetDefenders, data);

	return damage;
}

/*

	TanO attacker -- doTargetCombatAction

*/

int CombatManager::doTargetCombatAction(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* tano, SortedVector<DefenderHitList*>* targetDefenders, const CreatureAttackData& data) const {
	int damage = 0;

	Locker clocker(tano, attacker);

	DefenderHitList* hitList = new DefenderHitList();

	if (hitList == nullptr) {
		return 0;
	}

	// Create DefenderHitList for the defender and add it to targetDefenders Vector
	targetDefenders->add(hitList);
	hitList->setDefender(tano);

	if (tano->isCreatureObject()) {
		CreatureObject* defenderObject = tano->asCreatureObject();

		if (defenderObject->getWeapon() != nullptr) {
			damage = tanoTargetCombatAction(attacker, weapon, defenderObject, hitList, data);
		}
	} else {
		// TODO: Implement TanO vs TanO damage -- This currently has no use
	}

	return damage;
}

/*

	TanO attacker && CreO Defender -- doTargetCombatAction

*/

int CombatManager::tanoTargetCombatAction(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defenderObject, DefenderHitList* targetHitList, const CreatureAttackData& data) const {
	if (defenderObject == nullptr || !defenderObject->isAttackableBy(attacker))
		return 0;

	if (defenderObject->isEntertaining()) {
		defenderObject->stopEntertaining();
	}

	float damageMultiplier = data.getDamageMultiplier();

	int damage = 0;

	if (damageMultiplier != 0) {
		damage = calculateDamage(attacker, weapon, defenderObject, data) * damageMultiplier;
	}

	damageMultiplier = 1.0f;
	int hitVal = getHitChance(attacker, defenderObject, weapon, data, damage, data.getAccuracyBonus());

	uint8 hitLocation = 0;

	switch (hitVal) {
	case MISS:
		doMiss(attacker, weapon, defenderObject, damage);
		damageMultiplier = 0.0f;
		break;
	case HIT:
		break;
	case BLOCK:
		doBlock(attacker, weapon, defenderObject, damage);
		damageMultiplier = 0.5f;
		break;
	case DODGE:
		doDodge(attacker, weapon, defenderObject, damage);
		damageMultiplier = 0.0f;
		break;
	case COUNTER:
		doCounterAttack(attacker, weapon, defenderObject, damage);
		if (!defenderObject->hasState(CreatureState::PEACE)) {
			defenderObject->executeObjectControllerAction(STRING_HASHCODE("attack"), attacker->getObjectID(), "");
		}
		damageMultiplier = 0.0f;
		break;
	case RICOCHET:
		damageMultiplier = 0.0f;
		break;
	default:
		break;
	}

	if (hitVal != MISS) {
		int poolsToDamage = calculatePoolsToDamage(data.getPoolsToDamage());

		if (poolsToDamage == 0) {
			return 0;
		}

		if (defenderObject->hasAttackDelay()) {
			defenderObject->removeAttackDelay();
		}

		if (damageMultiplier != 0 && damage != 0) {
			damage = applyDamage(attacker, weapon, defenderObject, targetHitList, damage, damageMultiplier, poolsToDamage, hitLocation, data);
		}

		if (defenderObject->isDead() || defenderObject->isIncapacitated()) {
			// This broadcasts the dead or incapacitated postures immediately
			defenderObject->updatePostures(true);
		}
	} else {
		damage = 0;
	}

	defenderObject->updatePostures(false);

	// Set DefenderHitList values to be used for CombatSpam broadcasts
	targetHitList->setHit(hitVal);
	targetHitList->setInitialDamage(damage);
	targetHitList->setDamageMultiplier(damageMultiplier);
	targetHitList->setHitLocation(hitLocation);

	uint32 animationCRC = data.getCommand()->getAnimation(attacker, defenderObject, weapon, hitLocation, damage).hashCode();

	CombatAction* combatAction = new CombatAction(attacker, defenderObject, animationCRC, hitVal, CombatManager::DEFAULTTRAIL);
	attacker->broadcastMessage(combatAction, true);

	return damage;
}

/*
	Broadcast CombatAction Packets
	-- Handles Animations

*/

void CombatManager::broadcastCombatAction(CreatureObject* attacker, WeaponObject* weapon, SortedVector<DefenderHitList*> targetDefenders, const CreatureAttackData& data) const {
	if (attacker == nullptr) {
		return;
	}

	DefenderHitList* hitList = targetDefenders.get(0);

	if (hitList != nullptr) {
		TangibleObject* defenderObject = hitList->getDefender();

		if (defenderObject != nullptr) {
			const String& animation = data.getCommand()->getAnimation(attacker, defenderObject, weapon, hitList->getHitLocation(), hitList->getInitialDamage());

			uint32 animationCRC = 0;

			if (!animation.isEmpty()) {
				animationCRC = animation.hashCode();
			}

			fatal(animationCRC != 0, "animationCRC is 0");

			uint64 weaponID = weapon->getObjectID();

			CombatAction* combatAction = new CombatAction(attacker, targetDefenders, animationCRC, data.getTrails(), weaponID);
			attacker->broadcastMessage(combatAction, true);
		}
	}

	if (data.changesAttackerPosture()) {
		attacker->updatePostures(false);
	}

	const String& effect = data.getCommand()->getEffectString();

	if (!effect.isEmpty()) {
		attacker->playEffect(effect);
	}
}

void CombatManager::finalCombatSpam(TangibleObject* attacker, WeaponObject* weapon, SortedVector<DefenderHitList*> targetDefenders, const CreatureAttackData& data) const {
	if (attacker == nullptr) {
		return;
	}

	Zone* zone = attacker->getZone();

	if (zone == nullptr) {
		return;
	}

	CloseObjectsVector* vec = (CloseObjectsVector*)attacker->getCloseObjects();
	SortedVector<QuadTreeEntry*> closeObjects;

	if (vec != nullptr) {
		closeObjects.removeAll(vec->size(), 10);
		vec->safeCopyReceiversTo(closeObjects, CloseObjectsVector::PLAYERTYPE);
	} else {
#ifdef COV_DEBUG
		info("Null closeobjects vector in CombatManager::finalCombatSpam", true);
#endif
		zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), COMBAT_SPAM_RANGE, &closeObjects, true);
	}

	for (int i = 0; i < targetDefenders.size(); ++i) {
		DefenderHitList* hitList = targetDefenders.get(i);

		if (hitList == nullptr) {
			continue;
		}

		TangibleObject* defender = hitList->getDefender();

		// Combat Spam
		if (defender != nullptr) {
			int jediMitigation = hitList->getJediMitigation();
			int foodMit = hitList->getFoodMitigation();
			int broadcastDamage = hitList->getInitialDamage() - jediMitigation - foodMit;
			int hit = hitList->getHit();

			// Initial attack combat spam
			if (!data.isStateOnlyAttack()) {
				if (hit == RICOCHET || hit == DODGE) {
					data.getCommand()->sendAttackCombatSpam(attacker, defender, hit, hitList->getInitialDamage(), data);
				} else {
					data.getCommand()->sendAttackCombatSpam(attacker, defender, hit, broadcastDamage, data);
				}
			}

			if (defender->isCreatureObject()) {
				CreatureObject* defenderCreo = defender->asCreatureObject();

				// Mitigation Combat spam
				if (defenderCreo != nullptr) {
					int feedbackDmg = hitList->getForceFeedback();
					int forceAbsorb = hitList->getForceAbsorb();
					int psgDmgAbsorbed = hitList->getPsgMitigation();
					int armorDmgAbsorbed = hitList->getArmorMitigation();

					if (jediMitigation > 0 && !data.isForceAttack()) {
						sendMitigationCombatSpam(defenderCreo, nullptr, jediMitigation, FORCEARMOR);
					} else if (jediMitigation > 0) {
						sendMitigationCombatSpam(defenderCreo, nullptr, jediMitigation, FORCESHIELD);
					}

					if (feedbackDmg > 0) {
						broadcastCombatSpam(defender, attacker, nullptr, feedbackDmg, "cbt_spam", "forcefeedback_hit", 1);
					}

					if (forceAbsorb > 0) {
						sendMitigationCombatSpam(defenderCreo, nullptr, forceAbsorb, FORCEABSORB);
					}

					ManagedReference<ArmorObject*> psg = getPSGArmor(defenderCreo);

					if (psgDmgAbsorbed > 0) {
						sendMitigationCombatSpam(defenderCreo, psg, psgDmgAbsorbed, PSG);
					}

					ManagedReference<ArmorObject*> armor = nullptr;
					armor = getArmorObject(defenderCreo, hitList->getHitLocation());

					if (armor != nullptr && armorDmgAbsorbed > 0) {
						sendMitigationCombatSpam(defenderCreo, armor, armorDmgAbsorbed, ARMOR);
					}

					if (!defenderCreo->isIncapacitated() && !defenderCreo->isDead()) {
						if (attacker->isCreatureObject() && hit == HIT) {
							applyStates(attacker->asCreatureObject(), defenderCreo, hitList, data);
						}

						woundCreatureTarget(defenderCreo, weapon, hitList->getPoolsToWound());
					}

					if (foodMit > 0) {
						sendMitigationCombatSpam(defenderCreo, weapon, foodMit, FOOD);
					}
				}
			}
		}
	}
}

// broadcast CombatSpam packets
void CombatManager::broadcastCombatSpam(TangibleObject* attacker, TangibleObject* defender, TangibleObject* item, int damage, const String& file, const String& stringName, byte color) const {
	if (attacker == nullptr)
		return;

	Zone* zone = attacker->getZone();

	if (zone == nullptr)
		return;

	CloseObjectsVector* vec = (CloseObjectsVector*)attacker->getCloseObjects();
	SortedVector<QuadTreeEntry*> closeObjects;

	if (vec != nullptr) {
		closeObjects.removeAll(vec->size(), 10);
		vec->safeCopyReceiversTo(closeObjects, CloseObjectsVector::PLAYERTYPE);
	} else {
#ifdef COV_DEBUG
		info("Null closeobjects vector in CombatManager::broadcastCombatSpam", true);
#endif
		zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), COMBAT_SPAM_RANGE, &closeObjects, true);
	}

	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* object = static_cast<SceneObject*>(closeObjects.get(i));

		if (object->isPlayerCreature() && attacker->isInRange(object, COMBAT_SPAM_RANGE)) {
			CreatureObject* receiver = static_cast<CreatureObject*>(object);
			CombatSpam* spam = new CombatSpam(attacker, defender, receiver, item, damage, file, stringName, color);
			receiver->sendMessage(spam);
		}
	}
}

void CombatManager::sendMitigationCombatSpam(CreatureObject* defender, TangibleObject* item, uint32 damage, int type) const {
	if (defender == nullptr || !defender->isPlayerCreature())
		return;

	int color = 0; // text color
	String file = "";
	String stringName = "";

	switch (type) {
	case PSG:
		color = 1; // green, confirmed
		file = "cbt_spam";
		stringName = "shield_damaged";
		break;
	case FORCESHIELD:
		color = 1; // green, unconfirmed
		file = "cbt_spam";
		stringName = "forceshield_hit";
		item = nullptr;
		break;
	case FORCEFEEDBACK:
		color = 2; // red, confirmed
		file = "cbt_spam";
		stringName = "forcefeedback_hit";
		item = nullptr;
		break;
	case FORCEABSORB:
		color = 0; // white, unconfirmed
		file = "cbt_spam";
		stringName = "forceabsorb_hit";
		item = nullptr;
		break;
	case FORCEARMOR:
		color = 1; // green, confirmed
		file = "cbt_spam";
		stringName = "forcearmor_hit";
		item = nullptr;
		break;
	case ARMOR:
		color = 1; // green, confirmed
		file = "cbt_spam";
		stringName = "armor_damaged";
		break;
	case FOOD:
		color = 0; // white, confirmed
		file = "combat_effects";
		stringName = "mitigate_damage";
		item = nullptr;
		break;
	default:
		break;
	}

	CombatSpam* spam = new CombatSpam(defender, nullptr, defender, item, damage, file, stringName, color);
	defender->sendMessage(spam);
}

/*
*
*	Other Combat Functions Below
*
*/

// Get Attackers Area Targets
Reference<SortedVector<ManagedReference<TangibleObject*>>*> CombatManager::getAreaTargets(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, const CreatureAttackData& data) const {
	float creatureVectorX = attacker->getPositionX();
	float creatureVectorY = attacker->getPositionY();

	float directionVectorX = defenderObject->getPositionX() - creatureVectorX;
	float directionVectorY = defenderObject->getPositionY() - creatureVectorY;

	Reference<SortedVector<ManagedReference<TangibleObject*>>*> defenders = new SortedVector<ManagedReference<TangibleObject*>>();

	Zone* zone = attacker->getZone();

	if (zone == nullptr)
		return defenders;

	PlayerManager* playerManager = zone->getZoneServer()->getPlayerManager();

	int damage = 0;

	int range = data.getAreaRange();

	if (data.getCommand()->isConeAction()) {
		int coneRange = data.getConeRange();

		if (coneRange > -1) {
			range = coneRange;
		} else {
			range = data.getRange();
		}
	}

	if (range < 0) {
		range = weapon->getMaxRange();
	}

	if (data.isSplashDamage())
		range += data.getRange();

	if (weapon->isThrownWeapon() || weapon->isHeavyWeapon())
		range = weapon->getMaxRange() + data.getAreaRange();

	try {
		// zone->rlock();

		CloseObjectsVector* vec = (CloseObjectsVector*)attacker->getCloseObjects();

		SortedVector<QuadTreeEntry*> closeObjects;

		if (vec != nullptr) {
			closeObjects.removeAll(vec->size(), 10);
			vec->safeCopyTo(closeObjects);
		} else {
#ifdef COV_DEBUG
			attacker->info("Null closeobjects vector in CombatManager::getAreaTargets", true);
#endif
			zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), 128, &closeObjects, true);
		}

		for (int i = 0; i < closeObjects.size(); ++i) {
			SceneObject* object = static_cast<SceneObject*>(closeObjects.get(i));

			TangibleObject* tano = object->asTangibleObject();
			CreatureObject* creo = object->asCreatureObject();

			if (tano == nullptr) {
				continue;
			}

			if (object == attacker || object == defenderObject) {
				// error("object is attacker");
				continue;
			}

			if (!tano->isAttackableBy(attacker)) {
				// error("object is not attackable");
				continue;
			}

			uint64 tarParentID = object->getParentID();

			if (attacker->isPlayerCreature() && tarParentID != 0 && attacker->getParentID() != tarParentID) {
				Reference<CellObject*> targetCell = object->getParent().get().castTo<CellObject*>();
				CreatureObject* attackerCreO = attacker->asCreatureObject();

				if (attackerCreO != nullptr && targetCell != nullptr) {
					ManagedReference<SceneObject*> parentSceneObject = targetCell->getParent().get();

					if (parentSceneObject != nullptr) {
						BuildingObject* building = parentSceneObject->asBuildingObject();

						if (building != nullptr && !building->isAllowedEntry(attackerCreO)) {
							continue;
						}
					}

					const ContainerPermissions* perms = targetCell->getContainerPermissions();

					// This portion of the check is specific for locked dungeons doors since they do not inherit perms from parent
					if (!perms->hasInheritPermissionsFromParent() && (attacker->getRootParent() == object->getRootParent())) {
						if (!targetCell->checkContainerPermission(attackerCreO, ContainerPermissions::WALKIN)) {
							continue;
						}
					}
				}
			}

			if (attacker->getWorldPosition().distanceTo(object->getWorldPosition()) - attacker->getTemplateRadius() - object->getTemplateRadius() > range) {
				// error("not in range " + String::valueOf(range));
				continue;
			}

			if (data.isSplashDamage() || weapon->isThrownWeapon() || weapon->isHeavyWeapon()) {
				if (defenderObject->getWorldPosition().distanceTo(tano->getWorldPosition()) - tano->getTemplateRadius() > data.getAreaRange())
					continue;
			}

			if (creo != nullptr && creo->isFeigningDeath() == false && creo->isIncapacitated()) {
				// error("object is incapacitated");
				continue;
			}

			if (data.getCommand()->isConeAction() && !checkConeAngle(tano, data.getConeAngle(), creatureVectorX, creatureVectorY, directionVectorX, directionVectorY)) {
				// error("object is not in cone angle");
				continue;
			}

			// zone->runlock();

			try {
				if (!(weapon->isThrownWeapon()) && !(data.isSplashDamage()) && !(weapon->isHeavyWeapon())) {
					if (CollisionManager::checkLineOfSight(object, attacker)) {
						defenders->put(tano);
						attacker->addDefender(tano);
					}
				} else {
					if (CollisionManager::checkLineOfSight(object, defenderObject)) {
						defenders->put(tano);
						attacker->addDefender(tano);
					}
				}
			} catch (Exception& e) {
				error(e.getMessage());
			} catch (...) {
				// zone->rlock();

				throw;
			}

			//			zone->rlock();
		}

		//		zone->runlock();
	} catch (...) {
		//		zone->runlock();

		throw;
	}

	return defenders;
}

/*
	Damage
*/

// Calculate Damage - CreO attacker & CreO defender
float CombatManager::calculateDamage(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, const CreatureAttackData& data) const {
	float damage = 0;
	int diff = 0;

	if (data.getMinDamage() > 0 && data.getMaxDamage() > 0) { // this is a special attack (force, etc)
		float minDmg = data.getMinDamage();
		float maxDmg = data.getMaxDamage();

		if (data.isForceAttack() && attacker->isPlayerCreature())
			getFrsModifiedForceAttackDamage(attacker, minDmg, maxDmg, data);

		float mod = attacker->isAiAgent() ? cast<AiAgent*>(attacker)->getSpecialDamageMult() : 1.f;
		damage = minDmg * mod;
		diff = (maxDmg * mod) - damage;
	} else {
		diff = calculateDamageRange(attacker, defender, weapon);
		float minDamage = weapon->getMinDamage();

		if (attacker->isPlayerCreature() && !weapon->isCertifiedFor(attacker))
			minDamage = 5;

		damage = minDamage;
	}

	if (diff > 0)
		damage += System::random(diff);

	damage = applyDamageModifiers(attacker, weapon, damage, data);

	damage += defender->getSkillMod("private_damage_susceptibility");

	if (attacker->isPlayerCreature()) {
		if (data.isForceAttack() && !defender->isPlayerCreature())
			damage *= 2 + System::random(1);
		else if (!data.isForceAttack())
			damage *= 1.5;
	}

	if (!data.isForceAttack() && weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK)
		damage *= 1.25;

	if (defender->isKnockedDown()) {
		damage *= 1.5f;
	} else if (data.isForceAttack() && data.getCommandName().hashCode() == STRING_HASHCODE("forcechoke")) {
		if (defender->isProne())
			damage *= 1.5f;
		else if (defender->isKneeling())
			damage *= 1.25f;
	}

	// Toughness reduction
	if (data.isForceAttack())
		damage = getDefenderToughnessModifier(defender, SharedWeaponObjectTemplate::FORCEATTACK, data.getDamageType(), damage);
	else
		damage = getDefenderToughnessModifier(defender, weapon->getAttackType(), weapon->getDamageType(), damage);

	// Force Defense skillmod damage reduction
	if (data.isForceAttack()) {
		int forceDefense = defender->getSkillMod("force_defense");

		if (forceDefense > 0)
			damage *= 1.f / (1.f + ((float)forceDefense / 100.f));
	}

	// PvP Damage Reduction.
	if (attacker->isPlayerCreature() && defender->isPlayerCreature() && !data.isForceAttack())
		damage *= 0.25;

	if (damage < 1)
		damage = 1;

	debug() << "damage to be dealt is " << damage;

	return damage;
}

// Calculate Damage - Creo Attacker & Tano Defender
float CombatManager::calculateDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender, const CreatureAttackData& data) const {
	float damage = 0;
	int diff = 0;

	if (data.getMinDamage() > 0 && data.getMaxDamage() > 0) { // this is a special attack (force, etc)
		float minDmg = data.getMinDamage();
		float maxDmg = data.getMaxDamage();

		if (data.isForceAttack() && attacker->isPlayerCreature())
			getFrsModifiedForceAttackDamage(attacker, minDmg, maxDmg, data);

		float mod = attacker->isAiAgent() ? cast<AiAgent*>(attacker)->getSpecialDamageMult() : 1.f;
		damage = minDmg * mod;
		diff = (maxDmg * mod) - damage;
	} else {
		float minDamage = weapon->getMinDamage(), maxDamage = weapon->getMaxDamage();

		if (attacker->isPlayerCreature() && !weapon->isCertifiedFor(attacker)) {
			minDamage = 5.f;
			maxDamage = 10.f;
		}

		damage = minDamage;
		diff = maxDamage - minDamage;
	}

	if (diff > 0)
		damage += System::random(diff);

	damage = applyDamageModifiers(attacker, weapon, damage, data);

	if (attacker->isPlayerCreature())
		damage *= 1.5;

	if (!data.isForceAttack() && weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK)
		damage *= 1.25;

	debug() << "damage to be dealt is " << damage;

	ManagedReference<LairObserver*> lairObserver = nullptr;
	SortedVector<ManagedReference<Observer*>> observers = defender->getObservers(ObserverEventType::OBJECTDESTRUCTION);

	for (int i = 0; i < observers.size(); i++) {
		lairObserver = cast<LairObserver*>(observers.get(i).get());
		if (lairObserver != nullptr)
			break;
	}

	if (lairObserver && lairObserver->getSpawnNumber() > 2)
		damage *= 3.5;

	return damage;
}

// Calculate Damage - TanO attacker & CreO defender
float CombatManager::calculateDamage(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, const CreatureAttackData& data) const {
	float damage = 0;

	int diff = calculateDamageRange(attacker, defender, weapon);
	float minDamage = weapon->getMinDamage();

	if (diff > 0)
		damage = System::random(diff) + (int)minDamage;

	damage += defender->getSkillMod("private_damage_susceptibility");

	if (defender->isKnockedDown())
		damage *= 1.5f;

	// Toughness reduction
	damage = getDefenderToughnessModifier(defender, weapon->getAttackType(), weapon->getDamageType(), damage);

	return damage;
}

int CombatManager::calculateDamageRange(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon) const {
	int attackType = weapon->getAttackType();
	int damageMitigation = 0;
	float minDamage = weapon->getMinDamage(), maxDamage = weapon->getMaxDamage();

	// restrict damage if a player is not certified (don't worry about mobs)
	if (attacker->isPlayerCreature() && !weapon->isCertifiedFor(cast<CreatureObject*>(attacker))) {
		minDamage = 5;
		maxDamage = 10;
	}

	debug() << "attacker base damage is " << minDamage << "-" << maxDamage;

	PlayerObject* defenderGhost = defender->getPlayerObject();

	// this is for damage mitigation
	if (defenderGhost != nullptr) {
		String mitString;
		switch (attackType) {
		case SharedWeaponObjectTemplate::MELEEATTACK:
			mitString = "melee_damage_mitigation_";
			break;
		case SharedWeaponObjectTemplate::RANGEDATTACK:
			mitString = "ranged_damage_mitigation_";
			break;
		default:
			break;
		}

		for (int i = 3; i > 0; i--) {
			if (defenderGhost->hasAbility(mitString + i)) {
				damageMitigation = i;
				break;
			}
		}

		if (damageMitigation > 0)
			maxDamage = minDamage + (maxDamage - minDamage) * (1 - (0.2 * damageMitigation));
	}

	float range = maxDamage - minDamage;

	debug() << "attacker weapon damage mod is " << maxDamage;

	return range < 0 ? 0 : (int)range;
}

float CombatManager::applyDamageModifiers(CreatureObject* attacker, WeaponObject* weapon, float damage, const CreatureAttackData& data) const {
	if (!data.isForceAttack()) {
		const auto weaponDamageMods = weapon->getDamageModifiers();

		for (int i = 0; i < weaponDamageMods->size(); ++i) {
			damage += attacker->getSkillMod(weaponDamageMods->get(i));
		}

		if (weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK)
			damage += attacker->getSkillMod("private_melee_damage_bonus");
		if (weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK)
			damage += attacker->getSkillMod("private_ranged_damage_bonus");
	}

	damage += attacker->getSkillMod("private_damage_bonus");

	int damageMultiplier = attacker->getSkillMod("private_damage_multiplier");

	if (damageMultiplier != 0)
		damage *= damageMultiplier;

	int damageDivisor = attacker->getSkillMod("private_damage_divisor");

	if (damageDivisor != 0)
		damage /= damageDivisor;

	return damage;
}

void CombatManager::getFrsModifiedForceAttackDamage(CreatureObject* attacker, float& minDmg, float& maxDmg, const CreatureAttackData& data) const {
	ManagedReference<PlayerObject*> ghost = attacker->getPlayerObject();

	if (ghost == nullptr)
		return;

	FrsData* playerData = ghost->getFrsData();
	int councilType = playerData->getCouncilType();

	float minMod = 0, maxMod = 0;
	int powerModifier = 0;

	if (councilType == FrsManager::COUNCIL_LIGHT) {
		powerModifier = attacker->getSkillMod("force_power_light");
		minMod = data.getFrsLightMinDamageModifier();
		maxMod = data.getFrsLightMaxDamageModifier();
	} else if (councilType == FrsManager::COUNCIL_DARK) {
		powerModifier = attacker->getSkillMod("force_power_dark");
		minMod = data.getFrsDarkMinDamageModifier();
		maxMod = data.getFrsDarkMaxDamageModifier();
	}

	if (powerModifier > 0) {
		if (minMod > 0)
			minDmg += (int)((powerModifier * minMod) + 0.5);

		if (maxMod > 0)
			maxDmg += (int)((powerModifier * maxMod) + 0.5);
	}
}

int CombatManager::calculatePoolsToDamage(int poolsToDamage) const {
	if (poolsToDamage & RANDOM) {
		int rand = System::random(100);

		if (rand < 50) {
			poolsToDamage = HEALTH;
		} else if (rand < 85) {
			poolsToDamage = ACTION;
		} else {
			poolsToDamage = MIND;
		}
	}

	return poolsToDamage;
}

int CombatManager::applyDamage(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, DefenderHitList* defenderHitList, int damage, float damageMultiplier, int poolsToDamage, uint8& hitLocation, const CreatureAttackData& data) const {
	if (poolsToDamage == 0 || damageMultiplier == 0 || defenderHitList == nullptr) {
		return 0;
	}

	float ratio = weapon->getWoundsRatio();
	float healthDamage = 0.f, actionDamage = 0.f, mindDamage = 0.f;

	if (defender->isInvulnerable()) {
		return 0;
	}

	String xpType;
	if (data.isForceAttack()) {
		xpType = "jedi_general";
	} else if (attacker->isPet()) {
		xpType = "creaturehandler";
	} else {
		xpType = weapon->getXpType();
	}

	bool healthDamaged = (!!(poolsToDamage & HEALTH) && data.getHealthDamageMultiplier() > 0.0f);
	bool actionDamaged = (!!(poolsToDamage & ACTION) && data.getActionDamageMultiplier() > 0.0f);
	bool mindDamaged = (!!(poolsToDamage & MIND) && data.getMindDamageMultiplier() > 0.0f);

	int numberOfPoolsDamaged = (healthDamaged ? 1 : 0) + (actionDamaged ? 1 : 0) + (mindDamaged ? 1 : 0);
	Vector<int> poolsToWound;

	int numSpillOverPools = 3 - numberOfPoolsDamaged;

	float spillMultPerPool = (0.1f * numSpillOverPools) / Math::max(numberOfPoolsDamaged, 1);
	int totalSpillOver = 0; // Accumulate our total spill damage

	// from screenshots, it appears that food mitigation and armor mitigation were independently calculated
	// and then added together.
	int foodBonus = defender->getSkillMod("mitigate_damage");
	foodBonus > 100 ? foodBonus = 100 : foodBonus;

	int totalFoodMit = 0;
	float logDamage = 0.f;

	if (healthDamaged) {
		static const uint8 bodyLocations[] = {HIT_BODY, HIT_BODY, HIT_LARM, HIT_RARM};
		hitLocation = bodyLocations[System::random(3)];

		float damageMultiplied = damage * data.getHealthDamageMultiplier();

		logDamage += damageMultiplied;
		healthDamage = getArmorReduction(attacker, weapon, defender, defenderHitList, damageMultiplied, hitLocation, data) * damageMultiplier;

		int foodMitigation = 0;

		if (foodBonus > 0) {
			foodMitigation = (int)(healthDamage * foodBonus / 100.f);
		}

		healthDamage -= foodMitigation;
		totalFoodMit += foodMitigation;

		int spilledDamage = (int)(healthDamage * spillMultPerPool); // Cut our damage by the spill percentage
		healthDamage -= spilledDamage;								// subtract spill damage from total damage
		totalSpillOver += spilledDamage;							// accumulate spill damage

		defender->inflictDamage(attacker, CreatureAttribute::HEALTH, (int)healthDamage, true, xpType, true, true);

		poolsToWound.add(CreatureAttribute::HEALTH);
	}

	if (actionDamaged) {
		static const uint8 legLocations[] = {HIT_LLEG, HIT_RLEG};
		hitLocation = legLocations[System::random(1)];

		float damageMultiplied = damage * data.getActionDamageMultiplier();

		logDamage += damageMultiplied;
		actionDamage = getArmorReduction(attacker, weapon, defender, defenderHitList, damageMultiplied, hitLocation, data) * damageMultiplier;

		int foodMitigation = 0;

		if (foodBonus > 0) {
			foodMitigation = (int)(actionDamage * foodBonus / 100.f);
		}

		actionDamage -= foodMitigation;
		totalFoodMit += foodMitigation;

		int spilledDamage = (int)(actionDamage * spillMultPerPool);
		actionDamage -= spilledDamage;
		totalSpillOver += spilledDamage;

		defender->inflictDamage(attacker, CreatureAttribute::ACTION, (int)actionDamage, true, xpType, true, true);

		poolsToWound.add(CreatureAttribute::ACTION);
	}

	if (mindDamaged) {
		hitLocation = HIT_HEAD;

		float damageMultiplied = damage * data.getMindDamageMultiplier();

		logDamage += damageMultiplied;
		mindDamage = getArmorReduction(attacker, weapon, defender, defenderHitList, damageMultiplied, hitLocation, data) * damageMultiplier;

		int foodMitigation = 0;

		if (foodBonus > 0) {
			foodMitigation = (int)(mindDamage * foodBonus / 100.f);
		}

		mindDamage -= foodMitigation;
		totalFoodMit += foodMitigation;

		int spilledDamage = (int)(mindDamage * spillMultPerPool);
		mindDamage -= spilledDamage;
		totalSpillOver += spilledDamage;

		defender->inflictDamage(attacker, CreatureAttribute::MIND, (int)mindDamage, true, xpType, true, true);

		poolsToWound.add(CreatureAttribute::MIND);
	}

	if (numSpillOverPools > 0) {
		int spillDamagePerPool = (int)(totalSpillOver / numSpillOverPools); // Split the spill over damage between the pools damaged
		int spillOverRemainder = (totalSpillOver % numSpillOverPools) + spillDamagePerPool;
		int spillToApply = (numSpillOverPools-- > 1 ? spillDamagePerPool : spillOverRemainder);

		if ((poolsToDamage ^ 0x7) & HEALTH) {
			defender->inflictDamage(attacker, CreatureAttribute::HEALTH, spillToApply, true, xpType, true, true);
		}

		if ((poolsToDamage ^ 0x7) & ACTION) {
			defender->inflictDamage(attacker, CreatureAttribute::ACTION, spillToApply, true, xpType, true, true);
		}

		if ((poolsToDamage ^ 0x7) & MIND) {
			defender->inflictDamage(attacker, CreatureAttribute::MIND, spillToApply, true, xpType, true, true);
		}
	}

	int totalDamage = (int)(healthDamage + actionDamage + mindDamage);
	defender->notifyObservers(ObserverEventType::DAMAGERECEIVED, attacker, totalDamage);

	if (attacker->isPlayerCreature()) {
		showHitLocationFlyText(attacker->asCreatureObject(), defender, hitLocation);
	}

	defenderHitList->setInitialDamage(logDamage);
	defenderHitList->setHitLocation(hitLocation);
	defenderHitList->setFoodMitigation(totalFoodMit);
	defenderHitList->setPoolsToWound(poolsToWound);

	return totalDamage;
}

int CombatManager::applyDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender, DefenderHitList* defenderHitList, int poolsToDamage, const CreatureAttackData& data) const {
	if (defender == nullptr || defenderHitList == nullptr || poolsToDamage == 0) {
		return 0;
	}

	if (defender->getPvpStatusBitmask() == CreatureFlag::NONE) {
		return 0;
	}

	int damage = calculateDamage(attacker, weapon, defender, data);

	float damageMultiplier = data.getDamageMultiplier();

	if (damageMultiplier != 0)
		damage *= damageMultiplier;

	defenderHitList->setInitialDamage(damage);

	String xpType;
	if (data.isForceAttack())
		xpType = "jedi_general";
	else if (attacker->isPet())
		xpType = "creaturehandler";
	else
		xpType = weapon->getXpType();

	if (defender->isTurret()) {
		int damageType = 0, armorPiercing = 1;

		if (!data.isForceAttack()) {
			damageType = weapon->getDamageType();
			armorPiercing = weapon->getArmorPiercing();

			if (weapon->isBroken())
				armorPiercing = 0;
		} else {
			damageType = data.getDamageType();
		}

		int armorReduction = getArmorTurretReduction(attacker, defender, damageType);

		if (armorReduction >= 0)
			damage *= getArmorPiercing(defender, armorPiercing);

		damage *= (1.f - (armorReduction / 100.f));
	}

	defender->inflictDamage(attacker, 0, damage, true, xpType, true, true);

	defender->notifyObservers(ObserverEventType::DAMAGERECEIVED, attacker, damage);

	return damage;
}

void CombatManager::woundCreatureTarget(CreatureObject* defender, WeaponObject* weapon, Vector<int> poolsToWound) const {
	if (defender == nullptr || poolsToWound.size() <= 0) {
		return;
	}

	Locker wlock(defender);

	float ratio = weapon->getWoundsRatio();

	if (System::random(100) < ratio) {
		int poolToWound = poolsToWound.get(System::random(poolsToWound.size() - 1));
		defender->addWounds(poolToWound, 1, true);
		defender->addWounds(poolToWound + 1, 1, true);
		defender->addWounds(poolToWound + 2, 1, true);
	}
}

/*
	Damage Over Time
*/

void CombatManager::applyDots(CreatureObject* attacker, CreatureObject* defender, const CreatureAttackData& data, int appliedDamage, int unmitDamage, int poolsToDamage) const {
	const Vector<DotEffect>* dotEffects = data.getDotEffects();

	if (defender->isInvulnerable())
		return;

	for (int i = 0; i < dotEffects->size(); i++) {
		const DotEffect& effect = dotEffects->get(i);

		if (defender->hasDotImmunity(effect.getDotType()) || effect.getDotDuration() == 0 || System::random(100) > effect.getDotChance())
			continue;

		const Vector<String>& defenseMods = effect.getDefenderStateDefenseModifers();
		int resist = 0;

		for (int j = 0; j < defenseMods.size(); j++)
			resist += defender->getSkillMod(defenseMods.get(j));

		int damageToApply = appliedDamage;
		uint32 dotType = effect.getDotType();

		if (effect.isDotDamageofHit()) {
			// determine if we should use unmitigated damage
			if (dotType != CreatureState::BLEEDING)
				damageToApply = unmitDamage;
		}

		int potency = effect.getDotPotency();

		if (potency == 0) {
			potency = 150;
		}

		uint8 pool = effect.getDotPool();

		if (pool == CreatureAttribute::UNKNOWN) {
			pool = getPoolForDot(dotType, poolsToDamage);
		}

		debug() << "entering addDotState with dotType:" << dotType;

		float damMod = attacker->isAiAgent() ? cast<AiAgent*>(attacker)->getSpecialDamageMult() : 1.f;
		defender->addDotState(attacker, dotType, data.getCommand()->getNameCRC(), effect.isDotDamageofHit() ? damageToApply * effect.getPrimaryPercent() / 100.0f : effect.getDotStrength() * damMod, pool, effect.getDotDuration(), potency, resist,
							  effect.isDotDamageofHit() ? damageToApply * effect.getSecondaryPercent() / 100.0f : effect.getDotStrength() * damMod);
	}
}

void CombatManager::applyWeaponDots(CreatureObject* attacker, CreatureObject* defender, WeaponObject* weapon) const {
	if (defender->isInvulnerable())
		return;

	if (!weapon->isCertifiedFor(attacker))
		return;

	for (int i = 0; i < weapon->getNumberOfDots(); i++) {
		if (weapon->getDotUses(i) <= 0)
			continue;

		int type = 0;
		int resist = 0;
		// utilizing this switch-block for easier *functionality* , present & future
		// SOE strings only provide this ONE specific type of mod (combat_bleeding_defense) and
		// there's no evidence (yet) of other 3 WEAPON dot versions also being resistable.
		switch (weapon->getDotType(i)) {
		case 1: // POISON
			type = CreatureState::POISONED;
			// resist = defender->getSkillMod("resistance_poison");
			break;
		case 2: // DISEASE
			type = CreatureState::DISEASED;
			// resist = defender->getSkillMod("resistance_disease");
			break;
		case 3: // FIRE
			type = CreatureState::ONFIRE;
			// resist = defender->getSkillMod("resistance_fire");
			break;
		case 4: // BLEED
			type = CreatureState::BLEEDING;
			resist = defender->getSkillMod("combat_bleeding_defense");
			break;
		default:
			break;
		}

		if (defender->hasDotImmunity(type))
			continue;

		if (weapon->getDotPotency(i) * (1.f - resist / 100.f) > System::random(100) && defender->addDotState(attacker, type, weapon->getObjectID(), weapon->getDotStrength(i), weapon->getDotAttribute(i), weapon->getDotDuration(i), -1, 0, (int)(weapon->getDotStrength(i) / 5.f)) > 0)
			if (weapon->getDotUses(i) > 0)
				weapon->setDotUses(weapon->getDotUses(i) - 1, i); // Unresisted despite mod, reduce use count.
	}
}

uint8 CombatManager::getPoolForDot(uint64 dotType, int poolsToDamage) const {
	uint8 pool = 0;

	switch (dotType) {
	case CreatureState::POISONED:
	case CreatureState::ONFIRE:
	case CreatureState::BLEEDING:
		if (poolsToDamage & HEALTH) {
			pool = CreatureAttribute::HEALTH;
		} else if (poolsToDamage & ACTION) {
			pool = CreatureAttribute::ACTION;
		} else if (poolsToDamage & MIND) {
			pool = CreatureAttribute::MIND;
		}
		break;
	case CreatureState::DISEASED:
		if (poolsToDamage & HEALTH) {
			pool = CreatureAttribute::HEALTH + System::random(2);
		} else if (poolsToDamage & ACTION) {
			pool = CreatureAttribute::ACTION + System::random(2);
		} else if (poolsToDamage & MIND) {
			pool = CreatureAttribute::MIND + System::random(2);
		}
		break;
	default:
		break;
	}

	return pool;
}

/*
	Accuracy and Modifiers
*/

float CombatManager::getWeaponRangeModifier(float currentRange, WeaponObject* weapon) const {
	float minRange = 0;
	float idealRange = 2;
	float maxRange = 5;

	float smallMod = 7;
	float bigMod = 7;

	minRange = (float)weapon->getPointBlankRange();
	idealRange = (float)weapon->getIdealRange();
	maxRange = (float)weapon->getMaxRange();

	smallMod = (float)weapon->getPointBlankAccuracy();
	bigMod = (float)weapon->getIdealAccuracy();

	if (currentRange >= maxRange)
		return (float)weapon->getMaxRangeAccuracy();

	if (currentRange <= minRange)
		return smallMod;

	// this assumes that we are attacking somewhere between point blank and ideal range
	float smallRange = minRange;
	float bigRange = idealRange;

	// check that assumption and correct if it's not true
	if (currentRange > idealRange) {
		smallMod = (float)weapon->getIdealAccuracy();
		bigMod = (float)weapon->getMaxRangeAccuracy();

		smallRange = idealRange;
		bigRange = maxRange;
	}

	if (bigRange == smallRange) // if they are equal, we know at least one is ideal, so just return the ideal accuracy mod
		return weapon->getIdealAccuracy();

	return smallMod + ((currentRange - smallRange) / (bigRange - smallRange) * (bigMod - smallMod));
}

int CombatManager::calculatePostureModifier(CreatureObject* creature, WeaponObject* weapon) const {
	CreaturePosture* postureLookup = CreaturePosture::instance();

	uint8 locomotion = creature->getLocomotion();

	if (!weapon->isMeleeWeapon())
		return postureLookup->getRangedAttackMod(locomotion);
	else
		return postureLookup->getMeleeAttackMod(locomotion);
}

int CombatManager::calculateTargetPostureModifier(WeaponObject* weapon, CreatureObject* targetCreature) const {
	CreaturePosture* postureLookup = CreaturePosture::instance();

	uint8 locomotion = targetCreature->getLocomotion();

	if (!weapon->isMeleeWeapon())
		return postureLookup->getRangedDefenseMod(locomotion);
	else
		return postureLookup->getMeleeDefenseMod(locomotion);
}

int CombatManager::getAttackerAccuracyModifier(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon) const {
	if (attacker->isAiAgent()) {
		return cast<AiAgent*>(attacker)->getChanceHit() * 100;
	} else if (attacker->isInstallationObject()) {
		return cast<InstallationObject*>(attacker)->getHitChance() * 100;
	}

	if (!attacker->isCreatureObject()) {
		return 0;
	}

	CreatureObject* creoAttacker = cast<CreatureObject*>(attacker);

	int attackerAccuracy = 0;

	const auto creatureAccMods = weapon->getCreatureAccuracyModifiers();

	for (int i = 0; i < creatureAccMods->size(); ++i) {
		const String& mod = creatureAccMods->get(i);
		attackerAccuracy += creoAttacker->getSkillMod(mod);
		attackerAccuracy += creoAttacker->getSkillMod("private_" + mod);

		if (creoAttacker->isStanding()) {
			attackerAccuracy += creoAttacker->getSkillMod(mod + "_while_standing");
		}
	}

	if (attackerAccuracy == 0)
		attackerAccuracy = -15; // unskilled penalty, TODO: this might be -50 or -125, do research

	attackerAccuracy += creoAttacker->getSkillMod("attack_accuracy") + creoAttacker->getSkillMod("dead_eye");

	// FS skill mods
	if (weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK)
		attackerAccuracy += creoAttacker->getSkillMod("melee_accuracy");
	else if (weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK)
		attackerAccuracy += creoAttacker->getSkillMod("ranged_accuracy");

	// now apply overall weapon defense mods
	if (weapon->isMeleeWeapon()) {
		switch (defender->getWeapon()->getGameObjectType()) {
		case SceneObjectType::PISTOL:
			attackerAccuracy += 20.f;
		/* no break */
		case SceneObjectType::CARBINE:
			attackerAccuracy += 55.f;
		/* no break */
		case SceneObjectType::RIFLE:
		case SceneObjectType::MINE:
		case SceneObjectType::SPECIALHEAVYWEAPON:
		case SceneObjectType::HEAVYWEAPON:
			attackerAccuracy += 25.f;
		}
	}

	return attackerAccuracy;
}

int CombatManager::getAttackerAccuracyBonus(CreatureObject* attacker, WeaponObject* weapon) const {
	int bonus = 0;

	bonus += attacker->getSkillMod("private_attack_accuracy");
	bonus += attacker->getSkillMod("private_accuracy_bonus");

	if (weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK)
		bonus += attacker->getSkillMod("private_melee_accuracy_bonus");
	if (weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK)
		bonus += attacker->getSkillMod("private_ranged_accuracy_bonus");

	return bonus;
}

/*
	Defenses
*/

int CombatManager::getDefenderDefenseModifier(CreatureObject* defender, WeaponObject* weapon, TangibleObject* attacker) const {
	int targetDefense = defender->isPlayerCreature() ? 0 : defender->getLevel();
	int buffDefense = 0;

	const auto defenseAccMods = weapon->getDefenderDefenseModifiers();

	for (int i = 0; i < defenseAccMods->size(); ++i) {
		const String& mod = defenseAccMods->get(i);
		targetDefense += defender->getSkillMod(mod);
		targetDefense += defender->getSkillMod("private_" + mod);
	}

	debug() << "Base target defense is " << targetDefense;

	// defense hardcap
	if (targetDefense > 125)
		targetDefense = 125;

	if (attacker->isPlayerCreature())
		targetDefense += defender->getSkillMod("private_defense");

	// SL bonuses go on top of hardcap
	for (int i = 0; i < defenseAccMods->size(); ++i) {
		const String& mod = defenseAccMods->get(i);
		targetDefense += defender->getSkillMod("private_group_" + mod);
	}

	// food bonus goes on top as well
	targetDefense += defender->getSkillMod("dodge_attack");
	targetDefense += defender->getSkillMod("private_dodge_attack");

	debug() << "Target defense after state affects and cap is " << targetDefense;

	return targetDefense;
}

int CombatManager::getDefenderSecondaryDefenseModifier(CreatureObject* defender) const {
	if (defender->isIntimidated() || defender->isBerserked() || defender->isVehicleObject())
		return 0;

	int targetDefense = defender->isPlayerCreature() ? 0 : defender->getLevel();
	ManagedReference<WeaponObject*> weapon = defender->getWeapon();

	const auto defenseAccMods = weapon->getDefenderSecondaryDefenseModifiers();

	for (int i = 0; i < defenseAccMods->size(); ++i) {
		const String& mod = defenseAccMods->get(i);
		targetDefense += defender->getSkillMod(mod);
		targetDefense += defender->getSkillMod("private_" + mod);
	}

	if (targetDefense > 125)
		targetDefense = 125;

	return targetDefense;
}

/*
	Hit Chance
*/

int CombatManager::getHitChance(TangibleObject* attacker, CreatureObject* targetCreature, WeaponObject* weapon, const CreatureAttackData& data, int damage, int accuracyBonus) const {
	int hitChance = 0;
	int attackType = weapon->getAttackType();
	CreatureObject* creoAttacker = nullptr;

	if (attacker->isCreatureObject()) {
		creoAttacker = attacker->asCreatureObject();

		if (creoAttacker != nullptr && data.isForceAttack()) {
			int attackerAccuracy = creoAttacker->getSkillMod(data.getCommand()->getAccuracySkillMod());
			int targetDefense = targetCreature->getSkillMod("force_defense");

			float attackerRoll = (float)System::random(249) + 1.f;
			float defenderRoll = (float)System::random(150) + 25.f;

			float accTotal = hitChanceEquation(attackerAccuracy, attackerRoll, targetDefense, defenderRoll);

			if (System::random(100) > accTotal)
				return MISS;
			else
				return HIT;
		}
	}

	debug() << "Calculating hit chance for " << attacker->getObjectID() << " Attacker accuracy bonus is " << accuracyBonus;
	float weaponAccuracy = 0.0f;
	// Get the weapon mods for range and add the mods for stance

	weaponAccuracy = getWeaponRangeModifier(attacker->getWorldPosition().distanceTo(targetCreature->getWorldPosition()) - targetCreature->getTemplateRadius() - attacker->getTemplateRadius(), weapon);
	// accounts for steadyaim, general aim, and specific weapon aim, these buffs will clear after a completed combat action

	if (creoAttacker != nullptr && weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK)
		weaponAccuracy += creoAttacker->getSkillMod("private_aim");

	debug() << "Attacker weapon accuracy is " << weaponAccuracy;

	int attackerAccuracy = getAttackerAccuracyModifier(attacker, targetCreature, weapon);
	debug() << "Base attacker accuracy is " << attackerAccuracy;

	// need to also add in general attack accuracy (mostly gotten from posture and states)

	int bonusAccuracy = 0;

	if (creoAttacker != nullptr)
		bonusAccuracy = getAttackerAccuracyBonus(creoAttacker, weapon);

	// this is the scout/ranger creature hit bonus that only works against creatures (not NPCS)
	if (targetCreature->isCreature() && creoAttacker != nullptr)
		bonusAccuracy += creoAttacker->getSkillMod("creature_hit_bonus");

	debug() << "Attacker total bonus is " << bonusAccuracy;

	int postureAccuracy = 0;

	if (creoAttacker != nullptr)
		postureAccuracy = calculatePostureModifier(creoAttacker, weapon);

	debug() << "Attacker posture accuracy is " << postureAccuracy;

	int targetDefense = getDefenderDefenseModifier(targetCreature, weapon, attacker);
	debug() << "Defender defense is " << targetDefense;

	int postureDefense = calculateTargetPostureModifier(weapon, targetCreature);

	debug() << "Defender posture defense is " << postureDefense;
	float attackerRoll = (float)System::random(249) + 1.f;
	float defenderRoll = (float)System::random(150) + 25.f;

	// TODO (dannuic): add the trapmods in here somewhere (defense down trapmods)
	float accTotal = hitChanceEquation(attackerAccuracy + weaponAccuracy + accuracyBonus + postureAccuracy + bonusAccuracy, attackerRoll, targetDefense + postureDefense, defenderRoll);

	debug() << "Final hit chance is " << accTotal;

	if (System::random(100) > accTotal) // miss, just return MISS
		return MISS;

	debug() << "Attack hit successfully";

	// now we have a successful hit, so calculate secondary defenses if there is a damage component
	if (damage > 0) {
		ManagedReference<WeaponObject*> targetWeapon = targetCreature->getWeapon();
		const auto defenseAccMods = targetWeapon->getDefenderSecondaryDefenseModifiers();
		const String& def = defenseAccMods->get(0); // FIXME: this is hacky, but a lot faster than using contains()

		// saber block is special because it's just a % chance to block based on the skillmod
		if (def == "saber_block") {
			if (!(attacker->isTurret() || weapon->isThrownWeapon()) && ((weapon->isHeavyWeapon() || weapon->isSpecialHeavyWeapon() || (weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK)) && ((System::random(100)) < targetCreature->getSkillMod(def))))
				return RICOCHET;
			else
				return HIT;
		}

		targetDefense = getDefenderSecondaryDefenseModifier(targetCreature);

		debug() << "Secondary defenses are " << targetDefense;

		if (targetDefense <= 0)
			return HIT; // no secondary defenses

		// add in a random roll
		targetDefense += System::random(199) + 1;

		// TODO: posture defense (or a simplified version thereof: +10 standing, -20 prone, 0 crouching) might be added in to this calculation, research this
		// TODO: dodge and counterattack might get a  +25 bonus (even when triggered via DA), research this

		int cobMod = targetCreature->getSkillMod("private_center_of_being");
		debug() << "Center of Being mod is " << cobMod;

		targetDefense += cobMod;
		debug() << "Final modified secondary defense is " << targetDefense;

		if (targetDefense > 50 + attackerAccuracy + weaponAccuracy + accuracyBonus + postureAccuracy + bonusAccuracy + attackerRoll) { // successful secondary defense, return type of defense

			debug() << "Secondaries defenses prevailed";
			// defense acuity returns random: case 0 BLOCK, case 1 DODGE or default COUNTER
			if (targetWeapon == nullptr || def == "unarmed_passive_defense") {
				int randRoll = System::random(2);
				switch (randRoll) {
				case 0:
					return BLOCK;
				case 1:
					return DODGE;
				case 2:
				default:
					return COUNTER;
				}
			}

			if (def == "block")
				return BLOCK;
			else if (def == "dodge")
				return DODGE;
			else if (def == "counterattack")
				return COUNTER;
			else			// shouldn't get here
				return HIT; // no secondary defenses available on this weapon
		}
	}

	return HIT;
}

float CombatManager::hitChanceEquation(float attackerAccuracy, float attackerRoll, float targetDefense, float defenderRoll) const {
	float roll = (attackerRoll - defenderRoll) / 50;
	int8 rollSign = (roll > 0) - (roll < 0);

	float accTotal = 75.f + (float)roll;

	for (int i = 1; i <= 4; i++) {
		if (roll * rollSign > i) {
			accTotal += (float)rollSign * 25.f;
			roll -= rollSign * i;
		} else {
			accTotal += roll / ((float)i) * 25.f;
			break;
		}
	}

	accTotal += attackerAccuracy - targetDefense;

	debug() << "HitChance\n"
			<< "\tTarget Defense " << targetDefense << "\n"
			<< "\tAccTotal " << accTotal << "\n";

	return accTotal;
}

int CombatManager::getSpeedModifier(CreatureObject* attacker, WeaponObject* weapon) const {
	int speedMods = 0;

	const auto weaponSpeedMods = weapon->getSpeedModifiers();

	for (int i = 0; i < weaponSpeedMods->size(); ++i) {
		speedMods += attacker->getSkillMod(weaponSpeedMods->get(i));
	}

	speedMods += attacker->getSkillMod("private_speed_bonus");

	if (weapon->getAttackType() == SharedWeaponObjectTemplate::MELEEATTACK) {
		speedMods += attacker->getSkillMod("private_melee_speed_bonus");
		speedMods += attacker->getSkillMod("melee_speed");
	} else if (weapon->getAttackType() == SharedWeaponObjectTemplate::RANGEDATTACK) {
		speedMods += attacker->getSkillMod("private_ranged_speed_bonus");
		speedMods += attacker->getSkillMod("ranged_speed");
	}

	return speedMods;
}

// Toughness Mitigation

float CombatManager::getDefenderToughnessModifier(CreatureObject* defender, int attackType, int damType, float damage) const {
	ManagedReference<WeaponObject*> weapon = defender->getWeapon();

	const auto defenseToughMods = weapon->getDefenderToughnessModifiers();

	if (attackType == weapon->getAttackType()) {
		for (int i = 0; i < defenseToughMods->size(); ++i) {
			int toughMod = defender->getSkillMod(defenseToughMods->get(i));
			if (toughMod > 0)
				damage *= 1.f - (toughMod / 100.f);
		}
	}

	int jediToughness = defender->getSkillMod("jedi_toughness");
	if (damType != SharedWeaponObjectTemplate::LIGHTSABER && jediToughness > 0)
		damage *= 1.f - (jediToughness / 100.f);

	return damage < 0 ? 0 : damage;
}

/*

	Armor Reduction and Calculations - Player, Ai, Turret, Vehcile

*/

ArmorObject* CombatManager::getArmorObject(CreatureObject* defender, uint8 hitLocation) const {
	Vector<ManagedReference<ArmorObject*>> armor = defender->getWearablesDeltaVector()->getArmorAtHitLocation(hitLocation);

	if (armor.isEmpty())
		return nullptr;

	return armor.get(System::random(armor.size() - 1));
}

ArmorObject* CombatManager::getPSGArmor(CreatureObject* defender) const {
	SceneObject* psg = defender->getSlottedObject("utility_belt");

	if (psg != nullptr && psg->isPsgArmorObject())
		return cast<ArmorObject*>(psg);

	return nullptr;
}

int CombatManager::getArmorObjectReduction(ArmorObject* armor, int damageType) const {
	float resist = 0;

	switch (damageType) {
	case SharedWeaponObjectTemplate::KINETIC:
		resist = armor->getKinetic();
		break;
	case SharedWeaponObjectTemplate::ENERGY:
		resist = armor->getEnergy();
		break;
	case SharedWeaponObjectTemplate::ELECTRICITY:
		resist = armor->getElectricity();
		break;
	case SharedWeaponObjectTemplate::STUN:
		resist = armor->getStun();
		break;
	case SharedWeaponObjectTemplate::BLAST:
		resist = armor->getBlast();
		break;
	case SharedWeaponObjectTemplate::HEAT:
		resist = armor->getHeat();
		break;
	case SharedWeaponObjectTemplate::COLD:
		resist = armor->getCold();
		break;
	case SharedWeaponObjectTemplate::ACID:
		resist = armor->getAcid();
		break;
	case SharedWeaponObjectTemplate::LIGHTSABER:
		resist = armor->getLightSaber();
		break;
	}

	return Math::max(0, (int)resist);
}

int CombatManager::getArmorNpcReduction(AiAgent* defender, int damageType) const {
	float resist = 0;

	switch (damageType) {
	case SharedWeaponObjectTemplate::KINETIC:
		resist = defender->getKinetic();
		break;
	case SharedWeaponObjectTemplate::ENERGY:
		resist = defender->getEnergy();
		break;
	case SharedWeaponObjectTemplate::ELECTRICITY:
		resist = defender->getElectricity();
		break;
	case SharedWeaponObjectTemplate::STUN:
		resist = defender->getStun();
		break;
	case SharedWeaponObjectTemplate::BLAST:
		resist = defender->getBlast();
		break;
	case SharedWeaponObjectTemplate::HEAT:
		resist = defender->getHeat();
		break;
	case SharedWeaponObjectTemplate::COLD:
		resist = defender->getCold();
		break;
	case SharedWeaponObjectTemplate::ACID:
		resist = defender->getAcid();
		break;
	case SharedWeaponObjectTemplate::LIGHTSABER:
		resist = defender->getLightSaber();
		break;
	}

	return (int)resist;
}

int CombatManager::getArmorVehicleReduction(VehicleObject* defender, int damageType) const {
	float resist = 0;

	switch (damageType) {
	case SharedWeaponObjectTemplate::KINETIC:
		resist = defender->getKinetic();
		break;
	case SharedWeaponObjectTemplate::ENERGY:
		resist = defender->getEnergy();
		break;
	case SharedWeaponObjectTemplate::ELECTRICITY:
		resist = defender->getElectricity();
		break;
	case SharedWeaponObjectTemplate::STUN:
		resist = defender->getStun();
		break;
	case SharedWeaponObjectTemplate::BLAST:
		resist = defender->getBlast();
		break;
	case SharedWeaponObjectTemplate::HEAT:
		resist = defender->getHeat();
		break;
	case SharedWeaponObjectTemplate::COLD:
		resist = defender->getCold();
		break;
	case SharedWeaponObjectTemplate::ACID:
		resist = defender->getAcid();
		break;
	case SharedWeaponObjectTemplate::LIGHTSABER:
		resist = defender->getLightSaber();
		break;
	}

	return (int)resist;
}

// Armor mitigation

int CombatManager::getArmorReduction(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, DefenderHitList* hitList, float damage, int hitLocation, const CreatureAttackData& data) const {
	int damageType = 0, armorPiercing = 1;

	if (hitList == nullptr) {
		return 0;
	}

	if (!data.isForceAttack()) {
		damageType = weapon->getDamageType();
		armorPiercing = weapon->getArmorPiercing();

		if (weapon->isBroken())
			armorPiercing = 0;
	} else {
		damageType = data.getDamageType();
	}

	if (defender->isAiAgent()) {
		float armorReduction = getArmorNpcReduction(cast<AiAgent*>(defender), damageType);

		if (armorReduction >= 0)
			damage *= getArmorPiercing(cast<AiAgent*>(defender), armorPiercing);

		if (armorReduction > 0)
			damage *= (1.f - (armorReduction / 100.f));

		return damage;
	} else if (defender->isVehicleObject()) {
		float armorReduction = getArmorVehicleReduction(cast<VehicleObject*>(defender), damageType);

		if (armorReduction >= 0)
			damage *= getArmorPiercing(cast<VehicleObject*>(defender), armorPiercing);

		if (armorReduction > 0)
			damage *= (1.f - (armorReduction / 100.f));

		return damage;
	}

	float jediMit = hitList->getJediMitigation();

	if (!data.isForceAttack()) {
		// Force Armor
		float rawDamage = damage;

		int forceArmor = defender->getSkillMod("force_armor");
		if (forceArmor > 0) {
			float dmgAbsorbed = rawDamage - (damage *= 1.f - (forceArmor / 100.f));
			defender->notifyObservers(ObserverEventType::FORCEARMOR, attacker, dmgAbsorbed);

			jediMit += dmgAbsorbed;
			hitList->setJediMitigation(jediMit);
		}
	} else {
		float jediBuffDamage = 0;
		float rawDamage = damage;

		// Force Shield
		int forceShield = defender->getSkillMod("force_shield");
		if (forceShield > 0) {
			jediBuffDamage = rawDamage - (damage *= 1.f - (forceShield / 100.f));
			defender->notifyObservers(ObserverEventType::FORCESHIELD, attacker, jediBuffDamage);

			jediMit += jediBuffDamage;
			hitList->setJediMitigation(jediMit);
		}

		// Force Feedback
		int forceFeedback = defender->getSkillMod("force_feedback");

		if (forceFeedback > 0 && (defender->hasBuff(BuffCRC::JEDI_FORCE_FEEDBACK_1) || defender->hasBuff(BuffCRC::JEDI_FORCE_FEEDBACK_2))) {
			float feedbackDmg = rawDamage * (forceFeedback / 100.f);

			int forceDefense = defender->getSkillMod("force_defense");

			if (forceDefense > 0)
				feedbackDmg *= 1.f / (1.f + ((float)forceDefense / 100.f));

			float splitDmg = feedbackDmg / 3;

			attacker->inflictDamage(defender, CreatureAttribute::HEALTH, splitDmg, true, true, true);
			attacker->inflictDamage(defender, CreatureAttribute::ACTION, splitDmg, true, true, true);
			attacker->inflictDamage(defender, CreatureAttribute::MIND, splitDmg, true, true, true);
			defender->notifyObservers(ObserverEventType::FORCEFEEDBACK, attacker, feedbackDmg);
			defender->playEffect("clienteffect/pl_force_feedback_block.cef", "");
			hitList->setForceFeedback(feedbackDmg);
		}

		// Force Absorb
		if (defender->getSkillMod("force_absorb") > 0 && defender->isPlayerCreature()) {
			float absorbDam = damage * 0.4f;

			defender->notifyObservers(ObserverEventType::FORCEABSORB, attacker, absorbDam);
			hitList->setForceAbsorb(absorbDam);
		}
	}

	// PSG
	ManagedReference<ArmorObject*> psg = getPSGArmor(defender);

	if (psg != nullptr && !psg->isVulnerable(damageType)) {
		float armorReduction = getArmorObjectReduction(psg, damageType);
		float dmgAbsorbed = damage;

		damage *= getArmorPiercing(psg, armorPiercing);

		if (armorReduction > 0)
			damage *= 1.f - (armorReduction / 100.f);

		dmgAbsorbed -= damage;
		if (dmgAbsorbed > 0) {
			int psgMit = hitList->getPsgMitigation();

			psgMit += dmgAbsorbed;
			hitList->setPsgMitigation(psgMit);
		}

		Locker plocker(psg);

		psg->inflictDamage(psg, 0, damage * 0.2, true, true);
	}

	// Standard Armor
	ManagedReference<ArmorObject*> armor = nullptr;

	armor = getArmorObject(defender, hitLocation);

	if (armor != nullptr && !armor->isVulnerable(damageType)) {
		float armorReduction = getArmorObjectReduction(armor, damageType);
		float dmgAbsorbed = damage;

		// use only the damage applied to the armor for piercing (after the PSG takes some off)
		damage *= getArmorPiercing(armor, armorPiercing);

		if (armorReduction > 0) {
			damage *= (1.f - (armorReduction / 100.f));
			dmgAbsorbed -= damage;

			int armorMit = hitList->getArmorMitigation();

			armorMit += dmgAbsorbed;
			hitList->setArmorMitigation(armorMit);
		}

		// inflict condition damage
		Locker alocker(armor);

		armor->inflictDamage(armor, 0, damage * 0.2, true, true);
	}

	return damage;
}

int CombatManager::getArmorTurretReduction(CreatureObject* attacker, TangibleObject* defender, int damageType) const {
	int resist = 0;

	if (defender != nullptr && defender->isTurret()) {
		DataObjectComponentReference* data = defender->getDataObjectComponent();

		if (data != nullptr) {
			TurretDataComponent* turretData = cast<TurretDataComponent*>(data->get());

			if (turretData != nullptr) {
				switch (damageType) {
				case SharedWeaponObjectTemplate::KINETIC:
					resist = turretData->getKinetic();
					break;
				case SharedWeaponObjectTemplate::ENERGY:
					resist = turretData->getEnergy();
					break;
				case SharedWeaponObjectTemplate::ELECTRICITY:
					resist = turretData->getElectricity();
					break;
				case SharedWeaponObjectTemplate::STUN:
					resist = turretData->getStun();
					break;
				case SharedWeaponObjectTemplate::BLAST:
					resist = turretData->getBlast();
					break;
				case SharedWeaponObjectTemplate::HEAT:
					resist = turretData->getHeat();
					break;
				case SharedWeaponObjectTemplate::COLD:
					resist = turretData->getCold();
					break;
				case SharedWeaponObjectTemplate::ACID:
					resist = turretData->getAcid();
					break;
				case SharedWeaponObjectTemplate::LIGHTSABER:
					resist = turretData->getLightSaber();
					break;
				}
			}
		}
	}

	return resist;
}

float CombatManager::getArmorPiercing(TangibleObject* defender, int armorPiercing) const {
	int armorReduction = 0;

	if (defender->isAiAgent()) {
		AiAgent* aiDefender = cast<AiAgent*>(defender);
		armorReduction = aiDefender->getArmor();
	} else if (defender->isArmorObject()) {
		ArmorObject* armorDefender = cast<ArmorObject*>(defender);

		if (armorDefender != nullptr && !armorDefender->isBroken())
			armorReduction = armorDefender->getRating();
	} else if (defender->isVehicleObject()) {
		VehicleObject* vehicleDefender = cast<VehicleObject*>(defender);
		armorReduction = vehicleDefender->getArmor();
	} else {
		DataObjectComponentReference* data = defender->getDataObjectComponent();

		if (data != nullptr) {
			TurretDataComponent* turretData = cast<TurretDataComponent*>(data->get());

			if (turretData != nullptr) {
				armorReduction = turretData->getArmorRating();
			}
		}
	}

	if (armorPiercing > armorReduction)
		return pow(1.25, armorPiercing - armorReduction);
	else
		return pow(0.50, armorReduction - armorPiercing);
}

// Bomb Droid Detonation

float CombatManager::doDroidDetonation(CreatureObject* droid, CreatureObject* defender, float damage) const {
	if (defender->isInvulnerable()) {
		return 0;
	}
	if (defender->isCreatureObject()) {
		if (defender->isPlayerCreature())
			damage *= 0.25;
		// pikc a pool to target
		int pool = calculatePoolsToDamage(RANDOM);
		// we now have damage to use lets apply it
		float healthDamage = 0.f, actionDamage = 0.f, mindDamage = 0.f;
		// need to check armor reduction with just defender, blast and their AR + resists
		if (defender->isVehicleObject()) {
			int ar = cast<VehicleObject*>(defender)->getBlast();
			if (ar > 0)
				damage *= (1.f - (ar / 100.f));
			healthDamage = damage;
			actionDamage = damage;
			mindDamage = damage;
		} else if (defender->isAiAgent()) {
			int ar = cast<AiAgent*>(defender)->getBlast();
			if (ar > 0)
				damage *= (1.f - (ar / 100.f));
			healthDamage = damage;
			actionDamage = damage;
			mindDamage = damage;

		} else {
			// player
			static uint8 bodyHitLocations[] = {HIT_BODY, HIT_BODY, HIT_LARM, HIT_RARM};

			ArmorObject* healthArmor = getArmorObject(defender, bodyHitLocations[System::random(3)]);
			ArmorObject* mindArmor = getArmorObject(defender, HIT_HEAD);
			ArmorObject* actionArmor = getArmorObject(defender, HIT_LLEG); // This hits both the pants and feet regardless
			ArmorObject* psgArmor = getPSGArmor(defender);
			if (psgArmor != nullptr && !psgArmor->isVulnerable(SharedWeaponObjectTemplate::BLAST)) {
				float armorReduction = psgArmor->getBlast();
				if (armorReduction > 0)
					damage *= (1.f - (armorReduction / 100.f));

				Locker plocker(psgArmor);

				psgArmor->inflictDamage(psgArmor, 0, damage * 0.1, true, true);
			}
			// reduced by psg not check each spot for damage
			healthDamage = damage;
			actionDamage = damage;
			mindDamage = damage;
			if (healthArmor != nullptr && !healthArmor->isVulnerable(SharedWeaponObjectTemplate::BLAST) && (pool & HEALTH)) {
				float armorReduction = healthArmor->getBlast();
				if (armorReduction > 0)
					healthDamage *= (1.f - (armorReduction / 100.f));

				Locker hlocker(healthArmor);

				healthArmor->inflictDamage(healthArmor, 0, healthDamage * 0.1, true, true);
				return (int)healthDamage * 0.1;
			}
			if (mindArmor != nullptr && !mindArmor->isVulnerable(SharedWeaponObjectTemplate::BLAST) && (pool & MIND)) {
				float armorReduction = mindArmor->getBlast();
				if (armorReduction > 0)
					mindDamage *= (1.f - (armorReduction / 100.f));

				Locker mlocker(mindArmor);

				mindArmor->inflictDamage(mindArmor, 0, mindDamage * 0.1, true, true);
				return (int)mindDamage * 0.1;
			}
			if (actionArmor != nullptr && !actionArmor->isVulnerable(SharedWeaponObjectTemplate::BLAST) && (pool & ACTION)) {
				float armorReduction = actionArmor->getBlast();
				if (armorReduction > 0)
					actionDamage *= (1.f - (armorReduction / 100.f));

				Locker alocker(actionArmor);

				actionArmor->inflictDamage(actionArmor, 0, actionDamage * 0.1, true, true);
				return (int)actionDamage * 0.1;
			}
		}
		if ((pool & ACTION)) {
			defender->inflictDamage(droid, CreatureAttribute::ACTION, (int)actionDamage, true, true, false);
			return (int)actionDamage;
		}
		if ((pool & HEALTH)) {
			defender->inflictDamage(droid, CreatureAttribute::HEALTH, (int)healthDamage, true, true, false);
			return (int)healthDamage;
		}
		if ((pool & MIND)) {
			defender->inflictDamage(droid, CreatureAttribute::MIND, (int)mindDamage, true, true, false);
			return (int)mindDamage;
		}
		return 0;
	} else {
		return 0;
	}
}

// Calculate Weapon Speed

float CombatManager::calculateWeaponAttackSpeed(CreatureObject* attacker, WeaponObject* weapon, float skillSpeedRatio) const {
	int speedMod = getSpeedModifier(attacker, weapon);
	float jediSpeed = attacker->getSkillMod("combat_haste") / 100.0f;

	float attackSpeed = (1.0f - ((float)speedMod / 100.0f)) * skillSpeedRatio * weapon->getAttackSpeed();

	if (jediSpeed > 0)
		attackSpeed = attackSpeed - (attackSpeed * jediSpeed);

	return Math::max(attackSpeed, 1.0f);
}

// Fly Text - Miss, Counterattack, Block, Hit Location
void CombatManager::doMiss(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage) const {
	defender->showFlyText("combat_effects", "miss", 0xFF, 0xFF, 0xFF);
}

void CombatManager::doCounterAttack(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage) const {
	defender->showFlyText("combat_effects", "counterattack", 0, 0xFF, 0);
}

void CombatManager::doBlock(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage) const {
	defender->showFlyText("combat_effects", "block", 0, 0xFF, 0);
}

void CombatManager::doDodge(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage) const {
	defender->doCombatAnimation(STRING_HASHCODE("dodge"));
	defender->showFlyText("combat_effects", "dodge", 0, 0xFF, 0);
}

void CombatManager::showHitLocationFlyText(CreatureObject* attacker, CreatureObject* defender, uint8 location) const {
	if (defender->isVehicleObject())
		return;

	ShowFlyText* fly = nullptr;
	switch (location) {
	case HIT_HEAD:
		fly = new ShowFlyText(defender, "combat_effects", "hit_head", 0, 0, 0xFF, 1.0f);
		break;
	case HIT_BODY:
		fly = new ShowFlyText(defender, "combat_effects", "hit_body", 0xFF, 0, 0, 1.0f);
		break;
	case HIT_LARM:
		fly = new ShowFlyText(defender, "combat_effects", "hit_larm", 0xFF, 0, 0, 1.0f);
		break;
	case HIT_RARM:
		fly = new ShowFlyText(defender, "combat_effects", "hit_rarm", 0xFF, 0, 0, 1.0f);
		break;
	case HIT_LLEG:
		fly = new ShowFlyText(defender, "combat_effects", "hit_lleg", 0, 0xFF, 0, 1.0f);
		break;
	case HIT_RLEG:
		fly = new ShowFlyText(defender, "combat_effects", "hit_rleg", 0, 0xFF, 0, 1.0f);
		break;
	}

	if (fly != nullptr)
		attacker->sendMessage(fly);
}

// Special Attack Cost

bool CombatManager::applySpecialAttackCost(CreatureObject* attacker, WeaponObject* weapon, const CreatureAttackData& data) const {
	if (attacker->isAiAgent() || data.isForceAttack())
		return true;

	float force = weapon->getForceCost() * data.getForceCostMultiplier();

	if (force > 0) { // Need Force check first otherwise it can be spammed.
		ManagedReference<PlayerObject*> playerObject = attacker->getPlayerObject();
		if (playerObject != nullptr) {
			if (playerObject->getForcePower() <= force) {
				attacker->sendSystemMessage("@jedi_spam:no_force_power");
				return false;
			} else {
				playerObject->setForcePower(playerObject->getForcePower() - force);
				VisibilityManager::instance()->increaseVisibility(attacker, data.getCommand()->getVisMod()); // Give visibility
			}
		}
	}

	float health = weapon->getHealthAttackCost() * data.getHealthCostMultiplier();
	float action = weapon->getActionAttackCost() * data.getActionCostMultiplier();
	float mind = weapon->getMindAttackCost() * data.getMindCostMultiplier();

	health = attacker->calculateCostAdjustment(CreatureAttribute::STRENGTH, health);
	action = attacker->calculateCostAdjustment(CreatureAttribute::QUICKNESS, action);
	mind = attacker->calculateCostAdjustment(CreatureAttribute::FOCUS, mind);

	if (attacker->getHAM(CreatureAttribute::HEALTH) <= health)
		return false;

	if (attacker->getHAM(CreatureAttribute::ACTION) <= action)
		return false;

	if (attacker->getHAM(CreatureAttribute::MIND) <= mind)
		return false;

	if (health > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::HEALTH, health, true, true, true);

	if (action > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::ACTION, action, true, true, true);

	if (mind > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::MIND, mind, true, true, true);

	return true;
}

// Apply States
void CombatManager::applyStates(CreatureObject* creature, CreatureObject* targetCreature, DefenderHitList* hitList, const CreatureAttackData& data) const {
	const VectorMap<uint8, StateEffect>* stateEffects = data.getStateEffects();
	int stateAccuracyBonus = data.getStateAccuracyBonus();

	if (targetCreature->isInvulnerable()) {
		return;
	}

	Locker statelock(targetCreature, creature);

	int playerLevel = 0;
	if (targetCreature->isPlayerCreature()) {
		ZoneServer* server = targetCreature->getZoneServer();
		if (server != nullptr) {
			PlayerManager* pManager = server->getPlayerManager();
			if (pManager != nullptr) {
				playerLevel = pManager->calculatePlayerLevel(targetCreature) - 5;
			}
		}
	}

	// loop through all the states in the command
	for (int i = 0; i < stateEffects->size(); i++) {
		const StateEffect& effect = stateEffects->get(i);
		bool failed = false;
		uint8 effectType = effect.getEffectType();

		float accuracyMod = effect.getStateChance() + stateAccuracyBonus;
		if (data.isStateOnlyAttack()) {
			accuracyMod += creature->getSkillMod(data.getCommand()->getAccuracySkillMod());
		}

		// Check for state immunity.
		if (targetCreature->hasEffectImmunity(effectType)) {
			failed = true;
		}

		if (!failed) {
			const Vector<String>& exclusionTimers = effect.getDefenderExclusionTimers();

			// loop through any exclusion timers
			for (int j = 0; j < exclusionTimers.size(); j++) {
				if (!targetCreature->checkCooldownRecovery(exclusionTimers.get(j))) {
					failed = true;
				}
			}
		}

		float targetDefense = 0.f;

		// if recovery timer conditions aren't satisfied, it won't matter
		if (!failed) {
			const Vector<String>& defenseMods = effect.getDefenderStateDefenseModifiers();
			// add up all defenses against the state the target has
			for (int j = 0; j < defenseMods.size(); j++) {
				targetDefense += targetCreature->getSkillMod(defenseMods.get(j));
			}

			targetDefense /= 1.5;
			targetDefense += playerLevel;

			if (targetDefense > 90) {
				targetDefense = 90.f;
			}

			if (System::random(100) > accuracyMod - targetDefense) {
				failed = true;
			}

			// no reason to apply jedi defenses if primary defense was successful
			// and only perform extra rolls if the character is a Jedi
			if (!failed && targetCreature->isPlayerCreature() && targetCreature->getPlayerObject()->isJedi()) {
				const Vector<String>& jediMods = effect.getDefenderJediStateDefenseModifiers();
				// second chance for jedi, roll against their special defenses jedi_state_defense & resistance_states
				for (int j = 0; j < jediMods.size(); j++) {
					targetDefense = targetCreature->getSkillMod(jediMods.get(j));

					targetDefense /= 1.5;
					targetDefense += playerLevel;

					if (targetDefense > 90)
						targetDefense = 90.f;

					if (System::random(100) > accuracyMod - targetDefense) {
						failed = true;
						break;
					}
				}
			}
		}

		if (!failed) {
			if (effectType == CommandEffect::NEXTATTACKDELAY) {
				StringIdChatParameter stringId("combat_effects", "delay_applied_other");
				stringId.setTT(targetCreature->getObjectID());
				stringId.setDI(effect.getStateLength());
				creature->sendSystemMessage(stringId);
			}

			data.getCommand()->applyEffect(creature, targetCreature, effectType, effect.getStateStrength() + stateAccuracyBonus);

			if (data.changesDefenderPosture()) {
				targetCreature->updatePostures(true);
			}
		}

		// can move this to scripts, but only these states have fail messages
		if (failed) {
			switch (effectType) {
			case CommandEffect::KNOCKDOWN:
				if (!targetCreature->checkKnockdownRecovery() && targetCreature->getPosture() != CreaturePosture::UPRIGHT)
					targetCreature->setPosture(CreaturePosture::UPRIGHT);
				creature->sendSystemMessage("@cbt_spam:knockdown_fail");
				break;
			case CommandEffect::POSTUREDOWN:
				if (!targetCreature->checkPostureDownRecovery() && targetCreature->getPosture() != CreaturePosture::UPRIGHT)
					targetCreature->setPosture(CreaturePosture::UPRIGHT);
				creature->sendSystemMessage("@cbt_spam:posture_change_fail");
				break;
			case CommandEffect::POSTUREUP:
				if (!targetCreature->checkPostureUpRecovery() && targetCreature->getPosture() != CreaturePosture::UPRIGHT)
					targetCreature->setPosture(CreaturePosture::UPRIGHT);
				creature->sendSystemMessage("@cbt_spam:posture_change_fail");
				break;
			case CommandEffect::NEXTATTACKDELAY:
				targetCreature->showFlyText("combat_effects", "warcry_miss", 0xFF, 0, 0);
				break;
			case CommandEffect::INTIMIDATE:
				targetCreature->showFlyText("combat_effects", "intimidated_miss", 0xFF, 0, 0);
				break;
			default:
				break;
			}
		}

		// now check combat equilibrium
		if (!failed && (effectType == CommandEffect::KNOCKDOWN || effectType == CommandEffect::POSTUREDOWN || effectType == CommandEffect::POSTUREUP)) {
			int combatEquil = targetCreature->getSkillMod("combat_equillibrium");

			if (combatEquil > 100) {
				combatEquil = 100;
			}

			if ((combatEquil >> 1) > (int)System::random(100) && !targetCreature->isDead() && !targetCreature->isIntimidated()) {
				targetCreature->setPosture(CreaturePosture::UPRIGHT, false, true);
			}
		}

		// Send Combat Spam for state-only attacks.
		if (data.isStateOnlyAttack()) {
			if (failed) {
				data.getCommand()->sendAttackCombatSpam(creature, targetCreature, MISS, 0, data);
			} else {
				data.getCommand()->sendAttackCombatSpam(creature, targetCreature, HIT, 0, data);
			}
		}
	}
}

// Cone AoE attack angle
bool CombatManager::checkConeAngle(SceneObject* target, float angle, float creatureVectorX, float creatureVectorY, float directionVectorX, float directionVectorY) const {
	float Target1 = target->getPositionX() - creatureVectorX;
	float Target2 = target->getPositionY() - creatureVectorY;

	float resAngle = atan2(Target2, Target1) - atan2(directionVectorY, directionVectorX);
	float degrees = resAngle * 180 / M_PI;

	float coneAngle = angle / 2;

	if (degrees > coneAngle || degrees < -coneAngle) {
		return false;
	}

	return true;
}

/*

	Player Duels

*/

void CombatManager::requestDuel(CreatureObject* player, CreatureObject* targetPlayer) const {
	/* Pre: player != targetPlayer and not nullptr; player is locked
	 * Post: player requests duel to targetPlayer
	 */

	Locker clocker(targetPlayer, player);

	PlayerObject* ghost = player->getPlayerObject();
	PlayerObject* targetGhost = targetPlayer->getPlayerObject();

	if (ghost->requestedDuelTo(targetPlayer)) {
		StringIdChatParameter stringId("duel", "already_challenged");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		return;
	}

	player->debug() << "requesting duel with " << targetPlayer->getObjectID();

	ghost->addToDuelList(targetPlayer);

	if (targetGhost->requestedDuelTo(player)) {
		BaseMessage* pvpstat = new UpdatePVPStatusMessage(targetPlayer, player, targetPlayer->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE | CreatureFlag::AGGRESSIVE);
		player->sendMessage(pvpstat);

		for (int i = 0; i < targetGhost->getActivePetsSize(); i++) {
			ManagedReference<AiAgent*> pet = targetGhost->getActivePet(i);

			if (pet != nullptr) {
				BaseMessage* petpvpstat = new UpdatePVPStatusMessage(pet, player, pet->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE | CreatureFlag::AGGRESSIVE);
				player->sendMessage(petpvpstat);
			}
		}

		StringIdChatParameter stringId("duel", "accept_self");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		BaseMessage* pvpstat2 = new UpdatePVPStatusMessage(player, targetPlayer, player->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE | CreatureFlag::AGGRESSIVE);
		targetPlayer->sendMessage(pvpstat2);

		for (int i = 0; i < ghost->getActivePetsSize(); i++) {
			ManagedReference<AiAgent*> pet = ghost->getActivePet(i);

			if (pet != nullptr) {
				BaseMessage* petpvpstat = new UpdatePVPStatusMessage(pet, targetPlayer, pet->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE | CreatureFlag::AGGRESSIVE);
				targetPlayer->sendMessage(petpvpstat);
			}
		}

		StringIdChatParameter stringId2("duel", "accept_target");
		stringId2.setTT(player->getObjectID());
		targetPlayer->sendSystemMessage(stringId2);
	} else {
		StringIdChatParameter stringId3("duel", "challenge_self");
		stringId3.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId3);

		StringIdChatParameter stringId4("duel", "challenge_target");
		stringId4.setTT(player->getObjectID());
		targetPlayer->sendSystemMessage(stringId4);
	}
}

void CombatManager::requestEndDuel(CreatureObject* player, CreatureObject* targetPlayer) const {
	/* Pre: player != targetPlayer and not nullptr; player is locked
	 * Post: player requested to end the duel with targetPlayer
	 */

	Locker clocker(targetPlayer, player);

	PlayerObject* ghost = player->getPlayerObject();
	PlayerObject* targetGhost = targetPlayer->getPlayerObject();

	if (!ghost->requestedDuelTo(targetPlayer)) {
		StringIdChatParameter stringId("duel", "not_dueling");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		return;
	}

	player->debug() << "ending duel with " << targetPlayer->getObjectID();

	ghost->removeFromDuelList(targetPlayer);
	player->removeDefender(targetPlayer);

	if (targetGhost->requestedDuelTo(player)) {
		targetGhost->removeFromDuelList(player);
		targetPlayer->removeDefender(player);

		player->sendPvpStatusTo(targetPlayer);

		for (int i = 0; i < ghost->getActivePetsSize(); i++) {
			ManagedReference<AiAgent*> pet = ghost->getActivePet(i);

			if (pet != nullptr) {
				targetPlayer->removeDefender(pet);
				pet->sendPvpStatusTo(targetPlayer);

				ManagedReference<CreatureObject*> target = targetPlayer;

				Core::getTaskManager()->executeTask([=]() {
					 Locker locker(pet);

					 pet->removeDefender(target);
				 }, "PetRemoveDefenderLambda");
			}
		}

		StringIdChatParameter stringId("duel", "end_self");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		targetPlayer->sendPvpStatusTo(player);

		for (int i = 0; i < targetGhost->getActivePetsSize(); i++) {
			ManagedReference<AiAgent*> pet = targetGhost->getActivePet(i);

			if (pet != nullptr) {
				player->removeDefender(pet);
				pet->sendPvpStatusTo(player);

				ManagedReference<CreatureObject*> play = player;

				Core::getTaskManager()->executeTask([=]() {
					 Locker locker(pet);

					 pet->removeDefender(play);
				 }, "PetRemoveDefenderLambda2");
			}
		}

		StringIdChatParameter stringId2("duel", "end_target");
		stringId2.setTT(player->getObjectID());
		targetPlayer->sendSystemMessage(stringId2);
	}
}

void CombatManager::freeDuelList(CreatureObject* player, bool spam) const {
	/* Pre: player not nullptr and is locked
	 * Post: player removed and warned all of the objects from its duel list
	 */
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == nullptr || ghost->isDuelListEmpty())
		return;

	player->debug("freeing duel list");

	while (ghost->getDuelListSize() != 0) {
		ManagedReference<CreatureObject*> targetPlayer = ghost->getDuelListObject(0);
		PlayerObject* targetGhost = targetPlayer->getPlayerObject();

		if (targetPlayer != nullptr && targetGhost != nullptr && targetPlayer.get() != player) {
			try {
				Locker clocker(targetPlayer, player);

				ghost->removeFromDuelList(targetPlayer);
				player->removeDefender(targetPlayer);

				if (targetGhost->requestedDuelTo(player)) {
					targetGhost->removeFromDuelList(player);
					targetPlayer->removeDefender(player);

					player->sendPvpStatusTo(targetPlayer);

					for (int i = 0; i < ghost->getActivePetsSize(); i++) {
						ManagedReference<AiAgent*> pet = ghost->getActivePet(i);

						if (pet != nullptr) {
							targetPlayer->removeDefender(pet);
							pet->sendPvpStatusTo(targetPlayer);

							Core::getTaskManager()->executeTask([=]() {
								 Locker locker(pet);

								 pet->removeDefender(targetPlayer);
							 }, "PetRemoveDefenderLambda3");
						}
					}

					if (spam) {
						StringIdChatParameter stringId("duel", "end_self");
						stringId.setTT(targetPlayer->getObjectID());
						player->sendSystemMessage(stringId);
					}

					targetPlayer->sendPvpStatusTo(player);

					for (int i = 0; i < targetGhost->getActivePetsSize(); i++) {
						ManagedReference<AiAgent*> pet = targetGhost->getActivePet(i);

						if (pet != nullptr) {
							player->removeDefender(pet);
							pet->sendPvpStatusTo(player);

							ManagedReference<CreatureObject*> play = player;

							Core::getTaskManager()->executeTask([=]() {
								 Locker locker(pet);

								 pet->removeDefender(play);
							 }, "PetRemoveDefenderLambda4");
						}
					}

					if (spam) {
						StringIdChatParameter stringId2("duel", "end_target");
						stringId2.setTT(player->getObjectID());
						targetPlayer->sendSystemMessage(stringId2);
					}
				}

			} catch (ObjectNotDeployedException& e) {
				ghost->removeFromDuelList(targetPlayer);

				System::out << "Exception on CombatManager::freeDuelList()\n" << e.getMessage() << "\n";
			}
		}
	}
}

void CombatManager::declineDuel(CreatureObject* player, CreatureObject* targetPlayer) const {
	/* Pre: player != targetPlayer and not nullptr; player is locked
	 * Post: player declined Duel to targetPlayer
	 */

	Locker clocker(targetPlayer, player);

	PlayerObject* ghost = player->getPlayerObject();
	PlayerObject* targetGhost = targetPlayer->getPlayerObject();

	if (targetGhost->requestedDuelTo(player)) {
		targetGhost->removeFromDuelList(player);

		StringIdChatParameter stringId("duel", "cancel_self");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		StringIdChatParameter stringId2("duel", "cancel_target");
		stringId2.setTT(player->getObjectID());
		targetPlayer->sendSystemMessage(stringId2);

		player->debug() << "declined duel with " << targetPlayer->getObjectID();
	}
}

bool CombatManager::areInDuel(CreatureObject* player1, CreatureObject* player2) const {
	PlayerObject* ghost1 = player1->getPlayerObject().get();
	PlayerObject* ghost2 = player2->getPlayerObject().get();

	if (ghost1 != nullptr && ghost2 != nullptr) {
		if (ghost1->requestedDuelTo(player2) && ghost2->requestedDuelTo(player1))
			return true;
	}

	return false;
}

// Check for Temporary Enemy Flags

void CombatManager::checkForTefs(CreatureObject* attacker, CreatureObject* defender, bool* shouldGcwCrackdownTef, bool* shouldGcwTef, bool* shouldBhTef) const {
	if (*shouldGcwCrackdownTef && *shouldGcwTef && *shouldBhTef) {
		return;
	}

	ManagedReference<CreatureObject*> attackingCreature = nullptr;
	ManagedReference<CreatureObject*> targetCreature = defender->isPet() || defender->isVehicleObject() ? defender->getLinkedCreature() : defender;

	if (attacker->isPet()) {
		ManagedReference<PetControlDevice*> controlDevice = attacker->getControlDevice().get().castTo<PetControlDevice*>();

		if (controlDevice != nullptr) {
			ManagedReference<SceneObject*> lastCommander = controlDevice->getLastCommander().get();

			if (lastCommander != nullptr && lastCommander->isCreatureObject()) {
				attackingCreature = lastCommander->asCreatureObject();
			} else {
				attackingCreature = attacker->getLinkedCreature();
			}
		}
	} else {
		attackingCreature = attacker;
	}

	if (attackingCreature != nullptr && targetCreature != nullptr) {
		if (attackingCreature->isPlayerCreature() && targetCreature->isPlayerCreature() && !areInDuel(attackingCreature, targetCreature)) {
			if (!(*shouldGcwTef)) {
				if (attackingCreature->getFaction() != targetCreature->getFaction() && attackingCreature->getFactionStatus() == FactionStatus::OVERT && targetCreature->getFactionStatus() == FactionStatus::OVERT) {
					*shouldGcwTef = true;
				}
			}

			if (!(*shouldBhTef)) {
				if (attackingCreature->hasBountyMissionFor(targetCreature) || targetCreature->hasBountyMissionFor(attackingCreature)) {
					*shouldBhTef = true;
				}
			}
		}

		if (!(*shouldGcwCrackdownTef)) {
			if (attackingCreature->isPlayerObject() && targetCreature->isAiAgent()) {
				Reference<PlayerObject*> ghost = attackingCreature->getPlayerObject();

				if (ghost->hasCrackdownTefTowards(targetCreature->getFaction())) {
					*shouldGcwCrackdownTef = true;
				}
			}
			if (targetCreature->isPlayerObject() && attackingCreature->isAiAgent()) {
				Reference<PlayerObject*> ghost = targetCreature->getPlayerObject();

				if (ghost->hasCrackdownTefTowards(attackingCreature->getFaction())) {
					*shouldGcwCrackdownTef = true;
				}
			}
		}
	}
}

void CombatManager::initializeDefaultAttacks() {
	defaultRangedAttacks.add(STRING_HASHCODE("fire_1_single_light"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_1_single_medium"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_1_single_light_face"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_1_single_medium_face"));

	defaultRangedAttacks.add(STRING_HASHCODE("fire_3_single_light"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_3_single_medium"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_3_single_light_face"));
	defaultRangedAttacks.add(STRING_HASHCODE("fire_3_single_medium_face"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_light_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_light_0"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_medium_0"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_medium_0"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_light_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_light_1"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_medium_1"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_medium_1"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_light_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_light_2"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_medium_2"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_medium_2"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_light_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_light_3"));

	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_left_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_center_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_high_right_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_left_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_center_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_mid_right_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_left_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_right_medium_3"));
	defaultMeleeAttacks.add(STRING_HASHCODE("attack_low_center_medium_3"));
}
