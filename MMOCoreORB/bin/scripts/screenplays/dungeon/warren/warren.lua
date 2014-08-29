--Additional Includes
includeFile("dungeon/warren/mirla_convo_handler.lua")
includeFile("dungeon/warren/oevitt_piboi_convo_handler.lua")
includeFile("dungeon/warren/manx_try_convo_handler.lua")
includeFile("dungeon/warren/captain_heff_convo_handler.lua")

ALPHABET = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"}

WarrenScreenPlay = ScreenPlay:new {
	numberOfActs = 1,
	
	states = {
		mirla = {started = 1, done = 2}
	},
	
	elevatorRelock = 3600000, 
	
	lockedCells = {
		8575672
	},
	
	recycleMirla = 3600000,
	
	recycleReactor = 3600000,
	
	frontDoor = {x = -552, z = 1, y = -3820},
	
	turret = {template = "object/installation/turret/turret_block_med.iff", x = 25.156, z = -20, y = -71.22, cell = 8575695, respawn = 1200000},
	
	debris = {x = -54.5, z = -68, y = 0.7, cell = 8575723, respawn = 600000},
	
	terminals = {
		terminal_8575768 = {oid = 8575768, message = "@theme_park/warren/warren_system_messages:welcome_terminal", method="notifyTerminalMessage"},
		terminal_8575769 = {oid = 8575769, message = "@theme_park/warren/warren_system_messages:comm_terminal", method="notifyTerminalMessage"},
		terminal_8575770 = {oid = 8575770, message = "@theme_park/warren/warren_system_messages:diary_terminal", method="notifyTerminalMessage"},
		terminal_8575773 = {oid = 8575773, message = "@theme_park/warren/warren_system_messages:turret_pass_active", method="notifyTurretMessage"},
		terminal_8465579 = {oid = 8465579, message = "@theme_park/warren/warren_system_messages:cell_locked", method="notifyTerminalMessage"}
	},
	
	evidence = {
		evidence_8465570 = {oid = 8465570, item = "object/intangible/data_item/warren_evidence_01.iff", radialText = "@theme_park/warren/warren_system_messages:get_evidence"},
		evidence_8465585 = {oid = 8465585, item = "object/intangible/data_item/warren_evidence_02.iff", radialText = "@theme_park/warren/warren_system_messages:get_evidence"},
		evidence_8465590 = {oid = 8465590, item = "object/intangible/data_item/warren_evidence_03.iff", radialText = "@theme_park/warren/warren_system_messages:get_evidence"},
		evidence_8465591 = {oid = 8465591, item = "object/intangible/data_item/warren_evidence_04.iff", radialText = "@theme_park/warren/warren_system_messages:get_evidence"},
		evidence_8465584 = {oid = 8465584, item = "object/intangible/data_item/warren_encryption_key.iff", radialText = "@theme_park/warren/warren_system_messages:mnu_download"}
	},
	
	--Key is the container oid, value is a list of items to spawn in it.
	questItems = {
		container_8465549 = {oid=8465549, items={"object/tangible/mission/quest_item/warren_farewell_letter.iff"}, respawn=1800000},
		container_8465550 = {oid=8465550, items={"object/tangible/mission/quest_item/warren_controlroom_passkey1.iff"}, respawn=1800000},
		container_8555708 = {oid=8555708, items={"object/tangible/mission/quest_item/warren_controlroom_passkey2.iff"}, respawn=1800000},
		container_8555709 = {oid=8555709, items={"object/tangible/mission/quest_item/warren_controlroom_passkey3.iff"}, respawn=1800000},
		container_8555710 = {oid=8555710, items={"object/tangible/mission/quest_item/warren_controlroom_passkey4.iff"}, respawn=1800000},
		container_8465551 = {oid=8465551, items={"object/tangible/mission/quest_item/warren_turret_sequence.iff"}, respawn=1800000},
		container_8465598 = {oid=8465598, items={"object/tangible/mission/quest_item/warren_core_control_rod_s01.iff"}, respawn=1800000},
		container_8465597 = {oid=8465597, items={"object/tangible/mission/quest_item/warren_core_control_rod_s02.iff"}, respawn=1800000},
		container_8465580 = {oid=8465580, items={"object/tangible/mission/quest_item/warren_inquisitor_letter.iff"}, respawn=1800000}
	},
	
	buildingID = 8575671,
	
	reactorCore = 6425464,
	
	entranceKey = "object/tangible/mission/quest_item/warren_passkey_s01.iff"
}

registerScreenPlay("WarrenScreenPlay", true)

--------------------------------------
--   Initialize screenplay           -
--------------------------------------
function WarrenScreenPlay:start()
	if (isZoneEnabled("dantooine")) then
		self:lockDoors()
		self:spawnActiveAreas()
		self:spawnSceneObjects()
		self:spawnMobiles()
		self:fillContainers()
		self:setupTerminals()
		self:initializeDungeon()
	end
end

function WarrenScreenPlay:lockDoors()
	local pBuilding = getSceneObject(self.buildingID)
	createObserver(ENTEREDBUILDING, "WarrenScreenPlay", "notifyEnteredWarren", pBuilding)
end

function WarrenScreenPlay:spawnActiveAreas()
end

