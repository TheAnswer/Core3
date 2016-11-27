local ObjectManager = require("managers.object.object_manager")

EmperorsRetreatScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

		screenplayName = "EmperorsRetreatScreenPlay",
}

registerScreenPlay("EmperorsRetreatScreenPlay", true)

function EmperorsRetreatScreenPlay:start()
	if (isZoneEnabled("naboo")) then
		self:spawnMobiles()
		self:spawnSceneObjects()
	end
end

function EmperorsRetreatScreenPlay:spawnSceneObjects()
	local elevatorUp = spawnSceneObject("naboo", "object/tangible/terminal/terminal_elevator_up.iff", 13, 1, -36, 1418877, 1, 0, 0, 0)
	spawnSceneObject("naboo", "object/tangible/terminal/terminal_elevator_down.iff", 13, 20.5, -36, 1418877, 1, 0, 0, 0)

	if (elevatorUp ~= nil) then
		SceneObject(elevatorUp):setObjectMenuComponent("EmperorElevatorMenuComponent")
	end
end

EmperorElevatorMenuComponent = {}

function EmperorElevatorMenuComponent:fillObjectMenuResponse(pSceneObject, pMenuResponse, pPlayer)
	local menuResponse = LuaObjectMenuResponse(pMenuResponse)
	menuResponse:addRadialMenuItem(198, 3, "@elevator_text:up")
end

function EmperorElevatorMenuComponent:handleObjectMenuSelect(pSceneObject, pPlayer, selectedID)
	if (pPlayer == nil) then
		return 0
	end

	if (selectedID ~= 198) or (not CreatureObject(pPlayer):hasScreenPlayState(16, "imperial_theme_park")) then
		CreatureObject(pPlayer):sendSystemMessage("@theme_park_imperial/warning:emperor")
		return 0
	end

	if (CreatureObject(pPlayer):getParent() ~= SceneObject(pSceneObject):getParent()) then
		return 0
	end

	local z = SceneObject(pSceneObject):getPositionZ() + 20
	local x = CreatureObject(pPlayer):getPositionX()
	local y = CreatureObject(pPlayer):getPositionY()

	CreatureObject(pPlayer):playEffect("clienteffect", "elevator_ascend.cef")
	CreatureObject(pPlayer):teleport(x, z, y, SceneObject(pSceneObject):getParentID())

	return 0
end

function EmperorsRetreatScreenPlay:retreatPatrolDestReached(pMobile)
	if (pMobile == nil) then
		return 0
	end

	local curLoc = readData(SceneObject(pMobile):getObjectID() .. ":currentLoc")

	if (curLoc == 1) then
		writeData(SceneObject(pMobile):getObjectID() .. ":currentLoc", 2)
	else
		writeData(SceneObject(pMobile):getObjectID() .. ":currentLoc", 1)
	end

	createEvent(getRandomNumber(350,450) * 100, "EmperorsRetreatScreenPlay", "droidPatrol", pMobile, "")

	return 0
end

function EmperorsRetreatScreenPlay:droidPatrol(pMobile)
	if (pMobile == nil) then
		return
	end
	local name = readStringData(SceneObject(pMobile):getObjectID() .. ":name")
	local curLoc = readData(SceneObject(pMobile):getObjectID() .. ":currentLoc")
	local nextLoc

	if (name == "droid1") then
		if (curLoc == 1) then

			nextLoc = { 12.25, 0.2, -24.38, 1418874}
		else
			nextLoc = { -52.15, 0.2, -23.82, 1418884 }
		end
	end

	if (name == "droid2") then
		if (curLoc == 1) then

			nextLoc = { -8.91, 0.2, -12.67, 1418879}
		else
			nextLoc = { 23.83, 0.2, -40.42, 1418875 }
		end
	end

	if (name == "droid3") then
		if (curLoc == 1) then

			nextLoc = { 11.63, .2, -51.12, 1418876}
		else
			nextLoc = { -45.79, 0.2, -12.5, 1418879}
		end
	end

	AiAgent(pMobile):stopWaiting()
	AiAgent(pMobile):setWait(0)
	AiAgent(pMobile):setNextPosition(nextLoc[1], nextLoc[2], nextLoc[3], nextLoc[4])
	AiAgent(pMobile):executeBehavior()

end

