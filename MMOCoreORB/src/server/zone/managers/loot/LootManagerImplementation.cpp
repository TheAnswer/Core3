/*
 * LootManagerImplementation.cpp
 *
 *  Created on: Jun 20, 2011
 *      Author: Kyle
 */

#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/managers/crafting/CraftingManager.h"
#include "templates/LootItemTemplate.h"
#include "templates/LootGroupTemplate.h"
#include "server/zone/ZoneServer.h"
#include "LootGroupMap.h"
#include "server/zone/objects/tangible/component/lightsaber/LightsaberCrystalComponent.h"
#include "server/zone/managers/loot/LootValues.h"
#include "server/zone/managers/resource/ResourceManager.h"
#include "server/zone/Zone.h"

// #define DEBUG_LOOT_MAN

void LootManagerImplementation::initialize() {
	info("Loading configuration.");

	if (!loadConfigData()) {

		loadDefaultConfig();

		info("Failed to load configuration values. Using default.");
	}

	lootGroupMap = LootGroupMap::instance();
	lootGroupMap->initialize();

	info("Loaded " + String::valueOf(lootableArmorAttachmentMods.size()) + " lootable armor attachment stat mods.");
	info("Loaded " + String::valueOf(lootableClothingAttachmentMods.size()) + " lootable clothing attachment stat mods.");
	info("Loaded " + String::valueOf(lootableArmorMods.size()) + " lootable armor stat mods.");
	info("Loaded " + String::valueOf(lootableClothingMods.size()) + " lootable clothing stat mods.");
	info("Loaded " + String::valueOf(lootableOneHandedMeleeMods.size()) + " lootable one handed melee stat mods.");
	info("Loaded " + String::valueOf(lootableTwoHandedMeleeMods.size()) + " lootable two handed melee stat mods.");
	info("Loaded " + String::valueOf(lootableUnarmedMods.size()) + " lootable unarmed stat mods.");
	info("Loaded " + String::valueOf(lootablePistolMods.size()) + " lootable pistol stat mods.");
	info("Loaded " + String::valueOf(lootableRifleMods.size()) + " lootable rifle stat mods.");
	info("Loaded " + String::valueOf(lootableCarbineMods.size()) + " lootable carbine stat mods.");
	info("Loaded " + String::valueOf(lootablePolearmMods.size()) + " lootable polearm stat mods.");
	info("Loaded " + String::valueOf(lootableHeavyWeaponMods.size()) + " lootable heavy weapon stat mods.");
	info("Loaded " + String::valueOf(lootGroupMap->countLootItemTemplates()) + " loot items.");
	info("Loaded " + String::valueOf(lootGroupMap->countLootGroupTemplates()) + " loot groups.");

	info("Initialized.", true);
}

void LootManagerImplementation::stop() {
	lootGroupMap = nullptr;
	craftingManager = nullptr;
	objectManager = nullptr;
	zoneServer = nullptr;
}