function WarrenScreenPlay:spawnSceneObjects()
	local pDebris = spawnSceneObject("dantooine", "object/tangible/newbie_tutorial/debris.iff", self.debris.x, self.debris.z, self.debris.y, self.debris.cell, 1, 0, 0, 0)
	createObserver(OBJECTDESTRUCTION, "WarrenScreenPlay", "notifyDebrisDestroyed", pDebris)
end

function WarrenScreenPlay:spawnMobiles()
	spawnMobile("dantooine", "oevitt_piboi", 1, -558, 0, -3736, 0, 0)
	spawnMobile("dantooine", "mirla", 1, -59.5, -76, -35.3, 26, 8575725)
	spawnMobile("dantooine", "manx_try", 1, 27, -54, -118.8, -62, 8575714)
	spawnMobile("dantooine", "dirk_maggin", 1, -56.1, -28, -89.4, -140, 8575700)
	spawnMobile("naboo", "captain_heff", 1, 7.6, 12, 84.2, -180, 1688852)
	
	--First Room
	spawnMobile("dantooine", "hostile_huurton", 300, -7.8, -20, -9.9, 53, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, 5.9, -20, -8.5, -83, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, -21.8, -20, -6.8, 91, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, -19, -20, -24.4, 57, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, 1.6, -20, -24.9, -45, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, 12.7, -20, -19.6, 67, 8575677)
	spawnMobile("dantooine", "hostile_huurton", 300, -25.8, -20, -28.2, 55, 8575677) 
	spawnMobile("dantooine", "warren_imperial_worker", 300, -2.3, -20, -21.7, -34, 8575677)
	spawnMobile("dantooine", "warren_imperial_worker", 300, 1.5, -20, -12.6, 88, 8575677)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -14.9, -20, -3.8, 174, 8575677)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -19.1, -20, -23.5, -136, 8575677)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -3.6, -20, -19.9, -140, 8575677)
	
	--Second Room
	spawnMobile("dantooine", "warren_imperial_worker", 300, -23.6, -20, -50.2, 29, 8575678)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -28.2, -20, -47.8, 38, 8575678)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -26.1, -20, -36.7, 157, 8575678)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -5, -20, -48, -51, 8575678)
	spawnMobile("dantooine", "hostile_huurton", 300, -15.9, -20, -39.5, -144, 8575678)
	spawnMobile("dantooine", "hostile_huurton", 300, -22.5, -20, -39.4, -177, 8575678)
	spawnMobile("dantooine", "hostile_huurton", 300, -16.6, -20, -48.9, 57, 8575678)
	
	--Barracks
	spawnMobile("dantooine", "warren_imperial_worker", 300, -67.9, -18, -96.7, 78, 8575684)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -79.6, -18, -97.9, 18, 8575684)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -82, -18, -79.6, 41, 8575684)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -82.8, -18, -107, 153, 8575684)
	spawnMobile("dantooine", "hostile_huurton", 300, -61.9, -18, -111.7, -44, 8575684)
	spawnMobile("dantooine", "hostile_huurton", 300, -64.4, -18, -112.9, -42, 8575684)
	spawnMobile("dantooine", "hostile_huurton", 300, -69.9, -18, -105.7, -14, 8575684)
	spawnMobile("dantooine", "hostile_huurton", 300, -60.8, -18, -86.4, -14, 8575684)
	spawnMobile("dantooine", "hostile_huurton", 300, -69.8, -18, -73.2, 110, 8575684)
	
	--Droid Room
	spawnMobile("dantooine", "warren_scientist", 300, -23.8, -28, -40.2, 23, 8575689)
	spawnMobile("dantooine", "warren_scientist", 300, -23.2, -28, -42.3, 95, 8575689)
	spawnMobile("dantooine", "warren_scientist", 300, -25.6, -28, -45.8, 124, 8575689)
	spawnMobile("dantooine", "warren_scientist", 300, -1.2, -28, -39.4, 65, 8575689)
	spawnMobile("dantooine", "warren_scientist", 300, -0.3, -28, -41.9, 133, 8575689)
	spawnMobile("dantooine", "warren_scientist", 300, -2.3, -28, -44.6, -141, 8575689)
	spawnMobile("dantooine", "warren_agro_droid", 300, 6.8, -28, -52.3, -96, 8575689)
	spawnMobile("dantooine", "warren_agro_droid", 300, 9.1, -28, -37.1, -112, 8575689)
	spawnMobile("dantooine", "warren_agro_droid", 300, -14.6, -28, -31.7, -164, 8575689)
	spawnMobile("dantooine", "warren_agro_droid", 300, -33.4, -28, -38.4, 143, 8575689)
	spawnMobile("dantooine", "warren_agro_droid_boss", 300, 3.6, -28, -69, 0, 8575692)
	
	spawnMobile("dantooine", "warren_imperial_worker", 300, -59.6, -52, -71.7, -13, 8575709)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -61.4, -52, -70, -131, 8575709)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -65, -52, -72.5, 61, 8575709)
	spawnMobile("dantooine", "warren_imperial_worker", 300, -65.5, -52, -67.6, 150, 8575709)
	
	
	spawnMobile("dantooine", "hostile_huurton", 300, -71.8, -52, -11.9, 6, 8575719)
	spawnMobile("dantooine", "hostile_huurton", 300, -73.6, -52, -7.8, 170, 8575719)
	spawnMobile("dantooine", "hostile_huurton", 300, -60.9, -52, -9.8, -159, 8575719)
	spawnMobile("dantooine", "hostile_huurton", 300, -62.3, -52, -7.4, -173, 8575719)
	
	--Inquisitor Cell Room
	spawnMobile("dantooine", "warren_stormtrooper", 300, -16, -54, -140, -45, 8575716)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -19.3, -54, -138.6, 60, 8575716)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -20.9, -54, -132.8, 109, 8575716)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -32.1, -54, -141.2, 45, 8575716)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -30, -54, -143, 17, 8575716)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -30.4, -54, -138.3, 131, 8575716)
	
	spawnMobile("dantooine", "warren_imperial_officer", 300, -16.7, -54, -140.3, 25, 8575716)
	spawnMobile("dantooine", "warren_imperial_officer", 300, -16, -54, -145.6, -72, 8575716)
	spawnMobile("dantooine", "warren_imperial_officer", 300, -16.2, -54, -139.2, -136, 8575716)
	
	spawnMobile("dantooine", "teraud_loyalist", 300, -26.1, -54, -127.4, 100, 8575716)
	spawnMobile("dantooine", "teraud_loyalist", 300, -26.3, -54, -129.4, 58, 8575716)
	spawnMobile("dantooine", "teraud_loyalist", 300, -23.2, -54, -127.5, -47, 8575716)
	spawnMobile("dantooine", "teraud_loyalist", 300, -26, -54, -154.3, 18, 8575716)
	spawnMobile("dantooine", "teraud_loyalist", 300, -28.2, -54, -153.1, 39, 8575716)
	spawnMobile("dantooine", "teraud_loyalist", 300, -23, -54, -153.2, -9, 8575716)
	
	--Evidence Room 1
	spawnMobile("dantooine", "teraud_loyalist_cyborg", 300, 75.6, -68, 19.1, -2, 8575736)
	spawnMobile("dantooine", "doctor_knag", 300, 95.7, -68, 38.8, 90, 8575737)
	
	--Evidence Room 2
	spawnMobile("dantooine", "warren_imperial_officer", 300, 6.3, -60, 36.6, 58, 8575731)
	spawnMobile("dantooine", "warren_imperial_officer", 300, 9.6, -60, 39.3, -166, 8575731)
	spawnMobile("dantooine", "warren_imperial_officer", 300, 5.3, -60, 29.4, 46, 8575731)
	spawnMobile("dantooine", "cyborg_slice_hound", 300, -11.7, -60, 37.7, 89, 8575731)
	spawnMobile("dantooine", "cyborg_slice_hound", 300, -13.3, -60, 34.1, 121, 8575731)
	spawnMobile("dantooine", "cyborg_slice_hound", 300, -7.3, -60, 28.2, 71, 8575731)
	
	spawnMobile("dantooine", "cyborg_bol", 300, -52.6, -52, 17, -90, 8575726)
	spawnMobile("dantooine", "warren_scientist", 300, -46.4, -52, 16.7, 0, 8575726)
	
	spawnMobile("dantooine", "warren_scientist", 300, -84.7, -52, 0, 3, 8575727)
	spawnMobile("dantooine", "warren_scientist", 300, -84.6, -52, 2.6, 2, 8575727)
	spawnMobile("dantooine", "warren_scientist", 300, -100.5, -52, 13.4, 78, 8575727)
	spawnMobile("dantooine", "warren_scientist", 300, -98.5, -52, 20.9, 22, 8575727)
	spawnMobile("dantooine", "warren_scientist", 300, -87.5, -52, 30.2, 139, 8575727)
	spawnMobile("dantooine", "warren_scientist", 300, -79.4, -52, 28.3, 90, 8575727)
	
	--Evidence Room 3
	spawnMobile("dantooine", "cyborg_dune_lizard", 300, -48.8, -52, 50.5, 0, 8575729)
	spawnMobile("dantooine", "cyborg_dune_lizard", 300, -62.4, -52, 51.7, 0, 8575729)
	
	--At St room
	spawnMobile("dantooine", "warren_altered_atst", 300, 25.7, -84, -28.8, 0, 8575745)
	
	--4 locks room
	spawnMobile("dantooine", "warren_agro_droid", 300, 16.9, -20, -95.4, 0, 8575696)
	spawnMobile("dantooine", "warren_agro_droid", 300, 23, -20, -105.2, 0, 8575696)
	spawnMobile("dantooine", "computer_scientist", 300, 40, -20, -109.5, -50, 8575696)
	
	--Big room in middle
	spawnMobile("dantooine", "teraud_loyalist", 300, -28.3, -52, -38.8, -48, 8575740)
	spawnMobile("dantooine", "teraud_loyalist", 300, -32.6, -52, -41.9, -28, 8575740)
	spawnMobile("dantooine", "teraud_loyalist", 300, -36.3, -52, -40.5, 32, 8575740)
	spawnMobile("dantooine", "teraud_loyalist", 300, 2.4, -52, -65.3, 112, 8575740)
	spawnMobile("dantooine", "teraud_loyalist", 300, 2.2, -52, -68.6, 102, 8575740)
	spawnMobile("dantooine", "teraud_loyalist", 300, 5.6, -52, -73, 49, 8575740)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -25.1, -52, -50.2, 143, 8575740)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -29.6, -52, -52.7, 107, 8575740)
	spawnMobile("dantooine", "warren_stormtrooper", 300, -26.2, -52, -55.5, -1, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, -23.1, -52, -67.5, 176, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, -22.5, -52, -69.2, -129, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, -29.1, -52, -71.3, 127, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, 16.6, -52, -35.4, 53, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, 20.1, -52, -37.6, -3, 8575740)
	spawnMobile("dantooine", "hostile_huurton", 300, 24.4, -52, -36.5, -30, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, -17.7, -52, -41.2, 50, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, -15.5, -52, -44, 18, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, -12.5, -52, -42.5, -48, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, 29, -52, -56.4, 121, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, 30.9, -52, -53.3, 126, 8575740)
	spawnMobile("dantooine", "warren_imperial_officer", 300, 34.4, -52, -50.7, 153, 8575740)
	spawnMobile("dantooine", "teraud_loyalist_cyborg", 300, 11.7, -52, -57.7, 33, 8575740)
	spawnMobile("dantooine", "teraud_loyalist_cyborg", 300, 14.1, -52, -58.6, 0, 8575740)
	spawnMobile("dantooine", "teraud_loyalist_cyborg", 300, 15.7, -52, -55.5, -51, 8575740)
	
	--Irradiated Workers
	spawnMobile("dantooine", "warren_irradiated_worker", 300, 56.6, -54, -121.6, -13, 8575715)
	spawnMobile("dantooine", "warren_irradiated_worker", 300, 64, -54, -128.7, 140, 8575715)
	
	--Bors Teraud
	spawnMobile("dantooine", "bors_teraud", 300, 119.5, -52, 41.7, -171, 8575758)
