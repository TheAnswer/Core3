gnarled_rancor = Creature:new {
	objectName = "@mob/creature_names:gnarled_rancor",
	socialGroup = "rancor",
	faction = "",
	level = 46,
	chanceHit = 0.47,
	damageMin = 395,
	damageMax = 500,
	baseXp = 4552,
	baseHAM = 9700,
	baseHAMmax = 11900,
	armor = 1,
	resists = {0,130,-1,150,150,150,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 950,
	hideType = "hide_leathery",
	hideAmount = 876,
	boneType = "bone_mammal",
	boneAmount = 775,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 10,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/rancor_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/rancor_hue.iff",
	scale = 0.9,
	lootGroups = {
		{
			groups = {
				{group = "rancor_common", chance = 4000000},
				{group = "armor_all", chance = 2000000},
				{group = "weapons_all", chance = 2500000},
				{group = "wearables_all", chance = 1500000}
			},
			lootChance = 1920000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"knockdownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(gnarled_rancor, "gnarled_rancor")
