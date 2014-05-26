/*
 * MobileTest.cpp
 *
 *  Created on: Aug 10, 2013
 *      Author: TheAnswer
 */

#include "gtest/gtest.h"
#include "server/zone/templates/LootItemTemplate.h"
#include "server/zone/templates/LootGroupTemplate.h"
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

	HashTableIterator<uint32, Reference<CreatureTemplate*> > iter = CreatureTemplateManager::instance()->iterator();
	while (iter.hasNext()) {
		CreatureTemplate* creature = iter.next();
		std::string templateName( creature->getTemplateName().toCharArray() );

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
	}

	HashTableIterator<uint32, Reference<LairTemplate*> > itera = CreatureTemplateManager::instance()->lairTemplateIterator();
	while (itera.hasNext()) {
		LairTemplate* lair = itera.next();
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
