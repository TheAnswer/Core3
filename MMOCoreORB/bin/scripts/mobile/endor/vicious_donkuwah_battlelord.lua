vicious_donkuwah_battlelord = Creature:new {
	objectName = "@mob/creature_names:vicious_donkuwah_battlelord",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "donkuwah_tribe",
	faction = "donkuwah_tribe",
	level = 51,
	chanceHit = 0.51,
	damageMin = 425,
	damageMax = 560,
	baseXp = 5007,
	baseHAM = 10000,
	baseHAMmax = 12200,
	armor = 0,
	resists = {35,0,0,50,50,-1,0,-1,-1},
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
		"object/mobile/dulok_male.iff",
		"object/mobile/dulok_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "donkuwah_common", chance = 9000000},
				{group = "wearables_all", chance = 1000000},
			},
			lootChance = 2020000
		}
	},
	weapons = {"donkuwah_weapons"},
	conversationTemplate = "",
	attacks = merge(fencermaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(vicious_donkuwah_battlelord, "vicious_donkuwah_battlelord")