bool LootManagerImplementation::loadConfigData() {
	Lua* lua = new Lua();
	lua->init();

	if (!lua->runFile("scripts/managers/loot_manager.lua")) {
		delete lua;
		return false;
	}

	levelChance = lua->getGlobalFloat("levelChance");
	baseChance = lua->getGlobalFloat("baseChance");
	baseModifier = lua->getGlobalFloat("baseModifier");
	yellowChance = lua->getGlobalFloat("yellowChance");
	yellowModifier = lua->getGlobalFloat("yellowModifier");
	exceptionalChance = lua->getGlobalFloat("exceptionalChance");
	exceptionalModifier = lua->getGlobalFloat("exceptionalModifier");
	legendaryChance = lua->getGlobalFloat("legendaryChance");
	legendaryModifier = lua->getGlobalFloat("legendaryModifier");
	skillModChance = lua->getGlobalFloat("skillModChance");
	junkValueModifier = lua->getGlobalFloat("junkValueModifier");

	LuaObject dotAttributeTable = lua->getGlobalObject("randomDotAttribute");

	if (dotAttributeTable.isValidTable() && dotAttributeTable.getTableSize() > 0) {
		for (int i = 1; i <= dotAttributeTable.getTableSize(); ++i) {
			int value = dotAttributeTable.getIntAt(i);
			randomDotAttribute.put(value);
		}
		dotAttributeTable.pop();
	}

	LuaObject dotStrengthTable = lua->getGlobalObject("randomDotStrength");

	if (dotStrengthTable.isValidTable() && dotStrengthTable.getTableSize() > 0) {
		for (int i = 1; i <= dotStrengthTable.getTableSize(); ++i) {
			int value = dotStrengthTable.getIntAt(i);
			randomDotStrength.put(value);
		}
		dotStrengthTable.pop();
	}

	LuaObject dotDurationTable = lua->getGlobalObject("randomDotDuration");

	if (dotDurationTable.isValidTable() && dotDurationTable.getTableSize() > 0) {
		for (int i = 1; i <= dotDurationTable.getTableSize(); ++i) {
			int value = dotDurationTable.getIntAt(i);
			randomDotDuration.put(value);
		}
		dotDurationTable.pop();
	}

	LuaObject dotPotencyTable = lua->getGlobalObject("randomDotPotency");

	if (dotPotencyTable.isValidTable() && dotPotencyTable.getTableSize() > 0) {
		for (int i = 1; i <= dotPotencyTable.getTableSize(); ++i) {
			int value = dotPotencyTable.getIntAt(i);
			randomDotPotency.put(value);
		}
		dotPotencyTable.pop();
	}

	LuaObject dotUsesTable = lua->getGlobalObject("randomDotUses");

	if (dotUsesTable.isValidTable() && dotUsesTable.getTableSize() > 0) {
		for (int i = 1; i <= dotUsesTable.getTableSize(); ++i) {
			int value = dotUsesTable.getIntAt(i);
			randomDotUses.put(value);
		}
		dotUsesTable.pop();
	}

	LuaObject modsTable = lua->getGlobalObject("lootableArmorAttachmentStatMods");
	loadLootableMods( &modsTable, &lootableArmorAttachmentMods );

	modsTable = lua->getGlobalObject("lootableClothingAttachmentStatMods");
	loadLootableMods( &modsTable, &lootableClothingAttachmentMods );

	modsTable = lua->getGlobalObject("lootableArmorStatMods");
	loadLootableMods( &modsTable, &lootableArmorMods );

	modsTable = lua->getGlobalObject("lootableClothingStatMods");
	loadLootableMods( &modsTable, &lootableClothingMods );

	modsTable = lua->getGlobalObject("lootableOneHandedMeleeStatMods");
	loadLootableMods( &modsTable, &lootableOneHandedMeleeMods );

	modsTable = lua->getGlobalObject("lootableTwoHandedMeleeStatMods");
	loadLootableMods( &modsTable, &lootableTwoHandedMeleeMods );

	modsTable = lua->getGlobalObject("lootableUnarmedStatMods");
	loadLootableMods( &modsTable, &lootableUnarmedMods );

	modsTable = lua->getGlobalObject("lootablePistolStatMods");
	loadLootableMods( &modsTable, &lootablePistolMods );

	modsTable = lua->getGlobalObject("lootableRifleStatMods");
	loadLootableMods( &modsTable, &lootableRifleMods );

	modsTable = lua->getGlobalObject("lootableCarbineStatMods");
	loadLootableMods( &modsTable, &lootableCarbineMods );

	modsTable = lua->getGlobalObject("lootablePolearmStatMods");
	loadLootableMods( &modsTable, &lootablePolearmMods );

	modsTable = lua->getGlobalObject("lootableHeavyWeaponStatMods");
	loadLootableMods( &modsTable, &lootableHeavyWeaponMods );

	LuaObject luaObject = lua->getGlobalObject("jediCrystalStats");
	LuaObject crystalTable = luaObject.getObjectField("lightsaber_module_force_crystal");
	CrystalData* crystal = new CrystalData();
	crystal->readObject(&crystalTable);
	crystalData.put("lightsaber_module_force_crystal", crystal);
	crystalTable.pop();

	crystalTable = luaObject.getObjectField("lightsaber_module_krayt_dragon_pearl");
	crystal = new CrystalData();
	crystal->readObject(&crystalTable);
	crystalData.put("lightsaber_module_krayt_dragon_pearl", crystal);
	crystalTable.pop();
	luaObject.pop();

	delete lua;

	return true;
}

void LootManagerImplementation::loadLootableMods(LuaObject* modsTable, SortedVector<String>* mods) {

	if (!modsTable->isValidTable())
		return;

	for (int i = 1; i <= modsTable->getTableSize(); ++i) {
		String mod = modsTable->getStringAt(i);
		mods->put(mod);
	}

	modsTable->pop();

}

void LootManagerImplementation::loadDefaultConfig() {

}

void LootManagerImplementation::setCustomizationData(const LootItemTemplate* templateObject, TangibleObject* prototype) {
#ifdef DEBUG_LOOT_MAN
	info(true) << " ========== LootManagerImplementation::setCustomizationData -- called ==========";
#endif

	const Vector<String>* customizationData = templateObject->getCustomizationStringNames();
	const Vector<Vector<int> >* customizationValues = templateObject->getCustomizationValues();

	for (int i = 0; i < customizationData->size(); ++i) {
		const String& customizationString = customizationData->get(i);
		Vector<int>* values = &customizationValues->get(i);

		if (values->size() > 0) {
			int randomValue = values->get(System::random(values->size() - 1));

#ifdef DEBUG_LOOT_MAN
		info(true) << "Setting Customization String: " << customizationString << " To a value: " << randomValue;
#endif

			prototype->setCustomizationVariable(customizationString, randomValue, false);
		}
	}

#ifdef DEBUG_LOOT_MAN
	info(true) << " ========== LootManagerImplementation::setCustomizationData -- COMPLETE ==========";
#endif
}

