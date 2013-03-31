rogue_fambaa = Creature:new {
	objectName = "@mob/creature_names:rogue_fambaa",
	socialGroup = "self",
	pvpFaction = "",
	faction = "",
	level = 37,
	chanceHit = 0.42,
	damageMin = 330,
	damageMax = 370,
	baseXp = 3551,
	baseHAM = 9000,
	baseHAMmax = 11000,
	armor = 0,
	resists = {25,25,25,25,60,60,-1,25,-1},
	meatType = "meat_reptilian",
	meatAmount = 1000,
	hideType = "hide_leathery",
	hideAmount = 700,
	boneType = "bone_mammal",
	boneAmount = 675,
	milkType = "milk_wild",
	milk = 400,
	tamingChance = 0,
	ferocity = 9,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = NONE,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/fambaa_hue.iff"},
	lootGroups = {
	 {
	        groups = {
				{group = "fambaa_common", chance = 3300000}
			},
			lootChance = 6500000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack","postureDownChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(rogue_fambaa, "rogue_fambaa")