end

function WarrenScreenPlay:fillContainers()
	for k, v in pairs(self.questItems) do
		local pContainer = getSceneObject(v.oid)
		
		if (pContainer ~= nil) then
			local container = LuaSceneObject(pContainer)
			
			container:setContainerComponent("RespawnContainerContentsComponent")
			container:setContainerInheritPermissionsFromParent(false)
			container:setContainerDefaultDenyPermission(MOVEIN)
			container:setContainerDefaultAllowPermission(OPEN + MOVEOUT)
			
			if (container:getContainerObjectsSize() < 1) then		
				for j, item in pairs(v.items) do
					giveItem(pContainer, item, -1)
				end
			end
		end
	end
end

function WarrenScreenPlay:setupTerminals()
	for k, v in pairs(self.terminals) do
		local pTerminal = getSceneObject(v.oid)
		
		if (pTerminal ~= nil) then
			createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", v.method, pTerminal)
		end
	end
	
	--Control Room Terminals
	local pTerminal = getSceneObject(8465559)
	createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "notifyUseCRTerminal", pTerminal)
	pTerminal = getSceneObject(8465560)
	createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "notifyUseCRTerminal", pTerminal)
	pTerminal = getSceneObject(8465561)
	createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "notifyUseCRTerminal", pTerminal) 
	pTerminal = getSceneObject(8465562)
	createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "notifyUseCRTerminal", pTerminal)  
	
	--MCR Elevator
	pTerminal = getSceneObject(8575772)
	
	if (pTerminal ~= nil) then
		local terminal = LuaSceneObject(pTerminal)
		terminal:setObjectMenuComponent("MCRElevatorMenuComponent")
	end
	
	--Register spatial chat observer on terminals
	pTerminal = getSceneObject(8575773)
	
	if (pTerminal ~= nil) then
		createObserver(SPATIALCHATRECEIVED, "WarrenScreenPlay", "notifyTurretTerminalChatReceived", pTerminal)
	end
	
	pTerminal = getSceneObject(8465579)
	
	if (pTerminal ~= nil) then
		createObserver(SPATIALCHATRECEIVED, "WarrenScreenPlay", "notifyInquisitorTerminalChatReceived", pTerminal)
	end
	
	--Set inquisitor cell component
	pTerminal = getSceneObject(8575718)
	
	if (pTerminal ~= nil) then
		local cell = LuaSceneObject(pTerminal)
		cell:setContainerComponent("EnterInquisitorCellComponent")
	end
	
	--Set reactor room
	pTerminal = getSceneObject(8575715)
	
	if (pTerminal ~= nil) then
		local cell = LuaSceneObject(pTerminal)
		cell:setContainerComponent("EnterReactorRoomComponent")
	end
	
	--Reactor Core Switches
	pTerminal = getSceneObject(8465583)
	
	if (pTerminal ~= nil) then
		local terminal = LuaSceneObject(pTerminal)
		terminal:setObjectName("theme_park/warren/warren_system_messages", "switch_name_1")
		createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "useReactorCoreSwitch", pTerminal)
	end
	
	pTerminal = getSceneObject(8465578)
	
	if (pTerminal ~= nil) then
		local terminal = LuaSceneObject(pTerminal)
		terminal:setObjectName("theme_park/warren/warren_system_messages", "switch_name_2")
		createObserver(OBJECTRADIALOPENED, "WarrenScreenPlay", "useReactorCoreSwitch", pTerminal)
	end
	
	--Setup Evidence Terminals
	for k,v in pairs(self.evidence) do
		local pTerminal = getSceneObject(v.oid)
		
		if (pTerminal ~= nil) then
			local terminal = LuaSceneObject(pTerminal)
			terminal:setObjectMenuComponent("DownloadEvidenceMenuComponent")
		end
	end
	
	--Set control room lobby
	pTerminal = getSceneObject(8575749)
	
	if (pTerminal ~= nil) then
		local cell = LuaSceneObject(pTerminal)
		cell:setContainerComponent("EnterControlRoomLobbyComponent")
	end
