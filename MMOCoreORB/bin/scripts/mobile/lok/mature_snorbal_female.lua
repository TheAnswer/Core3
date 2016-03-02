mature_snorbal_female = Creature:new {
	objectName = "@mob/creature_names:mature_snorbal_female",
	socialGroup = "snorbal",
	faction = "",
	level = 31,
	chanceHit = 0.39,
	damageMin = 290,
	damageMax = 300,
	baseXp = 3097,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {140,140,20,-1,-1,-1,-1,20,-1},
	meatType = "meat_herbivore",
	meatAmount = 545,
	hideType = "hide_leathery",
	hideAmount = 440,
	boneType = "bone_mammal",
	boneAmount = 400,
	milkType = "milk_wild",
	milk = 280,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + HERD,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/elder_snorbal_female.iff"},
	scale = 1.05,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack",""},
		{"knockdownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(mature_snorbal_female, "mature_snorbal_female")
