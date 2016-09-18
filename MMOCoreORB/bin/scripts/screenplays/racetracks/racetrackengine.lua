local ObjectManager = require("managers.object.object_manager")

RaceTrack =ScreenPlay:new {}

function RaceTrack:createRaceTrack()
	for lc = 1, #self.trackConfig.waypoints , 1 do
		local pWaypointAA = spawnActiveArea(self.trackConfig.planetName, "object/active_area.iff", self.trackConfig.waypoints[lc].x, 0, self.trackConfig.waypoints[lc].y, self.trackConfig.waypointRadius, 0)

		if (pWaypointAA ~= nil) then
			createObserver(ENTEREDAREA, self.trackConfig.className, "enteredWaypoint" , pWaypointAA)
		end
	end
	self:createResetBestTimeEvent()
end

function RaceTrack:startRacing(pPlayer)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	clearScreenPlayData(pPlayer, self.trackConfig.trackName)
	self:createResetPlayerUnfinishedEvent(pPlayer)
	local waypointID = PlayerObject(pGhost):addWaypoint(self.trackConfig.planetName,self.trackConfig.trackCheckpoint .. "0",self.trackConfig.trackCheckpoint .. "0",self.trackConfig.waypoints[1].x,self.trackConfig.waypoints[1].y,WAYPOINTWHITE,true,true,WAYPOINTRACETRACK)
	local time = getTimestampMilli()
	writeScreenPlayData(pPlayer, self.trackConfig.trackName, "starttime", time)
	writeScreenPlayData(pPlayer, self.trackConfig.trackName, "waypoint", 1)
	CreatureObject(pPlayer):sendSystemMessage("@theme_park/racing/racing:go_fly")
	CreatureObject(pPlayer):playMusicMessage("sound/music_combat_bfield_lp.snd")
end

function RaceTrack:processWaypoint(pActiveArea, pObject)
	if not SceneObject(pObject):isPlayerCreature() then
		return 0
	end

	local lastIndex =  readScreenPlayData(pObject, self.trackConfig.trackName, "waypoint")
	if lastIndex ~= "" then
		local index = self:getWaypointIndex(pActiveArea)
		if tonumber(lastIndex)==index then
			if tonumber(index)==#self.trackConfig.waypoints then
				self:finalWaypoint(pActiveArea, pObject)
			else
				self:actuallyProcessWaypoint(pObject,index)
			end
		end
	end

	return 0
end

function RaceTrack:roundNumber(num)
	local mult = 10 ^ (self.trackConfig.timeResolution or 0 )
	return math.floor(num * mult + 0.5) / mult
end


function RaceTrack:actuallyProcessWaypoint(pPlayer, index)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local waypointID = PlayerObject(pGhost):addWaypoint(self.trackConfig.planetName,self.trackConfig.trackCheckpoint .. index,self.trackConfig.trackCheckpoint .. index,self.trackConfig.waypoints[index+1].x,self.trackConfig.waypoints[index+1].y,WAYPOINTWHITE,true,true,WAYPOINTRACETRACK)
	local seconds = self:getLaptime(pPlayer)
	CreatureObject(pPlayer):sendSystemMessage(self.trackConfig.trackLaptime .. index)
	CreatureObject(pPlayer):sendSystemMessage("Time " .. self:roundNumber(seconds/1000) .. "s")
	writeScreenPlayData(pPlayer,self.trackConfig.trackName, "waypoint", index+1)
	CreatureObject(pPlayer):playMusicMessage("sound/music_combat_bfield_lp.snd")
end

function RaceTrack:finalWaypoint(pActiveArea, pPlayer)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	CreatureObject(pPlayer):playMusicMessage("sound/music_combat_bfield_vict.snd")
	self:checkPersonalTime(pPlayer)
	self:checkServerRecordTime(pPlayer)
	clearScreenPlayData(pPlayer,self.trackConfig.trackName )
	PlayerObject(pGhost):removeWaypointBySpecialType(WAYPOINTRACETRACK)
end

function RaceTrack:getLaptime(pObject)
	local startTime = readScreenPlayData(pObject, self.trackConfig.trackName, "starttime")
	local seconds = getTimestampMilli() - tonumber(startTime)
	writeScreenPlayData(pObject, self.trackConfig.trackName, "laptime",seconds)
	return seconds
end


function RaceTrack:checkPersonalTime(pPlayer)
	local seconds = self:getLaptime(pPlayer)
	CreatureObject(pPlayer):sendSystemMessage("@theme_park/racing/racing:finish_message")
	CreatureObject(pPlayer):sendSystemMessage("Time " .. self:roundNumber(seconds/1000) .. "s")
	local personalTime = tonumber(readScreenPlayData(pPlayer, self.trackConfig.trackName ..".STATS", "PersonalRecordTime"))

	if personalTime == nil then
		personalTime = 9999999999
	end

	if personalTime > seconds then
		writeScreenPlayData(pPlayer, self.trackConfig.trackName ..".STATS", "PersonalRecordTime",seconds)
		CreatureObject(pPlayer):sendSystemMessage("@theme_park/racing/racing:new_record")
	end
end