end

function WarrenScreenPlay:initializeDungeon()
	self:activateTurret(nil)
	
	self:deactivateElevator()
	
	self:recycleMirlaPassword()
	
	self:recycleReactorRoom()
end

function WarrenScreenPlay:notifyUseCRTerminal(pTerminal, pPlayer)
	local terminal = LuaSceneObject(pTerminal)
	local creature = LuaCreatureObject(pPlayer)
	local pInventory = creature:getSlottedObject("inventory")
	
	if (pInventory == nil) then
		return
	end
	
	if (self:isMCRElevatorActive()) then
		return
	end
	
	local objectid = terminal:getObjectID()
	
	local pObj = nil
	
	if (objectid == 8465559) then
		pObj = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_controlroom_passkey1.iff", true)
		
		if (pObj ~= nil) then
			writeData("warren:elevator:lock1", 0)
		else
			terminal:showFlyText("theme_park/warren/warren_system_messages", "mcr_term_active", 0, 255, 0)
			return
		end
	elseif (objectid == 8465560) then
		pObj = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_controlroom_passkey2.iff", true)
		
		if (pObj ~= nil) then
			writeData("warren:elevator:lock2", 0)
		else
			terminal:showFlyText("theme_park/warren/warren_system_messages", "mcr_term_active", 0, 255, 0)
			return
		end
	elseif (objectid == 8465561) then
		pObj = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_controlroom_passkey3.iff", true)
		
		if (pObj ~= nil) then
			writeData("warren:elevator:lock3", 0)
		else
			terminal:showFlyText("theme_park/warren/warren_system_messages", "mcr_term_active", 0, 255, 0)
			return
		end
	elseif (objectid == 8465562) then
		pObj = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_controlroom_passkey4.iff", true)
		
		if (pObj ~= nil) then
			writeData("warren:elevator:lock4", 0)
		else
			terminal:showFlyText("theme_park/warren/warren_system_messages", "mcr_term_active", 0, 255, 0)
			return
		end
	else
		return
	end
	
	if (self:isMCRElevatorActive()) then
		--Then they must've just activated it? right?
		createEvent(self.elevatorRelock, "WarrenScreenPlay", "deactivateElevator", pTerminal)
	end
	
	terminal:showFlyText("theme_park/warren/warren_system_messages", "mcr_term_deactivated", 0, 255, 0)
