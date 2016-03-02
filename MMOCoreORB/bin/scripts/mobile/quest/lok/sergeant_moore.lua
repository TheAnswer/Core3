sergeant_moore = Creature:new {
	objectName = "@mob/creature_names:commoner",
	customName = "Sergeant Moore (Retired)",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 4,
	chanceHit = 0.24,
	damageMin = 40,
	damageMax = 45,
	baseXp = 62,
	baseHAM = 113,
	baseHAMmax = 138,
	armor = 0,
	resists = {15,15,15,15,15,15,15,-1,-1},
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
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = { "object/mobile/dressed_commoner_old_human_male_02.iff" },

	lootGroups = {},
	weapons = {},
	conversationTemplate = "sergeantMooreConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(sergeant_moore, "sergeant_moore")