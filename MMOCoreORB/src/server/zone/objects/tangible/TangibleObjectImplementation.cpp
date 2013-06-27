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

#include "TangibleObject.h"
#include "variables/SkillModMap.h"
#include "server/zone/managers/object/ObjectManager.h"
#include "server/zone/managers/skill/SkillModManager.h"
#include "server/zone/objects/scene/variables/CustomizationVariables.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/packets/tangible/TangibleObjectMessage3.h"
#include "server/zone/packets/tangible/TangibleObjectMessage6.h"
#include "server/zone/packets/tangible/TangibleObjectMessage7.h"
#include "server/zone/packets/tangible/TangibleObjectDeltaMessage3.h"
#include "server/zone/packets/tangible/TangibleObjectDeltaMessage6.h"
#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/zone/templates/SharedTangibleObjectTemplate.h"
#include "server/zone/objects/creature/CreatureFlag.h"
#include "server/zone/packets/tangible/UpdatePVPStatusMessage.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/crafting/CraftingManager.h"
#include "server/zone/objects/tangible/component/Component.h"
#include "server/zone/objects/factorycrate/FactoryCrate.h"
#include "server/zone/objects/player/sessions/SlicingSession.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "server/zone/Zone.h"
#include "tasks/ClearDefenderListsTask.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ComponentSlot.h"
#include "server/zone/templates/tangible/tool/RepairToolTemplate.h"
#include "server/zone/objects/tangible/tool/repair/RepairTool.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/objects/tangible/wearables/WearableObject.h"
#include "engine/engine.h"


void TangibleObjectImplementation::initializeTransientMembers() {
	SceneObjectImplementation::initializeTransientMembers();

	threatMap = NULL;

	setLoggingName("TangibleObject");

	if (faction !=  0x16148850 && faction != 0xDB4ACC54) {
		faction = 0;
	}
}

void TangibleObjectImplementation::loadTemplateData(SharedObjectTemplate* templateData) {
	SceneObjectImplementation::loadTemplateData(templateData);

	SharedTangibleObjectTemplate* tanoData = dynamic_cast<SharedTangibleObjectTemplate*>(templateData);

	if (tanoData == NULL)
		return;

	targetable = tanoData->getTargetable();

	maxCondition = tanoData->getMaxCondition();

	useCount = tanoData->getUseCount();

	optionsBitmask = tanoData->getOptionsBitmask();
	pvpStatusBitmask = tanoData->getPvpStatusBitmask();

	sliceable = tanoData->getSliceable();

	faction = tanoData->getFaction();

	threatMap = NULL;
}

void TangibleObjectImplementation::sendBaselinesTo(SceneObject* player) {
	info("sending tano baselines");

	Reference<TangibleObject*> thisPointer = _this.get();

	BaseMessage* tano3 = new TangibleObjectMessage3(thisPointer);
	player->sendMessage(tano3);

	BaseMessage* tano6 = new TangibleObjectMessage6(thisPointer);
	player->sendMessage(tano6);

	if (player->isPlayerCreature())
		sendPvpStatusTo(cast<CreatureObject*>(player));
}

void TangibleObjectImplementation::sendPvpStatusTo(CreatureObject* player) {
	uint32 newPvpStatusBitmask = pvpStatusBitmask;

	if (!(newPvpStatusBitmask & CreatureFlag::ATTACKABLE)) {
		if (isAttackableBy(player))
			newPvpStatusBitmask |= CreatureFlag::ATTACKABLE;
	} else if (!isAttackableBy(player))
		newPvpStatusBitmask -= CreatureFlag::ATTACKABLE;

	if (!(newPvpStatusBitmask & CreatureFlag::AGGRESSIVE)) {
		if (isAggressiveTo(player))
			newPvpStatusBitmask |= CreatureFlag::AGGRESSIVE;
	} else if (!isAggressiveTo(player))
		newPvpStatusBitmask -= CreatureFlag::AGGRESSIVE;

	BaseMessage* pvp = new UpdatePVPStatusMessage(_this.get(), newPvpStatusBitmask);
	player->sendMessage(pvp);
}

