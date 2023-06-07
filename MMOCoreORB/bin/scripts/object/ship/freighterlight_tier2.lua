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


object_ship_freighterlight_tier2 = object_ship_shared_freighterlight_tier2:new {

	name = "freighterlight_tier2",
	slideFactor = 1.83,
	chassisHitpoints = 1200,
	chassisMass = 5000,
	reactor = { name = "rct_generic", hitpoints = 891.769, armor = 892.1426,},
	engine = { name = "eng_incom_fusialthrust", hitpoints = 870.8876, armor = 892.5297, speed = 43.07119, pitch = 23.3361, roll = 28.84083, yaw = 18.3094, acceleration = 10.47344, rollRate = 51.51069, pitchRate = 46.58502, deceleration = 8.825667, yawRate = 39.08226,},
	shield_0 = { name = "shd_generic", hitpoints = 863.2429, armor = 883.3234, regen = 5.285785, front = 5260.168, back = 5329.212,},
	armor_0 = { name = "arm_generic", hitpoints = 2780.953, armor = 2843.214,},
	armor_1 = { name = "arm_generic", hitpoints = 2816.253, armor = 2843.832,},
	capacitor = { name = "cap_generic", hitpoints = 882.5841, armor = 871.087, rechargeRate = 98.38865, energy = 4834.216,},
}

ObjectTemplates:addTemplate(object_ship_freighterlight_tier2, "object/ship/freighterlight_tier2.iff")