--Automatically generated by SWGEmu Spawn Tool v0.12 loot editor.

mokk_tribe_tier_1 = {
	description = "",
	minimumLevel = 0,
	maximumLevel = -1,
	lootItems = {
		{groupTemplate = "armor_attachments", weight = 500000},
		{groupTemplate = "clothing_attachments", weight = 500000},
		{groupTemplate = "color_crystals", weight = 400000},
		{groupTemplate = "junk", weight = 3100000},
		{groupTemplate = "loot_kit_parts", weight = 2700000},
		{groupTemplate = "wearables_common", weight = 900000},
		{groupTemplate = "wearables_uncommon", weight = 900000},
		{groupTemplate = "resource_creature", weight = 750000},
		{groupTemplate = "resource_water", weight = 250000},
	}
}

addLootGroupTemplate("mokk_tribe_tier_1", mokk_tribe_tier_1)

--[[
mobiles:
	scripts/mobile/dantooine/mokk_harvester.lua	32
	scripts/mobile/dantooine/mokk_herbalist.lua	26
	scripts/mobile/dantooine/mokk_tribesman.lua	20
]]--