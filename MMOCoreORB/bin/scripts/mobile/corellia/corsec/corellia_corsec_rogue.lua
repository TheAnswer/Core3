corellia_corsec_rogue = Creature:new {
	objectName = "@mob/creature_names:corellia_corsec_rogue",
	socialGroup = "corsec",
	pvpFaction = "corsec",
	faction = "corsec",
	level = 17,
	chanceHit = 0.32,
	damageMin = 160,
	damageMax = 170,
	baseXp = 1102,
	baseHAM = 3500,
	baseHAMmax = 4300,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
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

	templates = {"object/mobile/dressed_mercenary_destroyer_hum_f.iff"
		},
	lootGroups = {
	    {
			groups = {
				{group = "junk", chance = 5500000},
				{group = "corsec_weapons", chance = 4000000},
				{group = "tailor_components", chance = 500000}
			},
			lootChance = 6500000
		}
	},
	weapons = {"ranged_weapons"},
	conversationTemplate = "",
	attacks = merge(brawlernovice,marksmannovice)
}

CreatureTemplates:addCreatureTemplate(corellia_corsec_rogue, "corellia_corsec_rogue")
