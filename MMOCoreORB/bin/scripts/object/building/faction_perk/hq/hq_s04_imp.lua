--Copyright (C) 2010 <SWGEmu>


--This File is part of Core3.

--This program is free software; you can redistribute
--it and/or modify it under the terms of the GNU Lesser
--General Public License as published by the Free Software
--Foundation; either version 2 of the License,
--or (at your option) any later version.

--This program is distributed in the hope that it will be useful,
--but WITHOUT ANY WARRANTY; without even the implied warranty of
--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
--See the GNU Lesser General Public License for
--more details.

--You should have received a copy of the GNU Lesser General
--Public License along with this program; if not, write to
--the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

--Linking Engine3 statically or dynamically with other modules
--is making a combined work based on Engine3.
--Thus, the terms and conditions of the GNU Lesser General Public License
--cover the whole combination.

--In addition, as a special exception, the copyright holders of Engine3
--give you permission to combine Engine3 program with free software
--programs or libraries that are released under the GNU LGPL and with
--code included in the standard release of Core3 under the GNU LGPL
--license (or modified versions of such code, with unchanged license).
--You may copy and distribute such a system following the terms of the
--GNU LGPL for Engine3 and the licenses of the other code concerned,
--provided that you include the source code of that other code when
--and as the GNU LGPL requires distribution of source code.

--Note that people who make modified versions of Engine3 are not obligated
--to grant this special exception for their modified versions;
--it is their choice whether to do so. The GNU Lesser General Public License
--gives permission to release a modified version without this exception;
--this exception also makes it possible to release a modified version


