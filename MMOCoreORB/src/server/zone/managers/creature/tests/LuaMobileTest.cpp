/*
 * MobileTest.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: TheAnswer
 */

#include "gtest/gtest.h"
#include "server/zone/templates/LootItemTemplate.h"
#include "server/zone/templates/LootGroupTemplate.h"
#include "server/zone/managers/faction/FactionManager.h"
#include "server/zone/managers/loot/LootGroupMap.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/managers/loot/lootgroup/LootGroupCollectionEntry.h"

class LuaMobileTest : public ::testing::Test {
public:

	LuaMobileTest() {
		// Perform creation setup here.
	}

	~LuaMobileTest() {
	}

	void SetUp() {
		// Perform setup of common constructs here.
	}

	void TearDown() {
		// Perform clean up of common constructs here.
	}

	void checkLootGroupEntryRecursive(LootGroupMap* lootGroupMap, String& entryName, Vector<String>* parentGroups) {
		if (entryName.isEmpty())
			return;

		//Check for infinite recursion
		for (int i = 0; i < parentGroups->size(); i++) {
			String parentName = parentGroups->get(i);

			EXPECT_FALSE( parentName == entryName ) << "Loot group " << std::string(parentName.toCharArray()) << " failed recursion check.";

			if (parentName == entryName)
				return;
		}

		if (lootGroupMap->lootGroupExists(entryName)) {

			LootGroupTemplate* lootGroupTemplate = lootGroupMap->getLootGroupTemplate(entryName);

			for (int j = 0; j < lootGroupTemplate->size(); j++) {

				String entry = lootGroupTemplate->getLootGroupEntryAt(j);

				parentGroups->add(entryName);

				checkLootGroupEntryRecursive(lootGroupMap, entry, parentGroups);
			}

		} else {
			Reference<LootItemTemplate*> itemTemplate = lootGroupMap->getLootItemTemplate( entryName );
			EXPECT_TRUE( itemTemplate != NULL ) << "Item template " << std::string(entryName.toCharArray()) << " from " << std::string(parentGroups->get(parentGroups->size() - 1).toCharArray()) << " was not found in LootGroupMap";
		}
	}
};

