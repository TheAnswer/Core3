foul_desecrator = Creature:new {
	objectName = "@mob/creature_names:kamurith_foul_desecrator",
	socialGroup = "kamurith",
	pvpFaction = "",
	faction = "",
	level = 60,
	chanceHit = 0.55,
	damageMin = 470,
	damageMax = 650,
	baseXp = 5830,
	baseHAM = 11000,
	baseHAMmax = 14000,
	armor = 1,
	resists = {25,15,15,15,15,15,20,20,-1},
	meatType = "meat_carnivore",
	meatAmount = 90,
	hideType = "hide_leathery",
	hideAmount = 65,
	boneType = "bone_avian",
	boneAmount = 50,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 10,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/voritor_lizard_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/voritor_lizard_hue.iff",
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"creatureareadisease",""},
		{"strongdisease",""}
	}
}

CreatureTemplates:addCreatureTemplate(foul_desecrator, "foul_desecrator")
