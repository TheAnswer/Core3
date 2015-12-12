local ObjectManager = require("managers.object.object_manager")
local ScreenPlay = require("screenplays.screenplay")
local FsMedicPuzzle = require("managers.jedi.village.phase1.fs_medic_puzzle")
local VillageJediManagerCommon = require("managers.jedi.village.village_jedi_manager_common")

require("screenplays.village.village_spawn_table")

-- Utils.
local Logger = require("utils.logger")
require("utils.helpers")

VillageJediManagerTownship = ScreenPlay:new {
	screenplayName = "VillageJediManagerTownship"
}

VILLAGE_TOTAL_NUMBER_OF_PHASES = 2 -- Temporarily set to 2 for testing until other phases begin development

local VILLAGE_PHASE_CHANGE_TIME = 24 * 60 * 60 * 1000 -- Testing value.
--local VILLAGE_PHASE_CHANGE_TIME = 5 * 60 * 1000
--local VILLAGE_PHASE_CHANGE_TIME = 3 * 7 * 24 * 60 * 60 * 1000 -- Three Weeks.

-- Set the current Village Phase for the first time.
function VillageJediManagerTownship.setCurrentPhaseInit()
	if (not hasServerEvent("VillagePhaseChange")) then
		VillageJediManagerTownship.setCurrentPhase(1)
		VillageJediManagerTownship.setCurrentPhaseID(1)
		createServerEvent(VILLAGE_PHASE_CHANGE_TIME, "VillageJediManagerTownship", "switchToNextPhase", "VillagePhaseChange")
	end
end

function VillageJediManagerTownship.setCurrentPhaseID(phaseID)
	setQuestStatus("Village:phaseID", phaseID)
end

function VillageJediManagerTownship.getCurrentPhaseID()
	local curPhase = tonumber(getQuestStatus("Village:phaseID"))

	if (curPhase == nil) then
		return 1
	end

	return curPhase
end

-- Set the current Village Phase.
function VillageJediManagerTownship.setCurrentPhase(nextPhase)
	setQuestStatus("Village:CurrentPhase", nextPhase)
end

function VillageJediManagerTownship.getCurrentPhase()
	local curPhase = tonumber(getQuestStatus("Village:CurrentPhase"))

	if (curPhase == nil) then
		return 1
	end

	return curPhase
end

function VillageJediManagerTownship:switchToNextPhase()
	local currentPhase = VillageJediManagerTownship.getCurrentPhase()
	local phaseID = VillageJediManagerTownship.getCurrentPhaseID()
	VillageJediManagerTownship:despawnMobiles(currentPhase)
	VillageJediManagerTownship:despawnSceneObjects(currentPhase)
	VillageJediManagerTownship:handlePhaseChangeActiveQuests(phaseID, currentPhase)

	currentPhase = currentPhase + 1
	if currentPhase > VILLAGE_TOTAL_NUMBER_OF_PHASES then
		currentPhase = 1
	end

	VillageJediManagerTownship.setCurrentPhase(currentPhase)
	VillageJediManagerTownship.setCurrentPhaseID(phaseID + 1)
	VillageJediManagerTownship:spawnMobiles(currentPhase, false)
	VillageJediManagerTownship:spawnSceneObjects(currentPhase, false)
	Logger:log("Switching village phase to " .. currentPhase, LT_INFO)

	-- Schedule another persistent event.
	if (not hasServerEvent("VillagePhaseChange")) then
		createServerEvent(VILLAGE_PHASE_CHANGE_TIME, "VillageJediManagerTownship", "switchToNextPhase", "VillagePhaseChange")
	end
end

function VillageJediManagerTownship:start()
	if (isZoneEnabled("dathomir")) then
		Logger:log("Starting the Village Township Screenplay.", LT_INFO)
		VillageJediManagerTownship.setCurrentPhaseInit()
		VillageJediManagerTownship:spawnMobiles(VillageJediManagerTownship.getCurrentPhase(), true)
		VillageJediManagerTownship:spawnSceneObjects(VillageJediManagerTownship.getCurrentPhase(), true)
	end
end

-- Spawning functions.