void LootManagerImplementation::setCustomObjectName(TangibleObject* object, const LootItemTemplate* templateObject) {
	const String& customName = templateObject->getCustomObjectName();

	if (!customName.isEmpty()) {
		if (customName.charAt(0) == '@') {
			StringId stringId(customName);

			object->setObjectName(stringId, false);
		} else {
			object->setCustomObjectName(customName, false);
		}
	}
}

int LootManagerImplementation::calculateLootCredits(int level) {
	int maxcredits = (int) round((.03f * level * level) + (3 * level) + 50);
	int mincredits = (int) round((((float) maxcredits) * .5f) + (2.0f * level));

	int credits = mincredits + System::random(maxcredits - mincredits);

	return credits;
}

void LootManagerImplementation::setRandomLootValues(TransactionLog& trx, TangibleObject* prototype, const LootItemTemplate* itemTemplate, int level, float excMod) {
	auto debugAttributes = ConfigManager::instance()->getLootDebugAttributes();

	float modifier = LootValues::STATIC;
	float chance = LootValues::getLevelRankValue(level) * levelChance;

	if (excMod >= legendaryModifier) {
		modifier = LootValues::LEGENDARY;
	} else if (excMod >= exceptionalModifier) {
		modifier = LootValues::EXCEPTIONAL;
	} else if (System::random(yellowChance) <= chance) {
		modifier = LootValues::ENHANCED;
	} else if (System::random(baseChance) <= chance) {
		modifier = LootValues::EXPERIMENTAL;
	}

	auto lootValues = LootValues(itemTemplate, level, modifier);
	prototype->updateCraftingValues(&lootValues, true);

	if (lootValues.getDynamicValues() > 0) {
		prototype->addMagicBit(false);
	}

#ifdef LOOTVALUES_DEBUG
	lootValues.debugAttributes(prototype, itemTemplate);
#endif // LOOTVALUES_DEBUG

	if (debugAttributes) {
		JSONSerializationType attrDebug;

		for (int i = 0; i < lootValues.getTotalExperimentalAttributes(); ++i) {
			const String& attribute = lootValues.getAttribute(i);

			JSONSerializationType attrDebugEntry;
			attrDebugEntry["mod"] = lootValues.getMaxPercentage(attribute);
			attrDebugEntry["pct"] = lootValues.getCurrentPercentage(attribute);
			attrDebugEntry["min"] = lootValues.getMinValue(attribute);
			attrDebugEntry["max"] = lootValues.getMaxValue(attribute);
			attrDebugEntry["final"] = lootValues.getCurrentValue(attribute);
			attrDebug[attribute] = attrDebugEntry;
		}

		if (!attrDebug.empty()) {
			trx.addState("lootAttributeDebug", attrDebug);
		}
	}
}

