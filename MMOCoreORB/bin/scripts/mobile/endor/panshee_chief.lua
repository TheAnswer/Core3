panshee_chief = Creature:new {
	objectName = "@mob/creature_names:panshee_chief",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "panshee_tribe",
	faction = "panshee_tribe",
	level = 48,
	chanceHit = 0.49,
	damageMin = 385,
	damageMax = 480,
	baseXp = 4643,
	baseHAM = 10000,
	baseHAMmax = 12200,
	armor = 1,
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
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_ewok_m_09.iff",
		"object/mobile/dressed_ewok_m_10.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 9000000},
				{group = "wearables_all", chance = 1000000},
			},
			lootChance = 1960000
		}
	},
	weapons = {"ewok_weapons"},
	conversationTemplate = "",
	attacks = merge(riflemanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(panshee_chief, "panshee_chief")
