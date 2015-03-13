protocol_droid_3po_silver = Creature:new {
	objectName = "@newbie_tutorial/system_messages:droid_name",
	socialGroup = "imperial",
	faction = "",
	level = 30,
	chanceHit = 0.390000,
	damageMin = 290,
	damageMax = 300,
	baseXp = 2914,
	baseHAM = 8400,
	baseHAMmax = 10200,
	armor = 0,
	resists = {0,45,0,-1,40,-1,40,-1,-1},
	effectImmunities = {STUN,BLIND,DIZZY,NEXTATTACKDELAY,INTIMIDATE},
	dotImmunities = {BLEEDING,POISONED,DISEASED},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.000000,
	ferocity = 0,
	pvpBitmask = NONE,
	creatureBitmask = NONE,
	diet = HERBIVORE,

	templates = {"object/mobile/3po_protocol_droid_silver.iff"},
	lootGroups = {},
	weapons = {"imperial_weapons_medium"},
	attacks = {},
	conversationTemplate = "protocol_droid_tutorial_convotemplate",--dont change this
	optionsBitmask = 264
}

CreatureTemplates:addCreatureTemplate(protocol_droid_3po_silver, "protocol_droid_3po_silver")