TangibleObject* LootManagerImplementation::createLootObject(TransactionLog& trx, const LootItemTemplate* templateObject, int level, bool maxCondition) {
	int uncappedLevel = level;

#ifdef DEBUG_LOOT_MAN
	info(true) << " ---------- LootManagerImplementation::createLootObject -- called ----------";
#endif

	trx.addState("lootVersion", 2);

	level = (level < LootManager::LEVELMIN) ? LootManager::LEVELMIN : level;
	level = (level > LootManager::LEVELMAX) ? LootManager::LEVELMAX : level;

	const String& directTemplateObject = templateObject->getDirectObjectTemplate();

	trx.addState("lootTemplate", directTemplateObject);
	trx.addState("lootLevel", level);
	trx.addState("lootMaxCondition", maxCondition);

#ifdef DEBUG_LOOT_MAN
	info(true) << "Item Template: " << directTemplateObject << "    Level = " << level << " Uncapped Level = " << uncappedLevel;
#endif

	if (templateObject->isRandomResourceContainer()) {
		return createLootResource(templateObject->getTemplateName(), "tatooine");
	}

	ManagedReference<TangibleObject*> prototype = zoneServer->createObject(directTemplateObject.hashCode(), 2).castTo<TangibleObject*>();

	if (prototype == nullptr) {
		error("could not create loot object: " + directTemplateObject);
		return nullptr;
	}

	Locker objLocker(prototype);

	prototype->createChildObjects();

	//Disable serial number generation on looted items that require no s/n
	if (!templateObject->getSuppressSerialNumber()) {
		String serial = craftingManager->generateSerial();
		prototype->setSerialNumber(serial);
	}

	prototype->setJunkDealerNeeded(templateObject->getJunkDealerTypeNeeded());
	float junkMinValue = templateObject->getJunkMinValue() * junkValueModifier;
	float junkMaxValue = templateObject->getJunkMaxValue() * junkValueModifier;
	float fJunkValue = junkMinValue+System::random(junkMaxValue-junkMinValue);

	if (templateObject->getJunkDealerTypeNeeded() > 1){
		fJunkValue = fJunkValue + (fJunkValue * ((float)level / 100)); // This is the loot value calculation if the item has a level
	}

	setCustomizationData(templateObject, prototype);
	setCustomObjectName(prototype, templateObject);

	float excMod = 1.0;

	float adjustment = floor((float)(((level > 50) ? level : 50) - 50) / 10.f + 0.5);

	trx.addState("lootAdjustment", adjustment);

	if (System::random(legendaryChance) >= legendaryChance - adjustment) {
		UnicodeString newName = prototype->getDisplayedName() + " (Legendary)";
		prototype->setCustomObjectName(newName, false);

		excMod = legendaryModifier;

		prototype->addMagicBit(false);

		legendaryLooted.increment();
		trx.addState("lootIsLegendary", true);
	} else if (System::random(exceptionalChance) >= exceptionalChance - adjustment) {
		UnicodeString newName = prototype->getDisplayedName() + " (Exceptional)";
		prototype->setCustomObjectName(newName, false);

		excMod = exceptionalModifier;

		prototype->addMagicBit(false);

		exceptionalLooted.increment();
		trx.addState("lootIsExceptional", true);
	}

	trx.addState("lootExcMod", excMod);

#ifdef DEBUG_LOOT_MAN
		info(true) << "Exceptional Modifier (excMod) = " << excMod << "  Adjustment = " << adjustment;
#endif

	if (prototype->isLightsaberCrystalObject()) {
		LightsaberCrystalComponent* crystal = cast<LightsaberCrystalComponent*> (prototype.get());

		if (crystal != nullptr)
			crystal->setItemLevel(uncappedLevel);
	}

	setRandomLootValues(trx, prototype, templateObject, uncappedLevel, excMod);

	if (excMod == 1.f && (prototype->getOptionsBitmask() & OptionBitmask::YELLOW)) {
		yellowLooted.increment();
		trx.addState("lootIsYellow", true);

		prototype->setJunkValue((int)(fJunkValue * 1.25));
	} else {
		if (excMod == 1.0) {
			prototype->setJunkValue((int)(fJunkValue));
		} else {
			prototype->setJunkValue((int)(fJunkValue * (excMod/2)));
		}
	}

	trx.addState("lootJunkValue", prototype->getJunkValue());

	// Add Dots to weapon objects.
	addStaticDots(prototype, templateObject, level);
	addRandomDots(prototype, templateObject, level, excMod);

	setSkillMods(prototype, templateObject, level, excMod);

	//add some condition damage where appropriate
	if (!maxCondition)
		addConditionDamage(prototype);

	trx.addState("lootConditionDmg", prototype->getConditionDamage());
	trx.addState("lootConditionMax", prototype->getMaxCondition());

#ifdef DEBUG_LOOT_MAN
	info(true) << " ---------- LootManagerImplementation::createLootObject -- COMPLETE ----------";
#endif

	return prototype;
}

TangibleObject* LootManagerImplementation::createLootResource(const String& resourceDataName, const String& resourceZoneName) {
	auto lootItemTemplate = lootGroupMap->getLootItemTemplate(resourceDataName);

	if (lootItemTemplate == nullptr || lootItemTemplate->getObjectType() != SceneObjectType::RESOURCECONTAINER) {
		return nullptr;
	}

	auto resourceManager = zoneServer->getResourceManager();

	if (resourceManager == nullptr) {
		return nullptr;
	}

	auto resourceSpawner = resourceManager->getResourceSpawner();

	if (resourceSpawner == nullptr) {
		return nullptr;
	}

	auto resourceMap = resourceSpawner->getResourceMap();

	if (resourceMap == nullptr) {
		return nullptr;
	}

	auto resourceList = resourceMap->getZoneResourceList(resourceZoneName);

	if (resourceList == nullptr) {
		return nullptr;
	}

	String resourceTypeName = resourceDataName.replaceAll("resource_container_", "");

	if (resourceTypeName == "") {
		return nullptr;
	}

	Vector<ManagedReference<ResourceSpawn*>> resourceIndex;

	for (int i = 0; i < resourceList->size(); ++i) {
		ManagedReference<ResourceSpawn*> resourceEntry = resourceList->elementAt(i).getValue();

		if (resourceEntry != nullptr && resourceEntry->isType(resourceTypeName)) {
			resourceIndex.add(resourceEntry);
		}
	}

	if (resourceIndex.size() > 0) {
		ManagedReference<ResourceSpawn*> resourceEntry = resourceIndex.get(System::random(resourceIndex.size()-1));

		if (resourceEntry != nullptr) {
			Locker rLock(resourceEntry);

			const auto& valueMap = lootItemTemplate->getAttributesMapCopy();

			if (!valueMap.hasExperimentalAttribute("quantity")) {
				return nullptr;
			}

			float min = Math::max(1.f, valueMap.getMinValue("quantity"));
			float max = Math::max(min, valueMap.getMaxValue("quantity"));

			ManagedReference<ResourceContainer*> resourceContainer = resourceEntry->createResource(System::random(max - min) + min);

			return resourceContainer;
		}
	}

	return nullptr;
}

