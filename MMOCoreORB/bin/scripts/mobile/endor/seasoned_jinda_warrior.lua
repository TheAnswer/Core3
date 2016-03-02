seasoned_jinda_warrior = Creature:new {
	objectName = "@mob/creature_names:seasoned_jinda_warrior",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	socialGroup = "jinda_tribe",
	faction = "",
	level = 35,
	chanceHit = 0.41,
	damageMin = 320,
	damageMax = 350,
	baseXp = 3460,
	baseHAM = 8800,
	baseHAMmax = 10800,
	armor = 0,
	resists = {-1,50,-1,50,50,70,50,-1,-1},
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
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/jinda_male.iff",
		"object/mobile/jinda_female.iff",
		"object/mobile/jinda_male_01.iff",
		"object/mobile/jinda_female_01.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 8100000},
				{group = "wearables_uncommon", chance = 1000000},
				{group = "armor_attachments", chance = 450000},
				{group = "clothing_attachments", chance = 450000}
			},
			lootChance = 1700000
		}
	},
	weapons = {"ewok_weapons"},
	conversationTemplate = "",
	attacks = merge(riflemanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(seasoned_jinda_warrior, "seasoned_jinda_warrior")
