/*
 * CombatManager.cpp
 *
 *  Created on: 24/02/2010
 *      Author: victor
 */

#include "CombatManager.h"
#include "CreatureAttackData.h"
#include "server/zone/objects/scene/variables/DeltaVector.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/events/LogoutTask.h"
#include "server/zone/objects/creature/CreatureState.h"
#include "server/zone/objects/creature/commands/CombatQueueCommand.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "server/zone/packets/object/CombatSpam.h"
#include "server/zone/packets/object/CombatAction.h"
#include "server/zone/packets/chat/ChatSystemMessage.h"
#include "server/zone/packets/tangible/UpdatePVPStatusMessage.h"
#include "server/zone/Zone.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/objects/creature/buffs/StateBuff.h"
#include "server/zone/managers/visibility/VisibilityManager.h"

#include "server/zone/objects/installation/components/TurretDataComponent.h"

const uint32 CombatManager::defaultAttacks[9] = {
		0x99476628, 0xF5547B91, 0x3CE273EC, 0x734C00C,
		0x43C4FFD0, 0x56D7CC78, 0x4B41CAFB, 0x2257D06B,
		0x306887EB
};


bool CombatManager::startCombat(CreatureObject* attacker, TangibleObject* defender, bool lockDefender) {
	if (attacker == defender)
		return false;

	if (attacker->getZone() == NULL || defender->getZone() == NULL)
		return false;

	if (attacker->isRidingMount())
		return false;

	if (!defender->isAttackableBy(attacker))
		return false;

	if (defender->isCreatureObject() && (cast<CreatureObject*>(defender))->isIncapacitated())
		return false;

	if (attacker->isPlayerCreature() && attacker->getPlayerObject()->isAFK())
		return false;
	// this is redundant (happens again in set/addDefender)
	//attacker->clearState(CreatureState::PEACE);

	Locker clocker(defender, attacker);

	attacker->setDefender(defender);
	defender->addDefender(attacker);

	return true;
}

bool CombatManager::attemptPeace(CreatureObject* attacker) {
	DeltaVector<ManagedReference<SceneObject*> >* defenderList = attacker->getDefenderList();

	for (int i = defenderList->size() - 1; i >= 0; --i) {
		ManagedReference<SceneObject*> object = defenderList->get(i);

		TangibleObject* defender = cast<TangibleObject*>( object.get());

		if (defender == NULL)
			continue;

		try {
			Locker clocker(defender, attacker);

			if (defender->hasDefender(attacker)) {

				if (defender->isCreatureObject()) {
					CreatureObject* creature = cast<CreatureObject*>(defender);

					if (creature->getMainDefender() != attacker || creature->hasState(CreatureState::PEACE) || creature->isDead() || attacker->isDead() || !creature->isInRange(attacker, 128.f)) {
						attacker->removeDefender(defender);
						defender->removeDefender(attacker);
					}
				} else {
					attacker->removeDefender(defender);
					defender->removeDefender(attacker);
				}
			} else {
				attacker->removeDefender(defender);
			}

			clocker.release();

		} catch (Exception& e) {
			error(e.getMessage());
			e.printStackTrace();
		}
	}

	if (defenderList->size() != 0) {
		//info("defenderList not empty, trying to set Peace State");

		attacker->setState(CreatureState::PEACE);

		return false;
	} else {
		attacker->clearCombatState(false);

		// clearCombatState() (rightfully) does not automatically set peace, so set it
		attacker->setState(CreatureState::PEACE);

		return true;
	}
}

void CombatManager::forcePeace(CreatureObject* attacker) {
	DeltaVector<ManagedReference<SceneObject*> >* defenderList = attacker->getDefenderList();

	for (int i = 0; i < defenderList->size(); ++i) {
		ManagedReference<SceneObject*> object = defenderList->get(i);

		if (!object->isTangibleObject())
			continue;

		TangibleObject* defender = cast<TangibleObject*>( object.get());

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



int CombatManager::doCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, CombatQueueCommand* command) {
	return doCombatAction(attacker, weapon, defenderObject, CreatureAttackData("",command));
}

int CombatManager::doCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, const CreatureAttackData& data) {
	//info("entering doCombat action with data ", true);

	if (!startCombat(attacker, defenderObject))
		return -1;

	//info("past start combat", true);

	if (attacker->hasAttackDelay() || !attacker->checkKnockdownRecovery())
		return -3;

	//info("past delay", true);

	if (attacker->isBerserked() && data.getCommandCRC() != 0xA8FEF90A) //berserk only works with attack
		return -3;

	//info("past berserk", true);

	if (!applySpecialAttackCost(attacker, weapon, data))
		return -2;

	//info("past special attack cost", true);

	int damage = 0;

	if (data.getCommand()->isAreaAction() || data.getCommand()->isConeAction())
		damage = doAreaCombatAction(attacker, weapon, defenderObject, data);
	else
		damage = doTargetCombatAction(attacker, weapon, defenderObject, data);

	if (damage > 0) {
		attacker->updateLastSuccessfulCombatAction();
	}

	return damage;
}



int CombatManager::doTargetCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* tano, const CreatureAttackData& data) {
	int damage = 0;

	Locker clocker(tano, attacker);

	if (!tano->isAttackableBy(attacker))
		return 0;

	attacker->addDefender(tano);
	tano->addDefender(attacker);

	if (tano->isCreatureObject()) {
		CreatureObject* defender = cast<CreatureObject*>( tano);

		damage = doTargetCombatAction(attacker, weapon, defender, data);
	} else {
		int poolsToDamage = calculatePoolsToDamage(data.getPoolsToDamage());

		damage = applyDamage(attacker, weapon, tano, damage, poolsToDamage, data);

		String combatSpam = data.getCombatSpam();

		broadcastCombatAction(attacker, tano, weapon, data, 0x01);
		broadcastCombatSpam(attacker, tano, weapon, damage, combatSpam + "_hit");
	}

	tano->notifyObservers(ObserverEventType::DAMAGERECEIVED, attacker, damage);
	return damage;
}

int CombatManager::doTargetCombatAction(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, const CreatureAttackData& data) {
	if (defender->isEntertaining())
		defender->stopEntertaining();

	int hitVal = 0;
	float damageMultiplier = data.getDamageMultiplier();

	// need to calculate damage here to get proper client spam
	int damage = 0;

	if (damageMultiplier != 0)
		damage = calculateDamage(attacker, weapon, defender, data) * damageMultiplier;

	damageMultiplier = 1.0f;
	hitVal = getHitChance(attacker, defender, weapon, damage, data.getAccuracyBonus() + attacker->getSkillMod(data.getCommand()->getAccuracySkillMod()));

	broadcastCombatAction(attacker, defender, weapon, data, hitVal);
	String combatSpam = data.getCombatSpam();

	// FIXME: probably need to add getCombatSpamBlock(), etc in data and store it in commands explicitly to avoid malformed text

	switch (hitVal) {
	case MISS:
		doMiss(attacker, weapon, defender, damage, combatSpam + "_miss");
		return 0;
		break;
	case HIT:
		broadcastCombatSpam(attacker, defender, weapon, damage, combatSpam + "_hit");
		break;
	case BLOCK:
		doBlock(attacker, weapon, defender, damage, combatSpam + "_block");
		damageMultiplier = 0.5f;
		break;
	case DODGE:
		doDodge(attacker, weapon, defender, damage, combatSpam + "_evade");
		damageMultiplier = 0.0f;
		break;
	case COUNTER:
		doCounterAttack(attacker, weapon, defender, damage, combatSpam + "_counter");
		damageMultiplier = 0.0f;
		break;
	case RICOCHET:
		doLightsaberBlock(attacker, weapon, defender, damage, combatSpam + "_block");
		damageMultiplier = 0.0f;
		break;
	default:
		break;
	}

	int poolsToDamage = calculatePoolsToDamage(data.getPoolsToDamage());
	// TODO: animations are probably determined by which pools are damaged (high, mid, low, combos, etc)
	if (damage != 0 && damageMultiplier != 0 && poolsToDamage != 0) {
		damage = applyDamage(attacker, weapon, defender, damage, damageMultiplier, poolsToDamage, data);
		if (defender->hasAttackDelay())
			defender->removeAttackDelay();
	}

	if (!defender->isDead()) {
		applyStates(attacker, defender, data);
		applyDots(attacker, defender, data, damage);
		applyWeaponDots(attacker, defender, weapon);
	}

	return damage;
}

void CombatManager::applyDots(CreatureObject* attacker, CreatureObject* defender, const CreatureAttackData& data, int appliedDamage) {
	VectorMap<uint64, DotEffect>* dotEffects = data.getDotEffects();

	for (int i = 0; i < dotEffects->size(); i++) {
		DotEffect effect = dotEffects->get(i);

		if (effect.getDotDuration() == 0 || System::random(100) > effect.getDotChance()) continue;

		Vector<String> defenseMods = effect.getDefenderStateDefenseModifers();
		int resist = 0;

		for (int j = 0; j < defenseMods.size(); j++)
			resist += defender->getSkillMod(defenseMods.get(j));

		//info("entering addDotState", true);

		defender->addDotState(effect.getDotType(), data.getCommand()->getNameCRC(), effect.isDotDamageofHit() ? appliedDamage : effect.getDotStrength(), effect.getDotPool(), effect.getDotDuration(), effect.getDotPotency(), resist);
	}
}

