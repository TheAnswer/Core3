bewitching_pubam_dark_shaman = Creature:new {
	objectName = "@mob/creature_names:bewitching_pubam_dark_shaman",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "pubam",
	faction = "pubam",
	level = 30,
	chanceHit = 0.39,
	damageMin = 310,
	damageMax = 330,
	baseXp = 3097,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {0,0,0,40,40,-1,-1,-1,-1},
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
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dulok_male.iff"},
	lootGroups = {
		 {
	        groups = {
				{group = "ewok", chance = 10000000}
			},
			lootChance = 1600000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = brawlermaster
}

CreatureTemplates:addCreatureTemplate(bewitching_pubam_dark_shaman, "bewitching_pubam_dark_shaman")
