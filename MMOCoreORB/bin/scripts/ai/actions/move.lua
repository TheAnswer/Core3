require("ai.ai")
require("ai.interrupts")

--MoveBase = Ai:new {}
MoveBase = createClass(Ai)

function MoveBase:checkConditions(pAgent)
	if (pAgent ~= nil) then
		local agent = LuaAiAgent(pAgent)
		local creature = LuaCreatureObject(pAgent)
		if (creature:getPosture() == UPRIGHT and agent:setDestination() > 0) then
			return true
		end
	end
	return false
end

function MoveBase:doAction(pAgent)
	if (pAgent ~= nil) then
		local agent = LuaAiAgent(pAgent)
		
		if (agent:getCurrentSpeed() > 0) then 
			agent:completeMove()
		end
		
		if (self:findNextPosition(pAgent)) then
			return BEHAVIOR_RUNNING
		else
			return BEHAVIOR_SUCCESS
		end
	end
	return BEHAVIOR_FAILURE
end

-- default action is to run
function MoveBase:findNextPosition(pAgent)
	if (pAgent ~= nil) then
		local agent = LuaAiAgent(pAgent)
		if (agent:findNextPosition(agent:getMaxDistance(), false)) then
			return true
		end
	end
	return false
end

--[[function MoveBase:interrupt(pAgent, pObject, msg)
	if msg == STARTCOMBAT and pAgent == pObject then
		Ai:startCombatInterrupt(pAgent, pObject)
	end
	
	return 0
end]]

Move = createClass(MoveBase, Interrupt)
MoveDefault = createClass(MoveBase, DefaultInterrupt)
MovePack = createClass(MoveBase, PackInterrupt)