void CombatManager::applyWeaponDots(CreatureObject* attacker, CreatureObject* defender, WeaponObject* weapon) {

	//ManagedReference<WeaponObject*> attackerWeapon = attacker->getWeapon();

	// Get attacker's weapon they have.
	ManagedReference<WeaponObject*> attackerWeapon = cast<WeaponObject*>(weapon);

	for (int i = 0; i < attackerWeapon->getNumberOfDots(); i++) {
		if (attackerWeapon->getDotUses(i) <= 0)
			continue;

		int resist = 0;

		if (attackerWeapon->getDotType(i) == 1) { // Poison.
			resist = defender->getSkillMod("resistance_poison");
			int power = defender->addDotState(CreatureState::POISONED, attackerWeapon->getServerObjectCRC(), attackerWeapon->getDotStrength(i), attackerWeapon->getDotAttribute(i), attackerWeapon->getDotDuration(i), attackerWeapon->getDotPotency(i), resist);

			if (power > 0) { // Unresisted, reduce use count.
				if (attackerWeapon->getDotUses(i) > 0) {
					attackerWeapon->setDotUses(attackerWeapon->getDotUses(i) - 1, i);
				}
			}
		}

		if (attackerWeapon->getDotType(i) == 2) { // Disease.
			resist = defender->getSkillMod("resistance_disease");
			int power = defender->addDotState(CreatureState::DISEASED, attackerWeapon->getServerObjectCRC(), attackerWeapon->getDotStrength(i), attackerWeapon->getDotAttribute(i), attackerWeapon->getDotDuration(i), attackerWeapon->getDotPotency(i), resist);

			if (power > 0) { // Unresisted, reduce use count.
				if (attackerWeapon->getDotUses(i) > 0) {
					attackerWeapon->setDotUses(attackerWeapon->getDotUses(i) - 1, i);
				}
			}
		}

		if (attackerWeapon->getDotType(i) == 3) { // Fire.
			resist = defender->getSkillMod("resistance_fire");
			int power = defender->addDotState(CreatureState::ONFIRE, attackerWeapon->getServerObjectCRC(), attackerWeapon->getDotStrength(i), attackerWeapon->getDotAttribute(i), attackerWeapon->getDotDuration(i), attackerWeapon->getDotPotency(i), resist);

			if (power > 0) { // Unresisted, reduce use count.
				if (attackerWeapon->getDotUses(i) > 0) {
					attackerWeapon->setDotUses(attackerWeapon->getDotUses(i) - 1, i);
				}
			}
		}

		if (attackerWeapon->getDotType(i) == 4) { // Bleeding.
			resist = defender->getSkillMod("resistance_bleeding") + defender->getSkillMod("combat_bleeding_defense");
			int power = defender->addDotState(CreatureState::BLEEDING, attackerWeapon->getServerObjectCRC(), attackerWeapon->getDotStrength(i), attackerWeapon->getDotAttribute(i), attackerWeapon->getDotDuration(i), attackerWeapon->getDotPotency(i), resist);

			if (power > 0) { // Unresisted, reduce use count.
				if (attackerWeapon->getDotUses(i) > 0) {
					attackerWeapon->setDotUses(attackerWeapon->getDotUses(i) - 1, i);
				}
			}
		}
	}
}

float CombatManager::getWeaponRangeModifier(float currentRange, WeaponObject* weapon) {
	float minRange = 0;
	float idealRange = 2;
	float maxRange = 5;

	float smallMod = 7;
	float bigMod = 7;

	if (weapon != NULL) {
		minRange = (float) weapon->getPointBlankRange();
		idealRange = (float) weapon->getIdealRange();
		maxRange = (float) weapon->getMaxRange();

		smallMod = (float) weapon->getPointBlankAccuracy();
		bigMod = (float) weapon->getIdealAccuracy();
	}

	// this assumes that we are attacking somewhere between point blank and ideal range
	float smallRange = minRange;
	float bigRange = idealRange;

	// check that assumption and correct if it's not true
	if (currentRange > idealRange) {
		if (weapon != NULL) {
			smallMod = (float) weapon->getIdealAccuracy();
			bigMod = (float) weapon->getMaxRangeAccuracy();
		} // else retain mod values = 7

		smallRange = idealRange;
		bigRange = maxRange;
	} else if (currentRange <= minRange)
		return smallMod;

	return smallMod + ((currentRange - smallRange) / (bigRange - smallRange) * (bigMod - smallMod));
}

int CombatManager::calculatePostureModifier(CreatureObject* creature, WeaponObject* weapon) {
	int accuracy = 0;

	if (creature->isKneeling())
		accuracy += 16;
	else if (creature->isProne())
		accuracy += 50;

	if (weapon->getAttackType() != WeaponObject::RANGEDATTACK)
		accuracy *= -1;

	if (creature->isPlayerCreature()) // moving the setter to DataTransform, leaving calculateSpeed to set the last combat pos
		creature->calculateSpeed();

	switch (CreaturePosture::instance()->getSpeed(creature->getPosture(), creature->getLocomotion())) {
	case CreatureLocomotion::FAST:
		accuracy -= 50;
		break;
	case CreatureLocomotion::SLOW:
		accuracy -= 10;
		break;
	}

	return accuracy;
}

int CombatManager::calculateTargetPostureModifier(WeaponObject* weapon, CreatureObject* targetCreature) {
	int accuracy = 0;

	if (targetCreature->isKneeling()) {
		if (weapon->isMeleeWeapon())
			accuracy += 16;
		else
			accuracy -= 16;
	} else if (targetCreature->isProne()) {
		if (weapon->isMeleeWeapon())
			accuracy += 25;
		else
			accuracy -= 25;
	}

	return accuracy;
}

int CombatManager::getAttackerAccuracyModifier(CreatureObject* attacker, WeaponObject* weapon) {
	int attackerAccuracy = 0;

	Vector<String>* creatureAccMods = weapon->getCreatureAccuracyModifiers();

	for (int i = 0; i < creatureAccMods->size(); ++i) {
		String mod = creatureAccMods->get(i);
		attackerAccuracy += attacker->getSkillMod(mod);
		attackerAccuracy += attacker->getSkillMod("private_" + mod);
	}

	attackerAccuracy += attacker->getSkillMod("attack_accuracy");

	return attackerAccuracy;
}

int CombatManager::getAttackerAccuracyBonus(CreatureObject* attacker, WeaponObject* weapon) {
	int bonus = 0;

	bonus += attacker->getSkillMod("private_attack_accuracy");
	bonus += attacker->getSkillMod("private_accuracy_bonus");

	if (weapon->getAttackType() == WeaponObject::MELEEATTACK)
		bonus += attacker->getSkillMod("private_melee_accuracy_bonus");
	if (weapon->getAttackType() == WeaponObject::RANGEDATTACK)
		bonus += attacker->getSkillMod("private_ranged_accuracy_bonus");

	bonus += calculatePostureModifier(attacker, weapon);

	if (attacker->isAiAgent()) {
		ManagedReference<AiAgent*> agent = dynamic_cast<AiAgent*>(attacker);
		bonus += agent->getChanceHit() * 100;
	}

	return bonus;
}

int CombatManager::getDefenderDefenseModifier(CreatureObject* defender, WeaponObject* weapon) {
	if (!defender->isPlayerCreature())
		return MIN(125, defender->getLevel());

	int targetDefense = 0;
	int buffDefense = 0;

	Vector<String>* defenseAccMods = weapon->getDefenderDefenseModifiers();

	for (int i = 0; i < defenseAccMods->size(); ++i) {
		String mod = defenseAccMods->get(i);
		targetDefense += defender->getSkillMod(mod);
		targetDefense += defender->getSkillMod("private_" + mod);
	}

	//info("Base target defense is " + String::valueOf(targetDefense), true);

	// defense hardcap
	if (targetDefense > 125)
		targetDefense = 125;

	targetDefense += defender->getSkillMod("private_defense");

	//info("Target defense after state affects and cap is " +  String::valueOf(targetDefense), true);

	return targetDefense;
}

int CombatManager::getDefenderSecondaryDefenseModifier(CreatureObject* defender) {
	if (defender->isIntimidated()) return 0;

	int targetDefense = 0;
	ManagedReference<WeaponObject*> weapon = defender->getWeapon();

	Vector<String>* defenseAccMods = weapon->getDefenderSecondaryDefenseModifiers();

	for (int i = 0; i < defenseAccMods->size(); ++i) {
		String mod = defenseAccMods->get(i);
		targetDefense += defender->getSkillMod(mod);
		targetDefense += defender->getSkillMod("private_" + mod);
	}

	if (targetDefense > 125)
		targetDefense = 125;

	return targetDefense;
}