void LootManagerImplementation::addConditionDamage(TangibleObject* prototype) {
	if (!prototype->isWeaponObject() && !prototype->isArmorObject()) {
		return;
	}

	int conditionMax = prototype->getMaxCondition();

	if (conditionMax < 0) {
		prototype->setMaxCondition(0, false);
		return;
	}

	int conditionDmg = std::round(conditionMax / 3.f);

	if (conditionDmg > 1) {
		prototype->setConditionDamage(System::random(conditionDmg), false);
	}
}

void LootManagerImplementation::setSkillMods(TangibleObject* object, const LootItemTemplate* templateObject, int level, float excMod) {
	if (!object->isWeaponObject() && !object->isWearableObject())
		return;

	const VectorMap<String, int>* skillMods = templateObject->getSkillMods();
	VectorMap<String, int> additionalMods;

	bool yellow = false;
	float modSqr = excMod * excMod;

	if (System::random(skillModChance / modSqr) == 0) {
		// if it has a skillmod the name will be yellow
		yellow = true;
		int modCount = 1;
		int roll = System::random(100);

		if(roll > (100 - modSqr))
			modCount += 2;

		if(roll < (5 + modSqr))
			modCount += 1;

		for(int i = 0; i < modCount; ++i) {
			//Mods can't be lower than -1 or greater than 25
			int max = (int) Math::max(-1.f, Math::min(25.f, (float) round(0.1f * level + 3)));
			int min = (int) Math::max(-1.f, Math::min(25.f, (float) round(0.075f * level - 1)));

			int mod = System::random(max - min) + min;

			if(mod == 0)
				mod = 1;

			String modName = getRandomLootableMod( object->getGameObjectType() );
			if( !modName.isEmpty() )
				additionalMods.put(modName, mod);
		}
	}

	if (object->isWearableObject() || object->isWeaponObject()) {
		ManagedReference<TangibleObject*> item = cast<TangibleObject*>(object);

		if(additionalMods.size() > 0 || skillMods->size() > 0)
			yellow = true;

		for (int i = 0; i < additionalMods.size(); i++) {
			item->addSkillMod(SkillModManager::WEARABLE, additionalMods.elementAt(i).getKey(), additionalMods.elementAt(i).getValue());
		}

		for (int i = 0; i < skillMods->size(); i++) {
			item->addSkillMod(SkillModManager::WEARABLE, skillMods->elementAt(i).getKey(), skillMods->elementAt(i).getValue());
		}
	}

	if (yellow)
		object->addMagicBit(false);
}

