/*
 * SharedWeaponObjectTemplate.h
 *
 *  Created on: 05/05/2010
 *      Author: victor
 */

#ifndef SHAREDWEAPONOBJECTTEMPLATE_H_
#define SHAREDWEAPONOBJECTTEMPLATE_H_

#include "server/zone/templates/SharedTangibleObjectTemplate.h"

class SharedWeaponObjectTemplate: public SharedTangibleObjectTemplate {
protected:
	StringParam weaponEffect;
	IntegerParam weaponEffectIndex;
	IntegerParam attackType;

	int damageType;

	int armorPiercing;

	int healthAttackCost;
	int actionAttackCost;
	int mindAttackCost;
	int forceCost;

	int pointBlankAccuracy;
	int pointBlankRange;

	int idealRange;
	int idealAccuracy;

	int maxRange;
	int maxRangeAccuracy;

	int minDamage;
	int maxDamage;

	float woundsRatio;

	int area;

	float attackSpeed;

	String combatSpam;
	String animationType;

	Vector<String> damageModifiers;
	Vector<String> speedModifiers;
	Vector<String> creatureAccuracyModifiers;
	Vector<String> creatureAimModifiers;
	Vector<String> defenderDefenseModifiers;
	Vector<String> defenderSecondaryDefenseModifiers;
	Vector<String> defenderToughnessModifiers;

	String xpType;

	uint32 weaponType;

public:
	SharedWeaponObjectTemplate() {
		damageType = 0;

		armorPiercing = 0;

		healthAttackCost = 0;
		actionAttackCost = 0;
		mindAttackCost = 0;
		forceCost = 0;

		pointBlankAccuracy = 0;
		pointBlankRange = 0;

		idealRange = 0;
		idealAccuracy = 0;

		maxRange = 0;
		maxRangeAccuracy = 0;

		minDamage = 0;
		maxDamage = 0;

		woundsRatio = 0;

		area = 0;

		attackSpeed = 0;

		weaponType = 0xFFFFFFFF;
	}

	~SharedWeaponObjectTemplate() {

	}

	void readObject(LuaObject* templateData);
	void readObject(IffStream* iffStream);

	void parseVariableData(const String& varName, Chunk* data);
	void parseVariableData(const String& varName, LuaObject* templateData);

	void parseFileData(IffStream* iffStream);

	inline String getXpType() const {
		return xpType;
	}

	inline int getActionAttackCost() const {
		return actionAttackCost;
	}

	inline int getArea() const {
		return area;
	}

	inline int getArmorPiercing() const {
		return armorPiercing;
	}

	float getAttackSpeed() const {
		return attackSpeed;
	}

	inline int getForceCost() const {
		return forceCost;
	}

	inline int getHealthAttackCost() const {
		return healthAttackCost;
	}

	inline int getIdealAccuracy() const {
		return idealAccuracy;
	}

	inline int getIdealRange() const {
		return idealRange;
	}

	inline int getMaxDamage() const {
		return maxDamage;
	}

	inline int getMaxRange() const {
		return maxRange;
	}

	inline int getMaxRangeAccuracy() const {
		return maxRangeAccuracy;
	}

	inline int getMinDamage() const {
		return minDamage;
	}

	inline int getMindAttackCost() const {
		return mindAttackCost;
	}

	inline int getPointBlankAccuracy() const {
		return pointBlankAccuracy;
	}

	inline int getPointBlankRange() const {
		return pointBlankRange;
	}

	float getWoundsRatio() const {
		return woundsRatio;
	}

	void setActionAttackCost(int actionAttackCost) {
		this->actionAttackCost = actionAttackCost;
	}

	void setArea(int area) {
		this->area = area;
	}

	void setArmorPiercing(int armorPiercing) {
		this->armorPiercing = armorPiercing;
	}

	void setAttackSpeed(float attackSpeed) {
		this->attackSpeed = attackSpeed;
	}

	void setForceCost(int forceCost) {
		this->forceCost = forceCost;
	}

	void setHealthAttackCost(int healthAttackCost) {
		this->healthAttackCost = healthAttackCost;
	}