void TangibleObjectImplementation::broadcastPvpStatusBitmask(){
	if (getZone() == NULL)
			return;

	if (closeobjects != NULL) {
		Zone* zone = getZone();

		//Locker locker(zone);

		CreatureObject* thisCreo = cast<CreatureObject*>(_this.get().get());

		SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects(closeobjects->size(), 10);

		closeobjects->safeCopyTo(closeObjects);

		for (int i = 0; i < closeObjects.size(); ++i) {
			SceneObject* obj = cast<SceneObject*>(closeObjects.get(i).get());

			if (obj != NULL && obj->isCreatureObject()) {
				CreatureObject* creo = cast<CreatureObject*>(obj);

				sendPvpStatusTo(creo);

				if (thisCreo != NULL)
					creo->sendPvpStatusTo(thisCreo);
			}

		}
	}
}

void TangibleObjectImplementation::setPvpStatusBitmask(int bitmask, bool notifyClient) {
	pvpStatusBitmask = bitmask;

	broadcastPvpStatusBitmask();

}

void TangibleObjectImplementation::synchronizedUIListen(SceneObject* player, int value) {
	// Send TANO7 Baseline
	TangibleObjectMessage7* tano7 = new TangibleObjectMessage7(_this.get());
	player->sendMessage(tano7);
}

void TangibleObjectImplementation::synchronizedUIStopListen(SceneObject* player, int value) {

}

void TangibleObjectImplementation::setSerialNumber(const String& serial) {
	uint32 bitmask = getOptionsBitmask();
	bitmask |= OptionBitmask::HASSERIAL;
	setOptionsBitmask(bitmask);
	objectSerial = serial;
}

void TangibleObjectImplementation::setDefender(SceneObject* defender) {
	if (defender == _this.get())
		return;

	if (defenderList.size() == 0) {
		addDefender(defender);
		return;
	}

	ManagedReference<SceneObject*> temp = NULL;

	int i = 0;
	for (; i < defenderList.size(); i++) {
		if (defenderList.get(i) == defender) {
			if (i == 0)
				return;

			temp = defenderList.get(0);

			TangibleObjectDeltaMessage6* dtano6 = new TangibleObjectDeltaMessage6(_this.get());
			dtano6->startUpdate(0x01);

			defenderList.set(0, defender, dtano6, 2);
			defenderList.set(i, temp, dtano6, 0);

			dtano6->close();

			broadcastMessage(dtano6, true);

			break;
		}
	}

	if (i == defenderList.size())
		addDefender(defender);
	else
		setCombatState();
}

void TangibleObjectImplementation::addDefender(SceneObject* defender) {
	if (defender == _this.get())
		return;

	for (int i = 0; i < defenderList.size(); ++i) {
		if (defender == defenderList.get(i))
			return;
	}

	//info("adding defender");

	TangibleObjectDeltaMessage6* dtano6 = new TangibleObjectDeltaMessage6(_this.get());
	dtano6->startUpdate(0x01);

	defenderList.add(defender, dtano6);

	dtano6->close();

	broadcastMessage(dtano6, true);

	setCombatState();

	notifyObservers(ObserverEventType::DEFENDERADDED, defender);
}

void TangibleObjectImplementation::removeDefenders() {
	//info("removing all defenders");
	if (defenderList.size() == 0) {
		//info("no defenders in list");
		return;
	}

	for (int i = 0; i < defenderList.size(); i++)
		notifyObservers(ObserverEventType::DEFENDERDROPPED, defenderList.get(i));

	TangibleObjectDeltaMessage6* dtano6 = new TangibleObjectDeltaMessage6(_this.get());
	dtano6->startUpdate(0x01);

	defenderList.removeAll(dtano6);

	dtano6->close();

	broadcastMessage(dtano6, true);

	//info("removed all defenders");
}

void TangibleObjectImplementation::removeDefender(SceneObject* defender) {
	//info("trying to remove defender");
	for (int i = 0; i < defenderList.size(); ++i) {
		if (defenderList.get(i) == defender) {
			info("removing defender");

			TangibleObjectDeltaMessage6* dtano6 = new TangibleObjectDeltaMessage6(_this.get());

			dtano6->startUpdate(0x01);

			if (defenderList.size() == 1)
				defenderList.removeAll(dtano6);
			else
				defenderList.remove(i, dtano6);

			dtano6->close();

			broadcastMessage(dtano6, true);

			notifyObservers(ObserverEventType::DEFENDERDROPPED, defender);

			//info("defender found and removed");
			break;
		}
	}

	if (defenderList.size() == 0)
		clearCombatState(false);

	//info("finished removing defender");
}

void TangibleObjectImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* object) {
	SceneObjectImplementation::fillAttributeList(alm, object);

	if (maxCondition > 0) {
		StringBuffer cond;
		cond << (maxCondition-(int)conditionDamage) << "/" << maxCondition;

		alm->insertAttribute("condition", cond);
	}

	alm->insertAttribute("volume", volume);

	if (!craftersName.isEmpty()) {

		alm->insertAttribute("crafter", craftersName);
	}
	if (!objectSerial.isEmpty()) {

		alm->insertAttribute("serial_number", objectSerial);
	}

	if(useCount > 1)
		alm->insertAttribute("quantity", useCount);
}

void TangibleObjectImplementation::setCustomizationVariable(byte type, int16 value, bool notifyClient) {
	customizationVariables.setVariable(type, value);

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateCustomizationString();
	dtano3->close();

	broadcastMessage(dtano3, true);
}

void TangibleObjectImplementation::setCustomizationVariable(const String& type, int16 value, bool notifyClient) {
	customizationVariables.setVariable(type, value);

	if(!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateCustomizationString();
	dtano3->close();

	broadcastMessage(dtano3, true);

}

void TangibleObjectImplementation::setCountdownTimer(unsigned int newUseCount, bool notifyClient) {
	if (useCount == newUseCount)
		return;

	useCount = newUseCount;

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateCountdownTimer();
	dtano3->close();

	broadcastMessage(dtano3, true);
}

void TangibleObjectImplementation::setUseCount(uint32 newUseCount, bool notifyClient) {
	if (useCount == newUseCount)
		return;

	setCountdownTimer(newUseCount, notifyClient);

	if (useCount < 1 && !isCreatureObject()) {
		destroyObjectFromWorld(true);

		destroyObjectFromDatabase(true);

		return;
	}
}

void TangibleObjectImplementation::decreaseUseCount() {
	setUseCount(useCount - 1, true);
}

void TangibleObjectImplementation::setMaxCondition(int maxCond, bool notifyClient) {
	if (maxCondition == maxCond)
		return;

	maxCondition = maxCond;

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateMaxCondition();
	dtano3->close();

	broadcastMessage(dtano3, true);
}

void TangibleObjectImplementation::setConditionDamage(float condDamage, bool notifyClient) {
	if (conditionDamage == condDamage)
		return;

	conditionDamage = condDamage;

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateConditionDamage();
	dtano3->close();

	broadcastMessage(dtano3, true);
}

int TangibleObjectImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, bool notifyClient) {
	float newConditionDamage = conditionDamage + damage;

	if (!destroy && newConditionDamage >= maxCondition)
		newConditionDamage = maxCondition - 1;
	else if (newConditionDamage >= maxCondition)
		newConditionDamage = maxCondition;

	setConditionDamage(newConditionDamage, notifyClient);

	if (attacker->isPlayerCreature()) {
		CreatureObject* player = cast<CreatureObject*>( attacker);

		if (damage > 0 && attacker != _this.get())
			getThreatMap()->addDamage(player, (uint32)damage);
	}

	if (newConditionDamage >= maxCondition)
		notifyObjectDestructionObservers(attacker, newConditionDamage);

	return 0;
}

int TangibleObjectImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, const String& xp, bool notifyClient) {
	float newConditionDamage = conditionDamage + damage;

	if (!destroy && newConditionDamage >= maxCondition)
		newConditionDamage = maxCondition - 1;

	setConditionDamage(newConditionDamage, notifyClient);

	if (attacker->isPlayerCreature()) {
		CreatureObject* player = cast<CreatureObject*>( attacker);

		if (damage > 0 && attacker != _this.get())
			getThreatMap()->addDamage(player, (uint32)damage, xp);
	}

	if (newConditionDamage >= maxCondition)
		notifyObjectDestructionObservers(attacker, newConditionDamage);

	return 0;
}

int TangibleObjectImplementation::notifyObjectDestructionObservers(TangibleObject* attacker, int condition) {
	notifyObservers(ObserverEventType::OBJECTDESTRUCTION, attacker, condition);

	if (threatMap != NULL)
		threatMap->removeAll();

	dropFromDefenderLists(attacker);

	return 1;
}

