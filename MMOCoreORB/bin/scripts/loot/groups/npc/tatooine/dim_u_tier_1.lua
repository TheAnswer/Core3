--Automatically generated by SWGEmu Spawn Tool v0.12 loot editor.

dim_u_tier_1 = {
	description = "",
	minimumLevel = 0,
	maximumLevel = -1,
	lootItems = {
		{groupTemplate = "dim_u_common", weight = 400000},
		{groupTemplate = "junk", weight = 3200000},
		{groupTemplate = "loot_kit_parts", weight = 1000000},
		{groupTemplate = "tailor_components", weight = 1000000},
		{groupTemplate = "wearables_common", weight = 1400000},
		{groupTemplate = "resource_gemstone", weight = 250000},
		{groupTemplate = "resource_metal", weight = 250000},
		{groupTemplate = "resource_ore", weight = 250000},
		{groupTemplate = "resource_water", weight = 250000},
		{groupTemplate = "melee_weapons_common", weight = 1000000},
		{groupTemplate = "ranged_weapons_common", weight = 1000000},
	}
}

addLootGroupTemplate("dim_u_tier_1", dim_u_tier_1)

--[[
mobiles:
	scripts/mobile/tatooine/dim_u_abbot.lua	11
	scripts/mobile/tatooine/dim_u_cleric.lua	9
	scripts/mobile/tatooine/dim_u_monastery_nun.lua	6
	scripts/mobile/tatooine/dim_u_monk.lua	6
	scripts/mobile/tatooine/dim_u_preacher.lua	8
	scripts/mobile/tatooine/dim_u_priestess.lua	8
]]--