grovo = Creature:new {
	objectName = "@monster_name:grovo",
	socialGroup = "nightsister",
	pvpFaction = "nightsister",
	faction = "nightsister",
	level = 75,
	chanceHit = 0.7,
	damageMin = 520,
	damageMax = 750,
	baseXp = 7207,
	baseHAM = 12000,
	baseHAMmax = 15000,
	armor = 1,
	resists = {25,60,25,100,100,100,25,25,-1},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_leathery",
	hideAmount = 1000,
	boneType = "bone_mammal",
	boneAmount = 950,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/nsister_rancor_grovo.iff"},
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
			lootChance = 4000000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareableeding",""},
		{"creatureareacombo",""}
	}
}

CreatureTemplates:addCreatureTemplate(grovo, "grovo")
