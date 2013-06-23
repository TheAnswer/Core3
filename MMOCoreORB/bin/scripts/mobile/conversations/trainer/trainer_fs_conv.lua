trainer_fs_convotemplate = ConvoTemplate:new {
	initialScreen = "convoscreen1",
	screens = {}
}

trainer_fs_convoscreen1 = ConvoScreen:new {
	id = "convoscreen1",
	leftDialog = "@skill_teacher:trainer_fs", -- The secrets of the Force will reveal themselves to those who seek them.
	stopConversation = "false",
	options = { 
		{"@skill_teacher:opt1_1", "convoscreen2"}, -- I'm interested in learning a skill.
		{"@skill_teacher:opt1_2", "convoscreen3"} -- What skills will I be able to learn next?
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen1);

-- Screen coming with the optionlist of all currently learnable skills
trainer_fs_convoscreen2 = ConvoScreen:new {
	id = "convoscreen2",
	leftDialog = "@skill_teacher:msg2_1", -- You may learn the following skills right now...
	stopConversation = "false",
	options = { 
		{"@skill_teacher:back", "convoscreen1" } -- Can we start again?
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen2);

-- coming with the optionlist for all the skills you are able to learn next?
trainer_fs_convoscreen3 = ConvoScreen:new {
	id = "convoscreen3",
	leftDialog = "@skill_teacher:msg2_2", -- Here are the skills I can teach you next, if you have gained enough experience...
	stopConversation = "false",
	options = {
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen3);

-- poping up when conversing with a trainer you are not qualified for
trainer_fs_convoscreen4 = ConvoScreen:new {
	id = "convoscreen4",
	leftDialog = "@skill_teacher:no_qualify", --You do not qualify for any of my teachings. Come back and speak with me later if you acquire the following skills...
	stopConversation = "false",
	options = { }
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen4);

-- after clicking on a learable skill
trainer_fs_convoscreen5 = ConvoScreen:new {
	id = "convoscreen5",
	leftDialog = "@skill_teacher:confirm", --Are you sure you wish to purchase this skill?
	stopConversation = "false",
	options = {
		{"@skill_teacher:yes", "convoscreen6"},
		{"@skill_teacher:no", "convoscreen7"}
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen5);

-- after clicking yes when asked "Are you sure..."
trainer_fs_convoscreen6 = ConvoScreen:new {
	id = "convoscreen6",
	leftDialog = "@skill_teacher:msg_yes", --Excellent! Let's get started then...
	stopConversation = "false",
	options = { }
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen6);

-- after clicking no when asked "Are you sure..."
trainer_fs_convoscreen7 = ConvoScreen:new {
	id = "convoscreen7",
	leftDialog = "@skill_teacher:msg_no", --Too rich for ya? Well, maybe I can help you with something else?
	stopConversation = "false",
	options = {
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen7);

-- after successfully learning a skill and still having skills left to learn (enogh xp already or not)
trainer_fs_convoscreen8 = ConvoScreen:new {
	id = "convoscreen8",
	leftDialog = "@skill_teacher:msg3_2", -- You're an exceptionally bright student! Perhaps we shall work together again soon.
	stopConversation = "false",
	options = {
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen8);

-- after successfully learning the last available skill from a trainer
trainer_fs_convoscreen9 = ConvoScreen:new {
	id = "convoscreen9",
	leftDialog = "@skill_teacher:surpass_trainer", --WOW! You have learned everything I have to teach... I suppose that now it's time for you to find your own path. Farewell %TT.
	stopConversation = "false",
	options = { }
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen9);

-- after clicking a skill from the "What skills will I be able to learn next?" optionlist
trainer_fs_convoscreen10 = ConvoScreen:new {
	id = "convoscreen10",
	leftDialog = "@skill_teacher:msg3_3", -- Let me give you some information about this skill...
	stopConversation = "false",
	options = { 
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen10);

-- after clicking "I'm interested in learning a skill." and not having enogh xp for any skill
trainer_fs_convoscreen11 = ConvoScreen:new {
	id = "convoscreen11",
	leftDialog = "@skill_teacher:error_empty_category", -- Sorry, you aren't quite up to the point where I can teach you anything else.
	stopConversation = "false",
	options = { 
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen11);

trainer_fs_convoscreen12 = ConvoScreen:new {
	id = "convoscreen12",
	leftDialog = "@skill_teacher:no_skill_pts", -- How can I teach you when you do not have any available skill points.
	stopConversation = "false",
	options = { 
		{"@skill_teacher:back", "convoscreen1" }
	}
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen12);

-- conversing with a trainer after reaching master in that profession
trainer_fs_convoscreen13 = ConvoScreen:new {
	id = "convoscreen13",
	leftDialog = "@skill_teacher:topped_out", -- I'm sorry, but I cannot teach you anymore. You have already learned everything I have to teach.
	stopConversation = "false",
	options = { }
}
trainer_fs_convotemplate:addScreen(trainer_fs_convoscreen13);

addConversationTemplate("trainer_fs_convotemplate", trainer_fs_convotemplate);

-- -- not sure where to put:
-- @skill_teacher:too_complicated 	-> I can't teach you anything. Everything I know is entirely too complicated for you.

-- -- same as @skill_teacher:topped_out:
-- @skill_teacher:msg3_1		-> There is nothing more I can teach you!

-- -- Messages to console, screenmessages:
-- @skill_teacher:nsf_skill_pts		-> You must first make %DI skill points available before I can teach you %TO.
-- @skill_teacher:prose_cost 		-> It will cost %DI credits to train in %TO. Would you like to continue?
-- @skill_teacher:prose_nsf 		-> You lack the %DI credits required for training in %TO.
-- @skill_teacher:prose_pay		-> Processing %DI credit payment for %TO training.
-- @skill_teacher:prose_skill_learned 	-> You successfully train in the %TO skill.
-- @skill_teacher:prose_train_failed	-> You were unable to complete %TO skill training.
-- @skill_teacher:success		-> You have successfully learned the skill.

-- -- unneeded(?):
-- @skill_teacher:opt1_3	-> Skills you have that I do not...
-- ->	@skill_teacher:msg2_3	-> Skills that I possess that you do not?! Well, what skill are you interested in?
-- @skill_teacher:opt1_4	-> All of your skills...
-- ->	@skill_teacher:msg2_4	-> All of my skills? Oh well, which one do you want to know about?

-- @skill_teacher:error_grant_skill	-> Hrmm... seems that I cant teach you that skill due to an internal error. TestTrac?
-- @skill_teacher:requires_jtl	-> You must enable the Star Wars Galaxies: Jump to Lightspeed expansion in order to converse with this npc.
-- @skill_teacher:msg1_1	-> Heya. What kind of skills would you like to know about

-- -- stuff for the message box popping up when talking to a trainer you are not qualified for:
-- @skill_teacher:no_qualify_prompt	-> You must have, at least, the following skills to learn from this skill trainer.
-- @skill_teacher:no_qualify_title	-> Trainer Skill Requirements





