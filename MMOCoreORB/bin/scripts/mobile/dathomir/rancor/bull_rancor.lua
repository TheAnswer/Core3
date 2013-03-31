bull_rancor = Creature:new {
	objectName = "@mob/creature_names:bull_rancor",
	socialGroup = "rancor",
	pvpFaction = "",
	faction = "",
	level = 65,
	chanceHit = 0.6,
	damageMin = 455,
	damageMax = 620,
	baseXp = 6288,
	baseHAM = 11000,
	baseHAMmax = 14000,
	armor = 1,
	resists = {35,60,15,80,80,80,15,15,-1},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_leathery",
	hideAmount = 900,
	boneType = "bone_mammal",
	boneAmount = 850,
	milk = 0,
	tamingChance = 0.01,
	ferocity = 11,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/bull_rancor.iff"},
	lootGroups = {
		{
			groups = {
				{group = "rancor_common", chance = 2800000},
				{group = "pistols", chance = 2800000},
				{group = "rifles", chance = 2800000},
				{group = "unarmed_melee", chance = 1600000}
			},
			lootChance = 3950000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareadisease",""},
		{"dizzyattack","dizzyChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(bull_rancor, "bull_rancor")