nightsister_rancor = Creature:new {
	objectName = "@mob/creature_names:nightsister_rancor",
	socialGroup = "nightsister",
	pvpFaction = "",
	faction = "nightsister",
	level = 55,
	chanceHit = 0.5,
	damageMin = 395,
	damageMax = 500,
	baseXp = 5373,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 1,
	resists = {30,60,30,100,100,100,30,30,-1},
	meatType = "meat_carnivore",
	meatAmount = 950,
	hideType = "hide_leathery",
	hideAmount = 876,
	boneType = "bone_mammal",
	boneAmount = 776,
	milk = 0,
	tamingChance = 0,
	ferocity = 10,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
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
		{"creatureareaknockdown","knockdownChance=30"},
		{"posturedownattack","postureDownChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(nightsister_rancor, "nightsister_rancor")
