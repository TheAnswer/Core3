-- When an interrupt gets called (startXXXInterrupt), pAgent and pObject both come in locked. This means that any object that
-- gets declared in the course of the method CANNOT be locked in any subsequent cpp call. For example, startAwarenessInterrupt
-- declares a pFollow object, and many calls are made using it. All of those calls MUST be threadsafe WITHOUT locking pFollow.

require("ai.ai")
local ObjectManager = require("managers.object.object_manager")

Interrupt = { }
function Interrupt:interrupt(pAgent, pObject, msg)
	if     msg == STARTCOMBAT        then self:startCombatInterrupt(pAgent, pObject)    -- pObject = sender of interrupt message
	elseif msg == OBJECTINRANGEMOVED then self:startAwarenessInterrupt(pAgent, pObject) -- pObject = object that moved TODO: should rename this
	elseif msg == DAMAGERECEIVED	 then self:startDamageInterrupt(pAgent, pObject)    -- pObject = source of damage
	end
end

function Interrupt:startDamageInterrupt(pAgent,pObject)
end

function Interrupt:startCombatInterrupt(pAgent, pObject)
end

function Interrupt:doAwarenessCheck(pAgent, pObject)
	--print("Shouldn't be here...")
	return false
end

function Interrupt:startAwarenessInterrupt(pAgent, pObject)
	--print("Shouldn't be here either...")
end


DefaultInterrupt = createClass(Interrupt)
function DefaultInterrupt:startCombatInterrupt(pAgent, pObject)
	if (pAgent ~= pObject) then return end
	if (pAgent ~= nil) then
		local agent = AiAgent(pAgent)
		agent:setBehaviorStatus(BEHAVIOR_SUSPEND)
		agent:resetBehaviorList()
		agent:executeBehavior()
	end
end

function DefaultInterrupt:doAwarenessCheck(pAgent, pObject)
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Entering doAwarenessCheck()") end
	if (pAgent == pObject) then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed unique objects") end
	if (pAgent == nil or pObject == nil) then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed non-nil objects") end

	if TangibleObject(pAgent):getPvpStatusBitmask() == NONE or CreatureObject(pAgent):isDead() or CreatureObject(pAgent):isIncapacitated() then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed self bitmask checks") end
	if AiAgent(pAgent):getNumberOfPlayersInRange() <= 0  or AiAgent(pAgent):isRetreating() or AiAgent(pAgent):isFleeing() or AiAgent(pAgent):isInCombat() then return false	end

	self:checkForReactionChat(pAgent, pObject)

	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed playersInRange, retreat, flee, and inCombat checks") end
	if AiAgent(pAgent):getFollowObject() ~= nil and AiAgent(pAgent):getFollowObject() ~= pObject then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed followObject checks") end

	local radius = AiAgent(pAgent):getAggroRadius()
	if radius == 0 then radius = DEFAULTAGGRORADIUS end

	-- TODO possibly tweak, but we need a cap on mob awareness distance
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Radius "..radius) end
	if not SceneObject(pObject):isInRangeWithObject(pAgent, radius * 2.4) then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed radius check") end

	if not SceneObject(pObject):isCreatureObject() then return false end -- don't aggro TANOs (lairs, turrets, etc)
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed target CREO check") end
	if TangibleObject(pAgent):getPvpStatusBitmask() == NONE or CreatureObject(pObject):isDead() or CreatureObject(pObject):isIncapacitated() then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed target bitmask checks") end
	
	-- if not in combat, ignore creatures in different cells
	local agentParentID = CreatureObject(pAgent):getBuildingParentID()
	local targetParentID = CreatureObject(pObject):getBuildingParentID()
	if agentParentID ~= targetParentID then	return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed parent checks") end
	
	if AiAgent(pAgent):isCamouflaged(pObject) or not AiAgent(pAgent):isAttackableBy(pObject) or not CreatureObject(pObject):isAttackableBy(pAgent) then return false end
	--if SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("Passed attackable checks") end
	
	--TODO (dannuic): this still seems wrong, but it's only for AI aggroing AI anyway
	if SceneObject(pObject):isAiAgent() then
		--AiAgent(pObject):info("attacking me!")
		
		--local creature = LuaCreatureObject(pAgent)
		local creatureFaction = CreatureObject(pAgent):getFaction()
		
		--local creatureTarget = LuaCreatureObject(pObject)
		local creatureTargetFaction = CreatureObject(pObject):getFaction()
		
		if not AiAgent(pObject):isAttackableBy(pAgent) then return false end
		--AiAgent(pAgent):info("Passed AiAgent target attackable checks")
		if (creatureFaction ~= 0 and creatureTargetFaction == 0) 
		    or (creatureFaction == 0 and creatureTargetFaction ~= 0) 
		    then return false 
		end
		--AiAgent(pAgent):info("Passed all AiAgent attackable checks")
	end
	
	-- All the checks are out of the way, now we know we want to notice them
	return true
end

