male_plains_tusk_cat = Creature:new {
	objectName = "@mob/creature_names:male_great_grass_plains_tusk_cat",
	socialGroup = "tusk_cat",
	faction = "",
	level = 16,
	chanceHit = 0.31,
	damageMin = 170,
	damageMax = 180,
	baseXp = 960,
	baseHAM = 2000,
	baseHAMmax = 2400,
	armor = 0,
	resists = {5,5,5,135,135,-1,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 75,
	hideType = "hide_bristley",
	hideAmount = 45,
	boneType = "bone_mammal",
	boneAmount = 40,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 9,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = KILLER + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/tusk_cat_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/tusk_cat_hue.iff",
	scale = 1.05,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"",""},
		{"blindattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(male_plains_tusk_cat, "male_plains_tusk_cat")
