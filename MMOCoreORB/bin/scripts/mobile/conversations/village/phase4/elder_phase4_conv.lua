villageElderPhase4ConvoTemplate = ConvoTemplate:new {
	initialScreen = "",
	templateType = "Lua",
	luaClassHandler = "villageElderConvoHandler",
	screens = {}
}

intro = ConvoScreen:new {
	id = "intro",
	leftDialog = "@conversation/village_elder_4:s_8aa931e3", -- Be careful, we've received information that the Sith Shadows will attack soon.
	stopConversation = "false",
	options = {
		{"@conversation/village_elder_4:s_e5a75b57", "our_bane"}, -- Who are the Sith Shadows?
		{"@conversation/village_elder_4:s_a098d669", "finish_us_off"}, -- Again? Haven't they learned their lesson?
	-- Add via convo handler
	--{"@conversation/village_elder_4:s_9dc8bf5d", ""}, -- I seem to have misplaced my Force crystal. May I have another?
	}
}
villageElderPhase4ConvoTemplate:addScreen(intro);

finish_us_off = ConvoScreen:new {
	id = "finish_us_off",
	leftDialog = "@conversation/village_elder_4:s_d1431da3", -- Apparently not. I think they are looking to finish us off this time.
	stopConversation = "false",
	options = {
		{"@conversation/village_elder_4:s_de1f69ee", "sure_hope_so"}, -- Well they are going to have a tough time with all the new defenses.
	}
}
villageElderPhase4ConvoTemplate:addScreen(finish_us_off);

need_more_help = ConvoScreen:new {
	id = "need_more_help",
	leftDialog = "@conversation/village_elder_4:s_d1431da3", -- Apparently not. I think they are looking to finish us off this time.
	stopConversation = "false",
	options = {
		{"@conversation/village_elder_4:s_de1f69ee", "need_more_help"}, -- Well they are going to have a tough time with all the new defenses.
	}
}
villageElderPhase4ConvoTemplate:addScreen(need_more_help);

our_bane = ConvoScreen:new {
	id = "our_bane",
	leftDialog = "@conversation/village_elder_4:s_448b67d0", -- They seem to be our bane. For some reason they assaulted our village many days ago. We've been rebuilding and reinforcing our village ever since.
	stopConversation = "false",
	options = {
		{"@conversation/village_elder_4:s_aba5c1a8", "sure_hope_so"}, -- You look pretty well defended.
	}
}
villageElderPhase4ConvoTemplate:addScreen(our_bane);

sure_hope_so = ConvoScreen:new {
	id = "sure_hope_so",
	leftDialog = "@conversation/village_elder_4:s_12b92ec4", -- I sure hope so, but I think we're going to need more help than just the shield generator and blaster turrets.
	stopConversation = "false",
	options = {
		{"@conversation/village_elder_4:s_17446822", "need_good_skirmishers"}, -- I am tough enough to pull the ears off a gundark.
		{"@conversation/village_elder_4:s_f24ae96", "need_good_soldiers"}, -- I'm a crack shot with a blaster.
		{"@conversation/village_elder_4:s_4d4da25b", "need_all_help"}, -- I am a medic. I could help heal your soldiers.
	}
}
villageElderPhase4ConvoTemplate:addScreen(sure_hope_so);

need_good_skirmishers = ConvoScreen:new {
	id = "need_good_skirmishers",
	leftDialog = "@conversation/village_elder_4:s_ae755ac3", -- Well we need good skirmishers to meet them head on. Speak to Captain Sarguillo, right over there.
	stopConversation = "true",
	options = {}
}
villageElderPhase4ConvoTemplate:addScreen(need_good_skirmishers);

need_good_soldiers = ConvoScreen:new {
	id = "need_good_soldiers",
	leftDialog = "@conversation/village_elder_4:s_e0741a0c", -- Captain Sarguillo has need of good soldiers like you. He's right over there.
	stopConversation = "true",
	options = {}
}
villageElderPhase4ConvoTemplate:addScreen(need_good_soldiers);

need_all_help = ConvoScreen:new {
	id = "need_all_help",
	leftDialog = "@conversation/village_elder_4:s_8c109ae0", -- I am sure Sivarra will need all the help she can get. You can find her at the medic tent to the Northwest.
	stopConversation = "true",
	options = {}
}
villageElderPhase4ConvoTemplate:addScreen(need_all_help);

already_have_crystal = ConvoScreen:new {
	id = "already_have_crystal",
	leftDialog = "@conversation/village_elder_4:s_199fe2af", -- I sense you already have a crystal and do not need another.
	stopConversation = "true",
	options = {}
}
villageElderPhase4ConvoTemplate:addScreen(already_have_crystal);

give_new_crystal = ConvoScreen:new {
	id = "give_new_crystal",
	leftDialog = "@conversation/village_elder_4:s_bdd3bae5", -- Here you are. Remember you may use the crystal to view your progress.
	stopConversation = "true",
	options = {}
}
villageElderPhase4ConvoTemplate:addScreen(give_new_crystal);

addConversationTemplate("villageElderPhase4ConvoTemplate", villageElderPhase4ConvoTemplate);