float CombatManager::getDefenderToughnessModifier(CreatureObject* defender, int damType, float damage) {
	ManagedReference<WeaponObject*> weapon = defender->getWeapon();

	Vector<String>* defenseToughMods = weapon->getDefenderToughnessModifiers();

	if (damType != WeaponObject::FORCEATTACK) {
		for (int i = 0; i < defenseToughMods->size(); ++i) {
			int toughMod = defender->getSkillMod(defenseToughMods->get(i));
			if (toughMod > 0) damage *= 1.f - (toughMod / 100.f);
		}
	}

	int jediToughness = defender->getSkillMod("jedi_toughness");
	if (damType != WeaponObject::LIGHTSABER && jediToughness > 0)
		damage *= 1.f - (jediToughness / 100.f);

	int foodBonus = defender->getSkillMod("mitigate_damage");
	if (foodBonus > 0)
		damage *= 1.f - (foodBonus / 100.f);

	return damage < 0 ? 0 : damage;
}


float CombatManager::hitChanceEquation(float attackerAccuracy, float accuracyBonus, float targetDefense) {
	float accTotal = 66.0 + accuracyBonus + (attackerAccuracy - targetDefense) / 2.0;

	/*StringBuffer msg;
	msg << "HitChance\n";
	msg << "\tTarget Defense " << targetDefense << "\n";
	msg << "\tTarget Defense Bonus" << defenseBonus << "\n";

	info(msg.toString());*/

	return accTotal;
}