function RaceTrack:checkServerRecordTime(pPlayer)
	local pGhost = CreatureObject(pPlayer):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local resPos =0
	local seconds = self:getLaptime(pPlayer)
	for lc = 1, 5 , 1 do
		local recordTime = readSharedMemory(self.trackConfig.trackName ..".recordtime."..lc)
		if recordTime == 0 then
			recordTime = 9999999999
		end
		if seconds < recordTime then
			resPos=lc
			break
		end
	end

	if self.trackConfig.debugMode==1 then
		printf("Position :"..resPos)
	end

	if resPos>0 then
		self:adjustResultPositions(resPos, CreatureObject(pPlayer):getFirstName(),seconds)
		if resPos==1 and PlayerObject(pGhost):hasBadge(self.trackConfig.badgeToAward) == false then
			CreatureObject(pPlayer):sendSystemMessage("@theme_park/racing/racing:beat_the_record")
			PlayerObject(pGhost):awardBadge(self.trackConfig.badgeToAward)
		else
			if resPos==1 then
				CreatureObject(pPlayer):sendSystemMessage("@theme_park/racing/racing:beat_the_record")
			else
				CreatureObject(pPlayer):sendSystemMessage("Congratulations! You have beaten position ".. resPos .. " for this track! Record saved")
			end
		end
	end
end

function RaceTrack:adjustResultPositions(resPos,playerName,seconds)
	for lc = 5,resPos,-1 do
		writeSharedMemory(self.trackConfig.trackName ..".recordtime."..lc,readSharedMemory(self.trackConfig.trackName ..".recordtime."..(lc-1)))
		writeStringSharedMemory(self.trackConfig.trackName ..".recordholder."..lc,readStringSharedMemory(self.trackConfig.trackName ..".recordholder."..(lc-1)))
	end

	if playerName == nil then
		playerName="[name error]"
	end

	writeStringSharedMemory(self.trackConfig.trackName ..".recordholder."..resPos,playerName)
	writeSharedMemory(self.trackConfig.trackName ..".recordtime."..resPos,seconds)
end

function RaceTrack:getWaypointIndex(pActiveArea)
	local index = 0
	local wpX = SceneObject(pActiveArea):getPositionX()
	local wpY = SceneObject(pActiveArea):getPositionY()
	for lc = 1, #self.trackConfig.waypoints, 1 do
		if self.trackConfig.waypoints[lc].x==wpX and self.trackConfig.waypoints[lc].y==wpY then
			index = lc
			break
		end
	end
	return index
end

function RaceTrack:displayPersonalBestTime(pObject,trackConfig)
	local personalTime = tonumber(readScreenPlayData(pObject, self.trackConfig.trackName ..".STATS", "PersonalRecordTime"))
	CreatureObject(pObject):sendSystemMessage("@theme_park/racing/racing:whats_my_time")
	if personalTime == nil then
		CreatureObject(pObject):sendSystemMessage("No Time Set!")
	else
		CreatureObject(pObject):sendSystemMessage("Time " .. self:roundNumber(personalTime/1000) .. "s")
	end
end

function RaceTrack:displayTrackBestTime(pObject,trackConfig)
	local numericPositions = {"1st","2nd","3rd","4th","5th"}
	CreatureObject(pObject):sendSystemMessage("Record Holders")
	local results =""
	for lc = 1, 5 , 1 do
		local recordTime = tonumber(readSharedMemory(self.trackConfig.trackName ..".recordtime."..lc))
		if recordTime == 0 then
			results=results .. numericPositions[lc].." : No Time Set!\n"
		else
			results=results .. numericPositions[lc].." : " .. self:roundNumber(recordTime/1000) .. "s by " .. readStringSharedMemory(self.trackConfig.trackName ..".recordholder."..lc) .."\n"
		end
	end
	CreatureObject(pObject):sendSystemMessage(results)
end

function RaceTrack:createResetBestTimeEvent()
	createEventActualTime(self.trackConfig.resetTime, self.trackConfig.className, "resetBestTimeEventHandler")
	if self.trackConfig.debugMode==1 then
		printf("Created CallBack Event for :" .. self.trackConfig.trackName .. "\n")
	end
end

function RaceTrack:resetBestTimeEventHandler()
	if self.trackConfig.debugMode==1 then
		printf("Doing resetBestTimeEventHandler Event for :" .. self.trackConfig.trackName .. "\n")
	end
	for lc = 1, 5 , 1 do
		deleteSharedMemory(self.trackConfig.trackName ..".recordtime."..lc)
		deleteSharedMemory(self.trackConfig.trackName ..".recordholder."..lc)
	end
	createEvent(24*60*60*1000, self.trackConfig.className, "resetBestTimeEventHandler",nil, "")
end

function RaceTrack:createResetPlayerUnfinishedEvent(pObject)
	createEvent(self.trackConfig.expiryTime*1000, self.trackConfig.className, "resetPlayerUnfinishedEventHandler",pObject, "")
	if self.trackConfig.debugMode==1 then
		printf("Created Reset Player CallBack Event for :" .. self.trackConfig.trackName .. " in \n")
	end
end

function RaceTrack:resetPlayerUnfinishedEventHandler(pObject)
	local pGhost = CreatureObject(pObject):getPlayerObject()

	if (pGhost == nil) then
		return
	end

	local startTime = tonumber(readScreenPlayData(pObject, self.trackConfig.trackName , "starttime"))
	if not(startTime == nil) then
		local time = getTimestampMilli()
		if  math.abs((time/1000) - (startTime/1000)) > (self.trackConfig.expiryTime-5) then
			clearScreenPlayData(pObject,self.trackConfig.trackName )
			PlayerObject(pGhost):removeWaypointBySpecialType(WAYPOINTRACETRACK)
			if self.trackConfig.debugMode==1 then
				printf("Reset Player for :" .. self.trackConfig.trackName .. "\n")
			end
		end
	end
end

return RaceTrack
