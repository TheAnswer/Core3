hidden_daggers_lieutenant = Creature:new {
	objectName = "@mob/creature_names:hidden_daggers_lieutenant",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "hidden_daggers",
	faction = "hidden_daggers",
	level = 15,
	chanceHit = 0.31,
	damageMin = 170,
	damageMax = 180,
	baseXp = 960,
	baseHAM = 2000,
	baseHAMmax = 2400,
	armor = 0,
	resists = {5,10,5,-1,-1,-1,-1,-1,-1},
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
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_criminal_thug_human_female_01.iff",
		"object/mobile/dressed_criminal_thug_human_female_02.iff",
		"object/mobile/dressed_criminal_thug_human_male_01.iff",
		"object/mobile/dressed_criminal_thug_human_male_02.iff",
		"object/mobile/dressed_ruffian_zabrak_female_01.iff",
		"object/mobile/dressed_criminal_thug_zabrak_male_01.iff",
		"object/mobile/dressed_villain_trandoshan_female_01.iff",
		"object/mobile/dressed_villain_trandoshan_male_01.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 4300000},
				{group = "loot_kit_parts", chance = 2000000},
				{group = "tailor_components", chance = 1000000},
				{group = "hidden_dagger_common", chance = 2700000}
			}
		}
	},
	weapons = {"ranged_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/slang",
	attacks = merge(brawlermid,marksmanmid)
}

CreatureTemplates:addCreatureTemplate(hidden_daggers_lieutenant, "hidden_daggers_lieutenant")
