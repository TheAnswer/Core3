r2_crafted = Creature:new {
	objectName = "@mob/creature_names:r2_crafted",
	socialGroup = "",
	pvpFaction = "",
	faction = "",
	level = 4,
	chanceHit = 0.24,
	damageMin = 135,
	damageMax = 150,
	baseXp = 0,
	baseHAM = 4000,
	baseHAMmax = 4500,
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
		"object/creature/npc/droid/crafted/r2_droid.iff"
	},		
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
}

CreatureTemplates:addCreatureTemplate(r2_crafted, "r2_crafted")