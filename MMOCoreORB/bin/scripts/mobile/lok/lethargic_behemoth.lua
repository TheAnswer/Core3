lethargic_behemoth = Creature:new {
	objectName = "@mob/creature_names:flit_lethargic_behemoth",
	socialGroup = "flit",
	faction = "",
	level = 23,
	chanceHit = 0.35,
	damageMin = 250,
	damageMax = 260,
	baseXp = 2219,
	baseHAM = 6300,
	baseHAMmax = 7700,
	armor = 0,
	resists = {110,110,-1,-1,0,-1,0,0,-1},
	meatType = "meat_avian",
	meatAmount = 11,
	hideType = "",
	hideAmount = 0,
	boneType = "bone_avian",
	boneAmount = 13,
	milk = 0,
	tamingChance = 0.05,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = AIENABLED,
	diet = CARNIVORE,

	templates = {"object/mobile/giant_flit.iff"},
	controlDeviceTemplate = "object/intangible/pet/flit_hue.iff",
	scale = 1.5,
	lootGroups = {},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"stunattack",""},
		{"knockdownattack",""}
	}
}

CreatureTemplates:addCreatureTemplate(lethargic_behemoth, "lethargic_behemoth")
