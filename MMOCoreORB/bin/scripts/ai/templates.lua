includeFile("default.lua")
includeFile("escort.lua")
includeFile("pet.lua")
includeFile("static.lua")
includeFile("deathWatch.lua")
includeFile("enclaveSentinel.lua")
includeFile("crackdown.lua")
includeFile("villageRaider.lua")
--TODO: write templates, put them in the base ai directory and get rid of current ai script directory structure
--includeAiFile("templates/example.lua")
--includeAiFile("templates/stationary.lua")
--includeAiFile("templates/stationarynoleash.lua")
--includeAiFile("templates/follow.lua")
--includeAiFile("templates/wait.lua")
--includeAiFile("templates/idlewander.lua")
--includeAiFile("templates/idlewait.lua")
--includeAiFile("templates/idlepet.lua")
--includeAiFile("templates/gettargetreactive.lua")
--includeAiFile("templates/manualescort.lua")
--includeAiFile("templates/selectattacksimple.lua")
--includeAiFile("templates/combatmovesimple.lua")
--includeAiFile("templates/gettargetpet.lua")
--includeAiFile("templates/selectattackpet.lua")
--includeAiFile("templates/combatmovepet.lua")

customMap = {
	{ "crackdown", {
		{NONE, "rootCrackdown"},
		{AWARE, "awareCrackdown"},
		{IDLE, "idleCrackdown"},
		{LOOKAT, "lookCrackdown"}
	}},

	{ "enclaveSentinel", {
		{AWARE, "awareEnclavesentinel"}
	}},

	{ "deathWatch", {
		{AWARE, "awareDeathwatch"}
	}},

	{ "villageRaider", {
		{AWARE, "awareVillageraider"}
	}},
}

bitmaskLookup = {
	{NONE, {
		{NONE, "rootDefault"},
		{AWARE, "awareDefault"},
		{IDLE, "idleDefault"},
		{ATTACK, "attackDefault"},
		{EQUIP, "equipDefault"},
		{TARGET, "targetDefault"},
		{MOVE, "moveDefault"},
		{LOOKAT, "lookDefault"},
		{AGGRO, "aggroDefault"},
		{SCARE, "scareDefault"},
		{KILL, "killDefault"},
		{STALK, 'stalkDefault'},
		{CRACKDOWNSCAN, "crackdownScanDefault"},
		{HEAL, "healDefault"},
		{CHATREACTION, "chatReactionDefault"}
	}},

	{NPC, {
		{LOOKAT, "failTest"},
		{SCARE, "failTest"}
	}},

	{PET, {
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"}
	}},

	{FACTION_PET, {
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"}
	}},

	{DROID_PET, {
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"}
	}},

	{ESCORT, {
		{IDLE, "runEscort"},
		{MOVE, "moveEscort"},
		{TARGET, "targetDefault"}
	}},

	{FOLLOW, {
		{IDLE, "runEscort"},
		{TARGET, "targetDefault"}
	}},

	{STATIC, {
		{IDLE, "wanderStatic"}
	}},

	{STATIONARY, {
		{IDLE, "idleStatic"}
	}},

	{NOAIAGGRO, {
		{NONE, "rootStatic"}
	}},

	{TEST, {
		{AWARE, "failTest"},
		{IDLE, "succeedTest"},
		{ATTACK, "failTest"},
		{EQUIP, "failTest"},
		{TARGET, "failTest"},
		{MOVE, "failTest"},
		{LOOKAT, "failTest"},
		{AGGRO, "failTest"},
		{SCARE, "failTest"}
	}}
}