end

function WarrenScreenPlay:isMCRElevatorActive()
	local lock1 = readData("warren:elevator:lock1")
	local lock2 = readData("warren:elevator:lock2")
	local lock3 = readData("warren:elevator:lock3")
	local lock4 = readData("warren:elevator:lock4")
	
	if (lock1 == 0) and (lock2 == 0) and (lock3 == 0) and (lock4 == 0) then
		return true
	end
	
	return false
end

function WarrenScreenPlay:recycleMirlaPassword()
	local code = self:generatePasscode(5)
	writeStringData("warren:mirla:password", code)
	
	--Also relock the inquisitor cell.
	writeData("warren:inquisitor:locked", 1)
	
	createEvent(self.recycleMirla, "WarrenScreenPlay", "recycleMirlaPassword", nil)
end

function WarrenScreenPlay:recycleReactorRoom()
	writeData("warren:reactor_switch_1:locked", 1)
	writeData("warren:reactor_switch_2:locked", 1)
	writeData("warren:reactor:active", 1)
	
	--Lock the reactor room door
	local pCell = getSceneObject(8575715)
	
	if (pCell ~= nil) then
		local cell = LuaSceneObject(pCell)
		cell:setContainerInheritPermissionsFromParent(false)
		cell:clearContainerDefaultAllowPermission(WALKIN)
		
		local pWarren = getSceneObject(self.buildingID)
		
		if (pWarren ~= nil) then
			local building = LuaBuildingObject(pWarren)
			building:broadcastSpecificCellPermissions(8575715)
		end
	end
	
	--Also, lock the door to the control room
	pCell = getSceneObject(8575755)
	
	if (pCell ~= nil) then
		local cell = LuaSceneObject(pCell)
		cell:setContainerInheritPermissionsFromParent(false)
		cell:clearContainerDefaultAllowPermission(MOVEIN)
		
		local pWarren = getSceneObject(self.buildingID)
		
		if (pWarren ~= nil) then
			local building = LuaBuildingObject(pWarren)
			building:broadcastSpecificCellPermissions(8575755)
		end
	end
end