void TangibleObjectImplementation::dropFromDefenderLists(TangibleObject* destructor) {
	if (defenderList.size() == 0)
		return;

	Reference<ClearDefenderListsTask*> task = new ClearDefenderListsTask(defenderList, _this.get());
	Core::getTaskManager()->executeTask(task);

	clearCombatState(true);
}

int TangibleObjectImplementation::healDamage(TangibleObject* healer, int damageType, float damageToHeal, bool notifyClient) {
	float newConditionDamage = conditionDamage - damageToHeal;

	int returnValue = damageToHeal;

	if (newConditionDamage < 0)
		returnValue = damageToHeal + newConditionDamage;

	if (returnValue < 0)
		returnValue = 0;

	setConditionDamage(MAX(0, newConditionDamage), notifyClient);

	return returnValue;
}

void TangibleObjectImplementation::setCustomObjectName(const UnicodeString& name, bool notifyClient) {
	customName = name;

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateName(name);
	dtano3->close();

	broadcastMessage(dtano3, true);
}

void TangibleObjectImplementation::setOptionsBitmask(uint32 bitmask, bool notifyClient) {
	if (bitmask == optionsBitmask)
		return;

	optionsBitmask = bitmask;

	if (!notifyClient)
		return;

	TangibleObjectDeltaMessage3* dtano3 = new TangibleObjectDeltaMessage3(_this.get());
	dtano3->updateOptionsBitmask();
	dtano3->close();

	broadcastMessage(dtano3, true);
}

void TangibleObjectImplementation::updateCraftingValues(CraftingValues* values,
		bool firstUpdate) {
	/// I know its kind dirty, but we want generics to have quantities
	/// Without needing their own classes
	if (values->hasProperty("quantity")) {
		setUseCount(values->getCurrentValue("quantity"));
	}

	if (values->hasProperty("charges")) {
		setUseCount(values->getCurrentValue("charges"));
	}

	if (values->hasProperty("charge")) {
		setUseCount(values->getCurrentValue("charge"));
	}
}

void TangibleObjectImplementation::setInitialCraftingValues(ManufactureSchematic* manufactureSchematic, int assemblySuccess) {

	if(manufactureSchematic == NULL || manufactureSchematic->getDraftSchematic() == NULL)
		return;

	ManagedReference<DraftSchematic* > draftSchematic = manufactureSchematic->getDraftSchematic();
	CraftingValues* craftingValues = manufactureSchematic->getCraftingValues();
	ManagedReference<CraftingManager* > craftingManager = getZoneServer()->getCraftingManager();

	float value, maxPercentage, currentPercentage, weightedSum;
	String itemName;

	// These 2 values are pretty standard, adding these
	itemName = "xp";
	value = float(draftSchematic->getXpAmount());
	craftingValues->addExperimentalProperty("", itemName, value, value, 0, true, CraftingManager::OVERRIDECOMBINE);

	itemName = "complexity";
	value = manufactureSchematic->getComplexity();
	craftingValues->addExperimentalProperty("", itemName, value, value, 0, true, CraftingManager::OVERRIDECOMBINE);

	float modifier = craftingManager->calculateAssemblyValueModifier(assemblySuccess);
	int subtitleCounter = 0;

	for (int i = 0; i < draftSchematic->getResourceWeightCount(); ++i) {

		// Grab the first weight group
		Reference<ResourceWeight* > resourceWeight = draftSchematic->getResourceWeight(i);

		// Getting the title ex: expDamage
		String experimentalTitle = resourceWeight->getExperimentalTitle();

		// Getting the subtitle ex: minDamage
		String property = resourceWeight->getPropertyName();

		weightedSum = 0;

		craftingValues->addExperimentalProperty(experimentalTitle, property,
				resourceWeight->getMinValue(), resourceWeight->getMaxValue(),
				resourceWeight->getPrecision(), resourceWeight->isFiller(),
				resourceWeight->getCombineType());

		for (int ii = 0; ii < resourceWeight->getPropertyListSize(); ++ii) {

			// Based on the script we cycle through each exp group

			// Get the type from the type/weight
			int type = (resourceWeight->getTypeAndWeight(ii) >> 4);

			// Get the calculation percentage
			float percentage = resourceWeight->getPropertyPercentage(ii);

			// add to the weighted sum based on type and percentage
			weightedSum += craftingManager->getWeightedValue(manufactureSchematic, type) * percentage;
		}

		// > 0 ensures that we don't add things when there is NaN value
		if (weightedSum > 0) {

			// This is the formula for max experimenting percentages
			maxPercentage = ((weightedSum / 10.0f) * .01f);

			// Based on the weighted sum, we can get the initial %
			currentPercentage = (craftingManager->getAssemblyPercentage(weightedSum)) * modifier;

			craftingValues->setMaxPercentage(property, maxPercentage);
			craftingValues->setCurrentPercentage(property, currentPercentage);

			subtitleCounter++;

		}
	}

	craftingValues->recalculateValues(true);

	if (applyComponentStats(manufactureSchematic)) {
		craftingValues->recalculateValues(true);
	}
}