function EmperorsRetreatScreenPlay:spawnMobiles()

	-- Inside
	local pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 11.2, 0.2, -31.5, 0, 1418874)
	self:setMoodString(pNpc, "npc_imperial")
	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 14.8, 0.2, -31.5, 0, 1418874)
	self:setMoodString(pNpc, "npc_imperial")

	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 10.8, 20, -27.2, 180, 1418886)
	self:setMoodString(pNpc, "npc_imperial")
	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 15.3, 20, -27.2, 180, 1418886)
	self:setMoodString(pNpc, "npc_imperial")

	spawnMobile("naboo", "ra7_bug_droid", 120, 4.0, 0.2, -45.7, 4, 1418876)

	pNpc = spawnMobile("naboo", "mouse_droid", 120, -52.15, 0.2, -23.82, -15, 1418884)
	writeData(SceneObject(pNpc):getObjectID() .. ":currentLoc", 1)
	writeStringData(SceneObject(pNpc):getObjectID() .. ":name", "droid1")
	createEvent(getRandomNumber(350,450) * 100, "EmperorsRetreatScreenPlay", "droidPatrol", pNpc, "")
	createObserver(DESTINATIONREACHED, "EmperorsRetreatScreenPlay", "retreatPatrolDestReached", pNpc)
	AiAgent(pNpc):setAiTemplate("manualescortwalk")
	AiAgent(pNpc):setFollowState(4)

	pNpc = spawnMobile("naboo", "mouse_droid", 120, 23.83, 0.2, -40.42, -25, 1418875)
	writeData(SceneObject(pNpc):getObjectID() .. ":currentLoc", 1)
	writeStringData(SceneObject(pNpc):getObjectID() .. ":name", "droid2")
	createEvent(getRandomNumber(350,450) * 100, "EmperorsRetreatScreenPlay", "droidPatrol", pNpc, "")
	createObserver(DESTINATIONREACHED, "EmperorsRetreatScreenPlay", "retreatPatrolDestReached", pNpc)
	AiAgent(pNpc):setAiTemplate("manualescortwalk")
	AiAgent(pNpc):setFollowState(4)

	pNpc = spawnMobile("naboo", "mouse_droid", 120, -45.79, 0.2, -12.5, -25, 1418879)
	writeData(SceneObject(pNpc):getObjectID() .. ":currentLoc", 1)
	writeStringData(SceneObject(pNpc):getObjectID() .. ":name", "droid3")
	createEvent(getRandomNumber(350,450) * 100, "EmperorsRetreatScreenPlay", "droidPatrol", pNpc, "")
	createObserver(DESTINATIONREACHED, "EmperorsRetreatScreenPlay", "retreatPatrolDestReached", pNpc)
	AiAgent(pNpc):setAiTemplate("manualescortwalk")
	AiAgent(pNpc):setFollowState(4)


	pNpc = spawnMobile("naboo", "stormtrooper", 120, -0.5, 0.2, -23.6, 76, 1418874)
	self:setMoodString(pNpc, "conversation")
	pNpc = spawnMobile("naboo", "stormtrooper", 120, 0.7, 0.2, -23.5, -20, 1418874)
	self:setMoodString(pNpc, "conversation")

	--Guard Towers
	spawnMobile("naboo", "stormtrooper", 450, 2536.8, 296, -3881.4, -90, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2536.8, 296, -3885.5, -90, 0)

	spawnMobile("naboo", "stormtrooper", 450, 2551, 352, -3647, -125, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2553.2, 352, -3650.7, -125, 0)

	spawnMobile("naboo", "stormtrooper", 450, 2342, 392, -3561.1, 145, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2345.3, 392, -3559, 145, 0)

	spawnMobile("naboo", "stormtrooper", 450, 2175.6, 362, -3698, 133, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2178, 362, -3695.1, 133, 0)

	spawnMobile("naboo", "stormtrooper", 450, 1711.5, 385, -3676, 130, 0)
	spawnMobile("naboo", "stormtrooper", 450, 1714.6, 385, -3673.6, 130, 0)

	spawnMobile("naboo", "stormtrooper", 450, 1909, 395, -4020.8, 20, 0)
	spawnMobile("naboo", "stormtrooper", 450, 1905.3, 395, -4018.9, 20, 0)

	spawnMobile("naboo", "stormtrooper", 450, 2122.8, 294, -4180.9, -15, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2118.9, 294, -4181.4, -15, 0)

	spawnMobile("naboo", "stormtrooper", 450, 2371.8, 325, -4095.5, 0, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2367.7, 325, -4095.5, 0, 0)

	--By the emperors retreat
	pNpc = spawnMobile("naboo", "at_st", 900, 2452.61, 292, -3961.49, 108, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")
	pNpc = spawnMobile("naboo", "at_st", 900, 2463.18, 292.089, -3929.16, -105, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")
	spawnMobile("naboo", "dark_trooper", 450, 2433.3, 292, -3968.4, 4, 0)
	spawnMobile("naboo", "dark_trooper", 450, 2452, 292, -3912.8, -170, 0)
	spawnMobile("naboo", "dark_trooper", 450, 2434.47, 292, -3932.06, 90, 0)
	spawnMobile("naboo", "dark_trooper", 450, 2437.01, 292, -3947.8, 147, 0)
	spawnMobile("naboo", "dark_trooper", 450, 2411.91, 292, -3986.99, -115, 0)
	spawnMobile("naboo", "imperial_pilot", 450, 2432.33, 292, -3887.25, -131, 0)
	pNpc = spawnMobile("naboo", "noble", 60, 2443.42, 292, -3893.91, 18, 0)
	self:setMoodString(pNpc, "conversation")
end