TEST_F(LuaMobileTest, LuaMobileTemplatesTest) {
	CreatureTemplateManager::DEBUG_MODE = 1;

	// Verify that all mobiles load
	ASSERT_EQ(CreatureTemplateManager::instance()->loadTemplates(), 0);

	// Verify loot group map loads
	LootGroupMap* lootGroupMap = LootGroupMap::instance();
	ASSERT_EQ(lootGroupMap->initialize(), 0);

	// Verify factions load
	FactionManager::instance()->loadData();
	ASSERT_FALSE(FactionManager::instance()->getFactionMap()->isEmpty());

	// Load Templates
	ASSERT_TRUE( TemplateManager::instance() != NULL );
	if( TemplateManager::instance()->loadedTemplatesCount == 0 ){
		TemplateManager::instance()->loadLuaTemplates();
	}

	// Test Creature Templates
	HashTableIterator<uint32, Reference<CreatureTemplate*> > creatureIterator = CreatureTemplateManager::instance()->iterator();
	while (creatureIterator.hasNext()) {
		CreatureTemplate* creature = creatureIterator.next();
		std::string templateName( creature->getTemplateName().toCharArray() );

		// Check configured templates
		Vector<String> objTemps = creature->getTemplates();
		EXPECT_FALSE( objTemps.isEmpty() ) << "Mobile " << templateName << " does not have any templates configured";
		for( int j=0; j< objTemps.size(); j++ ){
			SharedObjectTemplate* templateData = TemplateManager::instance()->getTemplate(objTemps.get(j).hashCode());
			std::string objName = objTemps.get(j).toCharArray();
			EXPECT_TRUE( templateData != NULL ) << "Mobile " << templateName << " has invalid template configured: " << objName;
		}

		// Verify that control device template is valid
		String controlDeviceTemplate = creature->getControlDeviceTemplate();
		if (!controlDeviceTemplate.isEmpty()) {
			SharedObjectTemplate* controlDeviceTemplateData = TemplateManager::instance()->getTemplate(controlDeviceTemplate.hashCode());
			EXPECT_TRUE( controlDeviceTemplateData != NULL ) << "Control device template " << controlDeviceTemplate.toCharArray() << " from " << templateName << " does not exist.";
			EXPECT_TRUE( controlDeviceTemplate.beginsWith("object/intangible/pet/") ) << "Control device template " << controlDeviceTemplate.toCharArray() << " from " << templateName << " is not a pet/droid control device template.";
		}

		// Verify that faction and pvpFaction are valid
		String faction = creature->getFaction();
		if (!faction.isEmpty()) {
			EXPECT_TRUE( FactionManager::instance()->isFaction(faction) ) << "Faction, " << faction.toCharArray() << ", from mobile template " << templateName << " does not exist.";
		}
		String pvpFaction = creature->getPvpFaction();
		if (!pvpFaction.isEmpty()) {
			EXPECT_TRUE( FactionManager::instance()->isFaction(pvpFaction) ) << "PvpFaction, " << pvpFaction.toCharArray() << ", from mobile template " << templateName << " does not exist.";
		}

		// Verify level
		int level = creature->getLevel();
		EXPECT_TRUE( level > 0 ) << "Level is not a positive value on mobile: " << templateName;

		// Verify hit chance
		float hitChance = creature->getChanceHit();
		EXPECT_TRUE( hitChance > 0 ) << "ChanceHit is not a positive value on mobile: " << templateName;

		// Verify xp
		int xp = creature->getBaseXp();
		EXPECT_TRUE( xp >= 0 ) << "Xp has a negative value on mobile: " << templateName;

		// Verify damage
		int minDamage = creature->getDamageMin();
		int maxDamage = creature->getDamageMax();
		EXPECT_TRUE( minDamage > 0 ) << "Min damage is not a positive value on mobile: " << templateName;
		EXPECT_TRUE( maxDamage >= minDamage ) << "Max damage is lower than min damage on mobile: " << templateName;

		// Verify HAM
		int minHam = creature->getBaseHAM();
		int maxHam = creature->getBaseHAMmax();
		EXPECT_TRUE( minHam > 0 ) << "Base ham is not a positive value on mobile: " << templateName;
		EXPECT_TRUE( maxHam >= minHam ) << "Base ham max is lower than base ham on mobile: " << templateName;

		// Verify armor
		int armor = creature->getArmor();
		EXPECT_TRUE( armor >= 0 && armor <= 3 ) << "Armor is not a valid value on mobile: " << templateName;

		// Verify resists
		float kinetic = creature->getKinetic();
		EXPECT_TRUE( kinetic >= -1 && kinetic <= 100 ) << "Kinetic resist is not a valid value on mobile: " << templateName;
		float energy = creature->getEnergy();
		EXPECT_TRUE( energy >= -1 && energy <= 100 ) << "Energy resist is not a valid value on mobile: " << templateName;
		float electricity = creature->getElectricity();
		EXPECT_TRUE( electricity >= -1 && electricity <= 100 ) << "Electricity resist is not a valid value on mobile: " << templateName;
		float stun = creature->getStun();
		EXPECT_TRUE( stun >= -1 && stun <= 100 ) << "Stun resist is not a valid value on mobile: " << templateName;
		float blast = creature->getBlast();
		EXPECT_TRUE( blast >= -1 && blast <= 100 ) << "Blast resist is not a valid value on mobile: " << templateName;
		float heat = creature->getHeat();
		EXPECT_TRUE( heat >= -1 && heat <= 100 ) << "Heat resist is not a valid value on mobile: " << templateName;
		float cold = creature->getCold();
		EXPECT_TRUE( cold >= -1 && cold <= 100 ) << "Cold resist is not a valid value on mobile: " << templateName;
		float acid = creature->getAcid();
		EXPECT_TRUE( acid >= -1 && acid <= 100 ) << "Acid resist is not a valid value on mobile: " << templateName;
		float lightSaber = creature->getLightSaber();
		EXPECT_TRUE( lightSaber >= -1 && lightSaber <= 100 ) << "LightSaber resist is not a valid value on mobile: " << templateName;

		// Verify creature resources
		String meatResources = "meat_domesticated,meat_wild,meat_herbivore,meat_carnivore,meat_reptilian,meat_avian,meat_insect";
		String hideResources = "hide_bristley,hide_leathery,hide_scaley,hide_wooly";
		String boneResources = "bone_avian,bone_mammal";
		String milkResources = "milk_domesticated,milk_wild";
		String meat = creature->getMeatType();
		float meatMax = creature->getMeatMax();
		if (!meat.isEmpty()) {
			EXPECT_TRUE( meatResources.contains(meat) ) << "Meat type on mobile " << templateName << " is not a valid meat resource";
			EXPECT_TRUE( meatMax > 0 ) << "Meat amount on mobile " << templateName << " is zero.";
		}
		String hide = creature->getHideType();
		float hideMax = creature->getHideMax();
		if (!hide.isEmpty()) {
			EXPECT_TRUE( hideResources.contains(hide) ) << "Hide type on mobile " << templateName << " is not a valid hide resource";
			EXPECT_TRUE( hideMax > 0 ) << "Hide amount on mobile " << templateName << " is zero.";
		}
		String bone = creature->getBoneType();
		float boneMax = creature->getBoneMax();
		if (!bone.isEmpty()) {
			EXPECT_TRUE( boneResources.contains(bone) ) << "Bone type on mobile " << templateName << " is not a valid bone resource";
			EXPECT_TRUE( boneMax > 0 ) << "Bone amount on mobile " << templateName << " is zero.";
		}
		String milk = creature->getMilkType();
		float milkMax = creature->getMilk();
		if (!milk.isEmpty()) {
			EXPECT_TRUE( milkResources.contains(milk) ) << "Milk type on mobile " << templateName << " is not a valid milk resource";
			EXPECT_TRUE( milkMax > 0 ) << "Milk amount on mobile " << templateName << " is zero.";
		}

		// Verify taming chance
		float tamingChance = creature->getTame();
		EXPECT_TRUE( tamingChance >= 0 && tamingChance <= 1 ) << "Taming chance is not a valid value on mobile: " << templateName;

		// Verify scale
		float scale = creature->getScale();
		EXPECT_TRUE( scale > 0 ) << "Scale is not a positive value on mobile: " << templateName;

		// Verify loot group percentages
		LootGroupCollection* groupCollection = creature->getLootGroups();
		if( groupCollection->count() > 0 ){


			for( int i = 0; i < groupCollection->count(); i++ ){

				LootGroupCollectionEntry* collectionEntry = groupCollection->get(i);
				LootGroups* groups = collectionEntry->getLootGroups();
				if( groups->count() > 0){

					int totalChance = 0;
					for( int j = 0; j < groups->count(); j++ ){

						LootGroupEntry* lootGroup = groups->get(j);
						totalChance += lootGroup->getLootChance();

						// Verify loot group is configured correctly
						LootGroupTemplate* foundGroup = lootGroupMap->getLootGroupTemplate( lootGroup->getLootGroupName() );
						std::string groupName( lootGroup->getLootGroupName().toCharArray() );
						EXPECT_TRUE( foundGroup != NULL ) << "Loot group " << groupName << " from " << templateName << " was not found in LootGroupMap";

					}

					EXPECT_EQ( 10000000, totalChance ) << "Loot groups total chance is incorrect " << templateName;
				}
			}
		}

		// Verify weapon groups exist
		Vector<String> weapons = creature->getWeapons();
		for (int i = 0; i < weapons.size(); i++) {
			String weaponGroup = weapons.get(i);
			std::string groupName( weaponGroup.toCharArray() );
			Vector<String> group = CreatureTemplateManager::instance()->getWeapons(weaponGroup);
			EXPECT_TRUE( group.size() > 0 ) << "Weapon group " << groupName << " from " << templateName << " was not found in weaponMap";
		}

		// Verify conversation template exist, and the mob has converse option bit
		uint32 convoTemplate = creature->getConversationTemplate();
		uint32 optionsBitmask = creature->getOptionsBitmask();
		if (convoTemplate != 0) {
			ConversationTemplate* convoTemp = CreatureTemplateManager::instance()->getConversationTemplate(convoTemplate);
			EXPECT_TRUE( convoTemp != NULL ) << "Conversation template from " << templateName << " was not found.";
			EXPECT_TRUE( optionsBitmask & OptionBitmask::CONVERSE ) << templateName << " has a convo template but not the CONVERSE options bit.";
		}

		// Verify that outfits exist
		String outfit = creature->getOutfit();
		if (!outfit.isEmpty()) {
			MobileOutfitGroup* outfitGroup = CreatureTemplateManager::instance()->getMobileOutfitGroup(outfit);
			EXPECT_TRUE( outfitGroup != NULL ) << "Outfit group " << outfit.toCharArray() << " from " << templateName << " was not found.";
		}
	}

	// Test Lair Templates
	HashTableIterator<uint32, Reference<LairTemplate*> > lairIterator = CreatureTemplateManager::instance()->lairTemplateIterator();
	while (lairIterator.hasNext()) {
		LairTemplate* lair = lairIterator.next();
		std::string templateName( lair->getName().toCharArray() );

		// Verify that mobiles exist and that their weighting is positive
		VectorMap<String, int>* mobiles = lair->getMobiles();
		for (int i = 0; i < mobiles->size(); i++) {
			int weighting = mobiles->elementAt(i).getValue();
			String mobile = mobiles->elementAt(i).getKey();
			std::string mobName = mobile.toCharArray();
			EXPECT_TRUE( CreatureTemplateManager::instance()->getTemplate(mobile) != NULL ) << "Mobile " << mobName << " in lair template " << templateName << " does not exist";
			EXPECT_TRUE( weighting > 0 ) << "Mobile " << mobName << " in lair template " << templateName << " has a non positive weighting";
		}

		// Verify that boss mobiles exist and that their count is positive
		VectorMap<String, int>* bossMobiles = lair->getBossMobiles();
		for (int i = 0; i < bossMobiles->size(); i++) {
			int count = bossMobiles->elementAt(i).getValue();
			String bossMob = bossMobiles->elementAt(i).getKey();
			std::string bossName = bossMob.toCharArray();
			EXPECT_TRUE( CreatureTemplateManager::instance()->getTemplate(bossMob) != NULL ) << "Boss mobile " << bossName << " in lair template " << templateName << " does not exist";
			EXPECT_TRUE( count > 0 ) << "Boss mobile " << bossName << " in lair template " << templateName << " has a non positive spawn count";
		}

		// Verify spawn limit is positive
		int limit = lair->getSpawnLimit();
		EXPECT_TRUE( limit > 0 ) << "Spawn limit in lair template " << templateName << " is not positive";

		// Verify any configured buildings exist
		for(int i=0; i<=4; i++){

			Vector<String>* buildings = lair->getBuildings( i );
			if( buildings == NULL )
				continue;

			for( int j=0; j < buildings->size(); j++ ){
				String buildingTemplate = buildings->get(j);
				std::string buildingStr = buildingTemplate.toCharArray();
				SharedObjectTemplate* templateObject = TemplateManager::instance()->getTemplate(buildingTemplate.hashCode());
				EXPECT_TRUE( templateObject != NULL && templateObject->isSharedTangibleObjectTemplate() ) << "Building template " << buildingStr << " in lair template " << templateName << " does not exist";
				if( lair->getBuildingType() == LairTemplate::LAIR ){
					EXPECT_TRUE( buildingTemplate.beginsWith( "object/tangible/lair/") ) << "Building template " << buildingStr << " in lair template " << templateName << " is not a child of object/tangible/lair/";
				}
				if( lair->getBuildingType() == LairTemplate::THEATER ){
					EXPECT_TRUE( buildingTemplate.beginsWith( "object/building/poi/") ) << "Building template " << buildingStr << " in lair template " << templateName << " is not a child of object/building/poi/";
				}
			}
		}

		// TODO: Add test to enforce LAIRs and THEATERs have at least one building configured

	}

	// Test Spawn Groups
	HashTableIterator<uint32, Reference<SpawnGroup*> > spawnIterator = CreatureTemplateManager::instance()->spawnGroupIterator();
	while (spawnIterator.hasNext()) {
		SpawnGroup* group = spawnIterator.next();
		std::string templateName( group->getTemplateName().toCharArray() );

		// Verify spawn limit is not negative
		int limit = group->getMaxSpawnLimit();
		EXPECT_TRUE( limit >= 0 ) << "Max spawn limit in spawn group " << templateName << " is negative";

		// Verify spawn list
		Vector<Reference<LairSpawn*> >* spawnList = group->getSpawnList();
		for (int i = 0; i < spawnList->size(); i++) {
			LairSpawn* spawn = spawnList->get(i);
			std::string lairName( spawn->getLairTemplateName().toCharArray() );

			// Verify lair template exists
			String lairTemplateName = spawn->getLairTemplateName();
			Reference<LairTemplate*> lairTemplate = CreatureTemplateManager::instance()->getLairTemplate(lairTemplateName.hashCode());
			EXPECT_TRUE( lairTemplate != NULL ) << "Lair template " << lairName << " in spawn group " << templateName << " does not exist.";

			// Verify spawn limit is at least -1
			float spawnLimit = spawn->getSpawnLimit();
			EXPECT_TRUE( spawnLimit >= -1 ) << "SpawnLimit for lairTemplate " << lairName << " in spawn group " << templateName << " is less than -1.";

			// Verify difficulties
			int minDiff = spawn->getMinDifficulty();
			int maxDiff = spawn->getMaxDifficulty();
			EXPECT_TRUE( minDiff > 0 ) << "MinDifficulty for lairTemplate " << lairName << " in spawn group " << templateName << " is not positive.";
			EXPECT_TRUE( maxDiff >= minDiff ) << "MaxDifficulty for lairTemplate " << lairName << " in spawn group " << templateName << " is less than min difficulty.";

			// Verify number to spawn is not negative
			int numberToSpawn = spawn->getNumberToSpawn();
			EXPECT_TRUE( numberToSpawn >= 0 ) << "NumberToSpawn for lairTemplate " << lairName << " in spawn group " << templateName << " is negative.";

			// Verify weighting is positive
			int weighting = spawn->getWeighting();
			EXPECT_TRUE( weighting > 0 ) << "Weighting for lairTemplate " << lairName << " in spawn group " << templateName << " is not positive.";

			// Verify size is at least 1
			float size = spawn->getSize();
			EXPECT_TRUE( size >= 1 ) << "Size for lairTemplate " << lairName << " in spawn group " << templateName << " is less than 1.";
		}
	}

	// Test Destroy Mission Spawn Groups
	HashTableIterator<uint32, Reference<SpawnGroup*> > missionIterator = CreatureTemplateManager::instance()->destroyMissionGroupIterator();
	while (missionIterator.hasNext()) {
		SpawnGroup* group = missionIterator.next();
		std::string templateName( group->getTemplateName().toCharArray() );

		// Verify spawn list
		Vector<Reference<LairSpawn*> >* spawnList = group->getSpawnList();
		for (int i = 0; i < spawnList->size(); i++) {
			LairSpawn* spawn = spawnList->get(i);
			std::string lairName( spawn->getLairTemplateName().toCharArray() );

			// Verify lair template exists
			String lairTemplateName = spawn->getLairTemplateName();
			Reference<LairTemplate*> lairTemplate = CreatureTemplateManager::instance()->getLairTemplate(lairTemplateName.hashCode());
			EXPECT_TRUE( lairTemplate != NULL ) << "Lair template " << lairName << " in spawn group " << templateName << " does not exist.";

			// Verify difficulties
			int minDiff = spawn->getMinDifficulty();
			int maxDiff = spawn->getMaxDifficulty();
			EXPECT_TRUE( minDiff > 0 ) << "MinDifficulty for lairTemplate " << lairName << " in spawn group " << templateName << " is not positive.";
			EXPECT_TRUE( maxDiff >= minDiff ) << "MaxDifficulty for lairTemplate " << lairName << " in spawn group " << templateName << " is less than min difficulty.";

			// Verify size is at least 1
			float size = spawn->getSize();
			EXPECT_TRUE( size >= 1 ) << "Size for lairTemplate " << lairName << " in spawn group " << templateName << " is less than 1.";
		}
	}
}

