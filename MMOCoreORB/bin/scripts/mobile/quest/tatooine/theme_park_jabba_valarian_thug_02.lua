theme_park_jabba_valarian_thug_02 = Creature:new {
	objectName = "@mob/creature_names:valarian_thug",
	socialGroup = "valarian",
	faction = "valarian",
	mobType = MOB_NPC,
	level = 11,
	chanceHit = 0.29,
	damageMin = 120,
	damageMax = 130,
	baseXp = 430,
	baseHAM = 1000,
	baseHAMmax = 1200,
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
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {
		"object/mobile/dressed_tatooine_valarian_thug.iff",
		"object/mobile/dressed_criminal_thug_aqualish_female_01.iff",
		"object/mobile/dressed_criminal_thug_aqualish_female_01.iff"
	},
	lootGroups = {
	    {
			groups = {
				{group = "theme_park_loot_jabba_valarian_thug_02", chance = 10000000},
			},
			lootChance = 10000000
		}
	},

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "pirate_weapons_light",
	secondaryWeapon = "unarmed",
	conversationTemplate = "",
	
	-- primaryAttacks and secondaryAttacks should be separate skill groups specific to the weapon type listed in primaryWeapon and secondaryWeapon
	-- Use merge() to merge groups in creatureskills.lua together. If a weapon is set to "none", set the attacks variable to empty brackets
	primaryAttacks = merge(brawlermid,marksmanmid),
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(theme_park_jabba_valarian_thug_02, "theme_park_jabba_valarian_thug_02")
