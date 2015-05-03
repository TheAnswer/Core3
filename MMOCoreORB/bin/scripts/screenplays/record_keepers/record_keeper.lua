require("screenplays.screenplay")
local ObjectManager = require("managers.object.object_manager")

RecordKeeper =ScreenPlay:new {
	quests = { "dummy"},
	keeperName = "dummy",
	faction = 0
}

function RecordKeeper:start()
-- no op
end

function RecordKeeper:wipeQuests(pObject)
	if (pObject == nil) then
		return
	end

	for k,v in pairs(self.quests) do
		-- we wipe out all screen play data for this theme park/quest
		local park = _G[v]
		park:resetThemePark(pObject)
	end
	writeScreenPlayData(pObject, self.keeperName, "completed", 1)
end

function RecordKeeper:resetQuests(pObject)
	for k,v in pairs(self.quests) do
		local park = _G[v]
		park:resetCurrentMission(pObject)
	end
	writeScreenPlayData(pObject, self.keeperName, "completed", 1)
end

function RecordKeeper:available(pObject)
	if (pObject == nil) then
		return false
	end

	-- read the variable
	local val = readScreenPlayData(pObject, self.keeperName, "completed")
	if val == "1" then
		return false
	else
		return true
	end
end

function RecordKeeper:hasStartedPark(pObject)
	if (pObject == nil) then
		return false
	end

	local count = 0
	for k,v in pairs(self.quests) do
		local park = _G[v]
		count = count + (park:getCurrentMissionNumber(1,pObject) - 1)
	end
	return count > 0
end

function RecordKeeper:needsFaction()
	if self.faction == 0 then
		return false
	else
		return true
	end
end

function RecordKeeper:hasFaction(faction, pCreature)
	if pCreature == nil then
		return false
	end

	if CreatureObject(pCreature):getFaction() == faction then
		return true
	else
		return false
	end
end

