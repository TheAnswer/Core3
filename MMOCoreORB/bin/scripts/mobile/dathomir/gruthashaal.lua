gruthashaal = Creature:new {
	objectName = "@mob/creature_names:gruthashaal",
	socialGroup = "",
	pvpFaction = "",
	faction = "",
	level = 40,
	chanceHit = 10,
	damageMin = 240,
	damageMax = 350,
	baseXp = 3250,
	baseHAM = 4500,
	baseHAMmax = 7800,
	armor = 3,
	resists = {5,5,5,5,5,5,5,5,-1},
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
	creatureBitmask = NONE,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = { "object/mobile/dressed_commoner_naboo_human_female_08.iff"
		},

	lootGroups = {
		{
			groups = {
				{group = "task_loot_ancient_lightsaber", chance = 10000000},
				
			},
			lootChance = 10000000
		},
	},
	weapons = {},
	conversationTemplate = "singular_nak_mission_target_convotemplate",
	attacks = merge(tkamaster, brawlermaster)
}

CreatureTemplates:addCreatureTemplate(gruthashaal, "gruthashaal")
