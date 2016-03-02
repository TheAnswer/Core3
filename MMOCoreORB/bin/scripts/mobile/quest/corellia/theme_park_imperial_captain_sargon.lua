theme_park_imperial_captain_sargon = Creature:new {
	objectName = "@mob/creature_names:crackdown_imperial_army_captain",
	socialGroup = "imperial",
	faction = "imperial",
	level = 30,
	chanceHit = 0.390000,
	damageMin = 290,
	damageMax = 300,
	baseXp = 2914,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {0,45,0,-1,40,-1,40,-1,-1},
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
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_imperial_captain_m.iff"},
	lootGroups = {
		{
			groups = {
				{group = "theme_park_loot_imperial_captain_sargon", chance = 10000000}
			},
			lootChance = 10000000
		}						
	},
	weapons = {"imperial_weapons_medium"},
	attacks = merge(brawlermaster,marksmanmaster,riflemanmaster,carbineermaster),
	conversationTemplate = "",
	optionsBitmask = AIENABLED
}

CreatureTemplates:addCreatureTemplate(theme_park_imperial_captain_sargon, "theme_park_imperial_captain_sargon")
