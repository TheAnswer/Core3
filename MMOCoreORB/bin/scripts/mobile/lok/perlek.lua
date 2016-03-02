perlek = Creature:new {
	objectName = "@mob/creature_names:perlek",
	socialGroup = "perlek",
	faction = "",
	level = 14,
	chanceHit = 0.3,
	damageMin = 140,
	damageMax = 150,
	baseXp = 714,
	baseHAM = 1500,
	baseHAMmax = 1900,
	armor = 0,
	resists = {105,125,0,0,0,0,0,-1,-1},
	meatType = "meat_avian",
	meatAmount = 250,
	hideType = "hide_leathery",
	hideAmount = 100,
	boneType = "bone_avian",
	boneAmount = 175,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/perlek.iff"},
	controlDeviceTemplate = "object/intangible/pet/perlek_hue.iff",
	lootGroups = {},
	weapons = {"creature_spit_small_yellow"},
	conversationTemplate = "",
	attacks = {
		{"intimidationattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(perlek, "perlek")
