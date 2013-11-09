le_repair_crafted_advanced = Creature:new {
	objectName = "@mob/creature_names:le_repair_droid",
	socialGroup = "",
	pvpFaction = "",
	faction = "",
	level = 4,
	chanceHit = 0.24,
	damageMin = 69,
	damageMax = 85,
	baseXp = 0,
	baseHAM = 5000,
	baseHAMmax = 6000,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = HERD,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/creature/npc/droid/crafted/le_repair_droid_advanced.iff"
	},		
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
}

CreatureTemplates:addCreatureTemplate(le_repair_crafted_advanced, "le_repair_crafted_advanced")