function DefaultInterrupt:checkForReactionChat(pAgent, pObject)
	if not SceneObject(pObject):isPlayerCreature() then return end

	if not AiAgent(pAgent):hasReactionChatMessages() then return end

	if SceneObject(pObject):getParentID() ~= SceneObject(pAgent):getParentID() then return end

	local dist = SceneObject(pObject):getDistanceTo(pAgent)

	if dist > 40 or dist < 30 then return end

	if not CreatureObject(pAgent):checkCooldownRecovery("reaction_chat") then return end

	if not AiAgent(pAgent):checkLineOfSight(pObject) then return end

	local factionString = AiAgent(pAgent):getFactionString()
	local aiFaction = CreatureObject(pAgent):getFaction()
	local targetFaction = CreatureObject(pObject):getFaction()
	local state = 0

	if aiFaction ~= 0 then
		if targetFaction == aiFaction then
			state = REACTION_NICE
		elseif targetFaction == 0 then
			state = REACTION_MID
		else
			state = REACTION_MEAN
		end

	elseif factionString ~= "" then
		local pGhost = CreatureObject(pObject):getPlayerObject()

		if pGhost ~= nil then
			local standing = PlayerObject(pGhost):getFactionStanding(factionString)
			if standing >= 3000 then
				state = REACTION_NICE
			elseif standing <= -3000 then
				state = REACTION_MEAN
			else
				state = REACTION_MID
			end
		end
	else
		state = REACTION_MID
	end

	SceneObject(pAgent):faceObject(pObject)

	if SceneObject(pObject):isFacingObject(pAgent) then
		AiAgent(pAgent):sendReactionChat(REACTION_HI, state)
	else
		AiAgent(pAgent):sendReactionChat(REACTION_BYE, state)
	end
end

-- put this in a different function so that the generic checks are re-usable
function DefaultInterrupt:startAwarenessInterrupt(pAgent, pObject)
	if (pAgent == pObject) then return end
	if (pAgent == nil or pObject == nil) then return end

	if not SceneObject(pObject):isCreatureObject() then return false end -- don't aggro TANOs (lairs, turrets, etc)
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("a") end
	
	if CreatureObject(pAgent):isDead() or CreatureObject(pAgent):isIncapacitated() then return end
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("b") end
	if CreatureObject(pObject):isDead() or CreatureObject(pObject):isIncapacitated() then return end
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("c") end

	if AiAgent(pAgent):isInCombat() then return end
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("d") end

	if not AiAgent(pAgent):checkLineOfSight(pObject) then return end
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("e") end
	
	if AiAgent(pAgent):isCamouflaged(pObject) then return end
	--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("f") end
	
	-- TODO (dannuic): tweak these formulae based on feedback
	-- TODO (dannuic): should we be using group levels here (if available)?
	local levelDiff = CreatureObject(pObject):getLevel() - CreatureObject(pAgent):getLevel()
	local mod = math.max(0.04, math.min((1 - (levelDiff/20)), 1.2))

	local radius = AiAgent(pAgent):getAggroRadius()
	if radius == 0 then radius = DEFAULTAGGRORADIUS end
	radius = radius*mod

	local inRange = SceneObject(pObject):isInRangeWithObject(pAgent, radius)

	local pFollow = AiAgent(pAgent):getFollowObject();
	
	if AiAgent(pAgent):isStalker() and AiAgent(pAgent):isAggressiveTo(pObject) and SceneObject(pObject):isInRangeWithObject(pAgent, radius*2) then
		--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("1") end
		if pFollow == nil and not inRange then
			--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("1a") end
			AiAgent(pAgent):setStalkObject(pObject)
			AiAgent(pAgent):setAlertDuration(10000)
			if CreatureObject(pObject):hasSkill("outdoors_ranger_novice") then
				CreatureObject(pObject):sendSystemMessageWithTO("@skl_use:notify_stalked", SceneObject(pAgent):getDisplayedName())
			end
		elseif inRange or CreatureObject(pObject):getCurrentSpeed() <= CreatureObject(pObject):getWalkSpeed() then
			--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("1b") end
			AiAgent(pAgent):addDefender(pObject) -- TODO (dannuic): do stalkers also agro when the target starts to move towards them?
		else
			--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("1c") end
			AiAgent(pAgent):setOblivious()
		end
	elseif AiAgent(pAgent):isAggressiveTo(pObject) and inRange then
		--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("2") end
		--if SceneObject(pObject):isAiAgent() then AiAgent(pObject):info("attacking me!") end) end
		AiAgent(pAgent):addDefender(pObject)
	elseif pFollow == nil and inRange and AiAgent(pAgent):isCreature() and (CreatureObject(pObject):getCurrentSpeed() >= CreatureObject(pObject):getWalkSpeed()) then
		--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("3") end
		AiAgent(pAgent):setWatchObject(pObject)
		AiAgent(pAgent):setAlertDuration(10000); -- TODO (dannuic): make this wait time more dynamic
		SceneObject(pAgent):showFlyText("npc_reaction/flytext", "alert", 255, 0, 0)
	elseif pObject == pFollow and AiAgent(pAgent):alertedTimeIsPast() and AiAgent(pAgent):getFollowState() == WATCHING then
		--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("4") end
		AiAgent(pAgent):setOblivious() -- if we're "standing still" (and they aren't aggressive) forget about us
	elseif pObject == pFollow and inRange and SceneObject(pObject):getParent() == nil then -- TODO: Do we want weaker mobs to run away when indoors? Revisit when indoor pathing is better
		--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("5") end
		-- TODO (dannuic): Not sure about this stuff, needs testing
		if SceneObject(pFollow):isCreatureObject() then
			local creoLevel = CreatureObject(pFollow):getLevel()
			local isBackwardsAggressive = SceneObject(pFollow):isAiAgent() and AiAgent(pFollow):isAggressiveTo(pAgent)
 
 			-- TODO (dannuic): Add in run away logic for nearby combat
			if AiAgent(pAgent):isCreature() and CreatureObject(pAgent):getLevel()*mod < creoLevel and (isBackwardsAggressive or SceneObject(pFollow):isPlayerCreature()) and CreatureObject(pObject):getCurrentSpeed() > 2*CreatureObject(pObject):getWalkSpeed() and SceneObject(pObject):isFacingObject(pAgent) then
				--if not SceneObject(pObject):isAiAgent() then AiAgent(pAgent):info("run!") end
				AiAgent(pAgent):runAway(pFollow, 64 - radius)
				AiAgent(pAgent):setAlertDuration(10000)
			end
		else AiAgent(pAgent):setOblivious() end
	else
		AiAgent(pAgent):setOblivious()
	end

	AiAgent(pAgent):stopWaiting();
	AiAgent(pAgent):executeBehavior();
