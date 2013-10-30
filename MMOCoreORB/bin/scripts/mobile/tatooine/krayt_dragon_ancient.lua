krayt_dragon_ancient = Creature:new {
	objectName = "@mob/creature_names:krayt_dragon_ancient",
	socialGroup = "Krayt",
	pvpFaction = "",
	faction = "",
	level = 336,
	chanceHit = 30.0,
	damageMin = 2270,
	damageMax = 4250,
	baseXp = 28549,
	baseHAM = 410000,
	baseHAMmax = 501000,
	armor = 3,
	resists = {95,95,95,95,65,95,95,95,-1},
	meatType = "meat_carnivore",
	meatAmount = 1000,
	hideType = "hide_bristley",
	hideAmount = 950,
	boneType = "bone_mammal",
	boneAmount = 905,
	milk = 0,
	tamingChance = 0,
	ferocity = 30,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/krayt_dragon_hue.iff"},
	lootGroups = {
		{
	        groups = {
				{group = "krayt_tissue_rare", chance = 3300000},
				{group = "krayt_dragon_common", chance = 3300000},
				{group = "pearls_flawless", chance = 3400000}				
			},
			lootChance = 6500000
		},
		{
	        groups = {
				{group = "composite_armor_looted", chance = 2500000},
				{group = "ubese_armor_looted", chance = 2500000},
				{group = "melee_two_handed", chance = 2500000},
				{group = "rifles", chance = 2500000}				
			},
			lootChance = 3500000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareacombo",""},
		{"dizzyattack","dizzyChance=50"},
		{"blindattack","blindChance=50"},
		{"intimidationattack","intimidationChance=50"},
		{"creatureareaknockdown","knockdownChance=50"},
		{"stunattack","stunChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(krayt_dragon_ancient, "krayt_dragon_ancient")
