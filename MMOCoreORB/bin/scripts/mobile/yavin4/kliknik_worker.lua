kliknik_worker = Creature:new {
	objectName = "@mob/creature_names:kliknik_worker",
	socialGroup = "kliknik",
	pvpFaction = "",
	faction = "",
	level = 23,
	chanceHit = 0.35,
	damageMin = 210,
	damageMax = 220,
	baseXp = 2219,
	baseHAM = 6800,
	baseHAMmax = 8300,
	armor = 0,
	resists = {15,25,15,15,15,-1,-1,-1,-1},
	meatType = "meat_carnivore",
	meatAmount = 6,
	hideType = "hide_scaley",
	hideAmount = 4,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0.25,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = PACK,
	optionsBitmask = 128,
	diet = CARNIVORE,

	templates = {"object/mobile/kliknik_hue.iff"},
	controlDeviceTemplate = "object/intangible/pet/kliknik_hue.iff",
	lootGroups = {
	 {
	        groups = {
				{group = "kliknik_common", chance = 10000000}
			},
			lootChance = 2000000
		}
	},
	weapons = {},
	conversationTemplate = "",
	attacks = {
		{"posturedownattack","postureDownChance=50"}
	}
}

CreatureTemplates:addCreatureTemplate(kliknik_worker, "kliknik_worker")
