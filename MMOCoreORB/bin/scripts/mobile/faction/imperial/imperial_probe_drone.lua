imperial_probe_drone = Creature:new {
	objectName = "@mob/creature_names:imperial_probot_drone",
	socialGroup = "imperial",
	faction = "imperial",
	mobType = MOB_DROID,
	level = 14,
	chanceHit = 0.31,
	damageMin = 150,
	damageMax = 160,
	baseXp = 710,
	baseHAM = 1440,
	baseHAMmax = 1680,
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
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/probot.iff"},
	lootGroups = {},
	conversationTemplate = "",

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "object/weapon/ranged/droid/droid_probot_ranged.iff",
	secondaryWeapon = "object/weapon/ranged/droid/droid_probot_ranged.iff",
	defaultAttack = "attack",

}

CreatureTemplates:addCreatureTemplate(imperial_probe_drone, "imperial_probe_drone")
