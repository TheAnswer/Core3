NabooMordranScreenPlay = ScreenPlay:new {
	numberOfActs = 1,
	
	screenplayName = "NabooMordranScreenPlay",

}

registerScreenPlay("NabooMordranScreenPlay", true)

function NabooMordranScreenPlay:start()
	if (isZoneEnabled("naboo")) then
		self:spawnMobiles()

	end
end

function NabooMordranScreenPlay:spawnMobiles()

		--mordran respawns between 15-20 minutes
	local pNpc = spawnMobile("naboo", "mordran", 1200, -1969, 6.4, -5295, 45, 0)
	spawnMobile("naboo", "outcast_gungan", 300, getRandomNumber(32) + -1985, 6.4, getRandomNumber(12) + -5302, 45, 0)
	spawnMobile("naboo", "outcast_gungan", 300, getRandomNumber(32) + -1985, 6.4, getRandomNumber(12) + -5302, 45, 0)
	spawnMobile("naboo", "outcast_gungan", 300, getRandomNumber(32) + -1985, 6.4, getRandomNumber(12) + -5302, 45, 0)
	pNpc = spawnMobile("naboo", "gungan_reject", 360, getRandomNumber(8) + -1974, 6.4, getRandomNumber(12) + -5302, 45, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")
	pNpc = spawnMobile("naboo", "gungan_reject", 360, getRandomNumber(8) + -1974, 6.4, getRandomNumber(12) + -5302, 45, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")
	pNpc = spawnMobile("naboo", "exiled_gungan", 360, getRandomNumber(8) + -1974, 6.4, getRandomNumber(12) + -5302, 45, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")
	pNpc = spawnMobile("naboo", "exiled_gungan", 360, getRandomNumber(8) + -1974, 6.4, getRandomNumber(12) + -5302, 45, 0)
	AiAgent(pNpc):setAiTemplate("idlewander")

end