int CombatManager::calculateDamageRange(CreatureObject* attacker, CreatureObject* defender, WeaponObject* weapon) {
	int attackType = weapon->getAttackType();
	int damageMitigation = 0;
	float minDamage = weapon->getMinDamage(), maxDamage = weapon->getMaxDamage();

	//info("attacker base damage is " + String::valueOf(minDamage) + "-"+ String::valueOf(maxDamage), true);

	PlayerObject* defenderGhost = defender->getPlayerObject();

	// this is for damage mitigation
	if (defenderGhost != NULL) {
		String mitString;
		switch (attackType){
		case WeaponObject::MELEEATTACK:
			mitString = "melee_damage_mitigation_";
			break;
		case WeaponObject::RANGEDATTACK:
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

	int maxDamageMultiplier = attacker->getSkillMod("private_max_damage_multiplier");
	int maxDamageDivisor = attacker->getSkillMod("private_max_damage_divisor");

	if (maxDamageMultiplier != 0)
		maxDamage *= maxDamageMultiplier;

	if (maxDamageDivisor != 0)
		maxDamage /= maxDamageDivisor;

	float range = maxDamage - minDamage;

	//info("attacker weapon damage mod is " + String::valueOf(maxDamage), true);

	return range < 0 ? 0 : (int)range;
}

int CombatManager::getDamageModifier(CreatureObject* attacker, WeaponObject* weapon) {
	int damageMods = 0;

	Vector<String>* weaponDamageMods = weapon->getDamageModifiers();

	for (int i = 0; i < weaponDamageMods->size(); ++i) {
		damageMods += attacker->getSkillMod(weaponDamageMods->get(i));
	}

	damageMods += attacker->getSkillMod("private_damage_bonus");

	if (weapon->getAttackType() == WeaponObject::MELEEATTACK)
		damageMods += attacker->getSkillMod("private_melee_damage_bonus");
	if (weapon->getAttackType() == WeaponObject::RANGEDATTACK)
		damageMods += attacker->getSkillMod("private_ranged_damage_bonus");

	int damageMultiplier = attacker->getSkillMod("private_damage_multiplier");

	if (damageMultiplier != 0)
		damageMods *= damageMultiplier;

	int damageDivisor = attacker->getSkillMod("private_damage_divisor");

	if (damageDivisor != 0)
		damageMods /= damageDivisor;

	return damageMods;
}

int CombatManager::getSpeedModifier(CreatureObject* attacker, WeaponObject* weapon) {
	int speedMods = 0;

	Vector<String>* weaponSpeedMods = weapon->getSpeedModifiers();

	for (int i = 0; i < weaponSpeedMods->size(); ++i) {
		speedMods += attacker->getSkillMod(weaponSpeedMods->get(i));
	}

	speedMods += attacker->getSkillMod("private_speed_bonus");

	if (weapon->getAttackType() == WeaponObject::MELEEATTACK)
		speedMods += attacker->getSkillMod("private_melee_speed_bonus");
	if (weapon->getAttackType() == WeaponObject::RANGEDATTACK)
		speedMods += attacker->getSkillMod("private_ranged_speed_bonus");

	return speedMods;
}



int CombatManager::getArmorObjectReduction(WeaponObject* weapon, ArmorObject* armor) {

	int damageType = weapon->getDamageType();

	float resist = 0;

	switch (damageType) {
	case WeaponObject::KINETIC:
		resist = armor->getKinetic();
		break;
	case WeaponObject::ENERGY:
		resist = armor->getEnergy();
		break;
	case WeaponObject::ELECTRICITY:
		resist = armor->getElectricity();
		break;
	case WeaponObject::STUN:
		resist = armor->getStun();
		break;
	case WeaponObject::BLAST:
		resist = armor->getBlast();
		break;
	case WeaponObject::HEAT:
		resist = armor->getHeat();
		break;
	case WeaponObject::COLD:
		resist = armor->getCold();
		break;
	case WeaponObject::ACID:
		resist = armor->getAcid();
		break;
	case WeaponObject::LIGHTSABER:
		resist = armor->getLightSaber();
		break;
	case WeaponObject::FORCE:
		resist = 0;
		break;
	}

	return MAX(0, (int)resist);

}

ArmorObject* CombatManager::getHealthArmor(CreatureObject* defender) {
	Vector<ManagedReference<ArmorObject*> > healthArmor = defender->getWearablesDeltaVector()->getArmorAtHitLocation(CombatManager::CHEST);

	if (System::random(1) == 0)
		healthArmor = defender->getWearablesDeltaVector()->getArmorAtHitLocation(CombatManager::ARMS);

	ManagedReference<ArmorObject*> armorToHit = NULL;

	if (!healthArmor.isEmpty())
		armorToHit = healthArmor.get(System::random(healthArmor.size() - 1));

	return armorToHit;
}

ArmorObject* CombatManager::getActionArmor(CreatureObject* defender) {
	Vector<ManagedReference<ArmorObject*> > actionArmor = defender->getWearablesDeltaVector()->getArmorAtHitLocation(CombatManager::LEGS);

	ManagedReference<ArmorObject*> armorToHit = NULL;

	if (!actionArmor.isEmpty())
		armorToHit = actionArmor.get(System::random(actionArmor.size() - 1));

	return armorToHit;
}

ArmorObject* CombatManager::getMindArmor(CreatureObject* defender) {
	Vector<ManagedReference<ArmorObject*> > mindArmor = defender->getWearablesDeltaVector()->getArmorAtHitLocation(CombatManager::HEAD);

	ManagedReference<ArmorObject*> armorToHit = NULL;

	if (!mindArmor.isEmpty())
		armorToHit = mindArmor.get(System::random(mindArmor.size() - 1));

	return armorToHit;
}

ArmorObject* CombatManager::getPSGArmor(CreatureObject* defender) {
	SceneObject* psg = defender->getSlottedObject("utility_belt");

	if (psg != NULL && psg->isPsgArmorObject())
		return cast<ArmorObject*>(psg);

	return NULL;
}


int CombatManager::getArmorNpcReduction(CreatureObject* attacker, AiAgent* defender, WeaponObject* weapon) {
	int damageType = weapon->getDamageType();

	float resist = 0;

	switch (damageType) {
	case WeaponObject::KINETIC:
		resist = defender->getKinetic();
		break;
	case WeaponObject::ENERGY:
		resist = defender->getEnergy();
		break;
	case WeaponObject::ELECTRICITY:
		resist = defender->getElectricity();
		break;
	case WeaponObject::STUN:
		resist = defender->getStun();
		break;
	case WeaponObject::BLAST:
		resist = defender->getBlast();
		break;
	case WeaponObject::HEAT:
		resist = defender->getHeat();
		break;
	case WeaponObject::COLD:
		resist = defender->getCold();
		break;
	case WeaponObject::ACID:
		resist = defender->getAcid();
		break;
	case WeaponObject::LIGHTSABER:
		resist = defender->getLightSaber();
		break;
	case WeaponObject::FORCE:
		resist = 0;
		break;
	}

	return (int)resist;
}

int CombatManager::getArmorReduction(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, float damage, int poolToDamage, const CreatureAttackData& data) {
	if (poolToDamage == 0)
		return 0;

	// the easy calculation
	if (defender->isAiAgent()) {
		damage *= getArmorPiercing(cast<AiAgent*>(defender), weapon);

		float armorReduction = getArmorNpcReduction(attacker, cast<AiAgent*>(defender), weapon);
		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		return damage;
	}

	// start with PSG reduction
	ManagedReference<ArmorObject*> psg = getPSGArmor(defender);

	if (psg != NULL && !psg->isVulnerable(weapon->getDamageType())) {
		float armorPiercing = getArmorPiercing(psg, weapon);
		float armorReduction =  getArmorObjectReduction(weapon, psg);

		if (armorPiercing > 1) damage *= armorPiercing;
		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		// inflict condition damage
		// TODO: this formula makes PSG's take more damage than regular armor, but that's how it was on live
		// it can be fixed by doing condition damage after all damage reductions
		psg->inflictDamage(psg, 0, damage * 0.1, true, true);
	}

	// Next is Jedi stuff
	if (data.getAttackType() == CombatManager::FORCEATTACK) {
		float jediBuffDamage = 0;
		float rawDamage = damage;

		// Force Shield
		int forceShield = defender->getSkillMod("force_shield");
		if (forceShield > 0)
			jediBuffDamage = rawDamage - (damage *= 1.f - (forceShield / 100.f));

		// Force Feedback
		int forceFeedback = defender->getSkillMod("force_feedback");
		if (forceFeedback > 0)
			attacker->inflictDamage(defender, System::random(2) * 3, rawDamage - (damage *= 1.f - (forceFeedback / 100.f)), true);

		// Force Absorb
		if (defender->getSkillMod("force_absorb") > 0 && defender->isPlayerCreature()) {
			ManagedReference<PlayerObject*> playerObject = defender->getPlayerObject();
			if (playerObject != NULL) playerObject->setForcePower(playerObject->getForcePower() + (damage * 0.5));
		}

		defender->notifyObservers(ObserverEventType::FORCEBUFFHIT, attacker, jediBuffDamage);
	} else if (data.getAttackType() == CombatManager::WEAPONATTACK) {
		// Force Armor
		float rawDamage = damage;

		int forceArmor = defender->getSkillMod("force_armor");
		if (forceArmor > 0)
			defender->notifyObservers(ObserverEventType::FORCEBUFFHIT, attacker, rawDamage - (damage *= 1.f - (forceArmor / 100.f)));
	}

	// now apply the rest of the damage to the regular armor
	ManagedReference<ArmorObject*> armor = NULL;

	if (poolToDamage & CombatManager::HEALTH)
		armor = getHealthArmor(defender);
	else if (poolToDamage & CombatManager::ACTION)
		armor = getActionArmor(defender);
	else if (poolToDamage & CombatManager::MIND)
		armor = getMindArmor(defender);

	if (armor != NULL && !armor->isVulnerable(weapon->getDamageType())) {
		// use only the damage applied to the armor for piercing (after the PSG takes some off)
		float armorPiercing = getArmorPiercing(armor, weapon);
		float armorReduction = getArmorObjectReduction(weapon, armor);

		damage *= armorPiercing;
		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		// inflict condition damage
		armor->inflictDamage(armor, 0, damage * 0.1, true, true);
	}

	return damage;
}

float CombatManager::getArmorPiercing(ArmorObject* armor, WeaponObject* weapon) {
	int armorPiercing = weapon->getArmorPiercing();
	if (weapon->isBroken())
		armorPiercing = 0;

	int armorReduction = 0;

	if (armor != NULL && !armor->isBroken())
		armorReduction = armor->getRating();

	if (armorPiercing > armorReduction)
		return pow(1.25, armorPiercing - armorReduction);
	else
		return pow(0.50, armorReduction - armorPiercing);
}

float CombatManager::getArmorPiercing(AiAgent* defender, WeaponObject* weapon) {
	int armorPiercing = weapon->getArmorPiercing();
	if (weapon->isBroken())
		armorPiercing = 0;

	int armorReduction = defender->getArmor();

	if (armorPiercing > armorReduction)
		return pow(1.25, armorPiercing - armorReduction);
	else
		return pow(0.50, armorReduction - armorPiercing);
}

float CombatManager::calculateDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender) {
	float damage = 0;

	float minDamage = weapon->getMinDamage(), maxDamage = weapon->getMaxDamage();

	int maxDamageMuliplier = attacker->getSkillMod("private_max_damage_multiplier");
	int maxDamageDivisor = attacker->getSkillMod("private_max_damage_divisor");

	float diff = maxDamage - minDamage;

	if (maxDamageMuliplier != 0)
		diff *= maxDamageMuliplier;

	if (maxDamageDivisor != 0)
		diff /= maxDamageDivisor;

	if (diff >= 0)
		damage = System::random(diff) + (int)minDamage;

	if (attacker->isPlayerCreature())
		damage *= 1.5;

	if (weapon->getAttackType() == WeaponObject::MELEEATTACK && attacker->isPlayerCreature())
		damage *= 1.25;

	damage += getDamageModifier(attacker, weapon);

	if (attacker->isPlayerCreature()) {
		if (!weapon->isCertifiedFor(attacker))
			damage /= 5;
	}

	//info("damage to be dealt is " + String::valueOf(damage), true);

	return damage * 10;
}

float CombatManager::calculateDamage(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, const CreatureAttackData& data) {
	float damage = 0;

	if (data.getDamage() > 0) { // this is a special attack (force, heavy weapon, etc)
		damage = data.getDamage();
		damage -= System::random(damage / 4);
	} else {
		int diff = calculateDamageRange(attacker, defender, weapon);
		float minDamage = weapon->getMinDamage();

		if (diff >= 0)
			damage = System::random(diff) + (int)minDamage;

		weapon->decay(attacker, damage);

		if (attacker->isPlayerCreature()) {
			if (!weapon->isCertifiedFor(attacker))
				damage /= 5;
		}
	}

	if (attacker->isPlayerCreature())
		damage *= 1.5;

	if (weapon->getAttackType() == WeaponObject::MELEEATTACK && attacker->isPlayerCreature())
		damage *= 1.25;

	damage += getDamageModifier(attacker, weapon);
	damage += defender->getSkillMod("private_damage_susceptibility");

	/*if (defender->isKneeling())
		damage *= 1.5f;*/

	if (defender->isKnockedDown() || defender->isProne())
		damage *= 1.33f;

	// Toughness reduction
	if (data.getAttackType() == CombatManager::FORCEATTACK)
		damage = getDefenderToughnessModifier(defender, WeaponObject::FORCEATTACK, damage);
	else
		damage = getDefenderToughnessModifier(defender, weapon->getDamageType(), damage);

	// PvP Damage Reduction.
	if (attacker->isPlayerCreature() && defender->isPlayerCreature())
		damage *= 0.25;

	//info("damage to be dealt is " + String::valueOf(damage), true);

	return damage;
}

int CombatManager::getHitChance(CreatureObject* creature, CreatureObject* targetCreature, WeaponObject* weapon, int damage, int accuracyBonus) {
	int hitChance = 0;
	int attackType = weapon->getAttackType();

	//info("Calculating hit chance", true);

	float weaponAccuracy = 0.0f;
	// Get the weapon mods for range and add the mods for stance
	weaponAccuracy = getWeaponRangeModifier(creature->getDistanceTo(targetCreature), weapon);
	// accounts for steadyaim, general aim, and specific weapon aim, these buffs will clear after a completed combat action
	if (weapon->getAttackType() == WeaponObject::RANGEDATTACK) weaponAccuracy += creature->getSkillMod("private_aim");

	//info("Attacker weapon accuracy is " + String::valueOf(weaponAccuracy), true);

	int attackerAccuracy = getAttackerAccuracyModifier(creature, weapon);

	//info("Base attacker accuracy is " + String::valueOf(attackerAccuracy), true);

	// need to also add in general attack accuracy (mostly gotten from posture and states)
	int totalBonus = getAttackerAccuracyBonus(creature, weapon);
	totalBonus += calculateTargetPostureModifier(weapon, targetCreature);

	//info("Attacker accuracy bonus is " + String::valueOf(accuracyBonus), true);

	int targetDefense = getDefenderDefenseModifier(targetCreature, weapon);

	// first (and third) argument is divided by 2, second isn't
	float accTotal = hitChanceEquation(attackerAccuracy + weaponAccuracy + accuracyBonus, totalBonus, targetDefense);

	// this is the scout/ranger creature hit bonus that only works against creatures (not NPCS)
	if (targetCreature->isCreature())
		accTotal += creature->getSkillMod("creature_hit_bonus");

	accTotal -= targetCreature->getSkillMod("dodge_attack");

	//info("Final hit chance is " + String::valueOf(accTotal), true);

	if (accTotal > 100)
		accTotal = 100.0;
	else if (accTotal < 0)
		accTotal = 0;

	if (System::random(100) > accTotal) // miss, just return MISS
		return MISS;

	// now we have a successful hit, so calculate secondary defenses if there is a damage component
	if (damage > 0) {
		targetDefense = getDefenderSecondaryDefenseModifier(targetCreature);

		if (targetDefense <= 0)
			return HIT; // no secondary defenses

		ManagedReference<WeaponObject*> targetWeapon = targetCreature->getWeapon();
		Vector<String>* defenseAccMods = targetWeapon->getDefenderSecondaryDefenseModifiers();
		String def = defenseAccMods->get(0);

		if (def == "saber_block") {
			if ((weapon->getAttackType() == WeaponObject::RANGEDATTACK) && ((System::random(100)) < targetCreature->getSkillMod(def)))
				return RICOCHET;
			else return HIT;
		}

		accTotal = hitChanceEquation(attackerAccuracy + weaponAccuracy + accuracyBonus, totalBonus, targetDefense);

		if (accTotal > 100)
			accTotal = 100.0;
		else if (accTotal < 0)
			accTotal = 0;

		int cobMod = targetCreature->getSkillMod("private_center_of_being");

		if (System::random(100) > accTotal || (cobMod > 0 && System::random(100) > hitChanceEquation(attackerAccuracy + weaponAccuracy + accuracyBonus, totalBonus, cobMod))) { // successful secondary defense, return type of defense

			// this means use defensive acuity, which mean random 1, 2, or 3
			if (targetWeapon == NULL)
				return System::random(2) + 1;

			if (def == "block")
				return BLOCK;
			else if (def == "dodge")
				return DODGE;
			else if (def == "counterattack")
				return COUNTER;
			else if (def == "unarmed_passive_defense")
				return System::random(2) + 1;
			else // shouldn't get here
				return HIT; // no secondary defenses available on this weapon
		}
	}

	return HIT;
}



float CombatManager::calculateWeaponAttackSpeed(CreatureObject* attacker, WeaponObject* weapon, float skillSpeedRatio) {
	int speedMod = getSpeedModifier(attacker, weapon);
	float jediSpeed = attacker->getSkillMod("combat_haste") / 100.0f;

	float attackSpeed = (1.0f - ((float) speedMod / 100.0f)) * skillSpeedRatio * weapon->getAttackSpeed();

	if (jediSpeed > 0)
		attackSpeed = attackSpeed * jediSpeed;

	return MAX(attackSpeed, 1.0f);
}

void CombatManager::doMiss(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "miss", 0xFF, 0xFF, 0xFF);

	broadcastCombatSpam(attacker, defender, weapon, damage, cbtSpam);
}

