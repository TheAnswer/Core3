theme_park_rebel_iris = Creature:new {
	objectName = "",
	socialGroup = "",
	faction = "",
	level = 10,
	chanceHit = 0.280000,
	damageMin = 90,
	damageMax = 110,
	baseXp = 356,
	baseHAM = 810,
	baseHAMmax = 990,
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
	pvpBitmask = ATTACKABLE,
	creatureBitmask = KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_brigade_captain_human_female_01.iff"
	},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 6400000},
				{group = "loot_kit_parts", chance = 2000000},
				{group = "color_crystals", chance = 100000},
				{group = "tailor_components", chance = 1500000}
			}
		}
	},
	weapons = {"pirate_weapons_light"},
	attacks = merge(brawlernovice,marksmannovice)
}

CreatureTemplates:addCreatureTemplate(theme_park_rebel_iris, "theme_park_rebel_iris")
