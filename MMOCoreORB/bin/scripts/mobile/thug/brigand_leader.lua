brigand_leader = Creature:new {
	objectName = "@mob/creature_names:brigand_leader",
	socialGroup = "brigand",
	pvpFaction = "",
	faction = "",
	level = 20,
	chanceHit = 0.330000,
	damageMin = 180,
	damageMax = 190,
	baseXp = 1803,
	baseHAM = 4500,
	baseHAMmax = 5500,
	armor = 0,
	resists = {10,20,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = ATTACKABLE + AGGRESSIVE,
	creatureBitmask = PACK + KILLER,
	diet = HERBIVORE,

	templates = 
		{
			"object/mobile/shared_dressed_brigade_captain_human_female_01.iff",
			"object/mobile/shared_dressed_brigade_captain_human_male_01.iff",
			"object/mobile/shared_dressed_brigade_general_human_female_01.iff",
			"object/mobile/shared_dressed_brigade_general_human_male_01.iff",
			"object/mobile/shared_dressed_brigade_officer_rodian_female_01.iff",
			"object/mobile/shared_dressed_brigade_officer_rodian_male_01.iff",
			"object/mobile/shared_dressed_brigade_sergeant_zabrak_female_01.iff",
			"object/mobile/shared_dressed_brigade_sergeant_zabrak_male_01.iff"
		},
	lootGroups = {},
	weapons = {"pirate_weapons_heavy"},
	attacks = merge(riflemanmaster,pistoleermaster,carbineermaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(brigand_leader, "brigand_leader")