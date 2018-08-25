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


object_building_faction_perk_hq_hq_s01_rebel_pvp = object_building_faction_perk_hq_shared_hq_s01_rebel_pvp:new {
	lotSize = 0,
	faction = "rebel",
	pvpFaction = "rebel",
	pvpStatusBitmask = OVERT,
	maintenanceCost = 0,
	baseMaintenanceRate = 0,
	dataObjectComponent = "DestructibleBuildingDataComponent",
	zoneComponent = "StructureZoneComponent",
	allowedZones = {"dantooine", "naboo", "rori","tatooine", "corellia", "lok", "talus"},
	constructionMarker = "object/building/player/construction/construction_player_house_generic_medium_style_01.iff",
	length = 7,
	width = 6,
	containerComponent = "GCWBaseContainerComponent",
	planetMapCategory = "rebel_hq",
	factionBaseType = 1,

	childObjects = {
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = 9.97, z = 0, y = 4.14, ox = 0, oy = -0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = -10.03, z = 0, y = 4.14, ox = 0, oy = -0.924, oz = 0, ow = 0.383, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = 12.97, z = 0, y = 11.14, ox = 0, oy = 0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = -13.03, z = 0, y = 11.14, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = 8.97, z = 0, y = 15.14, ox = 0, oy = 1, oz = 0, ow = 0, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_wall_weak_rebl_style_01.iff", x = -9.03, z = 0, y = 15.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = 6.97, z = 0, y = 1.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = -7.03, z = 0, y = 1.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = 12.97, z = 0, y = 7.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = -13.03, z = 0, y = 7.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = 12.97, z = 0, y = 15.14, ox = 0, oy = 0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = -13.03, z = 0, y = 15.14, ox = 0, oy = -0.383, oz = 0, ow = 0.924, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = 4.97, z = 0, y = 15.14, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = -1, containmentType = -1 },
		{ templateFile = "object/static/structure/military/military_column_weak_rebel_style_01.iff", x = -5.03, z = 0, y = 15.14, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = 16.3, z = 0, y = 9.72, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },
		{ templateFile = "object/installation/faction_perk/turret/tower_lg.iff", x = -16.3, z = 0, y = 9.72, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_hq_turret_control.iff", x = 1, z = 0, y = -1.76, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = 3, containmentType = -1 },
		{ templateFile = "object/tangible/terminal/terminal_hq_turret_control.iff", x = 1, z = 0, y = -0.6, ox = 0, oy = -0.707, oz = 0, ow = 0.707, cellid = 3, containmentType = -1 },

		{ templateFile = "object/installation/faction_perk/minefield/field_1x1.iff", x = 0, z = 0, y = 15.8, ox = 0, oy = 0, oz = 0, ow = 1, cellid = -1, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_hq_rebel.iff", x = 0, z = 0, y = -6, ox = 0, oy = 0, oz = 0, ow = 1, cellid = 3, containmentType = -1 },

		{ templateFile = "object/tangible/terminal/terminal_mission_rebel.iff", x = 4.03, z = 0, y = -0.16, ox = 0, oy = 0, oz = 0, ow = 1, cellid = 4, containmentType = -1 },
	},

	childCreatureObjects = {
		{ mobile = "fbase_rebel_cadet", x = 3.6, z = 0, y = 1.8, cellid = 4, respawn = 300, containmentType = -1, heading = 4.71 },
		{ mobile = "fbase_rebel_medic", x = -3.8, z = 0, y = -2.3, cellid = 6, respawn = 300, containmentType = -1, heading = 3.14},
		{ mobile = "fbase_rebel_squad_leader", x = -5.2, z = 0, y = 1, cellid = 2, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_rebel_soldier", x = 0, z = 0, y = -2.4, cellid = 3, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "fbase_rebel_guard_captain", x = 5.4, z = 0, y = -5.4, cellid = 5, respawn = 300, containmentType = -1, heading = 0},
		{ mobile = "rebel_recruiter", x = -1, z = 0, y = -5.5, cellid = 3, containmentType = -1, respawn = 60, heading = 0},
	},

}

ObjectTemplates:addTemplate(object_building_faction_perk_hq_hq_s01_rebel_pvp, "object/building/faction_perk/hq/hq_s01_rebel_pvp.iff")
