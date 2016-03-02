dark_initiate = Creature:new {
	objectName = "",
	customName = "Dark Initiate",
	socialGroup = "imperial",
	faction = "imperial",
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

	templates = {"object/mobile/dressed_feral_force_wielder.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "luthik_uwyr_mission_target_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(dark_initiate, "dark_initiate")
