boar_wolf = Creature:new {
	objectName = "@mob/creature_names:boar_wolf",
	socialGroup = "boar",
	pvpFaction = "",
	faction = "",
	level = 25,
	chanceHit = 0.36,
	damageMin = 240,
	damageMax = 250,
	baseXp = 2543,
	baseHAM = 8100,
	baseHAMmax = 9900,
	armor = 0,
	resists = {50,-1,30,-1,30,30,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 65,
	hideType = "hide_leathery",
	hideAmount = 40,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/boar_wolf.iff"},
	controlDeviceTemplate = "object/intangible/pet/boar_wolf_hue.iff",
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"stunattack","stunChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(boar_wolf, "boar_wolf")
