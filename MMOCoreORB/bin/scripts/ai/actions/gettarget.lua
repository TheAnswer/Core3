require("ai.ai")
require("ai.interrupts")

GetTargetBase = createClass(Ai)

function GetTargetBase:checkConditions(pAgent)
	if (pAgent ~= nil) then
		local creature = CreatureObject(pAgent)

		if (creature:isDead()) then
			local agent = AiAgent(pAgent)
			agent:clearCombatState(true)
			agent:setOblivious()
			return false
		end

		return true
	end
	return false
end

function GetTargetBase:doAction(pAgent)
	if (pAgent ~= nil) then
		local agent = AiAgent(pAgent)
		local creature = CreatureObject(pAgent)
		local ranLevel = getRandomNumber(creature:getLevel())

		local pTarget = agent:getTargetFromMap()
		--print(pTarget)
		if pTarget ~= nil and pTarget ~= agent:getFollowObject() then
		--print("1")
			agent:setFollowObject(pTarget)
			if (pTarget ~= nil) then agent:setDefender(pTarget) end
			if (agent:validateTarget()) then
				return BEHAVIOR_SUCCESS
			else
				agent:removeDefender()
			end
		elseif pTarget ~= nil and agent:validateTarget() then
		--print("2")
			if agent:followHasState(PEACE) and ranLevel == 1 then
				agent:clearCombatState(true)
				agent:setOblivious()
				return BEHAVIOR_FAILURE
			end
			agent:setDefender(pTarget)
			return BEHAVIOR_SUCCESS
		end

		pTarget = agent:getTargetFromDefenders()
		--print(pTarget)
		if pTarget ~= nil and pTarget ~= agent:getFollowObject() then
		--print("3")
			agent:setFollowObject(pTarget)
			if (pTarget ~= nil) then agent:setDefender(pTarget) end
			if (agent:validateTarget()) then
				return BEHAVIOR_SUCCESS
			else
				agent:removeDefender()
			end
		elseif pTarget ~= nil and agent:validateTarget() then
		--print("4")
			if agent:followHasState(PEACE) and ranLevel == 1 then
				agent:clearCombatState(true)
				agent:setOblivious()
				return BEHAVIOR_FAILURE
			end
			agent:setDefender(pTarget)
			return BEHAVIOR_SUCCESS
		end
		
		if (agent:isInCombat()) then
		--print("5")
			agent:clearCombatState(true)
			agent:setOblivious()
		end
	end
	return BEHAVIOR_FAILURE
end

GetTarget = createClass(GetTargetBase, Interrupt)
GetTargetPet = createClass(GetTargetBase, PetInterrupt)

function GetTargetPet:doAction(pAgent)
	if (pAgent ~= nil) then
		--print("1")
		local agent = AiAgent(pAgent)
		
		local command = agent:getLastCommand()
		if (command ~= PET_ATTACK and command ~= PET_GUARD) then
			return BEHAVIOR_FAILURE	
		end
	
		local creature = CreatureObject(pAgent)

		local pTarget = agent:getTargetFromMap()
		--print(pTarget)
		if (pTarget ~= nil and pTarget ~= agent:getFollowObject()) then
			--print("2")
			agent:setFollowObject(pTarget)
			agent:setDefender(pTarget)
			if (agent:validateTarget()) then
				--print("3")
				return BEHAVIOR_SUCCESS
			else
				--print("4")
				agent:removeDefender()
			end
		elseif pTarget ~= nil and agent:validateTarget() then
			--print("5")
			return BEHAVIOR_SUCCESS
		end


		pTarget = agent:getTargetFromDefenders()
		--print(pTarget)
		if (pTarget ~= nil and pTarget ~= agent:getFollowObject()) then
			--print("6")
			agent:setFollowObject(pTarget)
			agent:setDefender(pTarget)
			if (agent:validateTarget()) then
				--print("7")
				return BEHAVIOR_SUCCESS
			else
				--print("8")
				agent:removeDefender()
			end
		elseif pTarget ~= nil and agent:validateTarget() then
			--print("9")
			return BEHAVIOR_SUCCESS
		end
	end
	return BEHAVIOR_FAILURE
end

function GetTargetPet:terminate(pAgent)
	if pAgent ~= nil then
		local agent = AiAgent(pAgent)
		if agent:getBehaviorStatus() == BEHAVIOR_FAILURE then agent:restoreFollowObject() end
	end
	return 0
end
