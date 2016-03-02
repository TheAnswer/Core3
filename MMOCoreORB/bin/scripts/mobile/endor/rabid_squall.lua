rabid_squall = Creature:new {
	objectName = "@mob/creature_names:rabid_squall",
	socialGroup = "self",
	faction = "",
	level = 9,
	chanceHit = 0.27,
	damageMin = 80,
	damageMax = 90,
	baseXp = 292,
	baseHAM = 675,
	baseHAMmax = 825,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 8,
	hideType = "hide_bristley",
	hideAmount = 12,
	boneType = "bone_mammal",
	boneAmount = 8,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/squall_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/squall_hue.iff",
	scale = 1.15,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"",""},
		{"milddisease",""}
	}
}

CreatureTemplates:addCreatureTemplate(rabid_squall, "rabid_squall")
