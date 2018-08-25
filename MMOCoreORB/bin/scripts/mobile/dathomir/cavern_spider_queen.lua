cavern_spider_queen = Creature:new {
	objectName = "@mob/creature_names:gaping_spider_queen",
	socialGroup = "spider_nightsister",
	faction = "spider_nightsister",
	level = 46,
	chanceHit = 0.44,
	damageMin = 345,
	damageMax = 400,
	baseXp = 4552,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 0,
	resists = {160,160,140,125,30,-1,30,30,-1},
	meatType = "meat_insect",
	meatAmount = 40,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 9,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/gaping_spider_queen.iff"},
	hues = { 0, 1, 2, 3, 4, 5, 6, 7 },
	scale = 1.3,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"knockdownattack",""},
		{"mediumpoison",""}
	}
}

CreatureTemplates:addCreatureTemplate(cavern_spider_queen, "cavern_spider_queen")
