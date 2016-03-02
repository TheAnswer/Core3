tormented_bocatt = Creature:new {
	objectName = "@mob/creature_names:bocatt_tormented",
	socialGroup = "bocatt",
	faction = "",
	level = 26,
	chanceHit = 0.36,
	damageMin = 250,
	damageMax = 260,
	baseXp = 2637,
	baseHAM = 8100,
	baseHAMmax = 9900,
	armor = 0,
	resists = {20,20,-1,-1,150,150,-1,-1,-1},
	meatType = "meat_reptilian",
	meatAmount = 100,
	hideType = "hide_leathery",
	hideAmount = 60,
	boneType = "bone_mammal",
	boneAmount = 35,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/bocatt_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/bocatt_hue.iff",
	scale = 1.1,
	lootGroups = {},
	weapons = {"creature_spit_small_yellow"},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(tormented_bocatt, "tormented_bocatt")
