eopie = Creature:new {
	objectName = "@mob/creature_names:eopie",
	socialGroup = "eopie",
	faction = "",
	level = 7,
	chanceHit = 0.26,
	damageMin = 45,
	damageMax = 50,
	baseXp = 147,
	baseHAM = 405,
	baseHAMmax = 495,
	armor = 0,
	resists = {0,0,0,120,120,0,0,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 315,
	hideType = "hide_leathery",
	hideAmount = 250,
	boneType = "bone_mammal",
	boneAmount = 150,
	milkType = "milk_wild",
	milk = 150,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/eopie.iff"},
	controlDeviceTemplate = "object/intangible/pet/eopie_hue.iff",
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(eopie, "eopie")
