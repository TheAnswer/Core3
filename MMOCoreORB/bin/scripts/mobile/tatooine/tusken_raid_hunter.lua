tusken_raid_hunter = Creature:new {
	objectName = "@mob/creature_names:Tusken_raid_hunter",
	socialGroup = "tusken_raider",
	pvpFaction = "tusken_raider",
	faction = "tusken_raider",
	level = 51,
	chanceHit = 0.51,
	damageMin = 400,
	damageMax = 510,
	baseXp = 5007,
	baseHAM = 10000,
	baseHAMmax = 13000,
	armor = 0,
	resists = {30,30,0,30,-1,30,0,0,-1},
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
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {"object/mobile/tusken_raider.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 2800000},
				{group = "tusken_common", chance = 3700000},
				{group = "bone_armor_looted", chance = 750000},
				{group = "chitin_armor_looted", chance = 750000},
				{group = "armor_attachments", chance = 500000},
				{group = "clothing_attachments", chance = 500000},
				{group = "color_crystals", chance = 500000},
				{group = "crystals_good", chance = 500000}
			},
			lootChance = 4500000
		}		
	},
	weapons = {"tusken_weapons"},
	conversationTemplate = "",
	attacks = merge(marksmanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(tusken_raid_hunter, "tusken_raid_hunter")
