pubam_cub = Creature:new {
	objectName = "@mob/creature_names:pubam_cub",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	mobType = MOB_NPC,
	socialGroup = "pubam",
	faction = "pubam",
	level = 5,
	chanceHit = 0.25,
	damageMin = 45,
	damageMax = 50,
	baseXp = 113,
	baseHAM = 135,
	baseHAMmax = 165,
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
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dulok_male.iff",
		"object/mobile/dulok_female.iff"},
	lootGroups = {
		{
			groups = {
				{group = "ewok", chance = 9000000},
				{group = "wearables_uncommon", chance = 1000000},
			},
			lootChance = 1100000
		}
	},

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "general_unarmed",
	secondaryWeapon = "none",
	conversationTemplate = "",

	-- primaryAttacks and secondaryAttacks should be separate skill groups specific to the weapon type listed in primaryWeapon and secondaryWeapon
	-- Use merge() to merge groups in creatureskills.lua together. If a weapon is set to "none", set the attacks variable to empty brackets
	primaryAttacks = brawlernovice,
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(pubam_cub, "pubam_cub")
