desert_scavenger = Creature:new {
	objectName = "@mob/creature_names:borgle_desert_runner",
	socialGroup = "borgle",
	pvpFaction = "",
	faction = "",
	level = 37,
	chanceHit = 0.41,
	damageMin = 325,
	damageMax = 360,
	baseXp = 3642,
	baseHAM = 8700,
	baseHAMmax = 10700,
	armor = 0,
	resists = {25,50,25,75,-1,95,25,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 20,
	hideType = "hide_leathery",
	hideAmount = 5,
	boneType = "bone_avian",
	boneAmount = 5,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {"object/mobile/borgle_hue.iff"},
	scale = 1.25,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"blindattack","blindChance=50"},
		{"intimidationattack","intimidationChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(desert_scavenger, "desert_scavenger")
