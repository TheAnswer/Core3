herald_naboo_ketter = Creature:new {
	objectName = "",
	customName = "Ketter Yaaran",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 100,
	chanceHit = 1,
	damageMin = 645,
	damageMax = 1000,
	baseXp = 9429,
	baseHAM = 24000,
	baseHAMmax = 30000,
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
	pvpBitmask = NONE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_herald_pirate_naboo_human_male.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "heraldConvoTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(herald_naboo_ketter, "herald_naboo_ketter")
