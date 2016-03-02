savage_pugoriss = Creature:new {
	objectName = "@mob/creature_names:savage_pugoriss",
	socialGroup = "pugoriss",
	faction = "",
	level = 15,
	chanceHit = 0.3,
	damageMin = 160,
	damageMax = 170,
	baseXp = 831,
	baseHAM = 2400,
	baseHAMmax = 4000,
	armor = 0,
	resists = {0,105,0,0,0,0,0,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 350,
	hideType = "hide_leathery",
	hideAmount = 400,
	boneType = "bone_mammal",
	boneAmount = 350,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/pugoriss_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/pugoriss_hue.iff",
	scale = 1.05,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"intimidationattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(savage_pugoriss, "savage_pugoriss")
