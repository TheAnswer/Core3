nightsister_sentinel = Creature:new {
	objectName = "@mob/creature_names:nightsister_sentinal",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "nightsister",
	faction = "nightsister",
	level = 88,
	chanceHit = 0.8,
	damageMin = 545,
	damageMax = 800,
	baseXp = 8408,
	baseHAM = 21000,
	baseHAMmax = 26000,
	armor = 1,
	resists = {35,35,35,200,200,200,200,200,-1},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_dathomir_nightsister_sentinal.iff"},
	lootGroups = {
		{
			groups = {
				{group = "power_crystals", chance = 500000},
				{group = "color_crystals", chance = 500000},
				{group = "nightsister_common", chance = 2000000},
				{group = "armor_attachments", chance = 250000},
				{group = "clothing_attachments", chance = 250000},
				{group = "melee_weapons", chance = 2500000},
				{group = "rifles", chance = 1000000},
				{group = "pistols", chance = 1000000},
				{group = "carbines", chance = 1000000},
				{group = "wearables_common", chance = 500000},
				{group = "tailor_components", chance = 500000}
			}
		}
	},
	weapons = {"mixed_force_weapons"},
	conversationTemplate = "",
	attacks = merge(fencermid,swordsmanmid,tkamid,pikemanmaster,brawlermaster,forcewielder)
}

CreatureTemplates:addCreatureTemplate(nightsister_sentinel, "nightsister_sentinel")
