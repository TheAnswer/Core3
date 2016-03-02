record_keeper_imperial = Creature:new {
	objectName = "",
	customName = "Wurson Harro",
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
	optionsBitmask = INVULNERABLE + CONVERSABLE,
	diet = HERBIVORE,

	templates = {"object/mobile/theme_park_record_keeper_imperial.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "rk_record_keeper_imperialConversationTemplate",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(record_keeper_imperial, "record_keeper_imperial")