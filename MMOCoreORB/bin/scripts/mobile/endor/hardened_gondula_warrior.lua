hardened_gondula_warrior = Creature:new {
	objectName = "@mob/creature_names:hardened_gondula_warrior",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "gondula_tribe",
	faction = "gondula_tribe",
	level = 53,
	chanceHit = 0.53,
	damageMin = 410,
	damageMax = 530,
	baseXp = 5190,
	baseHAM = 10000,
	baseHAMmax = 13000,
	armor = 1,
	resists = {30,30,5,5,5,5,-1,5,-1},
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
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_ewok_m_06.iff",
		"object/mobile/dressed_ewok_m_09.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 10000000}
			},
			lootChance = 2060000
		}
	},
	weapons = {"ewok_weapons"},
	conversationTemplate = "",
	attacks = merge(riflemanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(hardened_gondula_warrior, "hardened_gondula_warrior")