void CombatManager::doCounterAttack(CreatureObject* creature, WeaponObject* weapon, CreatureObject* defender, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "counterattack", 0, 0xFF, 0);
	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(creature, defender, weapon, damage, cbtSpam);
}

void CombatManager::doBlock(CreatureObject* creature, WeaponObject* weapon, CreatureObject* defender, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "block", 0, 0xFF, 0);

	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(creature, defender, weapon, damage, cbtSpam);
}

void CombatManager::doLightsaberBlock(CreatureObject* creature, WeaponObject* weapon, CreatureObject* defender, int damage, const String& cbtSpam) {
	// No Fly Text.

	//creature->doCombatAnimation(defender, String("test_sword_ricochet").hashCode(), 0);

	broadcastCombatSpam(creature, defender, weapon, damage, cbtSpam);
}

void CombatManager::doDodge(CreatureObject* creature, WeaponObject* weapon, CreatureObject* defender, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "dodge", 0, 0xFF, 0);

	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(creature, defender, weapon, damage, cbtSpam);
}

bool CombatManager::applySpecialAttackCost(CreatureObject* attacker, WeaponObject* weapon, const CreatureAttackData& data) {
	if (attacker->isAiAgent())
		return true;

	float force = weapon->getForceCost() * data.getForceCostMultiplier();

	if (force > 0) { // Need Force check first otherwise it can be spammed.
		ManagedReference<PlayerObject*> playerObject = attacker->getPlayerObject();
		if (playerObject != NULL) {
			if (playerObject->getForcePower() <= force) {
				attacker->sendSystemMessage("@jedi_spam:no_force_power");
				return false;
			} else {
				playerObject->setForcePower(playerObject->getForcePower() - force);
			}
		}
	}

	float health = weapon->getHealthAttackCost() * data.getHealthCostMultiplier();
	float action = weapon->getActionAttackCost() * data.getActionCostMultiplier();
	float mind = weapon->getMindAttackCost() * data.getMindCostMultiplier();

	/*
	health -= MIN(health, health * attacker->getHAM(CreatureAttribute::STRENGTH) / 10000.f);
	action -= MIN(action, action * attacker->getHAM(CreatureAttribute::QUICKNESS) / 10000.f);
	mind -= MIN(mind, mind * attacker->getHAM(CreatureAttribute::FOCUS) / 10000.f);
	 */

	health = MAX(0, health - (float(attacker->getHAM(CreatureAttribute::STRENGTH)) / 10.f));
	action = MAX(0, action - (float(attacker->getHAM(CreatureAttribute::QUICKNESS)) / 10.f));
	mind = MAX(0, mind - (float(attacker->getHAM(CreatureAttribute::FOCUS)) / 10.f));

	if (attacker->getHAM(CreatureAttribute::HEALTH) <= health)
		return false;

	if (attacker->getHAM(CreatureAttribute::ACTION) <= action)
		return false;

	if (attacker->getHAM(CreatureAttribute::MIND) <= mind)
		return false;

	if (health > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::HEALTH, health, true);

	if (action > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::ACTION, action, true);

	if (mind > 0)
		attacker->inflictDamage(attacker, CreatureAttribute::MIND, mind, true);

	weapon->decay(attacker, health + action + mind);

	return true;
}

void CombatManager::applyStates(CreatureObject* creature, CreatureObject* targetCreature, const CreatureAttackData& data) {
	VectorMap<uint64, StateEffect>* stateEffects = data.getStateEffects();

	// loop through all the states in the command
	for (int i = 0; i < stateEffects->size(); i++) {
		StateEffect effect = stateEffects->get(i);
		bool failed = false;
		uint8 effectType = effect.getEffectType();

		if (System::random(100) > effect.getStateChance()) continue; // effect didn't trigger this attack and don't send a message

		Vector<String> exclusionTimers = effect.getDefenderExclusionTimers();
		// loop through any exclusion timers
		for (int j = 0; j < exclusionTimers.size(); j++)
			if (!targetCreature->checkCooldownRecovery(exclusionTimers.get(j))) failed = true;

		float targetDefense = 0.f;

		// if recovery timer conditions aren't satisfied, it won't matter
		if (!failed) {
			Vector<String> defenseMods = effect.getDefenderStateDefenseModifiers();
			// add up all defenses against the state the target has
			for (int j = 0; j < defenseMods.size(); j++)
				targetDefense += targetCreature->getSkillMod(defenseMods.get(j));

			targetDefense -= targetCreature->calculateBFRatio();

			// now roll to see if it gets applied
			int defDiff = targetDefense - 95;
			float lRatio = MAX(0.5, targetCreature->getLevel() / MAX(1, creature->getLevel()));
			if (defDiff > 0)
				targetDefense = 95 + defDiff * 0.25 * lRatio;

			if (targetDefense > 0 && System::random(100) < targetDefense)
				failed = true;
			/*
			 * old way...
			uint32 strength = effect.getStateStrength();
			if (strength == 0) strength = getAttackerAccuracyModifier(creature, creature->getWeapon());
			strength += creature->getSkillMod(data.getCommand()->getAccuracySkillMod());
			if (targetDefense > 0 && strength != 0 && System::random(100) > hitChanceEquation(strength, 0.0f, targetDefense))
				failed = true;
			 */

			// no reason to apply jedi defenses if primary defense was successful
			if (!failed) {
				targetDefense = 0.f;
				Vector<String> jediMods = effect.getDefenderJediStateDefenseModifiers();
				// second chance for jedi, roll against their special defense "jedi_state_defense"
				for (int j = 0; j < jediMods.size(); j++)
					targetDefense += targetCreature->getSkillMod(jediMods.get(j));

				// now roll again to see if it gets applied
				defDiff = targetDefense - 95;
				if (defDiff > 0)
					targetDefense = 95 + defDiff * 0.25 * lRatio;

				if (targetDefense > 0 && System::random(100) < targetDefense)
					failed = true;
				//if (targetDefense > 0 && strength != 0 && System::random(100) > hitChanceEquation(strength, 0.0f, targetDefense))
				//	failed = true;
			}
		}

		if (!failed)
			data.getCommand()->applyEffect(targetCreature, effectType, creature->getSkillMod(data.getCommand()->getAccuracySkillMod()));

		// can move this to scripts, but only these three states have fail messages
		if (failed) {
			switch (effectType) {
			case CommandEffect::KNOCKDOWN:
				creature->sendSystemMessage("@cbt_spam:knockdown_fail");
				break;
			case CommandEffect::POSTUREDOWN:
			case CommandEffect::POSTUREUP:
				creature->sendSystemMessage("@cbt_spam:posture_change_fail");
				break;
			case CommandEffect::NEXTATTACKDELAY:
				creature->sendSystemMessage("@combat_effects:warcry_miss");
				break;
			case CommandEffect::INTIMIDATE:
				creature->sendSystemMessage("@combat_effects:intimidated_miss");
				break;
			default:
				break;
			}
		}

		// now check combat equilibrium
		if (!failed && (effectType == CommandEffect::KNOCKDOWN || effectType == CommandEffect::POSTUREDOWN || effectType == CommandEffect::POSTUREUP)) {
			int combatEquil = targetCreature->getSkillMod("combat_equillibrium");

			if (combatEquil > 100)
				combatEquil = 100;

			if ((combatEquil >> 1) > (int) System::random(100))
				targetCreature->setPosture(CreaturePosture::UPRIGHT, true);
		}
	}
}