String LootManagerImplementation::getRandomLootableMod( unsigned int sceneObjectType ) {
	if( sceneObjectType == SceneObjectType::ARMORATTACHMENT ){
		return lootableArmorAttachmentMods.get(System::random(lootableArmorAttachmentMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::CLOTHINGATTACHMENT ){
		return lootableClothingAttachmentMods.get(System::random(lootableClothingAttachmentMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::ARMOR || sceneObjectType == SceneObjectType::BODYARMOR ||
			 sceneObjectType == SceneObjectType::HEADARMOR || sceneObjectType == SceneObjectType::MISCARMOR ||
			 sceneObjectType == SceneObjectType::LEGARMOR || sceneObjectType == SceneObjectType::ARMARMOR ||
			 sceneObjectType == SceneObjectType::HANDARMOR || sceneObjectType == SceneObjectType::FOOTARMOR ){
		return lootableArmorMods.get(System::random(lootableArmorMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::CLOTHING || sceneObjectType == SceneObjectType::BANDOLIER ||
			 sceneObjectType == SceneObjectType::BELT || sceneObjectType == SceneObjectType::BODYSUIT ||
		     sceneObjectType == SceneObjectType::CAPE || sceneObjectType == SceneObjectType::CLOAK ||
			 sceneObjectType == SceneObjectType::FOOTWEAR || sceneObjectType == SceneObjectType::DRESS ||
			 sceneObjectType == SceneObjectType::HANDWEAR || sceneObjectType == SceneObjectType::EYEWEAR ||
			 sceneObjectType == SceneObjectType::HEADWEAR || sceneObjectType == SceneObjectType::JACKET ||
			 sceneObjectType == SceneObjectType::PANTS || sceneObjectType == SceneObjectType::ROBE ||
			 sceneObjectType == SceneObjectType::SHIRT || sceneObjectType == SceneObjectType::VEST ||
			 sceneObjectType == SceneObjectType::WOOKIEGARB || sceneObjectType == SceneObjectType::MISCCLOTHING ||
			 sceneObjectType == SceneObjectType::SKIRT || sceneObjectType == SceneObjectType::WEARABLECONTAINER ||
			 sceneObjectType == SceneObjectType::JEWELRY || sceneObjectType == SceneObjectType::RING ||
			 sceneObjectType == SceneObjectType::BRACELET || sceneObjectType == SceneObjectType::NECKLACE ||
			 sceneObjectType == SceneObjectType::EARRING ){
		return lootableClothingMods.get(System::random(lootableClothingMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::ONEHANDMELEEWEAPON ){
		return lootableOneHandedMeleeMods.get(System::random(lootableOneHandedMeleeMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::TWOHANDMELEEWEAPON ){
		return lootableTwoHandedMeleeMods.get(System::random(lootableTwoHandedMeleeMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::MELEEWEAPON ){
		return lootableUnarmedMods.get(System::random(lootableUnarmedMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::PISTOL ){
		return lootablePistolMods.get(System::random(lootablePistolMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::RIFLE ){
		return lootableRifleMods.get(System::random(lootableRifleMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::CARBINE ){
		return lootableCarbineMods.get(System::random(lootableCarbineMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::POLEARM ){
		return lootablePolearmMods.get(System::random(lootablePolearmMods.size() - 1));
	}
	else if( sceneObjectType == SceneObjectType::SPECIALHEAVYWEAPON ){
		return lootableHeavyWeaponMods.get(System::random(lootableHeavyWeaponMods.size() - 1));
	}
	else{
		return "";
	}

}

bool LootManagerImplementation::createLoot(TransactionLog& trx, SceneObject* container, AiAgent* creature) {
	auto lootCollection = creature->getLootGroups();

	if (lootCollection == nullptr) {
		trx.abort() << "No lootCollection.";
		return false;
	}

	if (lootCollection->count() == 0) {
		trx.abort() << "Empty loot collection.";
		trx.discard();
		return false;
	}

	return createLootFromCollection(trx, container, lootCollection, creature->getLevel());
}

bool LootManagerImplementation::createLootFromCollection(TransactionLog& trx, SceneObject* container, const LootGroupCollection* lootCollection, int level) {
	uint64 objectID = 0;

	trx.addState("lootCollectionSize", lootCollection->count());

	Vector<int> chances;
	Vector<int> rolls;
	Vector<String> lootGroupNames;

	for (int i = 0; i < lootCollection->count(); ++i) {
		const LootGroupCollectionEntry* collectionEntry = lootCollection->get(i);
		int lootChance = collectionEntry->getLootChance();

		if (lootChance <= 0)
			continue;

		int roll = System::random(10000000);

		rolls.add(roll);

		if (roll > lootChance)
			continue;

 		// Start at 0
		int tempChance = 0;

		const LootGroups* lootGroups = collectionEntry->getLootGroups();

		//Now we do the second roll to determine loot group.
		roll = System::random(10000000);

		rolls.add(roll);

		//Select the loot group to use.
		for (int k = 0; k < lootGroups->count(); ++k) {
			const LootGroupEntry* groupEntry = lootGroups->get(k);

			lootGroupNames.add(groupEntry->getLootGroupName());

			tempChance += groupEntry->getLootChance();

			// Is this entry lower than the roll? If yes, then we want to try the next groupEntry.
			if (tempChance < roll)
				continue;

			objectID = createLoot(trx, container, groupEntry->getLootGroupName(), level);

			break;
		}
	}

	trx.addState("lootChances", chances);
	trx.addState("lootRolls", rolls);
	trx.addState("lootGroups", lootGroupNames);

	if (objectID == 0) {
		trx.abort() << "Did not win loot rolls.";
	}

	return objectID > 0 ? true : false;
}

uint64 LootManagerImplementation::createLoot(TransactionLog& trx, SceneObject* container, const String& lootMapEntry, int level, bool maxCondition) {
	String lootEntry = lootMapEntry;
	String lootGroup = "";

	int depthMax = 32;
	int depth = 0;

	while (lootGroupMap->lootGroupExists(lootEntry) && depthMax > depth++) {
		auto group = lootGroupMap->getLootGroupTemplate(lootEntry);

		if (group != nullptr) {
			lootGroup = lootEntry;
			lootEntry = group->getLootGroupEntryForRoll(System::random(10000000));
		}
	}

	Reference<const LootItemTemplate*> itemTemplate = lootGroupMap->getLootItemTemplate(lootEntry);

	if (itemTemplate == nullptr) {
		error() << "LootMapEntry does not exist: lootItem: " << lootEntry << " lootGroup: " << lootGroup << " lootMapEntry: " << lootMapEntry << " at search depth: " << depth;
		return 0;
	}

	trx.addState("lootMapEntry", lootMapEntry);

	TangibleObject* obj = nullptr;

	if (itemTemplate->isRandomResourceContainer()) {
		auto zone = container->getZone();

		if (zone == nullptr) {
			return 0;
		}

		obj = createLootResource(lootEntry, zone->getZoneName());
	} else {
		obj = createLootObject(trx, itemTemplate, level, maxCondition);
	}

	if (obj == nullptr) {
		return 0;
	}

	trx.setSubject(obj);

	if (container->transferObject(obj, -1, false, true)) {
		container->broadcastObject(obj, true);
	} else {
		obj->destroyObjectFromDatabase(true);
		trx.errorMessage() << "failed to transferObject to container.";
		return 0;
	}

	return obj != nullptr ? obj->getObjectID() : 0;
}

bool LootManagerImplementation::createLootSet(TransactionLog& trx, SceneObject* container, const String& lootGroup, int level, bool maxCondition, int setSize) {
	Reference<const LootGroupTemplate*> group = lootGroupMap->getLootGroupTemplate(lootGroup);

	if (group == nullptr) {
		warning("Loot group template requested does not exist: " + lootGroup);
		return false;
	}
	//Roll for the item out of the group.
	int roll = System::random(10000000);

	int lootGroupEntryIndex = group->getLootGroupIntEntryForRoll(roll);

	trx.addState("lootSetSize", setSize);
	trx.addState("lootGroup", lootGroup);

	for(int q = 0; q < setSize; q++) {
		String selection = group->getLootGroupEntryAt(lootGroupEntryIndex+q);
		Reference<const LootItemTemplate*> itemTemplate = lootGroupMap->getLootItemTemplate(selection);

		if (itemTemplate == nullptr) {
			warning("Loot item template requested does not exist: " + group->getLootGroupEntryForRoll(roll) + " for templateName: " + group->getTemplateName());
			return false;
		}

		TangibleObject* obj = nullptr;

		if (q == 0) {
			obj = createLootObject(trx, itemTemplate, level, maxCondition);
			trx.addState("lootSetNum", q);
		} else {
			TransactionLog trxSub = trx.newChild();

			trxSub.addState("lootSetSize", setSize);
			trxSub.addState("lootGroup", lootGroup);
			trxSub.addState("lootSetNum", q);

			obj = createLootObject(trxSub, itemTemplate, level, maxCondition);

			trxSub.setSubject(obj);
		}

		if (obj == nullptr)
			return false;

		trx.addRelatedObject(obj);

		if (container->transferObject(obj, -1, false, true)) {
			container->broadcastObject(obj, true);
		} else {
			trx.errorMessage() << "failed to transferObject " << obj->getObjectID() << " to container.";
			obj->destroyObjectFromDatabase(true);
			return false;
		}
	}

	return true;
}

void LootManagerImplementation::addStaticDots(TangibleObject* object, const LootItemTemplate* templateObject, int level) {
	if (object == nullptr)
		return;

	if (!object->isWeaponObject())
		return;

	ManagedReference<WeaponObject*> weapon = cast<WeaponObject*>(object);

	bool shouldGenerateDots = false;

	float dotChance = templateObject->getStaticDotChance();

	if (dotChance < 0)
		return;

	// Apply the Dot if the chance roll equals the number or is zero.
	if (dotChance == 0 || System::random(dotChance) == 0) { // Defined in loot item script.
		shouldGenerateDots = true;
	}

	if (shouldGenerateDots) {

		int dotType = templateObject->getStaticDotType();

		if (dotType < 1 || dotType > 4)
			return;

		const VectorMap<String, SortedVector<int> >* dotValues = templateObject->getStaticDotValues();
		int size = dotValues->size();

		// Check if they specified correct vals.
		if (size > 0) {
			weapon->addDotType(dotType);

			for (int i = 0; i < size; i++) {

				const String& property = dotValues->elementAt(i).getKey();
				const SortedVector<int>& theseValues = dotValues->elementAt(i).getValue();
				int min = theseValues.elementAt(0);
				int max = theseValues.elementAt(1);
				float value = 0;

				if (max != min) {
					value = calculateDotValue(min, max, level);
				}
				else { value = max; }

				if(property == "attribute") {
					if (min != max)
						value = System::random(max - min) + min;

					if (dotType != 2 && (value != 0 && value != 3 && value != 6)) {
						int numbers[] = { 0, 3, 6 }; // The main pool attributes.
						int choose = System::random(2);
						value = numbers[choose];
					}

					weapon->addDotAttribute(value);
				} else if (property == "strength") {
					weapon->addDotStrength(value);
				} else if (property == "duration") {
					weapon->addDotDuration(value);
				} else if (property == "potency") {
					weapon->addDotPotency(value);
				} else if (property == "uses") {
					weapon->addDotUses(value);
				}
			}

			weapon->addMagicBit(false);
		}
	}
}

void LootManagerImplementation::addRandomDots(TangibleObject* object, const LootItemTemplate* templateObject, int level, float excMod) {
	if (object == nullptr)
		return;

	if (!object->isWeaponObject())
		return;

	ManagedReference<WeaponObject*> weapon = cast<WeaponObject*>(object);

	bool shouldGenerateDots = false;

	float dotChance = templateObject->getRandomDotChance();

	if (dotChance < 0)
		return;

	float modSqr = excMod * excMod;

	// Apply the Dot if the chance roll equals the number or is zero.
	if (dotChance == 0 || System::random(dotChance / modSqr) == 0) { // Defined in loot item script.
		shouldGenerateDots = true;
	}

	if (shouldGenerateDots) {

		int number = 1;

		if (System::random(250 / modSqr) == 0)
			number = 2;

		for (int i = 0; i < number; i++) {
			int dotType = System::random(2) + 1;

			weapon->addDotType(dotType);

			int attMin = randomDotAttribute.elementAt(0);
			int attMax = randomDotAttribute.elementAt(1);
			float att = 0;

			if (attMin != attMax)
				att= System::random(attMax - attMin) + attMin;

			if (dotType != 2 && (att != 0 && att != 3 && att != 6)) {
				int numbers[] = { 0, 3, 6 }; // The main pool attributes.
				int choose = System::random(2);
				att = numbers[choose];
			}

			weapon->addDotAttribute(att);

			int strMin = randomDotStrength.elementAt(0);
			int strMax = randomDotStrength.elementAt(1);
			float str = 0;

			if (strMax != strMin)
				str = calculateDotValue(strMin, strMax, level);
			else
				str = strMax;

			if (excMod == 1.0 && (yellowChance == 0 || System::random(yellowChance) == 0)) {
				str *= yellowModifier;
			}

			if (dotType == 1)
				str = str * 2;
			else if (dotType == 3)
				str = str * 1.5;

			weapon->addDotStrength(str * excMod);

			int durMin = randomDotDuration.elementAt(0);
			int durMax = randomDotDuration.elementAt(1);
			float dur = 0;

			if (durMax != durMin)
				dur = calculateDotValue(durMin, durMax, level);
			else
				dur = durMax;

			if (excMod == 1.0 && (yellowChance == 0 || System::random(yellowChance) == 0)) {
				dur *= yellowModifier;
			}

			if (dotType == 2)
				dur = dur * 5;
			else if (dotType == 3)
				dur = dur * 1.5;

			weapon->addDotDuration(dur * excMod);

			int potMin = randomDotPotency.elementAt(0);
			int potMax = randomDotPotency.elementAt(1);
			float pot = 0;

			if (potMax != potMin)
				pot = calculateDotValue(potMin, potMax, level);
			else
				pot = potMax;

			if (excMod == 1.0 && (yellowChance == 0 || System::random(yellowChance) == 0)) {
				pot *= yellowModifier;
			}

			weapon->addDotPotency(pot * excMod);

			int useMin = randomDotUses.elementAt(0);
			int useMax = randomDotUses.elementAt(1);
			float use = 0;

			if (useMax != useMin)
				use = calculateDotValue(useMin, useMax, level);
			else
				use = useMax;

			if (excMod == 1.0 && (yellowChance == 0 || System::random(yellowChance) == 0)) {
				use *= yellowModifier;
			}

			weapon->addDotUses(use * excMod);
		}

		weapon->addMagicBit(false);
	}
}

float LootManagerImplementation::calculateDotValue(float min, float max, float level) {
	float randVal = (float)System::random(max - min);
	float value = Math::max(min, Math::min(max, randVal * (1 + (level / 1000)))); // Used for Str, Pot, Dur, Uses.

	if (value < min) {
		value = min;
	}

	return value;
}