function VillageJediManagerTownship:spawnMobiles(currentPhase, spawnStaticMobs)
	if (spawnStaticMobs == true) then
		local mobileTable = villageMobileSpawns[0]

		for i = 1, table.getn(mobileTable), 1 do
			local mobile = mobileTable[i]
			local pMobile = spawnMobile("dathomir", mobile[1], 0, mobile[2], mobile[3], mobile[4], mobile[5], 0)
			if (pMobile ~= nil) then
				CreatureObject(pMobile):setPvpStatusBitmask(0)
				if (mobile[6] ~= "") then
					self[mobile[6]](pMobile)
				end
				if (mobile[7] ~= "") then
					CreatureObject(pMobile):setOptionsBitmask(136)
					AiAgent(pMobile):setConvoTemplate(mobile[7])
				end
			end
		end
	end

	local mobileTable = villageMobileSpawns[currentPhase]

	for i = 1, table.getn(mobileTable), 1 do
		local mobile = mobileTable[i]
		local pMobile = spawnMobile("dathomir", mobile[1], 0, mobile[2], mobile[3], mobile[4], mobile[5], 0)

		if (pMobile ~= nil) then
			CreatureObject(pMobile):setPvpStatusBitmask(0)
			if (mobile[6] ~= "") then
				self[mobile[6]](pMobile)
			end
			if (mobile[7] ~= "") then
				CreatureObject(pMobile):setOptionsBitmask(136)
				AiAgent(pMobile):setConvoTemplate(mobile[7])
			end
			local mobileID = SceneObject(pMobile):getObjectID()
			writeData("village:npc:object:" .. i, mobileID)
		end
	end
end

-- Despawn and cleanup current phase mobiles.
function VillageJediManagerTownship:despawnMobiles(currentPhase)
	local mobileTable = villageMobileSpawns[currentPhase]
	for i = 1, table.getn(mobileTable), 1 do
		local objectID = readData("village:npc:object:" .. i)
		local pMobile = getSceneObject(objectID)

		if (pMobile ~= nil) then
			SceneObject(pMobile):destroyObjectFromWorld()
			deleteData("village:npc:object:" .. i)
		end
	end
end

function VillageJediManagerTownship:spawnSceneObjects(currentPhase, spawnStaticObjects)
	if (spawnStaticObjects == true) then
		local objectTable = villageObjectSpawns[0]
		foreach(objectTable, function(sceneObject)
			spawnSceneObject("dathomir", sceneObject[1], sceneObject[2], sceneObject[3], sceneObject[4], 0, sceneObject[5])
		end)
	end

	local objectTable = villageObjectSpawns[currentPhase]
	for i = 1, table.getn(objectTable), 1 do
		local sceneObject = objectTable[i]
		local pObject = spawnSceneObject("dathomir", sceneObject[1], sceneObject[2], sceneObject[3], sceneObject[4], 0, sceneObject[5])

		if (pObject ~= nil) then
			local objectID = SceneObject(pObject):getObjectID()
			writeData("village:scene:object:" .. i, objectID)
		end
	end
end

-- Despawn and cleanup current phase scene objects.
function VillageJediManagerTownship:despawnSceneObjects(currentPhase)
	local objectTable = villageObjectSpawns[currentPhase]
	for i = 1, table.getn(objectTable), 1 do
		local objectID = readData("village:scene:object:" .. i)
		local pObject = getSceneObject(objectID)

		if (pObject ~= nil) then
			SceneObject(pObject):destroyObjectFromWorld()
			deleteData("village:npc:object:" .. i)
		end
	end
end

function VillageJediManagerTownship:handlePhaseChangeActiveQuests(phaseID, currentPhase)
	local pMap = VillageJediManagerCommon.getActiveQuestList(phaseID)

	if (pMap == nil) then
		return
	end

	local questMap = LuaQuestVectorMap(pMap)
	local mapSize = questMap:getMapSize()

	for i = 1, mapSize, 1 do
		local playerID = tonumber(questMap:getMapKeyAtIndex(i - 1))

		local pPlayer = getSceneObject(playerID)

		if (pPlayer ~= nil) then
			ObjectManager.withCreaturePlayerObject(pPlayer, function(playerObject)
				if (playerObject:isOnline()) then
					self:doOnlinePhaseChangeFails(pPlayer, currentPhase)
				end
			end)
		end
	end
	
	VillageJediManagerCommon.removeActiveQuestList(phaseID)
end

function VillageJediManagerTownship:doOnlinePhaseChangeFails(pCreature, currentPhase)
	if (currentPhase == 1) then
		local FsReflex1 = require("managers.jedi.village.phase1.fs_reflex1")
		FsReflex1:doPhaseChangeFail(pCreature)

		local FsPatrol = require("managers.jedi.village.phase1.fs_patrol")
		FsPatrol:doPhaseChangeFail(pCreature)

		FsMedicPuzzle:doPhaseChange(pCreature)
	elseif (currentPhase == 2) then
		local FsReflex2 = require("managers.jedi.village.phase2.fs_reflex2")
		FsReflex2:doPhaseChangeFail(pCreature)

		local FsSad = require("managers.jedi.village.phase2.fs_sad")
		FsSad:doPhaseChangeFail(pCreature)
	end
end

