/*
 * CombatQueueCommand.h
 *
 *  Created on: 24/05/2010
 *      Author: victor
 */

#ifndef COMBATQUEUECOMMAND_H_
#define COMBATQUEUECOMMAND_H_

#include"server/zone/ZoneServer.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/managers/combat/CreatureAttackData.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "server/zone/objects/creature/CreatureState.h"
#include "server/zone/objects/creature/commands/effect/StateEffect.h"
#include "server/zone/objects/creature/commands/effect/DotEffect.h"
#include "server/zone/objects/creature/commands/effect/CommandEffect.h"
#include "QueueCommand.h"
#include "server/zone/managers/collision/PathFinderManager.h"

class CombatQueueCommand : public QueueCommand {
protected:
	float damage;
	float damageMultiplier;
	int accuracyBonus;
	float speedMultiplier;
	float speed;
	int poolsToDamage;

	float healthCostMultiplier;
	float actionCostMultiplier;
	float mindCostMultiplier;
	float forceCostMultiplier;
	float forceCost;

	int range;

	String accuracySkillMod;

	bool areaAction;
	bool coneAction;
	int coneAngle;
	int areaRange;

	String combatSpam;
	String stateSpam;
	uint32 animationCRC;
	String effectString;

	VectorMap<uint64, StateEffect> stateEffects;
	VectorMap<uint64, DotEffect> dotEffects;

	uint8 attackType;
	uint8 trails;

public:

	CombatQueueCommand(const String& name, ZoneProcessServer* server) : QueueCommand(name, server) {

		damage = 0;
		damageMultiplier = 1;
		accuracyBonus = 0;
		speedMultiplier = 1;
		speed = 0;
		healthCostMultiplier = 1;
		actionCostMultiplier = 1;
		mindCostMultiplier = 1;

		// Force Power is only set in Jedi-skills.
		forceCostMultiplier = 0;
		forceCost = 0;

		poolsToDamage = CombatManager::RANDOM;

		coneAngle = 30;

		//for weapon set -1
		range = -1;

		accuracySkillMod = "";

		areaRange = 0;
		areaAction = false;
		coneAction = false;

		combatSpam = "";
		animationCRC = 0;

		attackType = CombatManager::WEAPONATTACK;
		trails = CombatManager::DEFAULTTRAIL;
	}

	int doCombatAction(CreatureObject* creature, const uint64& target, const UnicodeString& arguments = "", ManagedReference<WeaponObject*> weapon = NULL) {
		ManagedReference<SceneObject*> targetObject = server->getZoneServer()->getObject(target);
		PlayerManager* playerManager = server->getPlayerManager();

		if (targetObject == NULL || !targetObject->isTangibleObject() || targetObject == creature)
			return INVALIDTARGET;

		float checkRange = range;

		if (weapon == NULL) {
			if(creature->getWeapon() == NULL) {
				return GENERALERROR;
			}
			else {
				weapon = creature->getWeapon();
			}
		}

		if (checkRange == -1) {
			checkRange = weapon->getMaxRange();
		}

		if (creature->isDead() || (creature->isPet() && creature->isIncapacitated()))
			return INVALIDLOCOMOTION;

		if (creature->isPlayerCreature()){
			if (creature->getPlayerObject() && creature->getPlayerObject()->isAFK()) {
				return GENERALERROR;
			}
		}

		if (creature->isKneeling() && weapon->isMeleeWeapon() && range == -1 && !weapon->isJediWeapon())
			return NOKNEELING;

		if (creature->isProne() && (!weapon->isRangedWeapon() || targetObject->isInRange(creature, 5)) && range == -1)
			return NOPRONE;

		if (!targetObject->isInRange(creature, checkRange))
			return TOOFAR;

		if (!CollisionManager::checkLineOfSight(creature, targetObject)) {
			creature->sendSystemMessage("@container_error_message:container18");
			return GENERALERROR;
		}

		CombatManager* combatManager = CombatManager::instance();

		try {
			int res = combatManager->doCombatAction(creature, weapon, cast<TangibleObject*>(targetObject.get()), CreatureAttackData(arguments, this));

			switch (res) {
			case -1:
				return INVALIDTARGET;
			case -2:
				return INSUFFICIENTHAM;
			case -3:
				return GENERALERROR;
			}

		} catch (Exception& e) {
			error("unreported exception caught in CombatQueueCommand::doCombatAction");
			error(e.getMessage());
			e.printStackTrace();
		}

		// only clear aiming states if command was successful
		creature->removeStateBuff(CreatureState::AIMING);
		creature->removeBuff(String("steadyaim").hashCode());

		return SUCCESS;
	}