int CombatManager::calculatePoolsToDamage(int poolsToDamage) {
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

int CombatManager::applyDamage(CreatureObject* attacker, WeaponObject* weapon, CreatureObject* defender, int damage, float damageMultiplier, int poolsToDamage, const CreatureAttackData& data) {
	if (poolsToDamage == 0 || damageMultiplier == 0)
		return 0;

	float ratio = weapon->getWoundsRatio();
	float healthDamage = 0.f, actionDamage = 0.f, mindDamage = 0.f;
	bool wounded = false;

	if (defender->isPlayerCreature() && defender->getPvpStatusBitmask() == CreatureFlag::NONE) {
		return 0;
	}

	String xpType;
	switch (data.getAttackType()) {
	case CombatManager::FORCEATTACK:
		xpType = "jedi_general";
		break;
	default:
		xpType = weapon->getXpType();
		break;
	}



	if (poolsToDamage & HEALTH) {
		healthDamage = getArmorReduction(attacker, weapon, defender, damage, HEALTH, data) * damageMultiplier * data.getHealthDamageMultiplier();
		defender->inflictDamage(attacker, CreatureAttribute::HEALTH, (int)healthDamage, true, xpType);
		if (!wounded && System::random(100) < ratio) {
			defender->addWounds(CreatureAttribute::HEALTH + System::random(2), 1, true);
			wounded = true;
		}
	}

	if (poolsToDamage & ACTION) {
		actionDamage = getArmorReduction(attacker, weapon, defender, damage, ACTION, data) * damageMultiplier * data.getActionDamageMultiplier();
		defender->inflictDamage(attacker, CreatureAttribute::ACTION, (int)actionDamage, true, xpType);
		if (!wounded && System::random(100) < ratio) {
			defender->addWounds(CreatureAttribute::ACTION + System::random(2), 1, true);
			wounded = true;
		}
	}

	if (poolsToDamage & MIND) {
		mindDamage = getArmorReduction(attacker, weapon, defender, damage, MIND, data) * damageMultiplier * data.getMindDamageMultiplier();
		defender->inflictDamage(attacker, CreatureAttribute::MIND, (int)mindDamage, true, xpType);
		if (!wounded && System::random(100) < ratio) {
			defender->addWounds(CreatureAttribute::MIND + System::random(2), 1, true);
			wounded = true;
		}
	}

	if (wounded)
		defender->addShockWounds(1, true);

	// This method can be called multiple times for area attacks.  Let the calling method decrease the powerup once
	if (!data.getCommand()->isAreaAction() && !data.getCommand()->isConeAction())
		weapon->decreasePowerupUses(attacker);

	return (int) (healthDamage + actionDamage + mindDamage);
}

int CombatManager::applyDamage(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defender, float damageMultiplier, int poolsToDamage, const CreatureAttackData& data) {
	if (poolsToDamage == 0)
		return 0;

	if (defender->isPlayerCreature() && defender->getPvpStatusBitmask() == CreatureFlag::NONE) {
		return 0;
	}

	// TODO: take into account special attack data when calculating damage
	int damage = calculateDamage(attacker, weapon, defender);

	String xpType;
	switch (data.getAttackType()) {
	case CombatManager::FORCEATTACK:
		xpType = "jedi_general";
		break;
	default:
		xpType = weapon->getXpType();
		break;
	}

	if(defender->isTurret()){
		damage *= (1.f - (getArmorTurretReduction(attacker, defender, weapon) / 100.f));
	}

	defender->inflictDamage(attacker, 0, damage, true, xpType);

	// This method can be called multiple times for area attacks.  Let the calling method decrease the powerup once
	if (!data.getCommand()->isAreaAction() && !data.getCommand()->isConeAction())
		weapon->decreasePowerupUses(attacker);

	return damage;
}

void CombatManager::broadcastCombatSpam(CreatureObject* attacker, TangibleObject* defender, TangibleObject* weapon, uint32 damage, const String& stringid) {
	Zone* zone = attacker->getZone();

	if (zone == NULL)
		return;

	//Locker _locker(zone);

	CloseObjectsVector* vec = (CloseObjectsVector*) attacker->getCloseObjects();

	SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;

	if (vec != NULL) {
		closeObjects.removeAll(vec->size(), 10);
		vec->safeCopyTo(closeObjects);
	} else {
		zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), 128, &closeObjects, true);
	}


	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* object = cast<SceneObject*>( closeObjects.get(i).get());

		if (object->isPlayerCreature() && attacker->isInRange(object, 70)) {
			CreatureObject* player = cast<CreatureObject*>( object);

			CombatSpam* msg = new CombatSpam(attacker, defender, weapon, damage, "cbt_spam", stringid, player);
			player->sendMessage(msg);
		}
	}
}

void CombatManager::broadcastCombatAction(CreatureObject * attacker, TangibleObject * defenderObject, WeaponObject* weapon, const CreatureAttackData & data, uint8 hit) {
	CombatAction* combatAction = NULL;

	uint32 animationCRC = data.getAnimationCRC();

	if (!attacker->isCreature() && animationCRC == 0)
		animationCRC = getDefaultAttackAnimation(attacker);

	// TODO: this needs to be fixed.
	if (attacker->isCreature() && animationCRC == 0) {
		if (attacker->getGameObjectType() == SceneObjectType::DROIDCREATURE || attacker->getGameObjectType() == SceneObjectType::PROBOTCREATURE) {
			animationCRC = String("droid_attack_light").hashCode();
		} else {
			animationCRC = String("creature_attack_light").hashCode();
		}

	}

	if (defenderObject->isCreatureObject())
		combatAction = new CombatAction(attacker, cast<CreatureObject*>(defenderObject), animationCRC, hit, data.getTrails(), weapon->getObjectID());
	else
		combatAction = new CombatAction(attacker, defenderObject, animationCRC, hit, data.getTrails(), weapon->getObjectID());

	attacker->broadcastMessage(combatAction, true);

	String effect = data.getCommand()->getEffectString();

	if (!effect.isEmpty())
		attacker->playEffect(effect);
}

void CombatManager::requestDuel(CreatureObject* player, CreatureObject* targetPlayer) {
	/* Pre: player != targetPlayer and not NULL; player is locked
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

	player->info("requesting duel");

	ghost->addToDuelList(targetPlayer);

	if (targetGhost->requestedDuelTo(player)) {
		BaseMessage* pvpstat = new UpdatePVPStatusMessage(targetPlayer,
				targetPlayer->getPvpStatusBitmask()
				| CreatureFlag::ATTACKABLE
				| CreatureFlag::AGGRESSIVE);
		player->sendMessage(pvpstat);

		StringIdChatParameter stringId("duel", "accept_self");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		BaseMessage* pvpstat2 = new UpdatePVPStatusMessage(player,
				player->getPvpStatusBitmask() | CreatureFlag::ATTACKABLE
				| CreatureFlag::AGGRESSIVE);
		targetPlayer->sendMessage(pvpstat2);

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

void CombatManager::requestEndDuel(CreatureObject* player, CreatureObject* targetPlayer) {
	/* Pre: player != targetPlayer and not NULL; player is locked
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

	player->info("ending duel");

	ghost->removeFromDuelList(targetPlayer);
	player->removeDefender(targetPlayer);

	if (targetGhost->requestedDuelTo(player)) {
		targetGhost->removeFromDuelList(player);
		targetPlayer->removeDefender(player);

		player->sendPvpStatusTo(targetPlayer);

		StringIdChatParameter stringId("duel", "end_self");
		stringId.setTT(targetPlayer->getObjectID());
		player->sendSystemMessage(stringId);

		targetPlayer->sendPvpStatusTo(player);

		StringIdChatParameter stringId2("duel", "end_target");
		stringId2.setTT(player->getObjectID());
		targetPlayer->sendSystemMessage(stringId2);
	}
}

void CombatManager::freeDuelList(CreatureObject* player, bool spam) {
	/* Pre: player not NULL and is locked
	 * Post: player removed and warned all of the objects from its duel list
	 */
	PlayerObject* ghost = player->getPlayerObject();

	if (ghost == NULL || ghost->isDuelListEmpty())
		return;

	player->info("freeing duel list");

	while (ghost->getDuelListSize() != 0) {
		ManagedReference<CreatureObject*> targetPlayer = ghost->getDuelListObject(0);
		PlayerObject* targetGhost = targetPlayer->getPlayerObject();

		if (targetPlayer != NULL && targetGhost != NULL && targetPlayer.get() != player) {
			try {
				Locker clocker(targetPlayer, player);

				ghost->removeFromDuelList(targetPlayer);
				player->removeDefender(targetPlayer);

				if (targetGhost->requestedDuelTo(player)) {
					targetGhost->removeFromDuelList(player);
					targetPlayer->removeDefender(player);

					player->sendPvpStatusTo(targetPlayer);

					if (spam) {
						StringIdChatParameter stringId("duel", "end_self");
						stringId.setTT(targetPlayer->getObjectID());
						player->sendSystemMessage(stringId);
					}

					targetPlayer->sendPvpStatusTo(player);

					if (spam) {
						StringIdChatParameter stringId2("duel", "end_target");
						stringId2.setTT(player->getObjectID());
						targetPlayer->sendSystemMessage(stringId2);
					}
				}


			} catch (ObjectNotDeployedException& e) {
				ghost->removeFromDuelList(targetPlayer);

				System::out << "Exception on CombatManager::freeDuelList()\n"
						<< e.getMessage() << "\n";
			}
		}
	}
}

