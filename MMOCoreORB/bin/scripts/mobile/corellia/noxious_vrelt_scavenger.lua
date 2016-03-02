noxious_vrelt_scavenger = Creature:new {
	objectName = "@mob/creature_names:noxious_vrelt_scavenger",
	socialGroup = "vrelt",
	faction = "",
	level = 10,
	chanceHit = 0.28,
	damageMin = 90,
	damageMax = 110,
	baseXp = 356,
	baseHAM = 810,
	baseHAMmax = 990,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 10,
	hideType = "hide_bristley",
	hideAmount = 10,
	boneType = "bone_mammal",
	boneAmount = 10,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + HERD + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/noxious_vrelt_scavenger.iff"},
	controlDeviceTemplate = "object/intangible/pet/stintaril_hue.iff",
	scale = 1.15,
	lootGroups = {},
	weapons = {"creature_spit_small_toxicgreen"},
	conversationTemplate = "",
	attacks = {
		{"stunattack",""},
		{"milddisease",""}
	}
}

CreatureTemplates:addCreatureTemplate(noxious_vrelt_scavenger, "noxious_vrelt_scavenger")
