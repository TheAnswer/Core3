haggard_pubam_battlelord = Creature:new {
	objectName = "@mob/creature_names:haggard_pubam_battlelord",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "pubam",
	faction = "pubam",
	level = 44,
	chanceHit = 0.5,
	damageMin = 420,
	damageMax = 550,
	baseXp = 4370,
	baseHAM = 11000,
	baseHAMmax = 13000,
	armor = 1,
	resists = {-1,-1,40,40,40,-1,-1,100,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dulok_male.iff",
		"object/mobile/dulok_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 10000000}
			},
			lootChance = 1880000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = brawlermaster
}

CreatureTemplates:addCreatureTemplate(haggard_pubam_battlelord, "haggard_pubam_battlelord")
