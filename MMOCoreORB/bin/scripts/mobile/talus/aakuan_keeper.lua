aakuan_keeper = Creature:new {
	objectName = "@mob/creature_names:aakuan_keeper",
	randomNameType = NAME_GENERIC_TAG,
	socialGroup = "aakuans",
	faction = "aakuans",
	level = 13,
	chanceHit = 0.3,
	damageMin = 140,
	damageMax = 150,
	baseXp = 714,
	baseHAM = 2000,
	baseHAMmax = 2400,
	armor = 0,
	resists = {0,0,0,0,0,0,0,-1,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_aakuan_keeper_zabrak_female_01.iff",
		"object/mobile/dressed_aakuan_keeper_zabrak_male_01.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 6600000},
				{group = "aakuan_common", chance = 2300000},
				{group = "armor_attachments", chance = 200000},
				{group = "rifles", chance = 300000},
				{group = "carbines", chance = 300000},
				{group = "pistols", chance = 300000}
			}
		}
	},
	weapons = {"melee_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/fancy",
	attacks = brawlermid
}

CreatureTemplates:addCreatureTemplate(aakuan_keeper, "aakuan_keeper")
