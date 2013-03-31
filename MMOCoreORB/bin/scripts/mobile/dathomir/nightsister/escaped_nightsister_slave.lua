escaped_nightsister_slave = Creature:new {
	objectName = "@mob/creature_names:escaped_nightsister_slave",
	socialGroup = "slave",
	pvpFaction = "",
	faction = "",
	level = 28,
	chanceHit = 0.36,
	damageMin = 270,
	damageMax = 280,
	baseXp = 2730,
	baseHAM = 7200,
	baseHAMmax = 8800,
	armor = 0,
	resists = {35,35,15,-1,-1,-1,-1,15,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE,
	creatureBitmask = NONE,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_dathomir_nightsister_escaped_slave.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(escaped_nightsister_slave, "escaped_nightsister_slave")