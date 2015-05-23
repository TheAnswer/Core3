cas_vankoo = Creature:new {
	objectName = "@mob/creature_names:cas_vankoo",
	socialGroup = "kimogila",
	faction = "bandit",
	level = 117,
	chanceHit = 3.4,
	damageMin = 725,
	damageMax = 1160,
	baseXp = 11109,
	baseHAM = 50000,
	baseHAMmax = 59000,
	armor = 2,
	resists = {75,75,0,10,10,10,10,0,0},
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
	creatureBitmask = PACK + KILLER + STALKER,
	optionsBitmask = 128,
	diet = HERBIVORE,

	templates = {"object/mobile/dressed_lok_cas_vankoo.iff"},
	lootGroups = {
		{
			groups = {
				{group = "junk", chance = 5200000},
				{group = "color_crystals", chance = 600000},
				{group = "crystals_good", chance = 400000},
				{group = "melee_polearm", chance = 600000},
				{group = "pistols", chance = 600000},
				{group = "clothing_attachments", chance = 800000},
				{group = "armor_attachments", chance = 800000},
				{group = "wearables_all", chance = 1000000}
			}
		}
	},
	weapons = {"cas_vankoo_weapons"},
	conversationTemplate = "",
	reactionStf = "@npc_reaction/townperson",
	attacks = merge(pistoleermaster,pikemanmaster,marksmanmaster,brawlermaster)
}

CreatureTemplates:addCreatureTemplate(cas_vankoo, "cas_vankoo")
