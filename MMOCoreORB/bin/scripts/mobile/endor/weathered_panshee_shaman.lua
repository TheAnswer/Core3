weathered_panshee_shaman = Creature:new {
	objectName = "@mob/creature_names:weathered_panshee_shaman",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "panshee_tribe",
	faction = "panshee_tribe",
	level = 27,
	chanceHit = 0.37,
	damageMin = 260,
	damageMax = 270,
	baseXp = 2730,
	baseHAM = 8100,
	baseHAMmax = 9900,
	armor = 0,
	resists = {30,30,30,30,30,30,30,30,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_ewok_m_03.iff",
		"object/mobile/dressed_ewok_m_07.iff",
		"object/mobile/dressed_ewok_m_11.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 9000000},
				{group = "wearables_uncommon", chance = 1000000},
			},
			lootChance = 1540000
		}
	},
	weapons = {"ewok_weapons"},
	conversationTemplate = "",
	attacks = merge(riflemanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(weathered_panshee_shaman, "weathered_panshee_shaman")