function VillageJediManagerTownship.initPukingVillager(pNpc)
	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doPukingVillager", pNpc) -- 2-5 minute initial delay
end

function VillageJediManagerTownship:doPukingVillager(pNpc)
	if (pNpc == nil) then
		return
	end

	CreatureObject(pNpc):doAnimation("heavy_cough_vomit")

	spatialChat(pNpc, "@quest/force_sensitive/fs_sick:fs_response0" .. getRandomNumber(1,7))

	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doPukingVillager", pNpc) -- 2-5 minute delay
end

function VillageJediManagerTownship.initPanickedVillager(pNpc)
	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doPanickedVillager", pNpc) -- 2-5 minute initial delay
end

function VillageJediManagerTownship:doPanickedVillager(pNpc)
	if (pNpc == nil) then
		return
	end

	local rand = getRandomNumber(1,20)

	CreatureObject(pNpc):doAnimation("weeping")

	if (rand < 10) then
		spatialChat(pNpc, "@quest/force_sensitive/fs_panicked:fs_response0" .. rand)
	else
		spatialChat(pNpc, "@quest/force_sensitive/fs_panicked:fs_response" .. rand)
	end

	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doPanickedVillager", pNpc) -- 2-5 minute delay
end

function VillageJediManagerTownship.initWoundedVillager(pNpc)
	if (pNpc == nil) then
		return
	end

	CreatureObject(pNpc):setPosture(KNOCKEDDOWN)
	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doWoundedVillager", pNpc) -- 2-5 minute initial delay
end

function VillageJediManagerTownship:doWoundedVillager(pNpc)
	if (pNpc == nil) then
		return
	end

	local rand = getRandomNumber(1,10)

	if (rand < 10) then
		spatialChat(pNpc, "@quest/force_sensitive/fs_lamentations:fs_response0" .. rand)
	else
		spatialChat(pNpc, "@quest/force_sensitive/fs_lamentations:fs_response" .. rand)
	end

	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doWoundedVillager", pNpc) -- 2-5 minute delay
end

function VillageJediManagerTownship.initMedDroid(pNpc)
	if (pNpc ~= nil) then
		SceneObject(pNpc):setContainerComponent("MedDroidContainerComponent")
	end
end

function VillageJediManagerTownship.initVillageRepairer(pNpc)
	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doVillageRepairer", pNpc) -- 2-5 minute initial delay
end

function VillageJediManagerTownship:doVillageRepairer(pNpc)
	if (pNpc == nil) then
		return
	end

	local rand = getRandomNumber(1,10)

	if (rand < 5) then
		CreatureObject(pNpc):doAnimation("manipulate_medium")
	else
		CreatureObject(pNpc):doAnimation("manipulate_high")
	end

	rand = getRandomNumber(1,2)

	if (rand == 1) then
		rand = getRandomNumber(1,7)
		spatialChat(pNpc, "@quest/force_sensitive/fs_wall_repair:fs_response0" .. rand)
	end

	createEvent(getRandomNumber(120, 300) * 1000, "VillageJediManagerTownship", "doVillageRepairer", pNpc) -- 2-5 minute delay
end

MedDroidContainerComponent = {}

function MedDroidContainerComponent:transferObject(pContainer, pObj, slot)
	local pPlayer = VillageJediManagerTownship:getObjOwner(pObj)

	if (pPlayer == nil or pContainer == nil) then
		return 0
	end

	FsMedicPuzzle:cureSymptoms(pPlayer, pContainer, pObj)

	return 1
end

function MedDroidContainerComponent:canAddObject(pContainer, pObj, slot)
	local pPlayer = VillageJediManagerTownship:getObjOwner(pObj)

	if (pPlayer == nil or pContainer == nil) then
		return -1
	end

	if (SceneObject(pObj):getTemplateObjectPath() ~= "object/tangible/item/quest/force_sensitive/fs_medic_puzzle_heal_pack.iff") then
		return -1
	end

	if FsMedicPuzzle:hasAnySymptoms(pPlayer, pContainer) then
		return true
	else
		return -1
	end
end

function MedDroidContainerComponent:removeObject(pContainer, pObj, slot)
	return -1
end

function VillageJediManagerTownship:getObjOwner(pObj)
	if (pObj == nil) then
		return nil
	end

	local pPlayerInv = SceneObject(pObj):getParent()

	if (pPlayerInv == nil) then
		return nil
	end

	local parent = SceneObject(pPlayerInv):getParent()

	if (parent == nil) then
		return nil
	end

	if (SceneObject(parent):isCreatureObject()) then
		return parent
	end

	return nil
end

registerScreenPlay("VillageJediManagerTownship", true)

return VillageJediManagerTownship
