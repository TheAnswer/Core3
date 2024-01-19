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


object_ship_z95_tier5 = object_ship_shared_z95_tier5:new {
	objectName = "@space/space_mobile_type:z95_tier5",

	name = "z95_tier5",
	type = "z95_tier5",
	faction = "rebel",
	difficulty = "tier5",

	slideFactor = 1.84,
	chassisHitpoints = 2457.6,
	chassisMass = 5000,

	gameObjectType = 536870919,

	pvpStatusBitmask = ATTACKABLE + AGGRESSIVE,
	shipBitmask = NONE,
	optionsBitmask = AIENABLED,

	customShipAiMap = "",

	reactor = {name = "rct_generic", hitpoints = 3185.05, armor = 3185.05},
	engine = {name = "eng_incom_fusialthrust", hitpoints = 3185.05, armor = 3185.05, speed = 52, acceleration = 12.63, deceleration = 9.79, yaw = 20.6, yawRate = 43.22, pitch = 26, pitchRate = 52, roll = 32.19, rollRate = 58.64},
	shield_0 = {name = "shd_generic", hitpoints = 3185.05, armor = 3185.05, front = 14014.2, back = 14014.2, regen = 23.15},
	armor_0 = {name = "arm_generic", hitpoints = 5435.82, armor = 5435.82},
	armor_1 = {name = "arm_generic", hitpoints = 5435.82, armor = 5435.82},
	capacitor = {name = "cap_generic", hitpoints = 3185.05, armor = 3185.05, energy = 5000, rechargeRate = 100},
	booster = {name = "bst_generic", hitpoints = 3185.05, armor = 3185.05, energy = 100, recharge = 3, consumption = 10, acceleration = 20, speed = 15},
	weapon_0 = {name = "wpn_z95_blaster", hitpoints = 3185.05, armor = 3185.05, rate = 0.78, drain = 1, maxDamage = 312.72, minDamage = 194.71, shieldEfficiency = 0.56, armorEfficiency = 0.56, ammo = 0, ammo_type = 0},
}

ObjectTemplates:addTemplate(object_ship_z95_tier5, "object/ship/z95_tier5.iff")
