domestic_krahbu = Creature:new {
	objectName = "@mob/creature_names:domestic_krahbu",
	socialGroup = "self",
	faction = "",
	level = 7,
	chanceHit = 0.26,
	damageMin = 50,
	damageMax = 55,
	baseXp = 147,
	baseHAM = 405,
	baseHAMmax = 495,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "meat_domesticated",
	meatAmount = 230,
	hideType = "hide_bristley",
	hideAmount = 160,
	boneType = "bone_mammal",
	boneAmount = 145,
	milkType = "milk_domesticated",
	milk = 110,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/krahbu_hue.iff"},
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(domestic_krahbu, "domestic_krahbu")
