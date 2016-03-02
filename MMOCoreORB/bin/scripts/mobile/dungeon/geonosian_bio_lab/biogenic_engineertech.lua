biogenic_engineertech = Creature:new {
	objectName = "@mob/creature_names:geonosian_scientist",
	socialGroup = "self",
	faction = "",
	level = 25,
	chanceHit = 0.35,
	damageMin = 120,
	damageMax = 230,
	baseXp = 4300,
	baseHAM = 6500,
	baseHAMmax = 7400,
	armor = 0,
	resists = {-1,-1,-1,-1,-1,-1,-1,-1,-1},
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
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED + INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_geonosian_scientist_01.iff",
		"object/mobile/dressed_geonosian_scientist_02.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "biogenicEngineerTechConvoTemplate",
	attacks = {}
}

CreatureTemplates:addCreatureTemplate(biogenic_engineertech, "biogenic_engineertech")
