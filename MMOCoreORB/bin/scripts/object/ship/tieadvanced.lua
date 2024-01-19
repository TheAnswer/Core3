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


object_ship_tieadvanced = object_ship_shared_tieadvanced:new {
	objectName = "@space/space_mobile_type:tieadvanced",

	name = "tieadvanced",
	type = "tieadvanced",
	faction = "imperial",
	difficulty = "",

	slideFactor = 1.84,
	chassisHitpoints = 322,
	chassisMass = 10000,

	gameObjectType = 536870919,

	pvpStatusBitmask = ATTACKABLE + AGGRESSIVE,
	shipBitmask = NONE,
	optionsBitmask = AIENABLED,

	customShipAiMap = "",

	reactor = {name = "rct_generic", hitpoints = 100, armor = 200},
	engine = {name = "eng_novaldex_eventhorizon", hitpoints = 100, armor = 200, speed = 52, acceleration = 21.4, deceleration = 10.7, yaw = 28.1, yawRate = 54.3, pitch = 28.1, pitchRate = 54.3, roll = 28.1, rollRate = 54.3},
	shield_0 = {name = "shd_generic", hitpoints = 200, armor = 200, front = 400, back = 700, regen = 5},
	armor_0 = {name = "arm_generic", hitpoints = 500, armor = 500},
	armor_1 = {name = "arm_generic", hitpoints = 500, armor = 500},
	capacitor = {name = "cap_generic", hitpoints = 100, armor = 200, energy = 1468.8, rechargeRate = 58.752},
	weapon_0 = {name = "wpn_generic", hitpoints = 200, armor = 200, rate = 0.573, drain = 27.648, maxDamage = 320, minDamage = 210, shieldEfficiency = 0, armorEfficiency = 0, ammo = 0, ammo_type = 0},
	weapon_1 = {name = "wpn_sienar_heatseeker_missile_s01", hitpoints = 200, armor = 200, rate = 3, drain = 27.648, maxDamage = 27.648, minDamage = 27.648, shieldEfficiency = 0, armorEfficiency = 0, ammo = 50, ammo_type = 2},
}

ObjectTemplates:addTemplate(object_ship_tieadvanced, "object/ship/tieadvanced.iff")
