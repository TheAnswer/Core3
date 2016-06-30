diax = Creature:new {
	objectName = "@mob/creature_names:diax",
	customName = "Diax",
	socialGroup = "nightsister",
	faction = "nightsister",
	level = 22,
	chanceHit = 0.33,
	damageMin = 230,
	damageMax = 240,
	baseXp = 1803,
	baseHAM = 4100,
	baseHAMmax = 5000,
	armor = 0,
	resists = {0,0,0,40,40,40,40,-1,-1},
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
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_diax.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "theme_park_nightsister_mission_giver_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(diax, "diax")
