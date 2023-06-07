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


object_ship_merchant_cruiser_light_tier1 = object_ship_shared_merchant_cruiser_light_tier1:new {

	name = "merchant_cruiser_light_tier1",
	slideFactor = 1.76,
	chassisHitpoints = 2400,
	chassisMass = 5000,
	reactor = { name = "rct_generic", hitpoints = 1446.892, armor = 1493.025,},
	engine = { name = "eng_incom_fusialthrust", hitpoints = 1496.867, armor = 1486.276, speed = 40.83089, pitch = 22.13525, roll = 27.58622, yaw = 18.00296, acceleration = 9.778885, rollRate = 51.86822, pitchRate = 45.01803, deceleration = 8.352395, yawRate = 38.08244,},
	shield_0 = { name = "shd_generic", hitpoints = 1471.223, armor = 1444.745, regen = 4.342142, front = 5947.757, back = 5758.784,},
	armor_0 = { name = "arm_generic", hitpoints = 3090.839, armor = 3083.645,},
	armor_1 = { name = "arm_generic", hitpoints = 3137.472, armor = 3173.694,},
	capacitor = { name = "cap_generic", hitpoints = 1476.815, armor = 1456.402, rechargeRate = 98.06148, energy = 4994.55,},
	weapon_0 = { name = "wpn_incom_blaster", hitpoints = 1488.697, armor = 1428.24, rate = 0.7465913, drain = 0.9787179, maxDamage = 99.83712, shieldEfficiency = 0.5457929, minDamage = 72.11392, ammo = 0, ammo_type = 0, armorEfficiency = 0.5370855,},
}

ObjectTemplates:addTemplate(object_ship_merchant_cruiser_light_tier1, "object/ship/merchant_cruiser_light_tier1.iff")