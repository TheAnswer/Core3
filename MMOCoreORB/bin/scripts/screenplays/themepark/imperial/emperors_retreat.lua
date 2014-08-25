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
	ObjectManager.withCreatureObject(pPlayer, function(creature)
		if (selectedID ~= 198) or (not creature:hasScreenPlayState("imperial_theme_park", 32)) then
			creature:sendSystemMessage("@theme_park_imperial/warning:emperor")
			return
		end

		local obj = SceneObject(pSceneObject)

		if (creature:getParent() ~= obj:getParent()) then
			return
		end

		local z = obj:getPositionZ() + 20
		local x = creature:getPositionX()
		local y = creature:getPositionY()

		creature:playEffect("clienteffect", "elevator_ascend.cef")
		creature:teleport(x, z, y, obj:getParentID())
	end)
end

function EmperorsRetreatScreenPlay:setMoodString(pNpc, mood)
	ObjectManager.withCreatureObject(pNpc, function(npc)
		npc:setMoodString(mood)
	end)
end

function EmperorsRetreatScreenPlay:spawnMobiles()

	-- Inside
	local pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 11.2, 0.2, -31.5, 0, 1418874)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "npc_imperial")
	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 14.8, 0.2, -31.5, 0, 1418874)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "npc_imperial")

	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 10.8, 20, -27.2, 180, 1418886)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "npc_imperial")
	pNpc = spawnMobile("naboo", "royal_imperial_guard", 120, 15.3, 20, -27.2, 180, 1418886)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "npc_imperial")

	spawnMobile("naboo", "ra7_bug_droid", 120, 4.0, 0.2, -45.7, 4, 1418876)

	spawnMobile("naboo", "mouse_droid", 120, -8.6, 0.2, -14.2, -15, 1418879)
	spawnMobile("naboo", "mouse_droid", 120, -39.9, 0.2, -12.4, -25, 1418879)

	pNpc = spawnMobile("naboo", "stormtrooper", 120, -0.5, 0.2, -23.6, 76, 1418874)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "conversation")
	pNpc = spawnMobile("naboo", "stormtrooper", 120, 0.7, 0.2, -23.5, -20, 1418874)
	EmperorsRetreatScreenPlay:setMoodString(pNpc, "conversation")

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
	spawnMobile("naboo", "at_st", 900, 2436.4, 292, -3912.1, 167, 0)
	spawnMobile("naboo", "at_st", 900, 2429.9, 292, -3913.1, 167, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2433.3, 292, -3968.4, 4, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2435.3, 292, -3968.4, 4, 0)
	spawnMobile("naboo", "imperial_corporal", 450, 2434.4, 292, -3967.1, 4, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2452, 292, -3912.8, -170, 0)
	spawnMobile("naboo", "stormtrooper", 450, 2454, 292, -3912.8, -170, 0)
	spawnMobile("naboo", "imperial_corporal", 450, 2452.9, 292, -3914.1, -170, 0)

end