TEST_F(LuaMobileTest, LuaLootGroupsTest) {

	LootGroupMap* lootGroupMap = LootGroupMap::instance();
	ASSERT_EQ(lootGroupMap->initialize(), 0);

	//Make sure that no loot items have the same name as a loot group
	HashTableIterator<String, Reference<LootItemTemplate*> > itemIter = lootGroupMap->itemTemplates.iterator();
	while (itemIter.hasNext()) {

		LootItemTemplate* lootItemTemplate = itemIter.next();
		String itemTemplateName( lootItemTemplate->getTemplateName().toCharArray() );

		EXPECT_FALSE( lootGroupMap->lootGroupExists(itemTemplateName) ) << "Loot item " << std::string(itemTemplateName.toCharArray()) << " has the same name as a loot group.";
	}

	HashTableIterator<String, Reference<LootGroupTemplate*> > iter = lootGroupMap->groupTemplates.iterator();
	while (iter.hasNext()) {

		LootGroupTemplate* lootGroupTemplate = iter.next();
		String groupTemplateName( lootGroupTemplate->getTemplateName().toCharArray() );

		// Check non-empty loot groups to make sure their chances total correctly
		if( lootGroupTemplate->getLootGroupEntryForRoll(-1).length() > 0  ){
			EXPECT_GT( lootGroupTemplate->getLootGroupEntryForRoll(10000000).length(), 0 ) << "Item total chance is less than 10000000: " << std::string(groupTemplateName.toCharArray());
			EXPECT_EQ( lootGroupTemplate->getLootGroupEntryForRoll(10000001).length(), 0 ) << "Item total chance is greater than 10000000: " << std::string(groupTemplateName.toCharArray());
		}

		// Check that all loot group entries are valid
		for( int i = 0; i < lootGroupTemplate->size(); i++ ){

			Vector<String> parentGroups;
			parentGroups.add(groupTemplateName);

			String entryName = lootGroupTemplate->getLootGroupEntryAt(i);

			checkLootGroupEntryRecursive(lootGroupMap, entryName, &parentGroups);
		}

	}

}
