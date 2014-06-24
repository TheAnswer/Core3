arachne_queen = Creature:new {
	objectName = "@mob/creature_names:queen_arachne",
	socialGroup = "arachne",
	pvpFaction = "",
	faction = "",
	level = 63,
	chanceHit = 0.6,
	damageMin = 445,
	damageMax = 600,
	baseXp = 6105,
	baseHAM = 11000,
	baseHAMmax = 14000,
	armor = 2,
	resists = {25,25,25,60,60,-1,60,-1,-1},
	meatType = "meat_insect",
	meatAmount = 60,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK + HERD + KILLER,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/queen_arachne.iff"},
	scale = 1.3,
	lootGroups = {},
	weapons = {"creature_spit_small_green","creature_spit_small_green"},
	conversationTemplate = "",
	attacks = {
		{"creatureareapoison",""},
		{"strongpoison",""}
	}
}

CreatureTemplates:addCreatureTemplate(arachne_queen, "arachne_queen")
