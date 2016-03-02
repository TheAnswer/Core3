theme_park_jabba_meatbeast = Creature:new {
	objectName = "@mob/creature_names:dewback_cannibal",
	socialGroup = "dewback",
	faction = "",
	level = 21,
	chanceHit = 0.33,
	damageMin = 230,
	damageMax = 240,
	baseXp = 1609,
	baseHAM = 5900,
	baseHAMmax = 7200,
	armor = 0,
	resists = {110,110,110,-1,110,-1,110,-1,-1},
	meatType = "meat_reptilian",
	meatAmount = 340,
	hideType = "hide_leathery",
	hideAmount = 260,
	boneType = "bone_mammal",
	boneAmount = 185,
	milk = 0,
	tamingChance = 0,
	ferocity = 6,
	pvpBitmask = ATTACKABLE,
	creatureBitmask = STALKER,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/dewback_hue.iff"},
	scale = 1.2,
	lootGroups = {
	    {
			groups = {
				{group = "theme_park_loot_chicken_leg", chance = 10000000},
			},
			lootChance = 10000000
		}			
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"",""},
		{"dizzyattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(theme_park_jabba_meatbeast, "theme_park_jabba_meatbeast")
