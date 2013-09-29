cor_swooper = Creature:new {
	objectName = "@mob/creature_names:swooper",
	socialGroup = "cor_swoop",
	pvpFaction = "cor_swoop",
	faction = "cor_swoop",
	level = 8,
	chanceHit = 0.270000,
	damageMin = 70,
	damageMax = 75,
	baseXp = 187,
	baseHAM = 405,
	baseHAMmax = 495,
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
	creatureBitmask = NONE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_criminal_swooper_human_female_01.iff",
		"object/mobile/dressed_criminal_swooper_human_male_01.iff",
		"object/mobile/dressed_criminal_swooper_zabrak_female_01.iff",
		"object/mobile/dressed_criminal_swooper_zabrak_male_01.iff"},
	lootGroups = {
      	{
			groups = {
				{group = "junk", chance = 5500000},
				{group = "loot_kit_parts", chance = 2500000},
				{group = "tailor_components", chance = 1500000},
				{group = "swooper_common", chance = 500000}
			},
			lootChance = 3000000
		}		
	},
	weapons = {"pirate_weapons_light"},
	attacks = merge(brawlernovice,marksmannovice)
}

CreatureTemplates:addCreatureTemplate(cor_swooper, "cor_swooper")
