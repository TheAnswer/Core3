giant_decay_mite_soldier = Creature:new {
	objectName = "@mob/creature_names:giant_decay_mite_soldier",
	socialGroup = "mite",
	faction = "",
	level = 17,
	chanceHit = 0.31,
	damageMin = 160,
	damageMax = 170,
	baseXp = 1102,
	baseHAM = 3500,
	baseHAMmax = 4300,
	armor = 0,
	resists = {120,120,0,0,0,-1,0,-1,-1},
	meatType = "meat_insect",
	meatAmount = 18,
	hideType = "hide_scaley",
	hideAmount = 16,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/decay_mite.iff"},
	scale = 1.5,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(giant_decay_mite_soldier, "giant_decay_mite_soldier")