bool TangibleObjectImplementation::applyComponentStats(ManufactureSchematic* manufactureSchematic) {

	if(manufactureSchematic == NULL || manufactureSchematic->getDraftSchematic() == NULL)
		return false;

	float max, min, currentvalue, propertyvalue;
	int precision;
	bool modified = false;
	bool hidden;
	String experimentalTitle, property;

	CraftingValues* craftingValues = manufactureSchematic->getCraftingValues();
	ManagedReference<DraftSchematic* > draftSchematic = manufactureSchematic->getDraftSchematic();

	for (int i = 0; i < manufactureSchematic->getSlotCount(); ++i) {

		Reference<IngredientSlot* > ingredientSlot = manufactureSchematic->getSlot(i);
		Reference<DraftSlot* > draftSlot = draftSchematic->getDraftSlot(i);

		if(ingredientSlot == NULL || !ingredientSlot->isComponentSlot() || !ingredientSlot->isFull())
			continue;

		ComponentSlot* compSlot = cast<ComponentSlot*>(ingredientSlot.get());

		if(compSlot == NULL)
			continue;

		ManagedReference<TangibleObject*> tano = compSlot->getPrototype();

		if (tano == NULL || !tano->isComponent())
			continue;

		ManagedReference<Component*> component = cast<Component*>( tano.get());

		if (this->isWearableObject() && !this->isArmorObject()) {

			if (component->getObjectTemplate()->getObjectName() == "@craft_clothing_ingredients_n:reinforced_fiber_panels" || component->getObjectTemplate()->getObjectName() == "@craft_clothing_ingredients_n:synthetic_cloth"){

				ManagedReference<CraftingManager* > craftingManager = getZoneServer()->getCraftingManager();

				for (int k = 0; k < component->getPropertyCount(); ++k) {
					const String property = component->getProperty(k);

					if (property == "" || property == "null") {
						continue;
					}

					String key = craftingManager->checkBioSkillMods(property);

					if (key == "") {
						continue;
					} else {
						float value = component->getAttributeValue(property);

						WearableObjectImplementation* clothing = cast<WearableObjectImplementation*>(this);


						VectorMap<String, int>* clothingMods = clothing->getWearableSkillMods();

						int existingValue = 0;

						if(clothingMods->contains(key)) {
							existingValue = clothingMods->get(key);
						}

						value += existingValue;

						if (value > 25)
							value = 25;

						clothing->addSkillMod(SkillModManager::WEARABLE, key, value);
					}
				}
			}
		} else {

			for (int j = 0; j < component->getPropertyCount(); ++j) {

				property = component->getProperty(j); // charges

				modified = true;

				if (craftingValues->hasProperty(property)) {

					max = craftingValues->getMaxValue(property);

					min = craftingValues->getMinValue(property);

					hidden = craftingValues->isHidden(property);

					currentvalue = craftingValues->getCurrentValue(property);

					propertyvalue = component->getAttributeValue(property) * draftSlot->getContribution();

					short combineType = craftingValues->getCombineType(property);

					switch(combineType) {
					case CraftingManager::LINEARCOMBINE:
						currentvalue += propertyvalue;
						min += propertyvalue;
						max += propertyvalue;

						craftingValues->setMinValue(property, min);
						craftingValues->setMaxValue(property, max);

						craftingValues->setCurrentValue(property, currentvalue);
						break;
					case CraftingManager::PERCENTAGECOMBINE:
						currentvalue += propertyvalue;
						min += propertyvalue;
						max += propertyvalue;

						craftingValues->setMinValue(property, min);
						craftingValues->setMaxValue(property, max);

						craftingValues->setCurrentPercentage(property, currentvalue);
						break;
					case CraftingManager::BITSETCOMBINE:
						currentvalue = (int)currentvalue | (int)propertyvalue;

						craftingValues->setCurrentValue(property , currentvalue);
						break;
					case CraftingManager::OVERRIDECOMBINE:
						// Do nothing because the values should override whatever is
						// on the component
						break;
					default:
						break;
					}

				} else {

					currentvalue = component->getAttributeValue(property);
					precision = component->getAttributePrecision(property);
					experimentalTitle = component->getAttributeTitle(property);

					craftingValues->addExperimentalProperty(experimentalTitle, property,
						currentvalue, currentvalue, precision, component->getAttributeHidden(property), CraftingManager::LINEARCOMBINE);
					craftingValues->setCurrentPercentage(property, 0);
					craftingValues->setMaxPercentage(property, 0);
					craftingValues->setCurrentValue(property, currentvalue);
				}
			}
		}
	}
	//craftingValues->toString();

	return modified;
}

