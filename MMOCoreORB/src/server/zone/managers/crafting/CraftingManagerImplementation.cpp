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

#include "CraftingManager.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ResourceSlot.h"
#include "server/zone/managers/crafting/labratories/SharedLabratory.h"
#include "server/zone/managers/crafting/labratories/ResourceLabratory.h"
#include "server/zone/managers/crafting/labratories/GeneticLabratory.h"

void CraftingManagerImplementation::initialize() {
	schematicMap = SchematicMap::instance();
	schematicMap->initialize(zoneServer.get());
	loadBioSkillMods();
	configureLabratories();
}

void CraftingManagerImplementation::awardSchematicGroup(PlayerObject* playerObject, Vector<String>& schematicgroups, bool updateClient) {
	schematicMap->addSchematics(playerObject, schematicgroups, updateClient);
}

void CraftingManagerImplementation::removeSchematicGroup(PlayerObject* playerObject, Vector<String>& schematicgroups, bool updateClient) {
	schematicMap->removeSchematics(playerObject, schematicgroups, updateClient);
}

void CraftingManagerImplementation::sendDraftSlotsTo(CreatureObject* player, uint32 schematicID) {
	schematicMap->sendDraftSlotsTo(player, schematicID);
}

void CraftingManagerImplementation::sendResourceWeightsTo(CreatureObject* player, uint32 schematicID) {
	schematicMap->sendResourceWeightsTo(player, schematicID);
}

int CraftingManagerImplementation::calculateAssemblySuccess(CreatureObject* player,
		DraftSchematic* draftSchematic, float effectiveness) {

	// assemblyPoints is 0-12
	/// City bonus should be 10
	float cityBonus = player->getSkillMod("private_spec_assembly");

	float assemblyPoints = ((float)player->getSkillMod(draftSchematic->getAssemblySkill())) / 10.0f;
	int failMitigate = (player->getSkillMod(draftSchematic->getAssemblySkill()) - 100 + cityBonus) / 7;

	if(failMitigate < 0)
		failMitigate = 0;
	if(failMitigate > 5)
		failMitigate = 5;

	// 0.85-1.15
	float toolModifier = 1.0f + (effectiveness / 100.0f);

	//Pyollian Cake

	float craftbonus = 0;
	if (player->hasBuff(BuffCRC::FOOD_CRAFT_BONUS)) {
		Buff* buff = player->getBuff(BuffCRC::FOOD_CRAFT_BONUS);

		if (buff != NULL) {
			craftbonus = buff->getSkillModifierValue("craft_bonus");
			toolModifier *= 1.0f + (craftbonus / 100.0f);
		}
	}

	int luckRoll = System::random(100) + cityBonus;

	if(luckRoll > (95 - craftbonus))
		return AMAZINGSUCCESS;

	if(luckRoll < (5 - craftbonus - failMitigate))
		luckRoll -= System::random(100);

	//if(luckRoll < 5)
	//	return CRITICALFAILURE;

	luckRoll += System::random(player->getSkillMod("luck") + player->getSkillMod("force_luck"));

	int assemblyRoll = (toolModifier * (luckRoll + (assemblyPoints * 5)));

	if (assemblyRoll > 70)
		return GREATSUCCESS;

	if (assemblyRoll > 60)
		return GOODSUCCESS;

	if (assemblyRoll > 50)
		return MODERATESUCCESS;

	if (assemblyRoll > 40)
		return SUCCESS;

	if (assemblyRoll > 30)
		return MARGINALSUCCESS;

	if (assemblyRoll > 20)
		return OK;

	return BARELYSUCCESSFUL;
}


int CraftingManagerImplementation::calculateExperimentationFailureRate(CreatureObject* player,
		ManufactureSchematic* manufactureSchematic, int pointsUsed) {
	SharedLabratory* lab = labs.get(manufactureSchematic->getLabratory());
	// Get the Weighted value of MA
	float ma = lab->getWeightedValue(manufactureSchematic, MA);

	// Get Experimentation skill
	String expSkill = manufactureSchematic->getDraftSchematic()->getExperimentationSkill();
	float expPoints = player->getSkillMod(expSkill);

	int failure = int((50.0f + (ma - 500.0f) / 40.0f + expPoints - 5.0f * float(pointsUsed)));

	return failure;
}

bool CraftingManagerImplementation::allowManufactureSchematic(ManufactureSchematic* manufactureSchematic) {
	SharedLabratory* lab = labs.get(manufactureSchematic->getLabratory());
	return lab->allowFactoryRun(manufactureSchematic);
}

