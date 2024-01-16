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


object_ship_star_destroyer = object_ship_shared_star_destroyer:new {
	objectName = "@space/space_mobile_type:star_destroyer",

	name = "star_destroyer",
	type = "star_destroyer",
	faction = "imperial",
	difficulty = "",

	slideFactor = 1.88,
	chassisHitpoints = 100000,
	chassisMass = 50000,

	shipBitmask = NONE,
	customShipAiMap = "",
	optionsBitmask = AIENABLED,

	reactor = {name = "rct_generic", hitpoints = 50000, armor = 25000},
	engine = {name = "eng_incom_fusialthrust", hitpoints = 50000, armor = 25000, speed = 0, acceleration = 0, deceleration = 0, yaw = 0, yawRate = 0, pitch = 0, pitchRate = 0, roll = 0, rollRate = 0},
	shield_0 = {name = "shd_generic", hitpoints = 50000, armor = 25000, front = 75000, back = 75000, regen = 5},
	shield_1 = {name = "shd_generic", hitpoints = 50000, armor = 25000, front = 75000, back = 75000, regen = 5},
	armor_0 = {name = "arm_generic", hitpoints = 50000, armor = 25000},
	armor_1 = {name = "arm_generic", hitpoints = 50000, armor = 25000},
	capacitor = {name = "cap_generic", hitpoints = 50000, armor = 25000, energy = 1740.8, rechargeRate = 69.632},
	bridge = {name = "bdg_generic", hitpoints = 200000, armor = 200000},
	hangar = {name = "hgr_generic", hitpoints = 200000, armor = 200000},
	targeting_station = {name = "tst_generic", hitpoints = 200000, armor = 200000},
}

ObjectTemplates:addTemplate(object_ship_star_destroyer, "object/ship/star_destroyer.iff")