	float getCommandDuration(CreatureObject *object, const UnicodeString& arguments) {
		return CombatManager::instance()->calculateWeaponAttackSpeed(object, object->getWeapon(), speedMultiplier);
	}

	inline float getHealthCostMultiplier() const {
		return healthCostMultiplier;
	}

	inline float getActionCostMultiplier() const {
		return actionCostMultiplier;
	}

	inline float getMindCostMultiplier() const {
		return mindCostMultiplier;
	}

	inline int getRange() const {
		return range;
	}

	inline String getAccuracySkillMod() const {
		return accuracySkillMod;
	}

	inline float getDamageMultiplier() const {
		return damageMultiplier;
	}

	inline int getAccuracyBonus() const {
		return accuracyBonus;
	}

	inline float getSpeedMultiplier() const {
		return speedMultiplier;
	}

	inline float getSpeed() const {
		return speed;
	}

	inline bool isAreaAction() const {
		return areaAction;
	}

	inline bool isConeAction() const {
		return coneAction;
	}

	inline int getConeAngle() const {
		return coneAngle;
	}

	inline int getAreaRange() const {
		return areaRange;
	}

	inline float getForceCostMultiplier() const {
		return forceCostMultiplier;
	}

	inline float getForceCost() const {
		return forceCost;
	}

	void setDamageMultiplier(float damageMultiplier) {
		this->damageMultiplier = damageMultiplier;
	}

	void setAccuracyBonus(int accuracyBonus) {
		this->accuracyBonus = accuracyBonus;
	}

	void setHealthCostMultiplier(float f) {
		this->healthCostMultiplier = f;
	}

	void setActionCostMultiplier(float f) {
		this->actionCostMultiplier = f;
	}

	void setMindCostMultiplier(float f) {
		this->mindCostMultiplier = f;
	}

	void setForceCostMultiplier(float f) {
		this->forceCostMultiplier = f;
	}

	void setForceCost(float f) {
		this->forceCost = f;
	}

	void setConeAngle(int i) {
		this->coneAngle = i;
	}

	void setAreaAction(bool b) {
		this->areaAction = b;
	}

	void setConeAction(bool b) {
		this->coneAction = b;
	}

	void setAreaRange(int i) {
		this->areaRange = i;
	}

	void setEffectString(String s) {
		this->effectString = s;
	}

	void setSpeedMultiplier(float speedMultiplier) {
		this->speedMultiplier = speedMultiplier;
	}

	void setSpeed(float speedd) {
		this->speed = speedd;
	}

	inline uint32 getAnimationCRC() const {
		return animationCRC;
	}

	inline String getEffectString() const {
		return effectString;
	}

	inline String getCombatSpam() const {
		return combatSpam;
	}

	inline int getPoolsToDamage() const {
		return poolsToDamage;
	}

	inline VectorMap<uint64, StateEffect>* getStateEffects() {
		return &stateEffects;
	}

	inline VectorMap<uint64, DotEffect>* getDotEffects() {
		return &dotEffects;
	}

	void setAnimationCRC(uint32 animationCRC) {
		this->animationCRC = animationCRC;
	}

	void setCombatSpam(String combatSpam) {
		this->combatSpam = combatSpam;
	}

	void setPoolsToDamage(int poolsToDamage) {
		this->poolsToDamage = poolsToDamage;
	}

	void setStateEffects(VectorMap<uint64, StateEffect> stateEffects) {
		this->stateEffects = stateEffects;
	}

	void addStateEffect(StateEffect stateEffect) {
		stateEffects.put(stateEffect.getEffectType(), stateEffect);
	}

	StateEffect getStateEffect(uint64 type) {
		return stateEffects.get(type);
	}

	void setDotEffects(VectorMap<uint64, DotEffect> dotEffects) {
		this->dotEffects = dotEffects;
	}

	inline float getDamage() const {
		return damage;
	}

	void setDamage(float dm) {
		this->damage = dm;
	}

	void addDotEffect(DotEffect dotEffect) {
		dotEffects.put(dotEffect.getDotType(), dotEffect);
	}

	DotEffect getDotEffect(uint64 type) {
		return dotEffects.get(type);
	}

	void setRange(int i) {
		this->range = i;
	}

	void setAccuracySkillMod(String acc) {
		this->accuracySkillMod = acc;
	}

	bool hasCombatSpam() {
		return !combatSpam.isEmpty();
	}

	bool isCombatCommand() {
		return true;
	}

	virtual bool isSquadLeaderCommand() {
		return false;
	}

