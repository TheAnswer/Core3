scientist_target_robber = Creature:new {
	objectName = "@mob/creature_names:scavenger",
	socialGroup = "thug",
	faction = "thug",
	mobType = MOB_NPC,
	level = 7,
	chanceHit = 0.260000,
	damageMin = 55,
	damageMax = 65,
	baseXp = 147,
	baseHAM = 270,
	baseHAMmax = 330,
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
	creatureBitmask = NONE,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_tatooine_scavenger.iff",
			"object/mobile/dressed_mercenary_commander_zab_m.iff",
			"object/mobile/human_male.iff",
			"object/mobile/dressed_criminal_thug_bothan_female_01.iff",
			"object/mobile/dressed_goon_twk_male_01.iff",
			"object/mobile/dressed_robber_twk_female_01.iff",
			"object/mobile/dressed_goon_twk_female_01.iff",
			"object/mobile/dressed_robber_human_female_01.iff",
			"object/mobile/dressed_villain_trandoshan_male_01.iff"
			},
	lootGroups = {
		{
			groups = {
				{group = "task_loot_rare_artifact", chance = 10000000}
			},
			lootChance = 10000000
		}
	},

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "pirate_weapons_medium",
	secondaryWeapon = "unarmed",
	
	-- primaryAttacks and secondaryAttacks should be separate skill groups specific to the weapon type listed in primaryWeapon and secondaryWeapon
	-- Use merge() to merge groups in creatureskills.lua together. If a weapon is set to "none", set the attacks variable to empty brackets
	primaryAttacks = merge(marksmannovice,brawlernovice),
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(scientist_target_robber, "scientist_target_robber")
