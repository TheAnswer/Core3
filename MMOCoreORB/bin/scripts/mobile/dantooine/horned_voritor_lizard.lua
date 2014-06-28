horned_voritor_lizard = Creature:new {
	objectName = "@mob/creature_names:horned_voritor_jungle_lizard",
	socialGroup = "voritor",
	pvpFaction = "",
	faction = "",
	level = 53,
	chanceHit = 0.5,
	damageMin = 395,
	damageMax = 500,
	baseXp = 5099,
	baseHAM = 10000,
	baseHAMmax = 12000,
	armor = 1,
	resists = {40,40,5,5,80,80,80,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 65,
	hideType = "hide_leathery",
	hideAmount = 40,
	boneType = "bone_avian",
	boneAmount = 50,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/voritor_lizard_hue.iff"},
	scale = 1.2,
	lootGroups = {
		{
			groups = {
				{group = "voritor_lizard_common", chance = 10000000}
			},
			lootChance = 2500000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareapoison",""},
		{"strongpoison",""}
	}
}

CreatureTemplates:addCreatureTemplate(horned_voritor_lizard, "horned_voritor_lizard")
