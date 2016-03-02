sandreaver = Creature:new {
	objectName = "@mob/creature_names:dewback_sandreaver",
	socialGroup = "dewback",
	faction = "",
	level = 26,
	chanceHit = 0.36,
	damageMin = 290,
	damageMax = 300,
	baseXp = 2637,
	baseHAM = 8200,
	baseHAMmax = 10000,
	armor = 0,
	resists = {10,10,10,-1,10,-1,10,-1,-1},
	meatType = "meat_reptilian",
	meatAmount = 340,
	hideType = "hide_leathery",
	hideAmount = 260,
	boneType = "bone_mammal",
	boneAmount = 185,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/dewback.iff"},
	controlDeviceTemplate = "object/intangible/pet/dewback_hue.iff",
	scale = 1.25,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"dizzyattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(sandreaver, "sandreaver")