function WarrenScreenPlay:useReactorCoreSwitch(pTerminal, pPlayer)
	local terminal = LuaSceneObject(pTerminal)
	local objid = terminal:getObjectID()
	
	local switch1 = readData("warren:reactor_switch_1:locked")
	local switch2 = readData("warren:reactor_switch_2:locked") 
	
	if (objid == 8465583) and (switch1 == 1) then
		writeData("warren:reactor_switch_1:locked", 0)
		terminal:showFlyText("theme_park/warren/warren_system_messages", "switch_one", 0, 255, 0)
		switch1 = 0
	elseif (objid == 8465578) and (switch2 == 1) then
		writeData("warren:reactor_switch_2:locked", 0)
		terminal:showFlyText("theme_park/warren/warren_system_messages", "switch_two", 0, 255, 0)
		switch2 = 0
	end
	
	if (switch1 == 0) and (switch2 == 0) then
		local pCell = getSceneObject(8575715)
		
		if (pCell ~= nil) then
			local cell = LuaSceneObject(pCell)
			cell:setContainerDefaultAllowPermission(WALKIN)
			
			local pWarren = getSceneObject(self.buildingID)
			
			if (pWarren ~= nil) then
				local building = LuaBuildingObject(pWarren)
				building:broadcastSpecificCellPermissions(8575715)
			end
		end
	end
end

function WarrenScreenPlay:generatePasscode(length)
	local code = ''
	
	for i = 1, length, 1 do
		code = code .. ALPHABET[getRandomNumber(1,26)]
	end
	
	return code
end

function WarrenScreenPlay:givePasskey1(pPlayer)
	if (pPlayer == nil) then
		return
	end
	
	local player = LuaSceneObject(pPlayer)
	local pPlayerObject = player:getSlottedObject("ghost")
	local playerObject = LuaPlayerObject(pPlayerObject)
	playerObject:addPermissionGroup("warren", true)
	local pInventory = player:getSlottedObject("inventory")
	
	if (pInventory == nil) then
		return
	end
	
	local pItem = giveItem(pInventory, self.entranceKey, -1)
end

function WarrenScreenPlay:respawnDebris(pDebris)
	if (pDebris == nil) then
		return
	end
	
	local pCell = getSceneObject(self.debris.cell)
	
	if (pCell ~= nil) then
		local cell = LuaSceneObject(pCell)
		local debris = LuaTangibleObject(pDebris)
		debris:setConditionDamage(0, false)
		cell:transferObject(pDebris, -1, true)
	end
end

function WarrenScreenPlay:notifyDebrisDestroyed(pDebris, pPlayer)
	local player = LuaCreatureObject(pPlayer)
	local debris = LuaSceneObject(pDebris)
	debris:destroyObjectFromWorld()
	player:clearCombatState(1)
	
	createEvent(self.debris.respawn, "WarrenScreenPlay", "respawnDebris", pDebris)
	
	return 0
end

function WarrenScreenPlay:deactivateElevator()
	writeData("warren:elevator:lock1", 1)
	writeData("warren:elevator:lock2", 1)
	writeData("warren:elevator:lock3", 1)
	writeData("warren:elevator:lock4", 1)
end

function WarrenScreenPlay:notifyEnteredWarren(pBuilding, pPlayer)
	local obj = LuaSceneObject(pPlayer)
	
	if (not obj:isPlayerCreature()) then
		return
	end
	
	local creature = LuaCreatureObject(pPlayer)
	
	local pInventory = obj:getSlottedObject("inventory")
	
	if (pInventory ~= nil) then
		local pItem = getContainerObjectByTemplate(pInventory, self.entranceKey, true)
		
		if (pItem ~= nil) then
			creature:sendSystemMessage("@theme_park/warren/warren_system_messages:access_granted") --ACCESS GRANTED: Passkey code accepted. Welcome to this Imperial Research Facility
		else
			creature:sendSystemMessage("@theme_park/warren/warren_system_messages:access_denied") --ACCESS DENIED:  YOU DO NOT HAVE THE PROPER PASSKEY TO ENTER THIS FACILITY
			--Kick them out to the front door.
			obj:teleport(self.frontDoor.x, self.frontDoor.z, self.frontDoor.y, 0)
		end
	end
	
	return 0
end

function WarrenScreenPlay:activateTurret(pTerminal)
	local objectID = readData("warren:turret:objectid")
	local pTurret = getSceneObject(objectID)
	
	if (pTurret ~= nil) then
		local pCell = getSceneObject(self.turret.cell)
	
		if (pCell ~= nil) then
			local cell = LuaSceneObject(pCell)
			local turret = LuaSceneObject(pTurret)
			cell:transferObject(pTurret, -1, true)
		end
	else
		local pTurret = spawnSceneObject("dantooine", self.turret.template, self.turret.x, self.turret.z, self.turret.y, self.turret.cell, -0.707107, 0, 0.707107, 0)
		
		if (pTurret ~= nil) then
			local turret = LuaSceneObject(pTurret)
			writeData("warren:turret:objectid", turret:getObjectID())
		end
	end
	
	local code = self:generatePasscode(4)
	writeStringData("warren:turret:code", code)
	writeData("warren:turret:active", 1)
end

function WarrenScreenPlay:deactivateTurret(pTerminal)
	writeData("warren:turret:active", 0)
	
	if (pTerminal ~= nil) then
		local terminal = LuaSceneObject(pTerminal)
		terminal:showFlyText("theme_park/warren/warren_system_messages", "turret_pass_deactivated", 0, 255, 0)
		
		local objectID = readData("warren:turret:objectid")
		local pTurret = getSceneObject(objectID)
		
		if (pTurret ~= nil) then
			local turret = LuaSceneObject(pTurret)
			turret:destroyObjectFromWorld()
		end
	end
	
	createEvent(self.turret.respawn, "WarrenScreenPlay", "activateTurret", pTerminal)
