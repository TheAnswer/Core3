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


object_building_faction_perk_hq_hq_s02_imp_pvp = object_building_faction_perk_hq_shared_hq_s02_imp_pvp:new {
	lotSize = 0,
	containerComponent = "GCWBaseContainerComponent",
	zoneComponent = "StructureZoneComponent",
	maintenanceCost = 0,
	baseMaintenanceRate = 0,
	faction = "imperial",
	pvpFaction = "imperial",
	pvpStatusBitmask = OVERT,
	dataObjectComponent = "DestructibleBuildingDataComponent",
	allowedZones = {"dantooine", "naboo", "rori","tatooine", "corellia", "lok", "talus"},
	constructionMarker = "object/building/player/construction/construction_player_house_generic_medium_style_01.iff",
	length = 7,
	width = 6,
	planetMapCategory = "imperial_hq",
	alwaysPublic = 1,
	factionBaseType = 1,

	skillMods = {
		{"private_medical_rating", 100 },
		{"private_med_wound_health", 100 },
		{"private_med_wound_action", 100 },
		{"private_faction_medical_rating", 25},
	},

	childObjects = {
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_16_style_01.iff", x = -18, z = 0, y = -3, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_16_style_01.iff", x = 18, z = 0, y = -3, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },

		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -7, z = 0, y = 24, ox = 0, oy = 0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -18, z = 0, y = 9, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -10, z = 0, y = 17, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 10, z = 0, y = 17, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -14, z = 0, y = 13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 14, z = 0, y = 13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -15, z = 0, y = -14, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 15, z = 0, y = -14, ox = 0, oy = -0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 7, z = 0, y = 24, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 18, z = 0, y = 9, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 8, z = 0, y = -17, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -8, z = 0, y = -17, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 4, z = 0, y = -13, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = -4, z = 0, y = -13, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_imperial_style_01.iff", x = 0, z = 0, y = -9, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/static/structure/corellia/corl_power_bubble_node.iff", x = 8, z = 0, y = -13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/corellia/corl_power_transformer_s01.iff", x = -8, z = 0, y = -13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 14, z = 0, y = 17, ox = 0, oy = -0, oz = 0, ow = 1, cellid = -1, containmentType = -1 }, -- left turret front
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = -14, z = 0, y = 17, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 }, -- right turret front
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 0, z = 0, y = -13, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/minefield/field_1x1.iff", x = 0, z = 0, y = 26.5, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_hq_turret_control.iff", x = -2.75, z = 0, y = 1, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = 2, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_hq_turret_control.iff", x = -2.75, z = 0, y = 0, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = 2, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_hq_turret_control.iff", x = -2.75, z = 0, y = -1, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = 2, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_mission.iff", x = -0.27, z = -6.75, y = -6.52, ox = 0, oy = 0.707, oz = 0, ow = -0.707, cellid = 5, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_mission_imperial.iff", x = -8.9, z = -6.75, y = -6.47, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 5, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_bank.iff", x = -8.42, z = -6.77, y = -15.03, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = 6, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_bank.iff", x = 9.49, z = -6.77, y = -14.98, ox = 0, oy = 0.707, oz = 0, ow = -0.707, cellid = 6, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_hq_imperial.iff", x = 0, z = 0.25, y = 1.86, ox = 0, oy = 0, oz = 0, ow = 1, cellid = 2, containmentType = -1 },
	},

	childCreatureObjects = {

		{ mobile = "fbase_command_security_guard_hard", x = 2.6, z = 0, y = -4.1, cellid = 3, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_master_sergeant_hard", x = -6.7, z = -6.75, y = -0.8, cellid = 5, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_sergeant_hard", x = -0.1, z = -6.75, y = -6.3, cellid = 5, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_medic_hard", x = 9.3, z = -6.75, y = -15.0, cellid = 6, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_sharpshooter_hard", x = 2.5, z = -6.75, y = -16.5, cellid = 6, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_warrant_officer_ii_hard", x = 7.9, z = -6.75, y = -16.9, cellid = 6, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_sharpshooter_hard", x = 8, z = -6.75, y = -6, cellid = 7, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_sharpshooter_hard", x = 3.6, z = -6.75, y = -8.8, cellid = 7, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_sharpshooter_hard", x = 3.6, z = -6.75, y = -.25, cellid = 7, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_corporal_hard", x = -5.5, z = -13.75, y = -3, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_swamp_trooper_hard", x = .5, z = -13.75, y = -3, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_medic_hard", x = .5, z = -13.75, y = -6.6, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_comm_operator_hard", x = -5.5, z = -13.75, y = -6.6, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_noncom_hard", x = 1.8, z = -13.75, y = -12.6, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_specialist_noncom_hard", x = -7.6, z = -13.75, y = -12.6, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "fbase_imperial_first_lieutenant_hard", x = -2.5, z = -13.75, y = -14.9, cellid = 9, respawn = 300, containmentType = -1, heading = 0 },
		{ mobile = "imperial_recruiter", x = 0, z = -13.75, y = -17, cellid = 9, containmentType = -1, respawn = 60, heading = 0 },

	},

}

ObjectTemplates:addTemplate(object_building_faction_perk_hq_hq_s02_imp_pvp, "object/building/faction_perk/hq/hq_s02_imp_pvp.iff")
