angler_hatchling = Creature:new {
	objectName = "@mob/creature_names:angler_hatchling",
	socialGroup = "angler",
	faction = "",
	level = 8,
	chanceHit = 0.27,
	damageMin = 70,
	damageMax = 75,
	baseXp = 235,
	baseHAM = 405,
	baseHAMmax = 495,
	armor = 0,
	resists = {0,0,0,-1,0,0,0,-1,-1},
	meatType = "meat_insect",
	meatAmount = 2,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/angler_hatchling.iff"},
	scale = 0.75,
	lootGroups = {},
	weapons = {"creature_spit_small_green", "creature_spit_small_green"},
	conversationTemplate = "",
	attacks = {
		{"mildpoison",""},
		{"posturedownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(angler_hatchling, "angler_hatchling")
