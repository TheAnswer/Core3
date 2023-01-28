-- Planet Region Definitions
--
-- {"regionName", x, y, shape and size, tier, {"spawnGroup1", ...}, maxSpawnLimit}
-- For circle and ring, x and y are the center point
-- For rectangles, x and y are the bottom left corner. x2 and y2 (see below) are the upper right corner
-- Shape and size is a table with the following format depending on the shape of the area:
--   - Circle: {CIRCLE, radius}
--   - Rectangle: {RECTANGLE, x2, y2}
--   - Ring: {RING, inner radius, outer radius}
-- Tier is a bit mask with the following possible values where each hexadecimal position is one possible configuration.
-- That means that it is not possible to have both a spawn area and a no spawn area in the same region, but
-- a spawn area that is also a no build zone is possible.

require("scripts.managers.planet.regions")

tatooine_regions = {
	-- No Build Zones
	{"northedge_tatooine_nobuild", -7980, 7640, {RECTANGLE, 8000, 8000}, NOBUILDZONEAREA},
	{"westedge_tatooine_nobuild", -7980, -7641, {RECTANGLE, -7640, 7640}, NOBUILDZONEAREA},
	{"southedge_tatooine_nobuild", -8000, -8000, {RECTANGLE, 8000, -7640}, NOBUILDZONEAREA},
	{"eastedge_tatooine_nobuild", 7640, -7640, {RECTANGLE, 8000, 7640}, NOBUILDZONEAREA},
	{"old_battlefield_nobuild_tat1", 2488, 4388, {CIRCLE, 192}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"old_battlefield_nobuild_tat2", 4950, 4650, {CIRCLE, 256}, NOSPAWNAREA + NOBUILDZONEAREA},

	-- Named Regions, POIs and Decor
	{"@tatooine_region_names:jabbas_palace", -5983, -6267, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION + NAVAREA},
	{"@tatooine_region_names:jabba_palace", -5857, -6175, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION + NAVAREA},
	{"tatooine_bib_newbie_1", -1430, -3773, {CIRCLE, 5}, UNDEFINEDAREA},
	--{"@tatooine_region_names:jundland_wastes", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:jundland_wastes_1", -4544, -1472, {RECTANGLE, -1376, -224}, NAMEDREGION},
	{"@tatooine_region_names:jundland_wastes_2", -4400, -2300, {RECTANGLE, -2912, -160}, NAMEDREGION},
	{"@tatooine_region_names:valley_of_the_wind", -6917, -5397, {RECTANGLE, -6330, -4330}, NAMEDREGION},
	--{"@tatooine_region_names:lowland_basin", 0, 0, {CIRCLE, 0}, NAMEDREGION},
	{"@tatooine_region_names:lowland_basin_1", -2656, -5696, {CIRCLE, 1376}, NAMEDREGION},
	{"@tatooine_region_names:arch_mesa", -289, 3851, {CIRCLE, 300}, NAMEDREGION},
	{"@tatooine_region_names:grand_arena_flats_1", 2592, 4096, {CIRCLE, 512}, NAMEDREGION},
	{"@tatooine_region_names:grand_arena_flats_2", 1568, 3872, {RECTANGLE, 2720, 4384}, NAMEDREGION},
	{"@tatooine_region_names:fort_tusken", -3900, 6260, {CIRCLE, 250}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION + NAVAREA},
	--{"@tatooine_region_names:fort_tusken_1", -3981, 6270, {CIRCLE, 250}, NAMEDREGION},
	{"@tatooine_region_names:jawa_mountain_fortress", -6144, 1856, {CIRCLE, 200}, NAMEDREGION + NOSPAWNAREA + NAVAREA},
	{"@tatooine_region_names:jawa_mountain_fortress_1", -6144, 1856, {CIRCLE, 300}, NOBUILDZONEAREA},
	{"@tatooine_region_names:mushroom_mesa", 900, 5567, {CIRCLE, 250}, NAMEDREGION},
	{"@tatooine_region_names:benzin_plateau_1", 880, 5456, {CIRCLE, 720}, NAMEDREGION},
	{"@tatooine_region_names:benzin_plateau_2", -192, 5024, {CIRCLE, 744}, NAMEDREGION},
	{"@tatooine_region_names:benzin_plateau_3", 1776, 5168, {CIRCLE, 624}, NAMEDREGION},
	{"@tatooine_region_names:benzin_plateau_4", 880, 4368, {CIRCLE, 650}, NAMEDREGION},
	{"@tatooine_region_names:eastern_oasis_1", 6244, -400, {CIRCLE, 150}, NAMEDREGION},
	{"@tatooine_region_names:oasis_northeast", 6665, 5457, {CIRCLE, 150}, NAMEDREGION},
	{"@tatooine_region_names:imperial_oasis", -5300, 2681, {CIRCLE, 90}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"@tatooine_region_names:imperial_oasis_1", -5311, 2684, {CIRCLE, 200}, NAMEDREGION},
	{"@tatooine_region_names:oasis_south", 1828, -6053, {CIRCLE, 150}, NAMEDREGION},
	{"@tatooine_region_names:krayt_graveyard", 7451, 4510, {CIRCLE, 500}, NAMEDREGION + NOBUILDZONEAREA},
	{"@tatooine_region_names:krayt_graveyard_1", 6560, 4096, {RECTANGLE, 7488, 4608}, NAMEDREGION + NOBUILDZONEAREA},
	{"@tatooine_region_names:w_dune_sea_1", -5952, -2176, {CIRCLE, 1344}, NAMEDREGION},
	{"@tatooine_region_names:w_dune_sea_2", -6112, -6016, {RECTANGLE, -4384, -2112}, NAMEDREGION},
	{"@tatooine_region_names:w_dune_sea_3", -4944, -4816, {CIRCLE, 950}, NAMEDREGION},
	{"@tatooine_region_names:e_dune_sea_1", 4496, 4848, {CIRCLE, 1296}, NAMEDREGION},
	{"@tatooine_region_names:e_dune_sea_2", 5424, 3952, {CIRCLE, 1136}, NAMEDREGION},
	{"@tatooine_region_names:e_dune_sea_3", 5632, 2080, {CIRCLE, 1152}, NAMEDREGION},
	{"@tatooine_region_names:e_dune_sea_4", 2880, 5440, {RECTANGLE, 3808, 7392}, NAMEDREGION},
	{"@tatooine_region_names:tusken_march", -5312, 6048, {CIRCLE, 1376}, NAMEDREGION},
	--{"@tatooine_region_names:southern_reach", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:southern_reach_1", 1664, -6336, {CIRCLE, 992}, NAMEDREGION},
	{"@tatooine_region_names:mesric_plateau_1", -336, 368, {CIRCLE, 1360}, NAMEDREGION},
	{"@tatooine_region_names:mesric_plateau_2", 1104, -784, {CIRCLE, 1648}, NAMEDREGION},
	{"@tatooine_region_names:mesric_plateau_3", 3424, -2336, {CIRCLE, 1600}, NAMEDREGION},
	{"@tatooine_region_names:mesric_plateau_4", 5200, -4720, {CIRCLE, 1610}, NAMEDREGION},
	{"@tatooine_region_names:mesric_plateau_5", 624, -3120, {CIRCLE, 1168}, NAMEDREGION},
	{"@tatooine_region_names:mospic_highlands_1", -3632, 4272, {CIRCLE, 1168}, NAMEDREGION},
	{"@tatooine_region_names:mospic_highlands_2", -1792, 3488, {CIRCLE, 640}, NAMEDREGION},
	{"@tatooine_region_names:mospic_highlands_3", -5280, 3500, {RECTANGLE, -2336, 4448}, NAMEDREGION},
	{"@tatooine_region_names:broken_ruins", -5722, 6479, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:hung_skeleton", -4824, 6530, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:darklighter_estate", -713, -6677, {CIRCLE, 500}, NAMEDREGION + NOBUILDZONEAREA},
	{"@tatooine_region_names:darklighter_estate_1", -706, -6714, {CIRCLE, 200}, NOSPAWNAREA},
	--{"tatooine_darklighter_estate", -673, -6723, {CIRCLE, 193}, UNDEFINEDAREA},
	--{"tatooine_darklighter_estate_nobuild", -673, -6723, {CIRCLE, 293}, UNDEFINEDAREA},
	{"@tatooine_region_names:bowling_rocks", -5233, 6237, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ancient_skeleton", -5802, 5408, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:crashed_lifepod", -6173, 5887, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:sand_swept_tanks", -4696, 5455, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:crash_debris_camp", -4463, 5782, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:krayt_skeleton", -6205, 5056, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:hovel_in_the_hill", -6353, 4653, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:burning_bone_alter", -6114, 3966, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:krayt_buried_alive", -6741, 3572, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:destroyed_wall", -4878, 2947, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:camp_on_tower", -6450, 2618, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:radar_dish", -5467, 2134, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:storage_tent", -6890, 1508, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ore_harvesters", -5784, -560, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ship_debris", -4928, 1258, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:stone_teepee", -5743, -1941, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:wind_harvest_complex", -7166, -2399, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ruins_of_old_building", -5549, -2425, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:strange_brick", -5115, -3357, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:hidden_x_wing", -4986, -4357, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:broken_water_tanks", -4324, -5019, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:long_bone_spine", -4943, -5544, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:large_rock_with_camp", -6711, -5668, {CIRCLE, 150}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:bridge_and_ruins", -3980, -3042, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:small_camp", -3321, -3717, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:maze", -3043, -5127, {CIRCLE, 175}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:wind_harvest_unit", -2910, -3248, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:broken_building_with_ruins", -1630, -5584, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:trifecta_of_stones", -3735, -6811, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:small_camp_1", -530, -5686, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:life_pod_and_debris", -860, -1227, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:elegent_tent", -2598, -2294, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:stone_druid_circle", -3675, -1649, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:stone_monument", -2667, -411, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:junk_heap", -2127, 683, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:photo_bio_camp", -3170, 3642, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:camp_with_two_tents", -1262, 5461, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:krayt_remains", 3387, -6593, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:gas_harvest_bank", 2215, -4479, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:listening_post", 5943, -3828, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ruins", 6471, -2234, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:sand_crawler_debris", 5664, -5760, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:small_grave_marker", 1877, -3029, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:pile_o_rocks", 871, -1057, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_platform_messed_up_walls", 7059, 7065, {CIRCLE, 150}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:4_pillars_connected_at_top", 6526, 5524, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:huge_cliff_fortress", 574, 362, {CIRCLE, 250}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:destroyed_aircraft_hanger", 359, 6568, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_platform_with_messed_up_pillars", 6399, 208, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_platform", 2330, 3256, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:two_grand_archways", 3491, 4127, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:two_desert_housings", 3546, -2508, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_desert_household_with_courtyard", 4993, -4682, {CIRCLE, 150}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_grand_archway", 5460, 2137, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:destroyed_platform", 2977, 6472, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:small_stone_hutt", 5776, 3975, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:ruins_on_a_wall", 3280, -3680, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:strange_arches_lined_up", -845, 2701, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:krayt_skeleton_on_a_hill", -2834, 6061, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:three_short_archways_lined_up_in_a_row", 1856, 1472, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:long_destroyed_wall", 2992, -480, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_platform_with_two_arches_on_top", 3657, -3375, {CIRCLE, 100}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:single_platform_with_one_tall_archway", 496, -3264, {CIRCLE, 75}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:cave_southwest", 6551, -1212, {CIRCLE, 300}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:cave_west", 5144, 622, {CIRCLE, 300}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:escape_pod", -3931, -4420, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:krayt_skeleton_1", -4649, -4361, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:lars_homestead", -2582, -5509, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:old_ben_kenobis hut", -4490, -2274, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:sarlacc_pit", -6161, -3375, {CIRCLE, 500}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"@tatooine_region_names:cave_central", 0, 0, {CIRCLE, 300}, NOSPAWNAREA + NOBUILDZONEAREA + NAMEDREGION},
	{"tatooine_force_shrine_01", -6506, -3668, {CIRCLE, 199}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_force_shrine_02", 5264, 113, {CIRCLE, 199}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_force_shrine_03", -3623, 5281, {CIRCLE, 199}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_force_shrine_04", 5633, 6015, {CIRCLE, 199}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_force_shrine_05", 5958, -5684, {CIRCLE, 199}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"sdungeon_tusken_village", -5322, -4444, {CIRCLE, 140}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_sdungeon_cave_01", 52, -91, {CIRCLE, 128}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_sdungeon_cave_02", -1479, -185, {CIRCLE, 128}, NOSPAWNAREA + NOBUILDZONEAREA},
	--{"tatooine_sdungeon_cave_03", 5145, 618, {CIRCLE, 128}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_sdungeon_cave_04", -3978, 6249, {CIRCLE, 200}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_sdungeon_cave_05", 6557, -1302, {CIRCLE, 128}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"tatooine_rebel_encampment", -784, -4451, {CIRCLE, 198}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"sdungeon_tusken_village_quest_giver_refugee_town", -5473, -3881, {CIRCLE, 128}, NOSPAWNAREA + NOBUILDZONEAREA},

	{"containers", -2689, -4634, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"grave_and_weeping_tree", -6195, 5108, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"krayt_big_game_hunt", -1228, -289, {CIRCLE, 256}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"metal_wreckage", -1484, 991, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"ruins_of_old_building", -4776, -3015, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"skeletons", -6275, 6017, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"standing_stones", -5231, 5236, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"streetlamp", -4659, 6355, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"three_small_buildings", -172, 1926, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"two_metal_poles", -5832, 3424, {CIRCLE, 20}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"village_ruins", 5695, 1877, {CIRCLE, 200}, NOSPAWNAREA + NOBUILDZONEAREA + NAVAREA},
	{"wind_harvest_unit", -2910, -6249, {CIRCLE, 50}, NOSPAWNAREA + NOBUILDZONEAREA},

	-- Unknown Named regions
	{"@tatooine_region_names:west_dune_sea_3", 0, 0, {CIRCLE, 0}, NAMEDREGION},
	{"@tatooine_region_names:burning_bone_altar", 0, 0, {CIRCLE, 0}, NAMEDREGION},
	{"@tatooine_region_names:arch_mesa", 0, 0, {CIRCLE, 0}, NAMEDREGION},
	{"@tatooine_region_names:canyon_maze", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:central_mountains", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:eastern_peaks", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:northeastern_desert", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:northern_wasteland", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:northwestern_mountains", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:southeastern_mountains", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:two_fortresses", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},
	{"@tatooine_region_names:desert", 0, 0, {CIRCLE, 0}, UNDEFINEDAREA},

	-- Cities
	{"@tatooine_region_names:bestine", -1218, -3688, {CIRCLE, 336}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:mos_eisley", 3460, -4768, {CIRCLE, 456}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:mos_entha", 1488, 3256, {CIRCLE, 424}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:mos_espa", -2940, 2190, {CIRCLE, 533}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:mos_taike", 3832, 2360, {CIRCLE, 264}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:wayfar", -5174, -6582, {CIRCLE, 230}, CITY + NOSPAWNAREA},
	{"@tatooine_region_names:anchorhead", 102, -5360, {CIRCLE, 125}, CITY + NOSPAWNAREA},
	{"bestine_nobuild_1", -1218, -3688, {CIRCLE, 700},  NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"mos_eisley_nobuild_1", 3460, -4768, {CIRCLE, 900}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"mos_entha_nobuild_1", 1488, 3256, {CIRCLE, 900}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"mos_espa_nobuild_1", -2712, 2464, {CIRCLE, 1100}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"mos_taike_nobuild_1", 3832, 2360, {CIRCLE, 500}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"wayfar_nobuild_1", -5174, -6582, {CIRCLE, 500}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},
	{"anchorhead_nobuild_1", 102, -5360, {CIRCLE, 400}, NOBUILDZONEAREA + NOWORLDSPAWNAREA},

	-- Spawn Areas
	{"@tatooine_region_names:anchorhead_easy_newbie", 0, -5350, {CIRCLE, 500}, SPAWNAREA, {"tatooine_anchorhead_easy_newbie"}, 64},
	{"@tatooine_region_names:anchorhead_medium_newbie", 0, -5350, {RING, 500, 900}, SPAWNAREA, {"tatooine_anchorhead_medium_newbie"}, 64},
	{"@tatooine_region_names:bestine_medium_newbie", -1216, -3660, {CIRCLE, 1200}, SPAWNAREA, {"tatooine_bestine_easy_newbie"}, 64},
	{"@tatooine_region_names:bestine_easy_newbie", -1212, -3676, {CIRCLE, 650}, SPAWNAREA, {"tatooine_bestine_medium_newbie"}, 64},
	{"@tatooine_region_names:mos_eisley_medium_newbie", 3400, -4775, {RING, 800, 1400}, SPAWNAREA, {"tatooine_mos_eisley_medium_newbie"}, 64},
	{"@tatooine_region_names:mos_eisley_easy_newbie", 3496, -4784, {CIRCLE, 800}, SPAWNAREA, {"tatooine_mos_eisley_easy_newbie"},64},
	{"@tatooine_region_names:mos_eisley_se_medium", 630, -7692, {RECTANGLE, 6750, -2100}, SPAWNAREA, {"tatooine_mos_eisley_se_medium"} ,256},
	{"@tatooine_region_names:mos_eisley_se_hard", -2909, -7700, {RECTANGLE, 7841, 930}, SPAWNAREA, {"tatooine_mos_eisley_se_hard"} ,256},
	{"@tatooine_region_names:mos_eisley_easy_newbie_sw", 2408, -5992, {RECTANGLE, 3346, -5012}, SPAWNAREA, {"tatooine_mos_eisley_easy_newbie_sw"} ,256},
	{"@tatooine_region_names:mos_entha_easy_newbie", 1490, 3165, {CIRCLE, 800}, SPAWNAREA, {"tatooine_mos_entha_easy_newbie"}, 64},
	{"@tatooine_region_names:mos_entha_medium_newbie", 1490, 3165, {RING, 800, 1500}, SPAWNAREA, {"tatooine_mos_entha_medium_newbie"}, 64},
	{"@tatooine_region_names:mos_entha_medium", -2895, 940, {RECTANGLE, 900, 5350}, SPAWNAREA, {"tatooine_mos_entha_medium"} ,256},
	{"@tatooine_region_names:mos_entha_weequay", -2419, 1909, {CIRCLE, 250}, SPAWNAREA, {"tatooine_mos_entha_weequay"} ,256},
	{"@tatooine_region_names:wayfar_easy_newbie", -5195, -6535, {CIRCLE, 600}, SPAWNAREA, {"tatooine_wayfar_easy_newbie"}, 64},
	{"@tatooine_region_names:wayfar_medium_newbie", -5195, -6535, {RING, 600, 1000}, SPAWNAREA, {"tatooine_wayfar_medium_newbie"}, 64},
	{"@tatooine_region_names:mos_espa_easy_newbie", -2950, 2222, {CIRCLE, 900}, SPAWNAREA, {"tatooine_mos_espa_easy_newbie"}, 64},
	{"@tatooine_region_names:mos_espa_medium_newbie", -2950, 2222, {RING, 900, 1600}, SPAWNAREA, {"tatooine_mos_espa_medium_newbie"}, 64},
	{"@tatooine_region_names:mos_taike_easy_newbie", 3835, 2350, {CIRCLE, 600}, SPAWNAREA, {"tatooine_mos_taike_easy_newbie"}, 64},
	{"@tatooine_region_names:mos_taike_medium_newbie", 3835, 2350, {RING, 600, 1100}, SPAWNAREA, {"tatooine_mos_taike_medium_newbie"}, 64},

	{"@tatooine_region_names:hard_krayt_ne", 6515, 4215, {CIRCLE, 785}, SPAWNAREA, {"tatooine_hard_krayt_ne"}, 8},
	{"@tatooine_region_names:north_eastern_krayt_background", 908, 930, {RECTANGLE, 7830, 5357}, SPAWNAREA, {"tatooine_north_eastern_krayt_background"}, 256},
	{"@tatooine_region_names:jundland_waste_1", -3488, 4240, {CIRCLE, 1125}, SPAWNAREA, {"tatooine_jundland_wastes"}, 64},
	{"@tatooine_region_names:jundland_waste_2", -3296, -1016, {CIRCLE, 1250}, SPAWNAREA, {"tatooine_jundland_wastes"}, 64},
	{"@tatooine_region_names:eastern_dune_sea", 2320, 784, {RECTANGLE, 8000, 7696}, SPAWNAREA, {"tatooine_hard_dune_sea"}, 256},
	{"@tatooine_region_names:western_dune_sea_1", -8000, -8000, {RECTANGLE, -3728, -432}, SPAWNAREA, {"tatooine_hard_dune_sea"}, 256},
	{"@tatooine_region_names:western_dune_sea_2", -2160, -5920, {CIRCLE, 1968}, SPAWNAREA, {"tatooine_hard_dune_sea"}, 256},
	{"@tatooine_region_names:western_dune_sea_3", -4904, -2536, {CIRCLE, 2424}, SPAWNAREA, {"tatooine_hard_dune_sea"}, 256},
	{"@tatooine_region_names:southwestern_desert", -7600, -7720, {RECTANGLE, -2915, 3933}, SPAWNAREA, {"tatooine_southwestern_desert"} ,256},
	{"@tatooine_region_names:medium_imperial_patrol", 408, -1042, {CIRCLE, 250}, SPAWNAREA, {"tatooine_medium_imperial_patrol"} ,256},
	{"@tatooine_region_names:medium_rebel_patrol", 2904, -1452, {CIRCLE, 250}, SPAWNAREA, {"tatooine_medium_rebel_patrol"} ,256},
	{"@tatooine_region_names:medium_rodian_patrol", -205, -2080, {CIRCLE, 250}, SPAWNAREA, {"tatooine_medium_rodian_patrol"} ,256},
	{"@tatooine_region_names:medium_squill_caves", 19, -22, {CIRCLE, 461}, SPAWNAREA, {"tatooine_medium_squill_caves"} ,256},
	{"@tatooine_region_names:hard_squill_caves", 7, 0, {CIRCLE, 274}, SPAWNAREA, {"tatooine_hard_squill_caves"} ,256},
	{"@tatooine_region_names:jabba_camps_sw", -6467, -7067, {CIRCLE, 500}, SPAWNAREA, {"tatooine_jabba_camps_sw"} ,256},
	{"@tatooine_region_names:medium_criminals", -3867, -4292, {CIRCLE, 250}, SPAWNAREA, {"tatooine_medium_criminals"} ,256},
	{"@tatooine_region_names:nw_tusken_medium", -7590, 3925, {RECTANGLE, -2900, 7780}, SPAWNAREA, {"tatooine_nw_tusken_medium"} ,256},
	{"@tatooine_region_names:north_eastern_hard", -2900, 5359, {RECTANGLE, 7800, 7782}, SPAWNAREA, {"tatooine_north_eastern_hard"} ,256},
	{"@tatooine_region_names:nw_tusken_hard", -4046, 6104, {CIRCLE, 521}, SPAWNAREA, {"tatooine_nw_tusken_hard"} ,256},
	{"@tatooine_region_names:northeast_imperial_medium", 4697, 2126, {CIRCLE, 250}, SPAWNAREA, {"tatooine_northeast_imperial_medium"} ,256},
	{"@tatooine_region_names:northeast_rebel_medium", 6192, 1562, {CIRCLE, 250}, SPAWNAREA, {"tatooine_northeast_rebel_medium"} ,256},
	{"@tatooine_region_names:nw_rebel_hard", -6644, 4789, {CIRCLE, 250}, SPAWNAREA, {"tatooine_nw_rebel_hard"} ,256},
	{"@tatooine_region_names:nw_imperial_hard", -5541, 7126, {CIRCLE, 250}, SPAWNAREA, {"tatooine_nw_imperial_hard"} ,256},
	{"@tatooine_region_names:jundlan_eopie", -6112, -3375, {CIRCLE, 256}, SPAWNAREA, {"tatooine_jundlan_eopie"} ,256},
	{"@tatooine_region_names:medium_womprats", -2500, -5400, {CIRCLE, 250}, SPAWNAREA, {"tatooine_medium_womprats"} ,256},


	{"@tatooine_region_names:world_spawner", 0, 0, {RECTANGLE, 0, 0}, SPAWNAREA + WORLDSPAWNAREA, {"tatooine_world"}, 2048},

	--[[
		tatooine_northern_wasteland
	tatooine_northwestern_mountains
	tatooine_southeastern_mountains
	tatooine_southern_reach
	tatooine_central_mountains
	tatooine_eastern_peaks
	tatooine_lowland_basin
	tatooine_northeastern_desert



	tatooine_easy_bugland_ne
	tatooine_easy_criminals_se
	tatooine_easy_desert_demons_sw
	tatooine_easy_militia_se
	tatooine_easy_pirate_sw
	tatooine_easy_swoopers_nw
	tatooine_easy_villiages_2_sw
	tatooine_easy_villiages_ne
	tatooine_easy_villiages_nw
	tatooine_easy_villiages_se
	tatooine_easy_villiages_sw
	tatooine_hard_alkhara_ne
	tatooine_hard_bugland_ne
	tatooine_hard_criminals_se
	tatooine_hard_jabba_thug_sw
	tatooine_hard_jawa_nw
	tatooine_hard_tusken_nw
	tatooine_hard_valarian_se
	tatooine_jabba_lowbie_zone
	tatooine_medium_bugland_ne
	tatooine_medium_criminals_se
	tatooine_medium_giant_worrt_nw
	tatooine_medium_jabba_thug_2_sw
	tatooine_medium_jabba_thug_sw
	tatooine_medium_jawa_nw
	tatooine_medium_kitonaks_nw
	tatooine_medium_kitonaks_se
	tatooine_medium_rodians_nw
	tatooine_medium_scyks_se
	tatooine_medium_scyks_sw
	tatooine_medium_slavers_ne
	tatooine_medium_slavers_se
	tatooine_medium_slavers_sw
	tatooine_medium_squill_sw
	tatooine_medium_swoopers_nw
	tatooine_medium_swoopers_se
	tatooine_medium_tusken_sw
	tatooine_medium_valarian_ne
	tatooine_medium_weequay_ne
	]]


	{"@tatooine_region_names:plateau", -1408, -7088, {RECTANGLE, 7760, 1376}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:eastern_oasis", 6289, -441, {CIRCLE, 500}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:northeastern_oasis", 5361, 3945, {CIRCLE, 1175}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:southern_oasis", 1032, -7755, {CIRCLE, 200}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:western_oasis", -7127, 1408, {CIRCLE, 500}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_1", -344, 344, {CIRCLE, 1648}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_2", 1200, -832, {CIRCLE, 1808}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_3", 3496, -2584, {CIRCLE, 1500}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_4", 1624, -6248, {CIRCLE, 850}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_5", 616, -3352, {CIRCLE, 900}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:mospic_high_range_6", -6008, -4600, {CIRCLE, 1100}, SPAWNAREA, {"tatooine_world"}, 256},
	{"@tatooine_region_names:grand_arena", 872, 5000, {CIRCLE, 1130}, SPAWNAREA, {"tatooine_world"}, 256},

	--[[
	{"easy_bugland_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_criminals_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_desert_demons_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_militia_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_pirate_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_swoopers_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villages_2_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villages_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villages_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villages_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villages_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villiages_2_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villiages_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villiages_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villiages_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"easy_villiages_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_criminals_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_giant_worrt_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_jabba_thug_2_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_jabba_thug_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_jawa_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_kitonaks_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_kitonaks_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_rodians_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_scyks_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_scyks_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_slavers_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_slavers_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_slavers_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_squill_sw", 0, 0, {CIRCLE, 256}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_swoopers_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_swoopers_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_tusken_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_valarian_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"medium_weequay_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_alkhara_ne", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_bugland_ne", 6560, -1250, {CIRCLE, 256}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_criminals_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_dune_sea", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_jabba_thug_sw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_jawa_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_tusken_nw", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	{"hard_valarian_se", 0, 0, {CIRCLE, 0}, SPAWNAREA , {"tatooine_world"} ,256},
	]]

	-- Espa Track
	{"mos_espa_circuit_track_1", 1943, 4792, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_2", 1546, 4959, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_3", 1316, 5434, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_4", 688, 5439, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_5", 156, 5326, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_6", -414, 5090, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_7", -664, 4832, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_8", -769, 4340, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_9", -710, 3993, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_10", 81, 4092, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_11", 594, 4284, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_12", 917, 3841, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_13", 1333, 4457, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_14", 1630, 4326, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},
	{"mos_espa_circuit_track_15", 2132, 4346, {CIRCLE, 30}, NOSPAWNAREA + NOBUILDZONEAREA},

	-- Deliver Missions
	--[[
	{"bestine_deliver_1", -1034, -3645, {CIRCLE, 5}, UNDEFINEDAREA},
	{"bestine_deliver_2", -1010, -3729, {CIRCLE, 2}, UNDEFINEDAREA},
	{"bestine_deliver_3", -1142, -3673, {CIRCLE, 2}, UNDEFINEDAREA},
	{"bestine_deliver_4", -1227, -3629, {CIRCLE, 5}, UNDEFINEDAREA},
	{"bestine_deliver_5", -1320, -3633, {CIRCLE, 7}, UNDEFINEDAREA},
	{"bestine_deliver_6", -1405, -3637, {CIRCLE, 2}, UNDEFINEDAREA},
	{"bestine_deliver_7", -1429, -3628, {CIRCLE, 5}, UNDEFINEDAREA},
	{"bestine_deliver_8", -1465, -3561, {CIRCLE, 10}, UNDEFINEDAREA},
	{"bestine_deliver_9", -1359, -3504, {CIRCLE, 7}, UNDEFINEDAREA},
	{"bestine_deliver_10", -1086, -3648, {CIRCLE, 2}, UNDEFINEDAREA},
	{"anchorhead_deliver_1", 70, -5317, {CIRCLE, 5}, UNDEFINEDAREA},
	{"anchorhead_deliver_2", 135, -5333, {CIRCLE, 5}, UNDEFINEDAREA},
	{"anchorhead_deliver_3", 115, -5404, {CIRCLE, 5}, UNDEFINEDAREA},
	{"anchorhead_deliver_4", 123, -5377, {CIRCLE, 5}, UNDEFINEDAREA},
	{"wayfar_deliver_1", -5217, -6494, {CIRCLE, 12}, UNDEFINEDAREA},
	{"wayfar_deliver_2", -5129, -6542, {CIRCLE, 5}, UNDEFINEDAREA},
	{"wayfar_deliver_3", -5095, -6528, {CIRCLE, 2}, UNDEFINEDAREA},
	{"wayfar_deliver_4", -5133, -6493, {CIRCLE, 5}, UNDEFINEDAREA},
	{"wayfar_deliver_5", -5115, -6589, {CIRCLE, 10}, UNDEFINEDAREA},
	{"wayfar_deliver_6", -5176, -6612, {CIRCLE, 7}, UNDEFINEDAREA},
	{"wayfar_deliver_7", -5218, -6579, {CIRCLE, 2}, UNDEFINEDAREA},
	{"wayfar_deliver_8", -5220, -6548, {CIRCLE, 7}, UNDEFINEDAREA},
	{"wayfar_deliver_9", -5277, -6558, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_taike_deliver_1", 3780, 2362, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_taike_deliver_2", 3857, 2343, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_taike_deliver_3", 3807, 2360, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_taike_deliver_4", 3951, 2283, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_eisley_deliver_1", 3527, -4701, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_eisley_deliver_2", 3527, -4721, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_eisley_deliver_3", 3358, -4649, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_eisley_deliver_4", 3330, -4638, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_eisley_deliver_5", 3382, -4675, {CIRCLE, 12}, UNDEFINEDAREA},
	{"mos_eisley_deliver_6", 3377, -4753, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_eisley_deliver_7", 3364, -4830, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_eisley_deliver_8", 3458, -4891, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_eisley_deliver_9", 3482, -5002, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_eisley_deliver_10", 3447, -5070, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_entha_deliver_1", 1450, 3138, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_entha_deliver_2", 1450, 3160, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_entha_deliver_3", 1449, 3183, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_entha_deliver_4", 1450, 3203, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_entha_deliver_5", 1637, 3124, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_entha_deliver_6", 1389, 3162, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_entha_deliver_7", 1391, 3203, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_entha_deliver_8", 1289, 3209, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_entha_deliver_9", 1723, 3131, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_entha_deliver_10", 1753, 3163, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_espa_deliver_1", -2942, 2432, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_espa_deliver_2", -2874, 2546, {CIRCLE, 5}, UNDEFINEDAREA},
	{"mos_espa_deliver_3", -2839, 2493, {CIRCLE, 2}, UNDEFINEDAREA},
	{"mos_espa_deliver_4", -2859, 2289, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_espa_deliver_5", -2890, 2251, {CIRCLE, 7}, UNDEFINEDAREA},
	{"mos_espa_deliver_6", -2860, 2212, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_espa_deliver_7", -2955, 2235, {CIRCLE, 12}, UNDEFINEDAREA},
	{"mos_espa_deliver_8", -3047, 2133, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_espa_deliver_9", -3100, 2149, {CIRCLE, 10}, UNDEFINEDAREA},
	{"mos_espa_deliver_10", -2914, 1985, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_espa_deliver_11", -2952, 2088, {CIRCLE, 15}, UNDEFINEDAREA},
	{"mos_espa_deliver_12", -2914, 2466, {CIRCLE, 10}, UNDEFINEDAREA},
	]]
}
