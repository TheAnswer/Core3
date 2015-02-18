foot_of_solitude = Creature:new {
	objectName = "@mob/creature_names:foot_of_solitude",
	socialGroup = "solitude",
	faction = "rebel",
	level = 9,
	chanceHit = 0.270000,
	damageMin = 80,
	damageMax = 90,
	baseXp = 292,
	baseHAM = 675,
	baseHAMmax = 825,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK,
	diet = HERBIVORE,

	templates = {"object/mobile/human_male.iff"},
	lootGroups = {},
	weapons = {"rebel_weapons_medium"},
	reactionStf = "@npc_reaction/townperson",
	attacks = merge(brawlernovice,marksmannovice)
}

CreatureTemplates:addCreatureTemplate(foot_of_solitude, "foot_of_solitude")
