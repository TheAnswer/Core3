fbase_storm_commando = Creature:new {
	objectName = "@mob/creature_names:fbase_storm_commando",
	randomNameType = NAME_STORMTROOPER_TAG,
	socialGroup = "imperial",
	faction = "imperial",
	level = 44,
	chanceHit = 0.47,
	damageMin = 365,
	damageMax = 440,
	baseXp = 4400,
	baseHAM = 9700,
	baseHAMmax = 11900,
	armor = 0,
	resists = {20,20,20,130,-1,130,-1,-1,-1},
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
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = 128,
	diet = HERBIVORE,
	scale = 1.05,

	templates = {"object/mobile/dressed_stormtrooper_commando_m.iff"},
	lootGroups = {
		{
			groups = {
				{group = "color_crystals", chance = 100000},
				{group = "junk", chance = 4700000},
				{group = "rifles", chance = 1000000},
				{group = "pistols", chance = 1000000},
				{group = "melee_weapons", chance = 1000000},
				{group = "carbines", chance = 1000000},
				{group = "clothing_attachments", chance = 100000},
				{group = "armor_attachments", chance = 100000},
				{group = "wearables_common", chance = 1000000}
			}
		}
	},
	weapons = {"st_bombardier_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/stormtrooper",
	personalityStf = "@hireling/hireling_stormtrooper",	
	attacks = merge(commandonovice,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(fbase_storm_commando, "fbase_storm_commando")