int CraftingManagerImplementation::calculateExperimentationSuccess(CreatureObject* player,
		DraftSchematic* draftSchematic, float effectiveness) {

	float cityBonus = player->getSkillMod("private_spec_experimentation");

	// assemblyPoints is 0-12
	float experimentingPoints = ((float)player->getSkillMod(draftSchematic->getExperimentationSkill())) / 10.0f;

	int failMitigate = (player->getSkillMod(draftSchematic->getAssemblySkill()) - 100 + cityBonus) / 7;
	if(failMitigate < 0)
		failMitigate = 0;
	if(failMitigate > 5)
		failMitigate = 5;

	// 0.85-1.15
	float toolModifier = 1.0f + (effectiveness / 100.0f);

	//Bespin Port

	float expbonus = 0;
	if (player->hasBuff(BuffCRC::FOOD_EXPERIMENT_BONUS)) {
		Buff* buff = player->getBuff(BuffCRC::FOOD_EXPERIMENT_BONUS);

		if (buff != NULL) {
			expbonus = buff->getSkillModifierValue("experiment_bonus");
			toolModifier *= 1.0f + (expbonus / 100.0f);
		}
	}

	/// Range 0-100
	int luckRoll = System::random(100) + cityBonus;

	if(luckRoll > (95 - expbonus))
		return AMAZINGSUCCESS;

	if(luckRoll < (5 - expbonus - failMitigate))
		luckRoll -= System::random(100);

	//if(luckRoll < 5)
	//	return CRITICALFAILURE;

	luckRoll += System::random(player->getSkillMod("luck") + player->getSkillMod("force_luck"));

	///
	int experimentRoll = (toolModifier * (luckRoll + (experimentingPoints * 4)));

	if (experimentRoll > 70)
		return GREATSUCCESS;

	if (experimentRoll > 60)
		return GOODSUCCESS;

	if (experimentRoll > 50)
		return MODERATESUCCESS;

	if (experimentRoll > 40)
		return SUCCESS;

	if (experimentRoll > 30)
		return MARGINALSUCCESS;

	if (experimentRoll > 20)
		return OK;

	return BARELYSUCCESSFUL;
}

String CraftingManagerImplementation::generateSerial() {

	StringBuffer ss;

	char a;

	ss << "(";

	for (int i = 0; i < 8; ++i) {

		a = (System::random(34));
		if (a < 9) {
			a = a + 48;
		} else {
			a -= 9;
			a = a + 97;
		}
		ss << a;
	}

	ss << ")";


	return ss.toString();
}

void CraftingManagerImplementation::experimentRow(ManufactureSchematic* schematic, CraftingValues* craftingValues,
		int rowEffected, int pointsAttempted, float failure, int experimentationResult) {
	int labratory = schematic->getLabratory();
	SharedLabratory* lab = labs.get(labratory);
	lab->experimentRow(craftingValues,rowEffected,pointsAttempted,failure,experimentationResult);
}

bool CraftingManagerImplementation::loadBioSkillMods() {
	Reference<Lua* > lua = new Lua();
	lua->init();

	if (!lua->runFile("scripts/crafting/bio_skill_mods.lua")) {
		return false;
	}

	LuaObject bioModsTable = lua->getGlobalObject("bioSkillMods");

	if (!bioModsTable.isValidTable())
		return false;

	for (int i = 1; i <= bioModsTable.getTableSize(); ++i) {
		String mod = bioModsTable.getStringAt(i);
		bioMods.put(mod);
	}

	bioModsTable.pop();

	return true;

}

String CraftingManagerImplementation::checkBioSkillMods(const String& property) {

	for (int l = 0; l < bioMods.size(); ++l) {

		String key = bioMods.elementAt(l);
		String statname = "cat_skill_mod_bonus.@stat_n:" + key;

		if (property == statname) {
			return key;
		}
	}

	return "";
}

void CraftingManagerImplementation::configureLabratories() {
	ResourceLabratory* resLab = new ResourceLabratory();
	resLab->initialize(zoneServer.get());
	labs.put(static_cast<int>(RESOURCE_LAB),resLab); //RESOURCE_LAB

	GeneticLabratory* genLab = new GeneticLabratory();
	genLab->initialize(zoneServer.get());
	labs.put(static_cast<int>(GENETIC_LAB), genLab); //GENETIC_LAB
}
void CraftingManagerImplementation::setInitialCraftingValues(TangibleObject* prototype, ManufactureSchematic* manufactureSchematic, int assemblySuccess) {
	if(manufactureSchematic == NULL || manufactureSchematic->getDraftSchematic() == NULL)
		return;
	int labratory = manufactureSchematic->getLabratory();
	SharedLabratory* lab = labs.get(labratory);
	lab->setInitialCraftingValues(prototype,manufactureSchematic,assemblySuccess);
}
