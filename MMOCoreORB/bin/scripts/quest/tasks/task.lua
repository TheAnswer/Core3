-- Base module for all tasks.
local ObjectManager = require("managers.object.object_manager")

Task = Object:new {
	taskName = "",
	taskStart = nil,
	taskFinish = nil
}

local TASK_STARTED = 0xABCD

-- Check if the task has been started for the player.
-- @param pCreatureObject pointer to the creature object of the player.
function Task:hasTaskStarted(pCreatureObject)
	return ObjectManager.withCreatureObject(pCreatureObject, function(creatureObject)
		return creatureObject:getScreenPlayState(self.taskName) == TASK_STARTED
	end) == true
end

-- Set the task started screen play state for the player.
-- @param pCreatureObject pointer to the creature object of the player.
function Task:setTaskStarted(pCreatureObject)
	ObjectManager.withCreatureObject(pCreatureObject, function(creatureObject)
		creatureObject:setScreenPlayState(TASK_STARTED, self.taskName)
	end)
end

-- Set the task finished screen play state for the player.
-- @param pCreatureObject pointer to the creature object of the player.
function Task:setTaskFinished(pCreatureObject)
	ObjectManager.withCreatureObject(pCreatureObject, function(creatureObject)
		creatureObject:removeScreenPlayState(TASK_STARTED, self.taskName)
	end)
end

-- Call the supplied function with the argument if the function is not nil.
-- An error will be issued if the function is nil.
-- @param theFunction the function to call.
-- @param argument the argument to use for the function.
function Task:callFunctionIfNotNil(theFunction, ...)
	if theFunction ~= nil then
		return theFunction(self, unpack(arg))
	else
		error("The function to call is nil in " .. Task.taskName .. ".", 2)
		return nil
	end
end

-- Start the task.
-- @param pCreatureObject pointer to the creature object of the player who should get the task started.
function Task:start(pCreatureObject)
	if not self:hasTaskStarted(pCreatureObject) then
		self:setTaskStarted(pCreatureObject)
		self:callFunctionIfNotNil(self.taskStart, pCreatureObject)
	end
end

-- Finish the task.
-- @param pCreatureObject pointer to the creature object of the player who should get the task finished.
function Task:finish(pCreatureObject)
	if self:hasTaskStarted(pCreatureObject) then
		self:callFunctionIfNotNil(self.taskFinish, pCreatureObject)
		self:setTaskFinished(pCreatureObject)
	end
end

return Task
