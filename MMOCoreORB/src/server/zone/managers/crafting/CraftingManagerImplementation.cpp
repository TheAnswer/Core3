/*
 				Copyright <SWGEmu>
		See file COPYING for copying conditions. */

#include "server/zone/managers/crafting/CraftingManager.h"
#include "server/zone/objects/resource/ResourceContainer.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ResourceSlot.h"
#include "server/zone/managers/crafting/labratories/SharedLabratory.h"
#include "server/zone/managers/crafting/labratories/ResourceLabratory.h"
#include "server/zone/managers/crafting/labratories/GeneticLabratory.h"
#include "server/zone/managers/crafting/labratories/DroidLabratory.h"

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

int CraftingManagerImplementation::calculateAssemblySuccess(CreatureObject* player,	DraftSchematic* draftSchematic, float effectiveness) {
	SharedLabratory* lab = labs.get(draftSchematic->getLabratory());
	return lab->calculateAssemblySuccess(player,draftSchematic,effectiveness);
}


int CraftingManagerImplementation::calculateExperimentationFailureRate(CreatureObject* player,
		ManufactureSchematic* manufactureSchematic, int pointsUsed) {
	SharedLabratory* lab = labs.get(manufactureSchematic->getLabratory());
	// Get the Weighted value of MA
	float ma = lab->getWeightedValue(manufactureSchematic, MA);

	// Get Experimentation skill
	String expSkill = manufactureSchematic->getDraftSchematic()->getExperimentationSkill();
	float expPoints = player->getSkillMod(expSkill) / 10.0f;

	int failure = int((50.0f + (ma - 500.0f) / 40.0f + expPoints - 5.0f * float(pointsUsed)));

	return failure;
}

bool CraftingManagerImplementation::allowManufactureSchematic(ManufactureSchematic* manufactureSchematic) {
	SharedLabratory* lab = labs.get(manufactureSchematic->getLabratory());
	return lab->allowFactoryRun(manufactureSchematic);
}

int CraftingManagerImplementation::getCreationCount(ManufactureSchematic* manufactureSchematic) {
	SharedLabratory* lab = labs.get(manufactureSchematic->getLabratory());
	return lab->getCreationCount(manufactureSchematic);
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

	if (!lua->runFile("scripts/managers/crafting/bio_skill_mods.lua")) {
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

	DroidLabratory* droidLab = new DroidLabratory();
	droidLab->initialize(zoneServer.get());
	labs.put(static_cast<int>(DROID_LAB), droidLab); //DROID_LAB

}
void CraftingManagerImplementation::setInitialCraftingValues(TangibleObject* prototype, ManufactureSchematic* manufactureSchematic, int assemblySuccess) {
	if(manufactureSchematic == NULL || manufactureSchematic->getDraftSchematic() == NULL)
		return;
	int labratory = manufactureSchematic->getLabratory();
	SharedLabratory* lab = labs.get(labratory);
	lab->setInitialCraftingValues(prototype,manufactureSchematic,assemblySuccess);
}
