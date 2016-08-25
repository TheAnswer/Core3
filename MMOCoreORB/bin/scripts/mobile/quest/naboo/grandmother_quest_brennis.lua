grandmother_quest_brennis = Creature:new {
	objectName = "@mob/creature_names:noble",
	socialGroup = "townsperson",
	faction = "townsperson",
	level = 6,
	chanceHit = 0.250000,
	damageMin = 50,
	damageMax = 55,
	baseXp = 113,
	baseHAM = 180,
	baseHAMmax = 220,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED + CONVERSABLE,
	diet = HERBIVORE,


	templates = {
		"object/mobile/dressed_noble_old_human_female_01.iff",
		"object/mobile/dressed_noble_old_human_female_02.iff",
		"object/mobile/dressed_noble_old_zabrak_female_01.iff",
		"object/mobile/dressed_noble_old_zabrak_female_02.iff"
	},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "brennis_doore_mission_target_convotemplate",
	attacks = {}
}

CreatureTemplates:addCreatureTemplate(grandmother_quest_brennis, "grandmother_quest_brennis")
