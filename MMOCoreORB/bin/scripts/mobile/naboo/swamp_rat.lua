swamp_rat = Creature:new {
	objectName = "@mob/creature_names:swamp_rat",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "swamp_rat",
	faction = "swamp_rat",
	level = 10,
	chanceHit = 0.27,
	damageMin = 90,
	damageMax = 110,
	baseXp = 430,
	baseHAM = 675,
	baseHAMmax = 825,
	armor = 0,
	resists = {0,0,0,0,0,0,10,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE + AGGRESSIVE + ENEMY,
	creatureBitmask = PACK + KILLER + STALKER,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_swamp_rat.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 5500000},
				{group = "rifles", chance = 1000000},
				{group = "melee_weapons", chance = 1000000},
				{group = "carbines", chance = 1000000},
				{group = "pistols", chance = 1000000},
				{group = "loot_kit_parts", chance = 500000}
			}
		}
	},
	weapons = {"pirate_weapons_medium"},
	reactionStf = "@npc_reaction/slang",
	attacks = merge(marksmannovice,brawlernovice)
}

CreatureTemplates:addCreatureTemplate(swamp_rat, "swamp_rat")
