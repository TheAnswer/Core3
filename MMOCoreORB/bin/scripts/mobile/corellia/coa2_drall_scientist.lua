coa2_drall_scientist = Creature:new {
	objectName = "",
	customName = "a Drall Scientist",
	socialGroup = "drall",
	pvpFaction = "rebel",
	faction = "rebel",
	level = 24,
	chanceHit = 0.35,
	damageMin = 230,
	damageMax = 240,
	baseXp = 2443,
	baseHAM = 6800,
	baseHAMmax = 8300,
	armor = 0,
	resists = {0,0,0,0,0,0,0,0,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {"object/mobile/drall_male.iff"},
	lootGroups = {},
	weapons = {"pirate_weapons_heavy"},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(coa2_drall_scientist, "coa2_drall_scientist")