singing_mountain_clan_rancor = Creature:new {
	objectName = "@mob/creature_names:singing_mountain_clan_rancor",
	socialGroup = "mtn_clan",
	pvpFaction = "",
	faction = "mtn_clan",
	level = 55,
	chanceHit = 0.5,
	damageMin = 370,
	damageMax = 450,
	baseXp = 5281,
	baseHAM = 11000,
	baseHAMmax = 13000,
	armor = 1,
	resists = {30,60,5,100,100,100,5,5,-1},
	meatType = "meat_carnivore",
	meatAmount = 620,
	hideType = "hide_leathery",
	hideAmount = 510,
	boneType = "bone_mammal",
	boneAmount = 480,
	milk = 0,
	tamingChance = 0,
	ferocity = 10,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + STALKER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/rancor.iff"},
	lootGroups = {
		{
			groups = {
				{group = "rancor_common", chance = 5000000},
				{group = "pistols", chance = 1250000},
				{group = "rifles", chance = 1250000},
				{group = "grenades_looted", chance = 500000},
				{group = "armor_looted", chance = 1000000},
				{group = "melee_unarmed", chance = 1000000}
			},
			lootChance = 2500000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"intimidationattack","intimidationChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(singing_mountain_clan_rancor, "singing_mountain_clan_rancor")