object_building_faction_perk_hq_hq_s04_imp = object_building_faction_perk_hq_shared_hq_s04_imp:new {
	lotSize = 0,
	containerComponent = "GCWBaseContainerComponent",
	zoneComponent = "StructureZoneComponent",
	maintenanceCost = 0,
	baseMaintenanceRate = 0,
	faction = "imperial",
	pvpFaction = "imperial",
	pvpStatusBitmask = 0,
	dataObjectComponent = "DestructibleBuildingDataComponent",
	allowedZones = {"dantooine", "naboo", "rori","tatooine", "corellia", "lok", "talus"},
	constructionMarker = "object/building/player/construction/construction_player_house_generic_medium_style_01.iff",
	length = 7,
	width = 6,
	alwaysPublic = 1,
	factionBaseType = 1,

	skillMods = {
		{"private_buff_mind", 100},
		{"private_med_battle_fatigue", 5},
		{"private_medical_rating", 100},
	},

	childObjects = {
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_16_style_01.iff", x = -17.99, z = 0, y = 9.01, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_16_style_01.iff", x = 18.01, z = 0, y = 9.01, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },

		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 7.01, z = 0, y = 28.01, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -6.99, z = 0, y = 28.01, ox = 0, oy = 0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 10.01, z = 0, y = 21.01, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -9.99, z = 0, y = 21.01, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 14.01, z = 0, y = 17.01, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -13.99, z = 0, y = 17.01, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -14.99, z = 0, y = -1.99, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -14.99, z = 0, y = -7.99, ox = 0, oy = 0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -14.99, z = 0, y = -13.99, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 15.01, z = 0, y = -1.99, ox = 0, oy = 0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 15.01, z = 0, y = -7.99, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 15.01, z = 0, y = -13.99, ox = 0, oy = 0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -7.99, z = 0, y = -16.99, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 8.01, z = 0, y = -16.99, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = -3.99, z = 0, y = -12.99, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 4.01, z = 0, y = -12.99, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_med_imperial_style_01.iff", x = 0.01, z = 0, y = -8.99, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/static/structure/corellia/corl_power_transformer_s01.iff", x = -13.99, z = 0, y = 13.01, ox = 0, oy = 1, oz = 0, ow = 0, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/corellia/corl_power_transformer_s01.iff", x = 14.01, z = 0, y = 13.01, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/corellia/corl_power_transformer_s02.iff", x = 8.01, z = 0, y = -7.99, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/corellia/corl_power_bubble_node.iff", x = -7.99, z = 0, y = -7.99, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/minefield/field_1x1.iff", x = 0, z = 0, y = 38, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 14, z = 0, y = 21, ox = 0, oy = -0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = -14, z = 0, y = 21, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = -18, z = 0, y = -5, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 18, z = 0, y = -5, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 0, z = 0, y = -13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_hq_imperial.iff", x = 0, z = 0.25, y = 1.86, ox = 0, oy = 0, oz = 0, ow = 1, cellid = 2, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_mission.iff", x = 11.99, z = -13.76, y = 1.9, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 9, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_mission_imperial.iff", x = 10.04, z = -11.5, y = -20.99, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 10, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_bank.iff", x = 1.08, z = -6.75, y = -6.44, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 5, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_bank.iff", x = 7.98, z = -6.75, y = -6.42, ox = 0, oy = 0.707, oz = 0, ow = -0.707, cellid = 5, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_insurance.iff", x = -7.47, z = -13.76, y = 11.99, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 8, containmentType = -1 },
	},

	childCreatureObjects = {

		{ mobile = "fbase_at_st", x = 30, z = 0, y = 18, cellid = -1, respawn = 720, containmentType = -1, heading = 3.14},
		{ mobile = "fbase_at_st", x = -30, z = 0, y = 18, cellid = -1, respawn = 720, containmentType = -1, heading = 0},
		{ mobile = "fbase_at_st", x = 0, z = 0, y = 44, cellid = -1, respawn = 720, containmentType = -1, heading = 0},
		{ mobile = "fbase_imperial_exterminator_hard", x = 5, z = 0, y = 50, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_dark_trooper_hard", x = 22, z = 0, y = 2, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 22, z = 0, y = 6, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 24, z = 0, y = 4, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 26, z = 0, y = 2, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_sniper_hard", x = 26, z = 0, y = 6, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_dark_trooper_hard", x = -22, z = 0, y = 2, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = -22, z = 0, y = 6, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = -24, z = 0, y = 4, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = -26, z = 0, y = 2, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_bombardier_hard", x = -26, z = 0, y = 6, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_dark_trooper_hard", x = -2, z = 0, y = 16, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 2, z = 0, y = 16, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 0, z = 0, y = 18, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = -2, z = 0, y = 20, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_rifleman_hard", x = 2, z = 0, y = 20, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_stormtrooper_squad_leader_hard", x = 10, z = 0, y = 50, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_sniper_hard", x = 14, z = 0, y = 50, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 10, z = 0, y = 46, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 14, z = 0, y = 46, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 10, z = 0, y = 42, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 14, z = 0, y = 42, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 10, z = 0, y = 38, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = 14, z = 0, y = 38, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_storm_commando_hard", x = -10, z = 0, y = 50, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -14, z = 0, y = 50, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -10, z = 0, y = 46, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_scout_trooper_hard", x = -14, z = 0, y = 46, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -10, z = 0, y = 42, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -14, z = 0, y = 42, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -10, z = 0, y = 38, cellid = -1, respawn = 360, containmentType = -1, heading = 0},
		{ mobile = "fbase_stormtrooper_hard", x = -14, z = 0, y = 38, cellid = -1, respawn = 360, containmentType = -1, heading = 0},

		{ mobile = "fbase_imperial_army_captain_hard", x = 4.1, z = 0, y = 0.8, cellid = 2, respawn = 300, containmentType = -1, heading = 1.57},
		{ mobile = "fbase_imperial_army_captain_hard", x = 4.1, z = 0, y = -1.8, cellid = 2, respawn = 300, containmentType = -1, heading = 1.57},

		{ mobile = "fbase_dark_trooper_hard", x = -3.5, z = 0, y = -4.2, cellid = 3, respawn = 300, containmentType = -1, heading = 1.57},

		{ mobile = "fbase_dark_trooper_hard", x = 4.4, z = -6.75, y = 4.9, cellid = 4, respawn = 300, containmentType = -1, heading = 4.71 },

		{ mobile = "fbase_dark_trooper_hard", x = 3.5, z = -6.75, y = -3, cellid = 5, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 5, z = -6.75, y = -3, cellid = 5, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 3.5, z = -6.75, y = -5, cellid = 5, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 5, z = -6.75, y = -5, cellid = 5, respawn = 300, containmentType = -1, heading = 0},

		{ mobile = "fbase_storm_commando_hard", x = -0.35, z = -13.75, y = -3.65, cellid = 7, respawn = 300, containmentType = -1, heading = 4.71 },
		{ mobile = "fbase_dark_trooper_hard", x = -6.5, z = -13.75, y = 5.5,cellid = 7, respawn = 300, containmentType = -1, heading = 3.14},
		{ mobile = "fbase_dark_trooper_hard", x = -3.4, z = -13.75, y = 3,cellid = 7, respawn = 300, containmentType = -1, heading = 3.14},

		{ mobile = "fbase_stormtrooper_medic_hard", x = -5.6, z = -13.75, y = 13.6, cellid = 8, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_storm_commando_hard", x = 7.3, z = -13.75, y = 14.3, cellid = 8, respawn = 300, containmentType = -1, heading = 3.14},

		{ mobile = "fbase_sand_trooper_hard", x = 14.2, z = -13.75, y = -10, cellid = 10, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_dark_trooper_hard", x = 15, z = -13.75, y = -4,cellid = 10, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_imperial_colonel_hard", x = 15.9, z = -13.75, y = -17.25,cellid = 10, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "imperial_recruiter", x = 11.5, z = -11.5, y = -21, cellid = 10, respawn = 60, containmentType = -1, heading = 1.59},

	},

}

ObjectTemplates:addTemplate(object_building_faction_perk_hq_hq_s04_imp, "object/building/faction_perk/hq/hq_s04_imp.iff")
