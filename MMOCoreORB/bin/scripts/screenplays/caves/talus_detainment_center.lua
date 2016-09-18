local ObjectManager = require("managers.object.object_manager")

TalusDetainmentCenterScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "TalusDetainmentCenterScreenPlay",

	buildingID = 9686208,

--9686208 bunker imperial detainment center
--9686346 terminal hq
--9686357 override terminal
--9686389 power regulator
--9686393 security terminal
--9686398 uplink terminal
}

registerScreenPlay("TalusDetainmentCenterScreenPlay", true)

function TalusDetainmentCenterScreenPlay:start()
	if (isZoneEnabled("talus")) then
		local pBuilding = getSceneObject(self.buildingID)
		createObserver(FACTIONBASEFLIPPED, "TalusDetainmentCenterScreenPlay", "flipBase", pBuilding)

		if getRandomNumber(100) >= 50 then
			self:spawnRebels(pBuilding)
		else
			self:spawnImperials(pBuilding)
		end
	end
end

function TalusDetainmentCenterScreenPlay:flipBase(pBuilding)
	if (pBuilding == nil) then
		return 1
	end

	BuildingObject(pBuilding):destroyChildObjects()

	if BuildingObject(pBuilding):getFaction() == FACTIONIMPERIAL then
		self:spawnRebels(pBuilding)
	elseif BuildingObject(pBuilding):getFaction() == FACTIONREBEL then
		self:spawnImperials(pBuilding)
	end

	return 0
end

function TalusDetainmentCenterScreenPlay:spawnImperials(pBuilding)
	BuildingObject(pBuilding):initializeStaticGCWBase(FACTIONIMPERIAL)

	BuildingObject(pBuilding):spawnChildSceneObject("object/tangible/gcw/flip_banner_onpole_imperial.iff", 3.5, -9.0, 35.1, 9686212, 1, 0, 0, 0)

	BuildingObject(pBuilding):spawnChildCreature("imperial_staff_corporal", 300, 3.5, -9.0, 32.6, 179, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, 4.8, -9.0, 35.1, 179, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, 2.2, -9.0, 35.1, 179, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("imperial_warrant_officer_i", 300, -52.6, -9.0, -6.6, -88, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_private", 300, -58.4, -9.0, 7.1, 150, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 300, -72.9, -9.0, 21.2,  165, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_staff_corporal", 300, -54.5, -9.0, 67.1, 179, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 300, -52.3, -9.0, 63.2, 90, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 300, -53.2, -9.0, 59.0, 90, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 300, -57.0, -9.0, 60.9, -120, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, -24.9, -9.0, 28.5, 90, 9686213)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, -24.8, -9.0, 26.0, 90, 9686213)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, 12.1, -17.0, 65.0, 179, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("command_security_guard", 300, 12.1, -17.0, 62.0, -1, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 30.4, -17.0, 60.7, 1, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 30.4, -17.0, 65.9, 179, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("imperial_army_captain", 300, 34.5, -17.0, 63.5, -90, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 300, -1.2, -23.0, 225.4, 179, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 300, -3.8, -23.0, 225.4, 179, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 300, -19.8, -23.0, 234.4, 179, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 1.0, -23.0, 192.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -2.0, -23.0, 192.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -5.0, -23.0, 192.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -8.0, -23.0, 192.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 1.0, -23.0, 196.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -2.0, -23.0, 196.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -5.0, -23.0, 196.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, -8.0, -23.0, 196.3, 90, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 300, 7.1, -23.0, 198.5, -135, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("imperial_inquisitor", 900, -80.5, -23.0, 259.4, 88, 9686223)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper_colonel", 600, 45.2, -23.0, 276.3, -179, 9686225)
	BuildingObject(pBuilding):spawnChildCreature("senior_prophet_of_the_dark_side", 10800, -2.7, -23.0, 268.5, 90, 9686224)
end

function TalusDetainmentCenterScreenPlay:spawnRebels(pBuilding)
	BuildingObject(pBuilding):initializeStaticGCWBase(FACTIONREBEL)

	BuildingObject(pBuilding):spawnChildSceneObject("object/tangible/gcw/flip_banner_onpole_rebel.iff", 3.5, -9.0, 35.1, 9686212, 1, 0, 0, 0)

	BuildingObject(pBuilding):spawnChildCreature("rebel_staff_corporal", 300, 5.3, -9.0, 29.7, -1, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, 6.9, -9.0, 33.9, 0, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, 0.0, -9.0, 33.9, -6, 9686212)
	BuildingObject(pBuilding):spawnChildCreature("rebel_warrant_officer_i", 300, -48.1, -9.0, -6.6, 90, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("rebel_corporal", 300, -58.4, -9.0, 3.2, 0, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("specforce_technician", 300, -78.2, -9.0, 21.5,  173, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("rebel_staff_corporal", 300, -52.0, -9.0, 61.6, 88, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("specforce_technician", 300, -57.0, -9.0, 58.7, -90, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("specforce_technician", 300, -57.0, -9.0, 47.0, -96, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("specforce_technician", 300, -52.0, -9.0, 43.3, 91, 9686214)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, -47.0, -9.0, 26.0, -176, 9686213)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, -31.2, -9.0, 28.8, -2, 9686213)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, 14.5, -17.0, 59.8, 85, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("specforce_marine", 300, 26.0, -17.0, 67.0, 73, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 25.0, -17.0, 58.8, -125, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 34.2, -17.0, 66.9, 4, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("rebel_army_captain", 300, 34.6, -17.0, 61.0, 98, 9686215)
	BuildingObject(pBuilding):spawnChildCreature("senior_specforce_infiltrator", 300, -17.3, -23.0, 227.3, 168, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("senior_specforce_infiltrator", 300, -18.0, -23.0, 232.7, -102, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("rebel_lieutenant_colonel", 300, -25.5, -23.0, 233.4, -93, 9686222)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 14.0, -23.0, 192.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 14.0, -23.0, 189.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 14.0, -23.0, 186.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 14.0, -23.0, 183.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 10.0, -23.0, 192.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 10.0, -23.0, 189.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 10.0, -23.0, 186.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 10.0, -23.0, 183.3, 0, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 300, 6.8, -23.0, 203.1, -9, 9686217)
	BuildingObject(pBuilding):spawnChildCreature("rebel_lieutenant_general", 600, -79.8, -23.0, 259.1, 89, 9686223)
	BuildingObject(pBuilding):spawnChildCreature("specops_alliance_free_agent", 600, 43.2, -23.0, 278.3, -177, 9686225)
	BuildingObject(pBuilding):spawnChildCreature("lesser_prophet_of_the_light_side", 10800, -3.7, -23.0, 269.5, 90, 9686224)
end

