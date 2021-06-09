local ObjectManager = require("managers.object.object_manager")

CorelliaStrongholdScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	buildingID = 2715899,
}

registerScreenPlay("CorelliaStrongholdScreenPlay", true)

function CorelliaStrongholdScreenPlay:start()
	if isZoneEnabled("corellia") then
		local pBuilding = getSceneObject(self.buildingID)

		if (pBuilding ~= nil) then
			createObserver(FACTIONBASEFLIPPED, "CorelliaStrongholdScreenPlay", "flipBase", pBuilding)

			if getRandomNumber(100) >= 50 then
				self:spawnRebels(pBuilding)
			else
				self:spawnImperials(pBuilding)
			end
		end
	end
end

function CorelliaStrongholdScreenPlay:flipBase(pBuilding)
	if (pBuilding == nil) then
		return 1
	end

	BuildingObject(pBuilding):destroyChildObjects()

	if BuildingObject(pBuilding):getFaction() == FACTIONIMPERIAL then
		self:spawnRebels(pBuilding)
	else
		self:spawnImperials(pBuilding)
	end

	return 0
end

function CorelliaStrongholdScreenPlay:spawnImperials(pBuilding)
	if (pBuilding == nil) then
		return
	end

	BuildingObject(pBuilding):initializeStaticGCWBase(FACTIONIMPERIAL)

	-- Imperial flipped (need to add AI *roaming* )
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4696.72, 25, -5797.99, 198.662, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4663.22, 25, -5781.71, 0.4, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4662.22, 25, -5769.71, 177.4, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4694.46, 25, -5803.03, 351.991, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4689.06, 25, -5796.24, 257.913, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4694.47, 25, -5806.45, 224.026, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4696.33, 25, -5804.40, 253.399, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4683.96, 25, -5743.68, 348.127, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4675.06, 25, -5746.75, 279.993, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4687.68, 25, -5754.00, 128.516, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4675.80, 25, -5752.56, 288.664, 0)
	BuildingObject(pBuilding):spawnChildCreature("dark_trooper", 360, 4679.01, 25, -5742.91, 9.66, 0)

	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 300, 4638.27, 25, -5777.52, 110, 0)
	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 300, 4637.55, 25, -5784.69, 185, 0)
	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 300, 4635.64, 25, -5802.22, 92, 0)
	BuildingObject(pBuilding):spawnChildCreature("imperial_major", 300, 4629.71, 25, -5770.51, 238, 0)
	BuildingObject(pBuilding):spawnChildCreature("imperial_surface_marshall", 300, 4531.47, 30.7809, -5823.83, 217, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4497.73, 28.5915, -5829.8, 277.53, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4680.98, 25, -5821.58, 250, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4678.34, 25, -5829.9, 145, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4675.82, 25, -5825, 195, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4678.32, 25, -5824.85, 256, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4688.47, 25, -5766.6, 272, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4693.39, 25, -5762.66, 34, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4695.82, 25, -5761.96, 325, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4691.89, 25, -5774.64, 209, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 300, 4691.92, 25, -5763.05, 240, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper_squad_leader", 300, 4675.99, 25, -5809.56, 216, 0)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper_squad_leader", 300, 4678.77, 25, -5757.9, 226, 0)

	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 400, -0.197, 0.125, -1.714, 0, 2715881)
	BuildingObject(pBuilding):spawnChildCreature("imperial_first_lieutenant", 400, 0.506, 0.125, -1.081, 0, 2716039)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper_medic", 400, 0.673, 0.125, -0.651, 0, 2716032)
	BuildingObject(pBuilding):spawnChildCreature("storm_commando", 400, -3.022, 0.125, -3.362, 0, 2716035)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 400, -0.207, 0.125, -1.570, 0, 2715952)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 400, 1.455, 0.125, -3.532, 0, 2715952)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 400, 0.879, 0.125, -2.578, 159, 2715909)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 400, -0.805, 0.125, -3.681, 226, 2715909)
	BuildingObject(pBuilding):spawnChildCreature("assault_trooper", 240, -0.197, 0.125, 1.538, 239, 2715902)
	BuildingObject(pBuilding):spawnChildCreature("imperial_surface_marshall", 400, 4.991, 0.125, -4.244, 64.122, 2715904)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 400, 0, 0.125, -2.85, 0, 2715916)
	BuildingObject(pBuilding):spawnChildCreature("imperial_noncom", 400, 0, 0.125, -5.25, 0, 2715916)
	BuildingObject(pBuilding):spawnChildCreature("stormtrooper", 400, 0.3, 0.125, 5.747, 202, 2715914)
	BuildingObject(pBuilding):spawnChildCreature("imperial_trooper", 400, 0.49, 0.125, -2.033, 0, 2715959)
	BuildingObject(pBuilding):spawnChildCreature("imperial_trooper", 400, -1.11, 0.125, -2.76, 0, 2715959)