void CombatManager::declineDuel(CreatureObject* player, CreatureObject* targetPlayer) {
	/* Pre: player != targetPlayer and not NULL; player is locked
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
	}
}

bool CombatManager::checkConeAngle(SceneObject* target, float angle,
		float creatureVectorX, float creatureVectorY, float directionVectorX,
		float directionVectorY) {
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

uint32 CombatManager::getDefaultAttackAnimation(CreatureObject* creature) {
	WeaponObject* weapon = creature->getWeapon();

	if (weapon->isRangedWeapon())
		return 0x506E9D4C;
	else
		return defaultAttacks[System::random(8)];
}

int CombatManager::doCombatAction(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* defender, CombatQueueCommand* command){

	if(weapon != NULL){
		if(!command->isAreaAction()){
			return doTargetCombatAction(attacker, weapon, defender, CreatureAttackData("",command));
		} else {
			return doAreaCombatAction(attacker, weapon, defender, CreatureAttackData("",command));
		}
	}

	return 0;
}

int CombatManager::doTargetCombatAction(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* tano, const CreatureAttackData& data) {

	int damage = 0;

	Locker clocker(tano, attacker);

	if (tano->isCreatureObject()) {
		CreatureObject* defenderObject = cast<CreatureObject*>( tano);
		damage = doTargetCombatAction(attacker, weapon, defenderObject, data);
	} else {
		// TODO: implement, tano->tano damage
	}

	return damage;
}

// called for single target attackers
int CombatManager::doTargetCombatAction(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defenderObject, const CreatureAttackData& data) {
	if(defenderObject == NULL || !defenderObject->isAttackableBy(attacker))
		return 0;

	attacker->addDefender(defenderObject);
	defenderObject->addDefender(attacker);

	float minDamage = weapon->getMinDamage(), maxDamage = weapon->getMaxDamage();
	float diff = maxDamage - minDamage;
	float damage = 0;
	float damageMultiplier = data.getDamageMultiplier();
	if (diff >= 0)
		damage = System::random(diff) + (int)minDamage;

	damage *= damageMultiplier;

	damage = getArmorReduction(attacker, weapon, defenderObject, damage, CombatManager::HEALTH, data);


	int hitVal = getHitChance(attacker, defenderObject, weapon, damage, data.getAccuracyBonus());

	String combatSpam = data.getCommand()->getCombatSpam();

	// FIXME: probably need to add getCombatSpamBlock(), etc in data and store it in commands explicitly to avoid malformed text

	CombatAction* combatAction = NULL;

	uint32 animationCRC = data.getAnimationCRC();
	combatAction = new CombatAction(attacker, defenderObject, animationCRC, hitVal, CombatManager::DEFAULTTRAIL);
	attacker->broadcastMessage(combatAction,true);

	switch (hitVal) {
	case MISS:
		doMiss(attacker, defenderObject, weapon, damage, combatSpam + "_miss");
		return 0;
		break;
	case HIT:
		broadcastCombatSpam(attacker, defenderObject, weapon, damage, combatSpam + "_hit");
		break;
	case BLOCK:
		doBlock(attacker, defenderObject, weapon, damage, combatSpam + "_block");
		damageMultiplier = 0.5f;
		break;
	case DODGE:
		doDodge(attacker, defenderObject, weapon, damage, combatSpam + "_evade");
		damageMultiplier = 0.0f;
		break;
	case COUNTER:
		doCounterAttack(attacker, defenderObject, weapon, damage, combatSpam + "_counter");
		damageMultiplier = 0.0f;
		break;
	case RICOCHET:
		doLightsaberBlock(attacker, defenderObject, weapon, damage, combatSpam + "_block");
		damageMultiplier = 0.0f;
		break;
	default:
		break;
	}


	// TODO:  create an apply damage(tano, creature)
	defenderObject->inflictDamage(attacker,CreatureAttribute::HEALTH, damage * damageMultiplier,true);



	return damage;
}





int CombatManager::applyDamage(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defenderObject, float damageMultiplier, int poolsToDamage, const CreatureAttackData& data){


	return 0;
}

int CombatManager::doAreaCombatAction(CreatureObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, const CreatureAttackData& data) {
	float creatureVectorX = attacker->getPositionX();
	float creatureVectorY = attacker->getPositionY();

	float directionVectorX = defenderObject->getPositionX() - creatureVectorX;
	float directionVectorY = defenderObject->getPositionY() - creatureVectorY;

	Zone* zone = attacker->getZone();

	if (zone == NULL)
		return 0;

	PlayerManager* playerManager = zone->getZoneServer()->getPlayerManager();

	int damage = 0;

	int range = data.getAreaRange();

	if (data.getCommand()->isConeAction()) {
		range = data.getRange();
	}

	if (range < 0) {
		range = weapon->getMaxRange();
	}

	if (weapon->isThrownWeapon() || weapon->isHeavyWeapon())
		range = weapon->getMaxRange() + data.getAreaRange();

	try {
		//zone->rlock();

		CloseObjectsVector* vec = (CloseObjectsVector*) attacker->getCloseObjects();

		SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;

		if (vec != NULL) {
			closeObjects.removeAll(vec->size(), 10);
			vec->safeCopyTo(closeObjects);
		} else {
			zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), 128, &closeObjects, true);
		}

		for (int i = 0; i < closeObjects.size(); ++i) {
			ManagedReference<SceneObject*> object = cast<SceneObject*>(closeObjects.get(i).get());

			if (!object->isTangibleObject()) {
				//error("object is not tangible");
				continue;
			}

			TangibleObject* tano = cast<TangibleObject*>(object.get());

			if (object == attacker) {
				//error("object is attacker");
				continue;
			}

			if (!attacker->isInRange(object, range)) {
				//error("not in range " + String::valueOf(range));
				continue;
			}

			if (weapon->isThrownWeapon() || weapon->isHeavyWeapon()) {
				if (!(tano == defenderObject) && !(tano->isInRange(defenderObject, data.getAreaRange())))
					continue;
			}

			if (tano->isCreatureObject() && (cast<CreatureObject*>(tano))->isIncapacitated()) {
				//error("object is incapacitated");
				continue;
			}

			if (data.getCommand()->isConeAction() && !checkConeAngle(tano, data.getConeAngle(), creatureVectorX, creatureVectorY, directionVectorX, directionVectorY)) {
				//error("object is not in cone angle");
				continue;
			}

			//			zone->runlock();

			try {
				if (tano == defenderObject || (!(weapon->isThrownWeapon()) && !(weapon->isHeavyWeapon()))) {
					if (CollisionManager::checkLineOfSight(object, attacker)) {
						damage += doTargetCombatAction(attacker, weapon, tano, data);
					}
				} else {
					if (CollisionManager::checkLineOfSight(object, defenderObject)) {
						damage += doTargetCombatAction(attacker, weapon, tano, data);
					}
				}
			} catch (Exception& e) {
				error(e.getMessage());
			} catch (...) {
				//zone->rlock();

				throw;
			}

			//			zone->rlock();
		}

		//		zone->runlock();
	} catch (...) {
		//		zone->runlock();

		throw;
	}

	weapon->decreasePowerupUses(attacker);
	return damage;
}

int CombatManager::doAreaCombatAction(TangibleObject* attacker, WeaponObject* weapon, TangibleObject* defenderObject, const CreatureAttackData& data){
	float creatureVectorX = attacker->getPositionX();
	float creatureVectorY = attacker->getPositionY();

	float directionVectorX = defenderObject->getPositionX() - creatureVectorX;
	float directionVectorY = defenderObject->getPositionY() - creatureVectorY;

	Zone* zone = attacker->getZone();

	if (zone == NULL)
		return 0;

	PlayerManager* playerManager = zone->getZoneServer()->getPlayerManager();

	int damage = 0;

	int range = data.getAreaRange();

	if (data.getCommand()->isConeAction()) {
		range = data.getRange();
	}

	if (range < 0) {
		range = weapon->getMaxRange();
	}

	try {
		//zone->rlock();

		CloseObjectsVector* vec = (CloseObjectsVector*) attacker->getCloseObjects();

		SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;

		if (vec != NULL) {
			closeObjects.removeAll(vec->size(), 10);
			vec->safeCopyTo(closeObjects);
		} else {
			zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), 128, &closeObjects, true);
		}

		for (int i = 0; i < closeObjects.size(); ++i) {
			ManagedReference<SceneObject*> object = cast<SceneObject*>(closeObjects.get(i).get());

			if (!object->isTangibleObject()) {
				//error("object is not tangible");
				continue;
			}

			TangibleObject* tano = cast<TangibleObject*>(object.get());

			if (object == attacker) {
				//error("object is attacker");
				continue;
			}

			if (!attacker->isInRange(object, range)) {
				//error("not in range " + String::valueOf(range));
				continue;
			}

			if (tano->isCreatureObject() && (cast<CreatureObject*>(tano))->isIncapacitated()) {
				//error("object is incapacitated");
				continue;
			}

			if (data.getCommand()->isConeAction() && !checkConeAngle(tano, data.getConeAngle(), creatureVectorX, creatureVectorY, directionVectorX, directionVectorY)) {
				//error("object is not in cone angle");
				continue;
			}

			//			zone->runlock();

			try {
				if (CollisionManager::checkLineOfSight(object, attacker)) {
					damage += doTargetCombatAction(attacker, weapon, tano, data);

				}
			} catch (Exception& e) {
				error(e.getMessage());
			} catch (...) {
				//zone->rlock();

				throw;
			}

			//			zone->rlock();
		}

		//		zone->runlock();
	} catch (...) {
		//		zone->runlock();

		throw;
	}

	return damage;
	return 0;
}


int CombatManager::getArmorReduction(TangibleObject* attacker, WeaponObject* weapon, CreatureObject* defender, float damage, int poolToDamage, const CreatureAttackData& data){
	if (poolToDamage == 0)
		return 0;

	if(weapon == NULL)
		return 0;

	// the easy calculation
	if (defender->isAiAgent()) {
		damage *= getArmorPiercing(cast<AiAgent*>(defender), weapon);

		float armorReduction = getArmorNpcReduction(NULL, cast<AiAgent*>(defender), weapon);
		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		return damage;
	}

	// start with PSG reduction
	ManagedReference<ArmorObject*> psg = getPSGArmor(defender);

	if (psg != NULL && !psg->isVulnerable(weapon->getDamageType())) {
		float armorPiercing = getArmorPiercing(psg, weapon);
		float armorReduction =  getArmorObjectReduction(weapon, psg);

		if (armorPiercing > 1) damage *= armorPiercing;
		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		// inflict condition damage
		// TODO: this formula makes PSG's take more damage than regular armor, but that's how it was on live
		// it can be fixed by doing condition damage after all damage reductions
		psg->inflictDamage(psg, 0, damage * 0.1, true, true);
	}

	if (data.getAttackType() == CombatManager::WEAPONATTACK) {
		// Force Armor
		float rawDamage = damage;

		int forceArmor = defender->getSkillMod("force_armor");
		if (forceArmor > 0)
			defender->notifyObservers(ObserverEventType::FORCEBUFFHIT, attacker, rawDamage - (damage *= 1.f - (forceArmor / 100.f)));
	}

	// now apply the rest of the damage to the regular armor
	ManagedReference<ArmorObject*> armor = NULL;

	if (poolToDamage & CombatManager::HEALTH)
		armor = getHealthArmor(defender);
	else if (poolToDamage & CombatManager::ACTION)
		armor = getActionArmor(defender);
	else if (poolToDamage & CombatManager::MIND)
		armor = getMindArmor(defender);

	if (armor != NULL && !armor->isVulnerable(weapon->getDamageType())) {
		// use only the damage applied to the armor for piercing (after the PSG takes some off)
		float armorPiercing = getArmorPiercing(armor, weapon);
		float armorReduction = getArmorObjectReduction(weapon, armor);

		damage *= armorPiercing;

		if (armorReduction > 0) damage *= (1.f - (armorReduction / 100.f));

		// inflict condition damage
		armor->inflictDamage(armor, 0, damage * 0.1, true, true);
	}

	return damage;
}

int CombatManager::getArmorTurretReduction(CreatureObject* attacker, TangibleObject* defender, WeaponObject* weapon){
	int resist = 0;

	if(defender != NULL && defender->isTurret() && weapon != NULL){
		DataObjectComponentReference* data = defender->getDataObjectComponent();
		if(data != NULL){

			TurretDataComponent* turretData = cast<TurretDataComponent*>(data->get());

			if(turretData != NULL) {

				int damageType = weapon->getDamageType();

				switch (damageType) {
				case WeaponObject::KINETIC:
					resist = turretData->getKinetic();
					break;
				case WeaponObject::ENERGY:
					resist = turretData->getEnergy();
					break;
				case WeaponObject::ELECTRICITY:
					resist = turretData->getElectricity();
					break;
				case WeaponObject::STUN:
					resist = turretData->getStun();
					break;
				case WeaponObject::BLAST:
					resist = turretData->getBlast();
					break;
				case WeaponObject::HEAT:
					resist = turretData->getHeat();
					break;
				case WeaponObject::COLD:
					resist = turretData->getCold();
					break;
				case WeaponObject::ACID:
					resist = turretData->getAcid();
					break;
				case WeaponObject::LIGHTSABER:
					resist = turretData->getLightSaber();
					break;
				case WeaponObject::FORCE:
					resist = 0;
					break;
				}
			}
		}
	}

	return resist;
}

int CombatManager::getHitChance(TangibleObject* tano, CreatureObject* targetCreature, WeaponObject* weapon, int damage, int accuracyBonus) {
	int hitChance = 0;
	int attackType = weapon->getAttackType();

	//info("Calculating hit chance", true);

	float weaponAccuracy = 0.0f;
	// Get the weapon mods for range and add the mods for stance
	weaponAccuracy = getWeaponRangeModifier(tano->getDistanceTo(targetCreature), weapon);

	int totalBonus = 0;
	//info("Attacker accuracy bonus is " + String::valueOf(accuracyBonus), true);

	int targetDefense = getDefenderDefenseModifier(targetCreature, weapon);

	if(tano->isTurret()){
		totalBonus += tano->getHitChance() * 100;
	}
	// first (and third) argument is divided by 2, second isn't
	float accTotal = hitChanceEquation(weaponAccuracy, totalBonus, targetDefense);

	if (accTotal > 100)
		accTotal = 100.0;
	else if (accTotal < 0)
		accTotal = 0;

	if (System::random(100) > accTotal) // miss, just return MISS
		return MISS;

	// now we have a successful hit, so calculate secondary defenses if there is a damage component
	if (damage > 0) {
		targetDefense = getDefenderSecondaryDefenseModifier(targetCreature);

		if (targetDefense <= 0)
			return HIT; // no secondary defenses

		ManagedReference<WeaponObject*> targetWeapon = targetCreature->getWeapon();
		Vector<String>* defenseAccMods = targetWeapon->getDefenderSecondaryDefenseModifiers();
		String def = defenseAccMods->get(0);

		if (def == "saber_block") {
			if ((weapon->getAttackType() == WeaponObject::RANGEDATTACK) && ((System::random(100)) < targetCreature->getSkillMod(def)))
				return RICOCHET;
			else return HIT;
		}

		accTotal = hitChanceEquation(weaponAccuracy , totalBonus, targetDefense);

		if (accTotal > 100)
			accTotal = 100.0;
		else if (accTotal < 0)
			accTotal = 0;

		int cobMod = targetCreature->getSkillMod("private_center_of_being");

		if (System::random(100) > accTotal || (cobMod > 0 && System::random(100) > hitChanceEquation(weaponAccuracy, totalBonus, cobMod))) { // successful secondary defense, return type of defense

			// this means use defensive acuity, which mean random 1, 2, or 3
			if (targetWeapon == NULL)
				return System::random(2) + 1;

			if (def == "block")
				return BLOCK;
			else if (def == "dodge")
				return DODGE;
			else if (def == "counterattack")
				return COUNTER;
			else if (def == "unarmed_passive_defense")
				return System::random(2) + 1;
			else // shouldn't get here
				return HIT; // no secondary defenses available on this weapon
		}
	}

	return HIT;
}

void CombatManager::broadcastCombatSpam(TangibleObject* attacker, TangibleObject* defender, TangibleObject* weapon, uint32 damage, const String& stringid) {
	Zone* zone = attacker->getZone();

	if (zone == NULL)
		return;

	//Locker _locker(zone);

	CloseObjectsVector* vec = (CloseObjectsVector*) attacker->getCloseObjects();

	SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;

	if (vec != NULL) {
		closeObjects.removeAll(vec->size(), 10);
		vec->safeCopyTo(closeObjects);
	} else {
		zone->getInRangeObjects(attacker->getWorldPositionX(), attacker->getWorldPositionY(), 128, &closeObjects, true);
	}


	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* object = cast<SceneObject*>( closeObjects.get(i).get());

		if (object->isPlayerCreature() && attacker->isInRange(object, 70)) {
			CreatureObject* player = cast<CreatureObject*>( object);

			CombatSpam* msg = new CombatSpam(attacker, defender, weapon, damage, "cbt_spam", stringid, player);
			player->sendMessage(msg);
		}
	}
}

void CombatManager::doMiss(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "miss", 0xFF, 0xFF, 0xFF);

	broadcastCombatSpam(attacker, defender, weapon, damage, cbtSpam);
}

void CombatManager::doCounterAttack(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "counterattack", 0, 0xFF, 0);
	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(attacker, defender,weapon, damage, cbtSpam);
}

void CombatManager::doBlock(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "block", 0, 0xFF, 0);

	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(attacker, defender, weapon, damage, cbtSpam);
}

void CombatManager::doLightsaberBlock(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon, int damage, const String& cbtSpam) {
	// No Fly Text.

	//creature->doCombatAnimation(defender, String("test_sword_ricochet").hashCode(), 0);

	broadcastCombatSpam(attacker, defender, weapon, damage, cbtSpam);
}

void CombatManager::doDodge(TangibleObject* attacker, CreatureObject* defender, WeaponObject* weapon, int damage, const String& cbtSpam) {
	defender->showFlyText("combat_effects", "dodge", 0, 0xFF, 0);

	//defender->doCombatAnimation(defender, String("dodge").hashCode(), 0);

	broadcastCombatSpam(attacker, defender, weapon, damage, cbtSpam);
}
