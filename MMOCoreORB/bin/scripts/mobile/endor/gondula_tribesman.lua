gondula_tribesman = Creature:new {
	objectName = "@mob/creature_names:gondula_tribesman",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "gondula_tribe",
	faction = "gondula_tribe",
	level = 11,
	chanceHit = 0.28,
	damageMin = 90,
	damageMax = 110,
	baseXp = 430,
	baseHAM = 810,
	baseHAMmax = 990,
	armor = 0,
	resists = {5,5,0,15,15,0,0,-1,-1},
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
		"object/mobile/dressed_ewok_f_01.iff",
		"object/mobile/dressed_ewok_f_07.iff",
		"object/mobile/dressed_ewok_f_08.iff",
		"object/mobile/dressed_ewok_f_09.iff",
		"object/mobile/dressed_ewok_m_01.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 10000000}
			},
			lootChance = 1220000
		}
	},
	weapons = {"ewok_weapons"},
	conversationTemplate = "",
	attacks = merge(marksmanmid,brawlermid)
}

CreatureTemplates:addCreatureTemplate(gondula_tribesman, "gondula_tribesman")