end


function CorelliaStrongholdScreenPlay:spawnRebels(pBuilding)
	if (pBuilding == nil) then
		return
	end

	BuildingObject(pBuilding):initializeStaticGCWBase(FACTIONREBEL)

	--Rebel flipped (need to add AI *roaming* )
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4691.72, 25, -5802.99, 198.662, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4668.22, 25, -5786.71, 0.4, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4667.22, 25, -5764.71, 177.4, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4699.46, 25, -5808.03, 351.991, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4684.06, 25, -5791.24, 257.913, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4699.47, 25, -5801.45, 224.026, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4691.33, 25, -5809.40, 253.399, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4688.96, 25, -5748.68, 348.127, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4670.06, 25, -5741.75, 279.993, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4682.68, 25, -5759.00, 128.516, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4670.80, 25, -5757.56, 288.664, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_commando", 360, 4674.01, 25, -5747.91, 9.66, 0)

	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 300, 4633.27, 25, -5772.52, 110, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 300, 4632.55, 25, -5789.69, 185, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 300, 4630.64, 25, -5807.22, 92, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_major", 300, 4622.71, 25, -5775.51, 238, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_surface_marshall", 300, 4536.47, 30.7809, -5828.83, 217, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4497.73, 28.5915, -5829.8, 277.53, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4680.98, 25, -5821.58, 250, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4678.34, 25, -5829.9, 145, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4675.82, 25, -5825, 195, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4678.32, 25, -5824.85, 256, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4688.47, 25, -5766.6, 272, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4693.39, 25, -5762.66, 34, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4695.82, 25, -5761.96, 325, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4691.89, 25, -5774.64, 209, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 300, 4691.92, 25, -5763.05, 240, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_staff_sergeant", 300, 4670.99, 25, -5804.56, 216, 0)
	BuildingObject(pBuilding):spawnChildCreature("rebel_staff_sergeant", 300, 4673.77, 25, -5752.9, 226, 0)

	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 400, -0.694, 0.125, -1.217, 0, 2715881)
	BuildingObject(pBuilding):spawnChildCreature("rebel_first_lieutenant", 400, 0.903, 0.125, -1.584, 0, 2716039)
	BuildingObject(pBuilding):spawnChildCreature("rebel_medic", 400, 0.173, 0.125, -0.151, 0, 2716032)
	BuildingObject(pBuilding):spawnChildCreature("rebel_master_sergeant", 400, -3.522, 0.125, -3.862, 0, 2716035)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, -0.707, 0.125, -1.170, 0, 2715952)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 1.955, 0.125, -3.132, 0, 2715952)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 0.379, 0.125, -2.178, 159, 2715909)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, -0.305, 0.125, -3.181, 226, 2715909)
	BuildingObject(pBuilding):spawnChildCreature("rebel_specforce_pathfinder", 240, -0.697, 0.125, 1.138, 239, 2715902)
	BuildingObject(pBuilding):spawnChildCreature("rebel_surface_marshall", 400, 4.491, 0.125, -4.744, 64.122, 2715904)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 0.5, 0.125, -2.35, 0, 2715916)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 0.5, 0.125, -5.75, 0, 2715916)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 0.8, 0.125, 5.247, 202, 2715914)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, 0.9, 0.125, -2.533, 0, 2715959)
	BuildingObject(pBuilding):spawnChildCreature("rebel_trooper", 400, -1.61, 0.125, -2.26, 0, 2715959)
end
