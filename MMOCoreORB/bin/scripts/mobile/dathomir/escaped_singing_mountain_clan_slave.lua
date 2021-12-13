escaped_singing_mountain_clan_slave = Creature:new {
	objectName = "@mob/creature_names:escaped_singing_mtn_clan_slave",
	randomNameType = NAME_GENERIC,
	randomNameTag = true,
	mobType = MOB_NPC,
	socialGroup = "slave",
	faction = "",
	level = 26,
	chanceHit = 0.36,
	damageMin = 260,
	damageMax = 270,
	baseXp = 2543,
	baseHAM = 7200,
	baseHAMmax = 8800,
	armor = 0,
	resists = {25,20,0,-1,0,-1,-1,0,-1},
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
	creatureBitmask = NONE,
	optionsBitmask = AIENABLED,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_dathomir_sing_mt_clan_escaped_slave.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 4500000},
				{group = "tailor_components", chance = 2500000},
				{group = "loot_kit_parts", chance = 1500000},
				{group = "wearables_common", chance = 1500000}
			}
		}
	},

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "primitive_weapons",
	secondaryWeapon = "unarmed",
	conversationTemplate = "",
	
	-- primaryAttacks and secondaryAttacks should be separate skill groups specific to the weapon type listed in primaryWeapon and secondaryWeapon
	-- Use merge() to merge groups in creatureskills.lua together. If a weapon is set to "none", set the attacks variable to empty brackets
	primaryAttacks = merge(pikemanmaster,fencermaster,brawlermaster),
	secondaryAttacks = { }
}

CreatureTemplates:addCreatureTemplate(escaped_singing_mountain_clan_slave, "escaped_singing_mountain_clan_slave")