end

function WarrenScreenPlay:refillContainer(pContainer)
	if (pContainer == nil) then
		return
	end
	
	local container = LuaSceneObject(pContainer)
	
	if (container:getContainerObjectsSize() > 0) then
		return
	end
	
	local objectID = container:getObjectID()
	
	for k, item in pairs(self.questItems["container_" .. objectID].items) do
		giveItem(pContainer, item, -1)
	end
end

function WarrenScreenPlay:notifyTerminalMessage(pTerminal, pPlayer)
	local terminal = LuaSceneObject(pTerminal)
	local message = self.terminals["terminal_" .. terminal:getObjectID()].message
	
	local suiManager = LuaSuiManager()
	suiManager:sendMessageBox(pTerminal, pPlayer, "@sui:swg", message, "@ok", "WarrenScreenPlay", "notifyOkPressed")
	
	return 0
end

function WarrenScreenPlay:notifyTurretMessage(pTerminal, pPlayer)
	local message = "@theme_park/warren/warren_system_messages:turret_pass_active"
	
	if (readData("warren:turret:active") == 0) then
		message = "@theme_park/warren/warren_system_messages:turret_pass_inactive"
	end

	local suiManager = LuaSuiManager()
	suiManager:sendMessageBox(pTerminal, pPlayer, "@sui:swg", message, "@ok", "WarrenScreenPlay", "notifyOkPressed")
	
	return 0
end

function WarrenScreenPlay:notifyTurretTerminalChatReceived(pTerminal, pChatMessage)
	if (readData("warren:turret:active") == 0) then
		return 0
	end
	
	local msg = getChatMessage(pChatMessage)

	if (msg == readStringData("warren:turret:code")) then
		self:deactivateTurret(pTerminal)
	end
	
	return 0
end

function WarrenScreenPlay:notifyInquisitorTerminalChatReceived(pTerminal, pChatMessage, objectID)
	local msg = getChatMessage(pChatMessage)
	
	if (msg == readStringData("warren:mirla:password")) and (readData("warren:inquisitor:locked") == 1) then
		local terminal = LuaSceneObject(pTerminal)
		terminal:showFlyText("theme_park/warren/warren_system_messages", "cell_open", 0, 255, 0)
		writeData("warren:inquisitor:locked", 0) 
	end
	
	return 0
end

function WarrenScreenPlay:notifyOkPressed()
end

WarrenTurrentMenuComponent = {}

function WarrenTurrentMenuComponent:fillObjectMenuResponse(pSceneObject, pMenuResponse, pPlayer)
	local suiManager = LuaSuiManager()
	suiManager:sendMessageBox(pSceneObject, pPlayer, "@sui:swg", readStringData("warren:turret:code"), "@ok", "WarrenScreenPlay", "notifyOkPressed")
end

function WarrenTurrentMenuComponent:handleObjectMenuSelect(pSceneObject, pPlayer, selectedID)
end

MCRElevatorMenuComponent = {}

function MCRElevatorMenuComponent:fillObjectMenuResponse(pSceneObject, pMenuResponse, pPlayer)
	local menuResponse = LuaObjectMenuResponse(pMenuResponse)
	menuResponse:addRadialMenuItem(199, 3, "@elevator_text:down")
end

function MCRElevatorMenuComponent:handleObjectMenuSelect(pSceneObject, pPlayer, selectedID)
	local creature = LuaCreatureObject(pPlayer)

	if (selectedID ~= 199) or (not WarrenScreenPlay:isMCRElevatorActive()) then
		creature:sendSystemMessage("This elevator terminal is locked.")
		return
	end

	local obj = LuaSceneObject(pSceneObject)
	
	if (creature:getParent() ~= obj:getParent()) then
		return
	end
	
	local z = obj:getPositionZ() - 20
	local x = creature:getPositionX()
	local y = creature:getPositionY()
	
	creature:playEffect("clienteffect", "elevator_descend.cef")	
	creature:teleport(x, z, y, obj:getParentID())
end

RespawnContainerContentsComponent = {}

function RespawnContainerContentsComponent:transferObject(pContainer, pObj, slot)
	return -1
end

function RespawnContainerContentsComponent:canAddObject(pContainer, pObj, slot)
	return -1
end

function RespawnContainerContentsComponent:removeObject(pContainer, pObj, slot)	
	if (pContainer ~= nil) then
		local container = LuaSceneObject(pContainer)
		createEvent(WarrenScreenPlay.questItems["container_" .. container:getObjectID()].respawn, "WarrenScreenPlay", "refillContainer", pContainer)
	end
	
	return -1
end

EnterInquisitorCellComponent = {}

function EnterInquisitorCellComponent:transferObject(pContainer, pObj, slot)
	local obj = LuaSceneObject(pObj)
	
	if (obj:isPlayerCreature()) and (readData("warren:inquisitor:locked") == 1) then
		obj:teleport(26, -50, -149, 8575717)
		return 0
	end
	
	return -1
end

function EnterInquisitorCellComponent:canAddObject(pContainer, pObj, slot)	
	return -1
end

function EnterInquisitorCellComponent:removeObject(pContainer, pObj, slot)	
	return -1