Reference<FactoryCrate*> TangibleObjectImplementation::createFactoryCrate(bool insertSelf) {

	String file;
	uint32 type = getGameObjectType();

	if(type & SceneObjectType::ARMOR)
		file = "object/factory/factory_crate_armor.iff";
	else if(type == SceneObjectType::CHEMICAL || type == SceneObjectType::PHARMACEUTICAL || type == SceneObjectType::PETMEDECINE)
		file = "object/factory/factory_crate_chemicals.iff";
	else if(type & SceneObjectType::CLOTHING)
		file = "object/factory/factory_crate_clothing.iff";
	else if(type == SceneObjectType::ELECTRONICS)
		file = "object/factory/factory_crate_electronics.iff";
	else if(type == SceneObjectType::FOOD || type == SceneObjectType::DRINK)
		file = "object/factory/factory_crate_food.iff";
	else if(type == SceneObjectType::FURNITURE)
		file = "object/factory/factory_crate_furniture.iff";
	else if(type & SceneObjectType::INSTALLATION)
		file = "object/factory/factory_crate_installation.iff";
	else if(type & SceneObjectType::WEAPON)
		file = "object/factory/factory_crate_weapon.iff";
	else
		file = "object/factory/factory_crate_generic_items.iff";

	ObjectManager* objectManager = ObjectManager::instance();

	Reference<FactoryCrate*> crate = (getZoneServer()->createObject(file.hashCode(), 2)).castTo<FactoryCrate*>();

	if (crate == NULL)
		return NULL;

	SharedTangibleObjectTemplate* tanoData = dynamic_cast<SharedTangibleObjectTemplate*>(templateObject.get());

	if (tanoData == NULL)
		return NULL;

	crate->setMaxCapacity(tanoData->getFactoryCrateSize());

	if (insertSelf) {
		crate->transferObject(_this.get(), -1, false);
	} else {

		ManagedReference<TangibleObject*> protoclone = cast<TangibleObject*>( objectManager->cloneObject(_this.get()));

		if (protoclone == NULL)
			return NULL;

		protoclone->setParent(NULL);
		crate->transferObject(protoclone, -1, false);
	}

	crate->setCustomObjectName(getCustomObjectName(), false);

	crate->setUseCount(1);

	return crate;
}

void TangibleObjectImplementation::addTemplateSkillMods(TangibleObject* targetObject) {
	SharedTangibleObjectTemplate* tano = dynamic_cast<SharedTangibleObjectTemplate*>(templateObject.get());

	if (tano == NULL)
		return;

	VectorMap<String, int>* mods = tano->getSkillMods();

	for (int i = 0; i < mods->size(); ++i) {
		VectorMapEntry<String, int> entry = mods->elementAt(i);

		targetObject->addSkillMod(SkillModManager::TEMPLATE, entry.getKey(), entry.getValue());
	}
}

void TangibleObjectImplementation::removeTemplateSkillMods(TangibleObject* targetObject) {
	SharedTangibleObjectTemplate* tano = dynamic_cast<SharedTangibleObjectTemplate*>(templateObject.get());

	if (tano == NULL)
		return;

	VectorMap<String, int>* mods = tano->getSkillMods();

	for (int i = 0; i < mods->size(); ++i) {
		VectorMapEntry<String, int> entry = mods->elementAt(i);

		targetObject->removeSkillMod(SkillModManager::TEMPLATE, entry.getKey(), entry.getValue());
	}
}

