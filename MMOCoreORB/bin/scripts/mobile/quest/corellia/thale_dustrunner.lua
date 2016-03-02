thale_dustrunner = Creature:new {
	objectName = "",
	customName = "Thale Dustrunner (Corsec Captain)",
	socialGroup = "corsec",
	faction = "corsec",
	level = 100,
	chanceHit = 1,
	damageMin = 645,
	damageMax = 1000,
	baseXp = 9336,
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
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_criminal_smuggler_human_male_01.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "krayt_dragon_skull_mission_giver_convotemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(thale_dustrunner, "thale_dustrunner")