	// this goes in command in order to allow for overriding for special commands
	virtual void applyEffect(CreatureObject* creature, uint8 effectType, uint32 mod) {
		CombatManager* combatManager = CombatManager::instance();
		StateEffect effect = getStateEffect(effectType);
		Reference<Buff*> buff = NULL;

		switch (effectType) {
		case CommandEffect::BLIND:
			creature->setBlindedState(effect.getStateLength());
			break;
		case CommandEffect::DIZZY:
			creature->setDizziedState(effect.getStateLength());
			break;
		case CommandEffect::INTIMIDATE:
			creature->setIntimidatedState(mod, effect.getStateLength());
			break;
		case CommandEffect::STUN:
			creature->setStunnedState(effect.getStateLength());
			break;
		case CommandEffect::KNOCKDOWN:
			if (creature->isKnockedDown() || creature->isProne()) {
				if (80 > System::random(100))
					creature->setPosture(CreaturePosture::UPRIGHT, true);
				break;
			}

			if (creature->isRidingMount()) {
				creature->updateCooldownTimer("mount_dismount", 0);
				creature->dismount();
			}

			if (!creature->isDead() && !creature->isIncapacitated())
				creature->setPosture(CreaturePosture::KNOCKEDDOWN);

			creature->updateKnockdownRecovery();
			creature->updateLastKnockdown();
			creature->sendSystemMessage("@cbt_spam:posture_knocked_down");

			break;
		case CommandEffect::POSTUREUP:
			if (creature->isRidingMount()) {
				creature->updateCooldownTimer("mount_dismount", 0);
				creature->dismount();
			}

			if (creature->getPosture() == CreaturePosture::PRONE) {
				creature->setPosture(CreaturePosture::CROUCHED);
				creature->sendSystemMessage("@cbt_spam:force_posture_change_1");
				creature->updatePostureUpRecovery();
			} else if (creature->getPosture() == CreaturePosture::CROUCHED) {
				creature->setPosture(CreaturePosture::UPRIGHT);
				creature->sendSystemMessage("@cbt_spam:force_posture_change_0");
				creature->updatePostureUpRecovery();
			}

			break;
		case CommandEffect::POSTUREDOWN:
			if (creature->isRidingMount()) {
				creature->updateCooldownTimer("mount_dismount", 0);
				creature->dismount();
			}

			if (creature->getPosture() == CreaturePosture::UPRIGHT) {
				creature->setPosture(CreaturePosture::CROUCHED);
				creature->sendSystemMessage("@cbt_spam:force_posture_change_1");
				creature->updatePostureDownRecovery();
			} else if (creature->getPosture() == CreaturePosture::CROUCHED) {
				creature->setPosture(CreaturePosture::PRONE);
				creature->sendSystemMessage("@cbt_spam:force_posture_change_2");
				creature->updatePostureDownRecovery();
			}

			break;
		case CommandEffect::NEXTATTACKDELAY:
			creature->setNextAttackDelay(mod, effect.getStateLength());
			break;
		case CommandEffect::HEALTHDEGRADE:
			buff = new Buff(creature, String("healthdegrade").hashCode(), effect.getStateLength(), BuffType::STATE);
			buff->setAttributeModifier(CreatureAttribute::CONSTITUTION, -1*effect.getStateStrength());
			buff->setAttributeModifier(CreatureAttribute::STRENGTH, -1*effect.getStateStrength());
			creature->addBuff(buff);
			break;
		case CommandEffect::ACTIONDEGRADE:
			buff = new Buff(creature, String("actiondegrade").hashCode(), effect.getStateLength(), BuffType::STATE);
			buff->setAttributeModifier(CreatureAttribute::QUICKNESS, -1*effect.getStateStrength());
			buff->setAttributeModifier(CreatureAttribute::STAMINA, -1*effect.getStateStrength());
			creature->addBuff(buff);
			break;
		case CommandEffect::MINDDEGRADE:
			buff = new Buff(creature, String("minddegrade").hashCode(), effect.getStateLength(), BuffType::STATE);
			buff->setAttributeModifier(CreatureAttribute::FOCUS, -1*effect.getStateStrength());
			buff->setAttributeModifier(CreatureAttribute::WILLPOWER, -1*effect.getStateStrength());
			creature->addBuff(buff);
			break;
		default:
			break;
		}
		return;
	}

	uint8 getAttackType() const {
		return attackType;
	}

	void setAttackType(uint8 attackType) {
		this->attackType = attackType;
	}

	uint8 getTrails() const {
		return trails;
	}

	void setTrails(uint8 trails) {
		this->trails = trails;
	}

};

#endif /* COMBATQUEUECOMMAND_H_ */