	void setIdealAccuracy(int idealAccuracy) {
		this->idealAccuracy = idealAccuracy;
	}

	void setIdealRange(int idealRange) {
		this->idealRange = idealRange;
	}

	void setMaxDamage(int maxDamage) {
		this->maxDamage = maxDamage;
	}

	void setMaxRange(int maxRange) {
		this->maxRange = maxRange;
	}

	void setMaxRangeAccuracy(int maxRangeAccuracy) {
		this->maxRangeAccuracy = maxRangeAccuracy;
	}

	void setMinDamage(int minDamage) {
		this->minDamage = minDamage;
	}

	void setMindAttackCost(int mindAttackCost) {
		this->mindAttackCost = mindAttackCost;
	}

	void setPointBlankAccuracy(int pointBlankAccuracy) {
		this->pointBlankAccuracy = pointBlankAccuracy;
	}

	void setPointBlankRange(int pointBlankRange) {
		this->pointBlankRange = pointBlankRange;
	}

	void setWoundsRatio(float woundsRatio) {
		this->woundsRatio = woundsRatio;
	}

	Vector<String>* getCertificationsRequired() {
		return &certificationsRequired;
	}

	Vector<String>* getCreatureAccuracyModifiers() {
		return &creatureAccuracyModifiers;
	}

	Vector<String>* getCreatureAimModifiers() {
		return &creatureAimModifiers;
	}

	Vector<String>* getDamageModifiers() {
		return &damageModifiers;
	}

	inline int getDamageType() const {
		return damageType;
	}

	Vector<String>* getDefenderDefenseModifiers() {
		return &defenderDefenseModifiers;
	}

	Vector<String>* getDefenderSecondaryDefenseModifiers() {
		return &defenderSecondaryDefenseModifiers;
	}

	Vector<String>* getDefenderToughnessModifiers() {
		return &defenderToughnessModifiers;
	}

	Vector<String>* getSpeedModifiers() {
		return &speedModifiers;
	}

	void setCertificationsRequired(Vector<String> certificationsRequired) {
		this->certificationsRequired = certificationsRequired;
	}

	void setCreatureAccuracyModifiers(Vector<String> creatureAccuracyModifiers) {
		this->creatureAccuracyModifiers = creatureAccuracyModifiers;
	}

	void setCreatureAimModifiers(Vector<String> creatureAimModifiers) {
		this->creatureAimModifiers = creatureAimModifiers;
	}

	void setDamageModifiers(Vector<String> damageModifiers) {
		this->damageModifiers = damageModifiers;
	}

	void setDamageType(int damageType) {
		this->damageType = damageType;
	}

	void setDefenderDefenseModifiers(Vector<String> defenderDefenseModifiers) {
		this->defenderDefenseModifiers = defenderDefenseModifiers;
	}

	void setDefenderSecondaryDefenseModifiers(Vector<String> defenderSecondaryDefenseModifiers) {
		this->defenderSecondaryDefenseModifiers	= defenderSecondaryDefenseModifiers;
	}

	void setDefenderToughnessModifiers(Vector<String> defenderToughnessModifiers) {
		this->defenderToughnessModifiers = defenderToughnessModifiers;
	}

	void setSpeedModifiers(Vector<String> speedModifiers) {
		this->speedModifiers = speedModifiers;
	}

	inline int getAttackType() const {
		return attackType;
	}

	String getWeaponEffect() const {
		return weaponEffect;
	}

	inline int getWeaponEffectIndex() const {
		return weaponEffectIndex;
	}

	void setAttackType(int attackType) {
		this->attackType = attackType;
	}

	void setWeaponEffect(String weaponEffect) {
		this->weaponEffect = weaponEffect;
	}

	void setWeaponEffectIndex(int weaponEffectIndex) {
		this->weaponEffectIndex = weaponEffectIndex;
	}

	String getCombatSpam() {
		return combatSpam;
	}

	String getAnimationType() {
		return animationType;
	}

	uint32 getWeaponType() const {
		return weaponType;
	}

	void setWeaponType(uint32 weaponType) {
		this->weaponType = weaponType;
	}
};

#endif /* SHAREDWEAPONOBJECTTEMPLATE_H_ */