end

EnterReactorRoomComponent = {}

function EnterReactorRoomComponent:transferObject(pContainer, pObj, slot)
	local obj = LuaSceneObject(pObj)
	
	if (obj:isPlayerCreature()) then
	
		--If reactor is active, then we need to check if the player has the rods to shut it off.
		if (readData("warren:reactor:active") == 1) then
			local creature = LuaCreatureObject(pObj) 
			--Check for both reactor rods.
			--object/tangible/mission/quest_item/warren_core_control_rod_s01.iff
			local pInventory = obj:getSlottedObject("inventory")
			
			if (pInventory ~= nil) then
				local pRod1 = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_core_control_rod_s01.iff", true)
				local pRod2 = getContainerObjectByTemplate(pInventory, "object/tangible/mission/quest_item/warren_core_control_rod_s02.iff", true)
				
				if (pRod1 ~= nil) and (pRod2 ~= nil) then
					--Remove from inventory, and shut off reactor overload.
					local rod1 = LuaSceneObject(pRod1)
					local rod2 = LuaSceneObject(pRod2)
					
					rod1:destroyObjectFromWorld()
					rod1:destroyObjectFromDatabase()
					rod2:destroyObjectFromWorld()
					rod2:destroyObjectFromDatabase()
					
					--Yes, in live it was sent twice, probably once for each rod...
					creature:sendSystemMessage("@theme_park/warren/warren_system_messages:insert_rod") --You insert the reactor core control rods into the reactor core.
					creature:sendSystemMessage("@theme_park/warren/warren_system_messages:insert_rod") --You insert the reactor core control rods into the reactor core.
					
					local pReactorCore = getSceneObject(6425464)
					
					if (pReactorCore ~= nil) then
						local reactorCore = LuaSceneObject(pReactorCore)
						reactorCore:showFlyText("theme_park/warren/warren_system_messages", "deactivate_core", 0, 255, 0)
					end
					
					writeData("warren:reactor:active", 0)
					
					--Unlock the door to the control room
					local pCell = getSceneObject(8575755)
		
					if (pCell ~= nil) then
						local cell = LuaSceneObject(pCell)
						cell:setContainerDefaultAllowPermission(WALKIN)
						
						local pWarren = getSceneObject(self.buildingID)
						
						if (pWarren ~= nil) then
							local building = LuaBuildingObject(pWarren)
							building:broadcastSpecificCellPermissions(8575755)
						end
						
						createEvent(WarrenScreenPlay.recycleReactor, "WarrenScreenPlay", "recycleReactorRoom", nil)
					end
				end
			end
		end
		
		obj:teleport(49.5, -54, -125, 8575714)
		return 0
	end
	
	return -1
end

function EnterReactorRoomComponent:canAddObject(pContainer, pObj, slot)	
	return -1
end

function EnterReactorRoomComponent:removeObject(pContainer, pObj, slot)	
	return -1
end

EnterControlRoomLobbyComponent = {}

function EnterControlRoomLobbyComponent:transferObject(pContainer, pObj, slot)
	local obj = LuaSceneObject(pObj)
	
	if (obj:isPlayerCreature()) and (readData("warren:reactor:active") == 1) then
		local creature = LuaCreatureObject(pObj)
		creature:sendSystemMessage("@theme_park/warren/warren_system_messages:core_warning") --WARNING: Reactor Core Overload Imminent. Control Center Lock-Down initiated.
	end
	
	return -1
end

function EnterControlRoomLobbyComponent:canAddObject(pContainer, pObj, slot)	
	return -1
end

function EnterControlRoomLobbyComponent:removeObject(pContainer, pObj, slot)	
	return -1
end

DownloadEvidenceMenuComponent = {}

function DownloadEvidenceMenuComponent:fillObjectMenuResponse(pSceneObject, pMenuResponse, pPlayer)
	local object = LuaSceneObject(pSceneObject)
	
	local menuResponse = LuaObjectMenuResponse(pMenuResponse)
	menuResponse:addRadialMenuItem(20, 3, WarrenScreenPlay.evidence["evidence_" .. object:getObjectID()].radialText)
end

function DownloadEvidenceMenuComponent:handleObjectMenuSelect(pSceneObject, pPlayer, selectedID)
	if (selectedID ~= 20) then
		return
	end
	
	local creature = LuaCreatureObject(pPlayer)
	local pDatapad = creature:getSlottedObject("datapad")
	local object = LuaSceneObject(pSceneObject)
	
	local pEvidence = getContainerObjectByTemplate(pDatapad, WarrenScreenPlay.evidence["evidence_" .. object:getObjectID()].item, false)
	
	if (pDatapad ~= nil) and (pEvidence == nil) then
		local pItem = giveItem(pDatapad, WarrenScreenPlay.evidence["evidence_" .. object:getObjectID()].item, -1)
		
		if (pItem ~= nil) then
			local suiManager = LuaSuiManager()
			suiManager:sendMessageBox(pSceneObject, pPlayer, "@sui:swg", "@theme_park/warren/warren_system_messages:download_complete", "@ok", "WarrenScreenPlay", "notifyOkPressed")
		end
	else
		creature:sendSystemMessage("@theme_park/warren/warren_system_messages:got_evidence") --You have already downloaded evidence from this terminal
	end
end
