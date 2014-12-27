heroOfTatHermitConvoTemplate = ConvoTemplate:new {
	initialScreen = "",
	templateType = "Lua",
	luaClassHandler = "heroOfTatHermitConvoHandler",
	screens = {}
}

intro = ConvoScreen:new {
	id = "intro",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_48a263f0", -- Well met, traveler.  What brings you to my small and lonely abode?
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_1bb7b1eb", "an_old_man"} -- I was curious to see who lived here.
	}
}
heroOfTatHermitConvoTemplate:addScreen(intro);

an_old_man = ConvoScreen:new {
	id = "an_old_man",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_b2929fdf", -- Merely an old man. I keep to myself and tend my books. I think. I philosophize.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_e9bd97f9", "watch_after_myself"} -- Here? In this cave?
	}
}
heroOfTatHermitConvoTemplate:addScreen(an_old_man);

watch_after_myself = ConvoScreen:new {
	id = "watch_after_myself",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_2a85c56b", -- I am able to watch after myself. These cold, lifeless walls do not disturb me any longer. Far too long have I lived here in solitude. I have become stoic to its effects.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_2d0b6a", "mind_your_manners"} -- What do you philosophize about, old man?
	}
}
heroOfTatHermitConvoTemplate:addScreen(watch_after_myself);

mind_your_manners = ConvoScreen:new {
	id = "mind_your_manners",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_d2a7ff43", -- Mind your manners, child. I philosophize of the state of our worlds...of these dark times.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_f297414", "pirates_and_criminals"} -- Dark times? What do you know of dark times?
	}
}
heroOfTatHermitConvoTemplate:addScreen(mind_your_manners);

pirates_and_criminals = ConvoScreen:new {
	id = "pirates_and_criminals",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_d38cc2cb", -- Pirates and criminals travel freely. The Old Republic has been shattered. The Jedi have been hunted to all but extinction. Over all of this hangs the shadow of the Empire.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_8bfb7334", "galaxy_in_need"}, -- You speak truly. The Empire rules ruthlessly and unjustly.
		{"@conversation/quest_hero_of_tatooine_hermit:s_55937d94", "no_bandying"} -- You speak boldly, my friend. Dangerously, in fact.
	}
}
heroOfTatHermitConvoTemplate:addScreen(pirates_and_criminals);

no_bandying = ConvoScreen:new {
	id = "no_bandying",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_d22f1e74", -- I have no interest in bandying words with you, child. Educate yourself and seek out enlightenment. Perhaps then we might have a civilized discussion.
	stopConversation = "true",
	options = {
	}
}
heroOfTatHermitConvoTemplate:addScreen(no_bandying);

galaxy_in_need = ConvoScreen:new {
	id = "galaxy_in_need",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_66fbfd08", -- It is my belief that the galaxy is in dire need of heroes--those individuals who have demonstrated beyond doubt their dedication to the path of light. You are brash and young, but perhaps one day you will become a hero and bring light to this darkened place.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_9b634554", "noble_intent"} -- I wish to become a hero.
	}
}
heroOfTatHermitConvoTemplate:addScreen(galaxy_in_need);


noble_intent = ConvoScreen:new {
	id = "noble_intent",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_96a66dea", -- A noble intent, but intentions in the face of a challenge are as dust upon a Tatooine windstorm. One must act and through one's actions, one might be proven a hero.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_7abf0913", "travel_the_world"} -- How do I prove that I am a hero?
	}
}
heroOfTatHermitConvoTemplate:addScreen(noble_intent);

travel_the_world = ConvoScreen:new {
	id = "travel_the_world",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_342ec337", -- Travel the world. Perform acts that demonstrate your heroism. Through these actions you will be transformed. A hero must be honorable, intelligent, courageous, and altruistic. You must seek to help the world despite the cost.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_66270434", "small_task"} -- I will seek to prove my character.
	}
}
heroOfTatHermitConvoTemplate:addScreen(travel_the_world);

small_task = ConvoScreen:new {
	id = "small_task",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_4fcf6919", -- Good. First, a small task for you to prove yourself worthy of my challenge...worthy of becoming a hero. Within the depths of this cave, there lies an item which has been stolen from my home by the vile creatures that live here. I need it to continue my studies. It is the skull of a squill. Find it, return to me, and I will present to you the ways in which you can become the Hero of Tatooine.
	stopConversation = "false",
	options = {
		{"@conversation/quest_hero_of_tatooine_hermit:s_d111fbf4", "go_then"} -- I will do this for you.
	}
}
heroOfTatHermitConvoTemplate:addScreen(small_task);

go_then = ConvoScreen:new {
	id = "go_then",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_599bebab", -- Go then. One cannot become a hero by spending all day chatting with a crazed old man...and may the Force be with you.
	stopConversation = "true",
	options = {
	}
}
heroOfTatHermitConvoTemplate:addScreen(go_then);

return_intro = ConvoScreen:new {
	id = "return_intro",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_2a82993c", -- I see that you have returned, my child. What do you seek?
	stopConversation = "false",
	options = {
		-- Handled by convo handler
		--{"@conversation/quest_hero_of_tatooine_hermit:s_af540d0", "proven_worthy"} -- I have the skull you are looking for.
		--{"@conversation/quest_hero_of_tatooine_hermit:s_da196589", "as_you_wish"} -- Nevermind.
	}
}
heroOfTatHermitConvoTemplate:addScreen(return_intro);

proven_worthy = ConvoScreen:new {
	id = "proven_worthy",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_bbb8e096", -- Excellent. You have proven yourself as worthy of my challenges. In order to deserve the Mark of the Hero, you must undergo four different tasks and gain the Marks of Courage, Honor, Intellect, and Altruism. Which task and mark would you like to learn about?
	stopConversation = "true",
	options = {
	}
}
heroOfTatHermitConvoTemplate:addScreen(proven_worthy);

as_you_wish = ConvoScreen:new {
	id = "as_you_wish",
	leftDialog = "@conversation/quest_hero_of_tatooine_hermit:s_6441a2a6", -- As you wish.
	stopConversation = "true",
	options = {
	}
}
heroOfTatHermitConvoTemplate:addScreen(as_you_wish);

addConversationTemplate("heroOfTatHermitConvoTemplate", heroOfTatHermitConvoTemplate);