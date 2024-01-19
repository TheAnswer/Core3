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


object_ship_tiefighter = object_ship_shared_tiefighter:new {
	objectName = "@space/space_mobile_type:tiefighter",

	name = "tiefighter",
	type = "tiefighter",
	faction = "imperial",
	difficulty = "",

	slideFactor = 1.75,
	chassisHitpoints = 412,
	chassisMass = 10000,

	gameObjectType = 536870919,

	pvpStatusBitmask = ATTACKABLE + AGGRESSIVE,
	shipBitmask = NONE,
	optionsBitmask = AIENABLED,

	customShipAiMap = "",

	reactor = {name = "rct_generic", hitpoints = 100, armor = 200},
	engine = {name = "eng_novaldex_eventhorizon", hitpoints = 100, armor = 200, speed = 48, acceleration = 14.2, deceleration = 6.3, yaw = 26.5, yawRate = 51.8, pitch = 26.5, pitchRate = 51.8, roll = 26.5, rollRate = 51.8},
	shield_0 = {name = "shd_generic", hitpoints = 200, armor = 200, front = 0, back = 0, regen = 5},
	armor_0 = {name = "arm_generic", hitpoints = 500, armor = 500},
	armor_1 = {name = "arm_generic", hitpoints = 500, armor = 500},
	capacitor = {name = "cap_generic", hitpoints = 100, armor = 200, energy = 1305.6, rechargeRate = 52.224},
	weapon_0 = {name = "wpn_generic", hitpoints = 200, armor = 200, rate = 0.509, drain = 24.576, maxDamage = 220, minDamage = 180, shieldEfficiency = 0, armorEfficiency = 0, ammo = 0, ammo_type = 0},
	weapon_1 = {name = "wpn_seinar_concussion_missile_s01", hitpoints = 200, armor = 200, rate = 3, drain = 24.576, maxDamage = 24.576, minDamage = 24.576, shieldEfficiency = 0, armorEfficiency = 0, ammo = 50, ammo_type = 2},
	weapon_2 = {name = "wpn_std_countermeasure", hitpoints = 100, armor = 200, rate = 1, drain = 24.576, maxDamage = 24.576, minDamage = 24.576, shieldEfficiency = 0, armorEfficiency = 0, ammo = 50, ammo_type = 7},
}

ObjectTemplates:addTemplate(object_ship_tiefighter, "object/ship/tiefighter.iff")
