--Automatically generated by SWGEmu Spawn Tool v0.12 loot editor.

nym_tier_2 = {
	description = "",
	minimumLevel = 0,
	maximumLevel = -1,
	lootItems = {
		{groupTemplate = "junk", weight = 5000000},
		{groupTemplate = "nyms_common", weight = 800000},
		{groupTemplate = "nyms_rare", weight = 400000},
		{groupTemplate = "ranged_weapons", weight = 1700000},
		{groupTemplate = "tailor_components", weight = 400000},
		{groupTemplate = "wearables_uncommon", weight = 1700000},
	}
}

addLootGroupTemplate("nym_tier_2", nym_tier_2)

--[[
mobiles:
	scripts/mobile/lok/nym_destroyer.lua	40
	scripts/mobile/lok/nym_droideka.lua	45
	scripts/mobile/lok/nym_patrol_elite.lua	41
]]--