VectorMap<String, int>* TangibleObjectImplementation::getTemplateSkillMods() {
	SharedTangibleObjectTemplate* tano = dynamic_cast<SharedTangibleObjectTemplate*>(templateObject.get());

	if (tano == NULL)
		return NULL;

	return tano->getSkillMods();
}

bool TangibleObjectImplementation::canRepair(CreatureObject* player) {
	if (player == NULL || !isASubChildOf(player))
		return false;

	SceneObject* inventory = player->getSlottedObject("inventory");

	if (inventory == NULL)
		return false;

	for (int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
		ManagedReference<SceneObject*> item = inventory->getContainerObject(i);
		if(item->isRepairTool()) {
			Reference<RepairToolTemplate*> repairTemplate = cast<RepairToolTemplate*>(item->getObjectTemplate());

			if (repairTemplate == NULL) {
				error("No RepairToolTemplate for: " + String::valueOf(item->getServerObjectCRC()));

				continue;
			}

			if (repairTemplate->getRepairType() & getGameObjectType()) {
				return true;
			}
		}
	}

	return false;
}

void TangibleObjectImplementation::repair(CreatureObject* player) {

	if(player == NULL || player->getZoneServer() == NULL)
		return;

	if(!isASubChildOf(player))
		return;

	if (getConditionDamage() == 0) {
		player->sendSystemMessage("That item is not in need of repair.");
		return;
	}

	//Condition is unrepairable
	if ((getMaxCondition() - getConditionDamage()) <= 0) {
		StringIdChatParameter cantrepair("error_message", "sys_repair_unrepairable");
		cantrepair.setTT(getDisplayedName());
		player->sendSystemMessage(cantrepair); //%TT's condition is beyond repair even for your skills.
		return;
	}

	SceneObject* inventory = player->getSlottedObject("inventory");
	if(inventory == NULL)
		return;

	ManagedReference<RepairTool*> repairTool = NULL;
	Reference<RepairToolTemplate*> repairTemplate = NULL;

	for(int i = 0; i < inventory->getContainerObjectsSize(); ++i) {
		ManagedReference<SceneObject*> item = inventory->getContainerObject(i);
		if(item->isRepairTool()) {
			repairTemplate = cast<RepairToolTemplate*>(item->getObjectTemplate());

			if (repairTemplate == NULL) {
				error("No RepairToolTemplate for: " + String::valueOf(item->getServerObjectCRC()));
				return;
			}

			if(repairTemplate->getRepairType() & getGameObjectType()) {
				repairTool = cast<RepairTool*>(item.get());
				break;
			}
			repairTemplate = NULL;
		}
	}

	if(repairTool == NULL)
		return;

	/// Luck Roll + Profession Mod(25) + Luck Tapes
	/// + Station Mod - BF

	/// Luck Roll
	int roll = System::random(100);
	int repairChance = roll;

	/// Profession Bonus
	if(player->hasSkill(repairTemplate->getSkill()))
		repairChance += 35;

	/// Get Skill mods
	repairChance += player->getSkillMod(repairTemplate->getSkillMod());
	repairChance += player->getSkillMod("crafting_repair");
	repairChance += player->getSkillMod("force_repair_bonus");

	/// use tool quality to lower chances if bad tool
	float quality = 1.f - (((100.f - repairTool->getQuality()) / 2) / 100.f);
	repairChance *= quality;

	ManagedReference<PlayerManager*> playerMan = player->getZoneServer()->getPlayerManager();

	/// Increase if near station
	if(playerMan->getNearbyCraftingStation(player, repairTemplate->getStationType()) != NULL) {
		repairChance += 15;
	}

	/// Subtract battle fatigue
	repairChance -= (player->getShockWounds() / 2);

	/// Subtract complexity
	repairChance -= (getComplexity() / 3);

	/// 5% random failure
	if(getMaxCondition() < 20 || roll < 5)
		repairChance = 0;

	if(roll > 95)
		repairChance = 100;

	String result = repairAttempt(repairChance);
	repairTool->destroyObjectFromWorld(true);

	player->sendSystemMessage(result);
}

ThreatMap* TangibleObjectImplementation::getThreatMap() {
	if (threatMap == NULL) {
		Reference<ThreatMap*> newMap = new ThreatMap(_this.get());

		threatMap.compareAndSet(NULL, newMap.get());
	}

	return threatMap;
}

