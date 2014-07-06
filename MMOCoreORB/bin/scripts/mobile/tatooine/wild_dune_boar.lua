wild_dune_boar = Creature:new {
	objectName = "@mob/creature_names:wild_dune_boar",
	socialGroup = "boar",
	pvpFaction = "",
	faction = "",
	level = 50,
	chanceHit = 0.5,
	damageMin = 395,
	damageMax = 500,
	baseXp = 4916,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 1,
	resists = {10,40,10,0,0,0,0,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 90,
	hideType = "hide_leathery",
	hideAmount = 80,
	boneType = "bone_mammal",
	boneAmount = 75,
	milk = 0,
	tamingChance = 0.15,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 0,
	diet = CARNIVORE,

	templates = {"object/mobile/zucca_boar.iff"},
	controlDeviceTemplate = "object/intangible/pet/zucca_boar_hue.iff",
	scale = 1.3,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"intimidationattack","intimidationChance=50"},
		{"stunattack","stunChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(wild_dune_boar, "wild_dune_boar")
