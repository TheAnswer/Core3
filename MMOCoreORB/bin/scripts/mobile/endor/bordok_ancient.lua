bordok_ancient = Creature:new {
	objectName = "@mob/creature_names:bordok_ancient",
	socialGroup = "bordok",
	faction = "",
	level = 45,
	chanceHit = 0.45,
	damageMin = 375,
	damageMax = 460,
	baseXp = 4370,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 0,
	resists = {145,30,30,170,170,-1,-1,-1,-1},
	meatType = "meat_herbivore",
	meatAmount = 250,
	hideType = "hide_leathery",
	hideAmount = 200,
	boneType = "bone_mammal",
	boneAmount = 125,
	milkType = "milk_wild",
	milk = 135,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/bordok_hue.iff"},
	scale = 1.2,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"blindattack",""},
		{"stunattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(bordok_ancient, "bordok_ancient")
