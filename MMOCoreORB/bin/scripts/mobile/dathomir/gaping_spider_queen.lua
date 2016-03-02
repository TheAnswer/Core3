gaping_spider_queen = Creature:new {
	objectName = "@mob/creature_names:gaping_spider_queen",
	socialGroup = "spider",
	faction = "",
	level = 47,
	chanceHit = 0.44,
	damageMin = 345,
	damageMax = 400,
	baseXp = 4643,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 0,
	resists = {165,165,140,125,30,-1,30,30,-1},
	meatType = "meat_insect",
	meatAmount = 12,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 9,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/gaping_spider_queen.iff"},
	controlDeviceTemplate = "object/intangible/pet/gaping_spider_hue.iff",
	scale = 1.15,
	lootGroups = {},
	weapons = {"creature_spit_small_toxicgreen"},
	conversationTemplate = "",
	attacks = {
		{"knockdownattack",""},
		{"mediumpoison",""}
	}
}

CreatureTemplates:addCreatureTemplate(gaping_spider_queen, "gaping_spider_queen")
