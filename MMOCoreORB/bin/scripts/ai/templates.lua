includeFile("default.lua")
includeFile("escort.lua")
includeFile("pet.lua")
includeFile("static.lua")
includeFile("deathWatch.lua")
includeFile("enclaveSentinel.lua")
includeFile("crackdown.lua")
includeFile("villageRaider.lua")

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
		{CHATREACTION, "chatReactionDefault"},
		{NOTIFYHELP, "notifyHelpDefault"},
		{HARVEST, "harvestPet"}
	}},

	{NPC, {
		{LOOKAT, "failTest"},
		{SCARE, "failTest"}
	}},

	{PET, {
		{NONE, "rootPet"},
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"},
		{HARVEST, "harvestPet"}
	}},

	{FACTION_PET, {
		{NONE, "rootPet"},
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"}
	}},

	{DROID_PET, {
		{NONE, "rootPet"},
		{AWARE, "awarePet"},
		{EQUIP, "equipPet"},
		{IDLE, "idlePet"},
		{TARGET, "targetPet"},
		{MOVE, "movePet"},
		{HARVEST, "harvestPet"}
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
