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


object_ship_ykl37r_tier3 = object_ship_shared_ykl37r_tier3:new {

	name = "ykl37r_tier3",
	slideFactor = 1.79,
	chassisHitpoints = 2400,
	chassisMass = 5000,
	reactor = { name = "rct_generic", hitpoints = 2080.755, armor = 2145.161,},
	engine = { name = "eng_incom_fusialthrust", hitpoints = 2128.506, armor = 2144.003, speed = 43.47858, pitch = 23.44729, roll = 29.99097, yaw = 19.09342, acceleration = 10.43502, rollRate = 52.69967, pitchRate = 48.05073, deceleration = 9.220764, yawRate = 39.65031,},
	shield_0 = { name = "shd_generic", hitpoints = 2141.82, armor = 2143.792, regen = 18.37355, front = 8501.395, back = 8445.832,},
	armor_0 = { name = "arm_generic", hitpoints = 4542.224, armor = 4557.917,},
	armor_1 = { name = "arm_generic", hitpoints = 4520.05, armor = 4446.161,},
	capacitor = { name = "cap_generic", hitpoints = 2082.372, armor = 2098.827, rechargeRate = 99.75916, energy = 4816.073,},
	weapon_0 = { name = "wpn_incom_blaster", hitpoints = 2100.111, armor = 2061.574, rate = 0.7754144, drain = 0.9926187, maxDamage = 321.1286, shieldEfficiency = 0.5493294, minDamage = 220.6941, ammo = 0, ammo_type = 0, armorEfficiency = 0.5596024,},
	weapon_1 = { name = "wpn_incom_blaster", hitpoints = 2090.968, armor = 2073.421, rate = 0.7617209, drain = 0.9826091, maxDamage = 316.511, shieldEfficiency = 0.5426804, minDamage = 213.5753, ammo = 0, ammo_type = 0, armorEfficiency = 0.5570778,},
	weapon_2 = { name = "wpn_seinar_concussion_missile_s01", hitpoints = 2156.933, armor = 2121.685, rate = 2.91845, drain = 0.9799531, maxDamage = 931.6467, shieldEfficiency = 0.7374239, minDamage = 431.6384, ammo = 4, ammo_type = 18, armorEfficiency = 0.7335144,},
}

ObjectTemplates:addTemplate(object_ship_ykl37r_tier3, "object/ship/ykl37r_tier3.iff")