CityScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "CityScreenPlay",

	planet = "",

	gcwMobs = {},
}

function CityScreenPlay:spawnGcwMobiles()
	if (isZoneEnabled(self.planet)) then
		local controllingFaction = getControllingFaction(self.planet)
		local difficulty = getWinningFactionDifficultyScaling(self.planet)

		for i = 1, #self.gcwMobs do
			self:spawnMob(i, controllingFaction, difficulty)
		end
	end
end

function CityScreenPlay:spawnMob(num, controllingFaction, difficulty)
	local mobsTable = self.gcwMobs

	if num <= 0 or num > #mobsTable then
		return
	end

	local mobTable = mobsTable[num]
	local pNpc = nil
	local npcTemplate = ""
	local npcMood = ""

	if controllingFaction == FACTIONIMPERIAL then
		npcTemplate = mobTable[1]
		npcMood = mobTable[8]
	elseif controllingFaction == FACTIONREBEL then
		npcTemplate = mobTable[2]
		npcMood = mobTable[9]
	end

	local scaling = ""
	if difficulty > 1 then
		scaling = "_hard"
	end

	pNpc = spawnMobile(self.planet, npcTemplate .. scaling, 0, mobTable[3], mobTable[4], mobTable[5], mobTable[6], mobTable[7])

	if pNpc ~= nil and npcMood ~= "" then
		self:setMoodString(pNpc, npcMood)
	end

	if pNpc ~= nil then
		createObserver(CREATUREDESPAWNED, self.screenplayName, "onDespawn", pNpc)
		writeData(SceneObject(pNpc):getObjectID(), num)
	end
end

function CityScreenPlay:onDespawn(pAiAgent)
	if pAiAgent == nil or not SceneObject(pAiAgent):isAiAgent() then
		printf("pAiAgent is nil or not an AiAgent")
		return
	end

	local oid = SceneObject(pAiAgent):getObjectID()
	local mobNumber = readData(oid)
	deleteData(oid)

	createEvent(300000, self.screenplayName, "respawn", nil, tostring(mobnumber))

	return 1
end

function CityScreenPlay:respawn(pAiAgent, args)
	local mobNumber = tonumber(args)
	local controllingFaction = getControllingFaction(self.planet)
	local difficulty = getWinningFactionDifficultyScaling(self.planet)

	self:spawnMob(mobNumber, controllingFaction, difficulty)
end