end


PackInterrupt = createClass(DefaultInterrupt)
function PackInterrupt:startCombatInterrupt(pAgent, pObject)
	if (pAgent ~= pObject) then
		if (pAgent ~= nil and pObject ~= nil) then
			if SceneObject(pObject):isAiAgent() then
				if AiAgent(pObject):getSocialGroup() ~= AiAgent(pAgent):getSocialGroup() or not AiAgent(pAgent):checkLineOfSight(pObject) then
					return
				end
			end

			-- if the source is not an AiAgent (like a lair) then don't check social group
			-- TODO (dannuic): change the range to calculate based on level difference and ferocity
			agent = AiAgent(pAgent)

			if agent:checkRange(pObject, 15) then
				agent:assist(pObject)
			end
		end
	end

	DefaultInterrupt:startCombatInterrupt(pAgent, pObject)
end


CreaturePetInterrupt = createClass(DefaultInterrupt)
function CreaturePetInterrupt:startCombatInterrupt(pAgent, pObject)
	if pAgent == nil or pObject == nil then return end
	local agent = AiAgent(pAgent)
	if agent:getOwner() ~= pObject then return end -- this is where the friend checks will go
	DefaultInterrupt:startCombatInterrupt(pAgent, pObject)

  --recover our pointer to agent
	agent = AiAgent(pAgent)
	agent:setBehaviorStatus(BEHAVIOR_SUSPEND)
	agent:resetBehaviorList()
	agent:executeBehavior()
end

DroidPetInterrupt = createClass(DefaultInterrupt)
function DroidPetInterrupt:startDamageInterrupt(pAgent,pObject)
	if pAgent == nil or pObject == nil then return end
	local agent = AiAgent(pAgent)
	--print("droid got damage\n")
	-- starting combat droids should flee if they arent combat capable and they get hit by damage
	if CreatureObject(pAgent):isCombatDroidPet() then
		-- if in combat run away if not a combat pet
		agent:runAway(pObject, 32)
		agent:stopWaiting()
		agent:executeBehavior()
		return
	end

	--recover our pointer to agent
	agent = AiAgent(pAgent)
	agent:setBehaviorStatus(BEHAVIOR_SUSPEND)
	agent:resetBehaviorList()
	agent:executeBehavior()
end

function DroidPetInterrupt:startCombatInterrupt(pAgent, pObject)
	if pAgent == nil or pObject == nil then return end
	local agent = AiAgent(pAgent)
	if agent:getOwner() ~= pObject then return end -- this is where the friend checks will go
	DefaultInterrupt:startCombatInterrupt(pAgent, pObject)

	--recover our pointer to agent
	agent = AiAgent(pAgent)
	agent:setBehaviorStatus(BEHAVIOR_SUSPEND)
	agent:resetBehaviorList()
	agent:executeBehavior()
end


FactionPetInterrupt = createClass(DefaultInterrupt)
function FactionPetInterrupt:startCombatInterrupt(pAgent, pObject)
	if pAgent == nil or pObject == nil then return end
	local agent = AiAgent(pAgent)
	if agent:getOwner() ~= pObject then return end -- this is where the friend checks will go

	DefaultInterrupt:startCombatInterrupt(pAgent, pObject)

  --recover our pointer to agent
	agent = AiAgent(pAgent)
	agent:setBehaviorStatus(BEHAVIOR_SUSPEND)
	agent:resetBehaviorList()
	agent:executeBehavior()
end
