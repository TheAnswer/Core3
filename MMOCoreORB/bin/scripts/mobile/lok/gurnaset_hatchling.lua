gurnaset_hatchling = Creature:new {
	objectName = "@mob/creature_names:gurnaset_hatchling",
	socialGroup = "gurnaset",
	faction = "",
	level = 5,
	chanceHit = 0.25,
	damageMin = 45,
	damageMax = 50,
	baseXp = 85,
	baseHAM = 135,
	baseHAMmax = 165,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 351,
	hideType = "hide_leathery",
	hideAmount = 278,
	boneType = "bone_mammal",
	boneAmount = 303,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = HERD,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/gurnaset_hatchling.iff"},
	scale = 0.65,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
	}
}

CreatureTemplates:addCreatureTemplate(gurnaset_hatchling, "gurnaset_hatchling")
