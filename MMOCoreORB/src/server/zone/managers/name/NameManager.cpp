/*
Copyright (C) 2007 <SWGEmu>

This File is part of Core3.

This program is free software; you can redistribute
it and/or modify it under the terms of the GNU Lesser
General Public License as published by the Free Software
Foundation; either version 2 of the License,
or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General
Public License along with this program; if not, write to
the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

Linking Engine3 statically or dynamically with other modules
is making a combined work based on Engine3.
Thus, the terms and conditions of the GNU Lesser General Public License
cover the whole combination.

In addition, as a special exception, the copyright holders of Engine3
give you permission to combine Engine3 program with free software
programs or libraries that are released under the GNU LGPL and with
code included in the standard release of Core3 under the GNU LGPL
license (or modified versions of such code, with unchanged license).
You may copy and distribute such a system following the terms of the
GNU LGPL for Engine3 and the licenses of the other code concerned,
provided that you include the source code of that other code when
and as the GNU LGPL requires distribution of source code.

Note that people who make modified versions of Engine3 are not obligated
to grant this special exception for their modified versions;
it is their choice whether to do so. The GNU Lesser General Public License
gives permission to release a modified version without this exception;
this exception also makes it possible to release a modified version
which carries forward this exception.
 */


#include "NameManager.h"

#include "server/zone/ZoneProcessServer.h"
#include "server/zone/ZoneClientSession.h"
#include "server/zone/objects/player/Races.h"
#include "server/zone/objects/creature/CreatureObject.h"

NameManager::NameManager() {
	initialize();

	profaneNames = new Vector<String>(55, 5); //based on the original number of banned words
	developerNames = new BannedNameSet();
	reservedNames = new BannedNameSet();
	fictionNames = new BannedNameSet();

	fillNames();

	setLogging(false);
}

NameManager::NameManager(ZoneProcessServer* serv) : Logger("NameManager") {
	server = serv;

	initialize();

	profaneNames = new Vector<String>(55, 5); //based on the original number of banned words
	developerNames = new BannedNameSet();
	reservedNames = new BannedNameSet();
	fictionNames = new BannedNameSet();

	fillNames();

	setLogging(false);

}

NameManager::~NameManager() {

	delete lua;

	delete(profaneNames);
	delete(developerNames);
	delete(reservedNames);
	delete(fictionNames);
}

void NameManager::initialize() {
	lua = new Lua();
	lua->init();

	info("loading configuration");
	if(!loadConfigData()) {

		loadDefaultConfig();

		error("Configuration error(s), using defaults");
	}

	info("initialized");

}

bool NameManager::loadConfigFile() {
	return lua->runFile("scripts/managers/name_manager.lua");
}

bool NameManager::loadConfigData() {
	if (!loadConfigFile())
		return false;

	letterMappings.put("a", lua->getGlobalString("a"));
	letterMappings.put("b", lua->getGlobalString("b"));
	letterMappings.put("c", lua->getGlobalString("c"));
	letterMappings.put("d", lua->getGlobalString("d"));
	letterMappings.put("e", lua->getGlobalString("e"));
	letterMappings.put("f", lua->getGlobalString("f"));
	letterMappings.put("g", lua->getGlobalString("g"));
	letterMappings.put("h", lua->getGlobalString("h"));
	letterMappings.put("i", lua->getGlobalString("i"));
	letterMappings.put("j", lua->getGlobalString("j"));
	letterMappings.put("k", lua->getGlobalString("k"));
	letterMappings.put("l", lua->getGlobalString("l"));
	letterMappings.put("m", lua->getGlobalString("m"));
	letterMappings.put("n", lua->getGlobalString("n"));
	letterMappings.put("o", lua->getGlobalString("o"));
	letterMappings.put("p", lua->getGlobalString("p"));
	letterMappings.put("q", lua->getGlobalString("q"));
	letterMappings.put("r", lua->getGlobalString("r"));
	letterMappings.put("s", lua->getGlobalString("s"));
	letterMappings.put("t", lua->getGlobalString("t"));
	letterMappings.put("u", lua->getGlobalString("u"));
	letterMappings.put("v", lua->getGlobalString("v"));
	letterMappings.put("w", lua->getGlobalString("w"));
	letterMappings.put("x", lua->getGlobalString("x"));
	letterMappings.put("y", lua->getGlobalString("y"));
	letterMappings.put("z", lua->getGlobalString("z"));

	letterMappings.put("qu", lua->getGlobalString("qu"));
	letterMappings.put("doublevowel", lua->getGlobalString("doublevowel"));
	letterMappings.put("doubleconsonent", lua->getGlobalString("doubleconsonent"));

	LuaObject organicPrefixesObject = lua->getGlobalObject("organicprefixes");
	for (int i = 1; i <= organicPrefixesObject.getTableSize(); ++i)
		organicPrefixes.add(organicPrefixesObject.getStringAt(i));

	organicPrefixesObject.pop();

	LuaObject organicSuffixesObject = lua->getGlobalObject("organicsuffixes");
	for (int i = 1; i <= organicSuffixesObject.getTableSize(); ++i)
		organicPrefixes.add(organicSuffixesObject.getStringAt(i));

	organicSuffixesObject.pop();

	LuaObject inorganicPrefixesObject = lua->getGlobalObject("inorganicprefixes");
	for (int i = 1; i <= inorganicPrefixesObject.getTableSize(); ++i)
		inorganicPrefixes.add(inorganicPrefixesObject.getStringAt(i));

	inorganicPrefixesObject.pop();

	LuaObject inorganicSuffixesObject = lua->getGlobalObject("inorganicsuffixes");
	for (int i = 1; i <= inorganicSuffixesObject.getTableSize(); ++i)
		inorganicSuffixes.add(inorganicSuffixesObject.getStringAt(i));

	inorganicSuffixesObject.pop();

	LuaObject npcFirstNamesObject = lua->getGlobalObject("npcfirstnames");
	for (int i = 1; i <= npcFirstNamesObject.getTableSize(); ++i)
		npcFirstNames.add(npcFirstNamesObject.getStringAt(i));

	npcFirstNamesObject.pop();

	LuaObject npcSurnamesObject = lua->getGlobalObject("npcsurnames");
	for (int i = 1; i <= npcSurnamesObject.getTableSize(); ++i)
		npcSurnames.add(npcSurnamesObject.getStringAt(i));

	npcSurnamesObject.pop();

	return true;
}

void NameManager::loadDefaultConfig() {


}

void NameManager::fillNames() {
	File* restrictedFile = new File("conf/restrictednames.lst");

	try {
		FileReader restrictedReader(restrictedFile);

		info("parsing restricted names list: restrictednames.lst", true);

		BannedNameSet* setp;

		String line;
		bool isset = false;

		while (restrictedReader.readLine(line)) {
			String name = line.trim().toLowerCase();

			if ((name.length() >= 2 && name.subString(0, 2).compareTo("--") == 0)
					|| name == "") {
				continue; //skip it
			} else if (name.indexOf("[profane]") != -1) {
				isset = false;
				continue;
			} else if (name.indexOf("[developer]") != -1) {
				isset = true;
				setp = developerNames;
				continue;
			} else if (name.indexOf("[fiction]") != -1) {
				isset = true;
				setp = fictionNames;
				continue;
			} else if (name.indexOf("[reserved]") != -1) {
				isset = true;
				setp = reservedNames;
				continue;
			} else if (isset) {
				setp->add(name);
				continue;
			} else {
				profaneNames->add(name);
				continue;
			}
		}
	} catch (FileNotFoundException&e ) {
	}

	delete restrictedFile;
}


bool NameManager::isProfane(String name) {
	uint16 i;

	name = name.toLowerCase();

	for (i = 0; i < profaneNames->size(); i++) {
		if (name.indexOf(profaneNames->get(i)) != -1)
			return true;
	}

	return false;
}

inline bool NameManager::isDeveloper(String name) {
	return developerNames->contains(name.toLowerCase());
}

inline bool NameManager::isFiction(String name) {
	return fictionNames->contains(name.toLowerCase());
}

inline bool NameManager::isReserved(String name) {
	return reservedNames->contains(name.toLowerCase());
}

int NameManager::validateName(CreatureObject* obj) {
	StringId* objectName = obj->getObjectName();
	UnicodeString name = obj->getCustomObjectName();
	int species = obj->getSpecies();

	return validateName(name.toString(), species);
}

int NameManager::validateName(const String& name, int species) {
	if (name.isEmpty())
		return NameManagerResult::DECLINED_EMPTY;

	if (name.length() > 40)
		return NameManagerResult::DECLINED_RACE_INAPP;

	String fname, lname;

	//Split the name into first and last
	int spc = name.indexOf(" ");
	if (spc != -1) {
		fname = name.subString(0, spc);
		lname = name.subString(spc + 1, name.length());
	} else {
		fname = name;
		lname = "";
	}

	if (isProfane(name))
		return NameManagerResult::DECLINED_PROFANE;

	if (isDeveloper(name))
		return NameManagerResult::DECLINED_DEVELOPER;

	if (isFiction(name))
		return NameManagerResult::DECLINED_FICT_RESERVED;

	if (isReserved(name) || isReserved(fname) )
		return NameManagerResult::DECLINED_RESERVED;

	//Make sure that only valid characters are allowed in the name.
	if (strspn(name.toCharArray(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz'- ") != name.length())
		return NameManagerResult::DECLINED_SYNTAX;

	if (species == -1)
		return NameManagerResult::ACCEPTED;

	if (fname.length() < 3 || fname.length() > 15 || lname.length() > 20)
		return NameManagerResult::DECLINED_RACE_INAPP;

	//Wookies are not allowed to have last names.
	if (!lname.isEmpty() && species == CreatureObject::WOOKIE)
		return NameManagerResult::DECLINED_RACE_INAPP;

	//If the name has a hyphen or apostrophe, make sure they are the proper species.
	if (name.indexOf("'") != -1 || name.indexOf("-") != -1) {
		//Must be a human, twilek, or moncal to have a hyphen or apostrophe.
		if (species != CreatureObjectImplementation::HUMAN && species != CreatureObjectImplementation::TWILEK && species != CreatureObjectImplementation::MONCAL && species != CreatureObjectImplementation::ZABRAK)
			return NameManagerResult::DECLINED_RACE_INAPP;

		//Moncal's aren't allowed to have apostrophes.
		if (species == CreatureObjectImplementation::MONCAL && name.indexOf("'") != -1)
			return NameManagerResult::DECLINED_RACE_INAPP;

		//Make sure they only have one hyphen and apostrophe in firstname.
		if (fname.indexOf('\'') != fname.lastIndexOf('\'') || fname.indexOf('-') != fname.lastIndexOf('-'))
			return NameManagerResult::DECLINED_RACE_INAPP;

		//Make sure they only have one hyphen and apostrophe in lastname.
		if (lname.indexOf('\'') != lname.lastIndexOf('\'') || lname.indexOf('-') != lname.lastIndexOf('-'))
			return NameManagerResult::DECLINED_RACE_INAPP;
	}

	return NameManagerResult::ACCEPTED;
}

const String NameManager::makeCreatureName(bool lastName) {

	String name = makeName(3 + System::random(6));

	if(lastName) {
		name += " ";
		name += makeName(3 + System::random(6));
	}

	return name;
}

String NameManager::makeName(int nameLength) {
	String name;

	do {
		name = "";

		/// Get first 2 letters
		name += chooseNextLetter(' ', ' ');
		name += chooseNextLetter(name[0], ' ');

		assert(name.length() != 0);

		while (name.length() < nameLength)
				name += chooseNextLetter(name[name.length() - 1], name[name.length() - 2]);

		name[0] = Character::toUpperCase(name[0]);

	} while (validateName(name, -1) != NameManagerResult::ACCEPTED);

	return name;
}

const String NameManager::makeResourceName(bool isOrganic) {
	int nameLength = 4 + System::random(6);
	String name;

	do {
		name = "";

		if (nameLength > 5 && System::random(2) == 1) {
			addPrefix(name, isOrganic);
		}

		if(name.length() < 2) {
			name += chooseNextLetter(' ', ' ');
			name += chooseNextLetter(name[0], ' ');
		}

		assert(name.length() != 0);

		while (name.length() < nameLength)
				name += chooseNextLetter(name[name.length() - 1], name[name.length() - 2]);

		if (!isVowel(name[name.length() - 1]) && name[name.length() - 1] != 'q' && System::random(1) == 1 )
			addSuffix(name, isOrganic);

		name[0] = Character::toUpperCase(name[0]);

	} while (validateName(name, -1) != NameManagerResult::ACCEPTED);

	return name;
}

char NameManager::chooseNextLetter(const char lastLetter, const char letterBeforeLast) {

	if (letterBeforeLast == ' ' && lastLetter == ' ')
		return 97 + System::random(25);

	StringBuffer last;
	last << Character::toLowerCase(lastLetter);

	String inclusion = "abcdefghijklmnopqrstuvwxyz";

	if (letterBeforeLast != ' ' && (!isVowel(lastLetter) && !isVowel(letterBeforeLast))) {

		inclusion = letterMappings.get("doubleconsonent");

	} else if ((letterBeforeLast != ' ' && isVowel(lastLetter) && isVowel(letterBeforeLast))) {

		inclusion = letterMappings.get("doublevowel");

	} else if ((letterBeforeLast != ' ' && lastLetter == 'u' && letterBeforeLast == 'q')) {

		inclusion = letterMappings.get("qu");

	} else if(letterMappings.contains(last.toString())) {

		inclusion = letterMappings.get(last.toString());
	}

	return chooseLetterInclusive(inclusion);
}

inline bool NameManager::isVowel(const char inChar) {
	if (inChar == 'a' || inChar == 'e' || inChar == 'i' || inChar == 'o'
		|| inChar == 'u' || inChar == 'y')
		return true;
	else
		return false;
}

char NameManager::chooseLetterInclusive(String include) {

	return include.charAt(System::random(include.length() - 1));
}

inline void NameManager::addPrefix(String& name, bool isOrganic) {

	if(isOrganic && organicPrefixes.size() > 0)
		name += organicPrefixes.get(System::random(organicPrefixes.size() - 1));

	if(!isOrganic && inorganicPrefixes.size() > 0)
		name += inorganicPrefixes.get(System::random(inorganicPrefixes.size() - 1));
}

inline void NameManager::addSuffix(String& name, bool isOrganic) {

	if(isOrganic && organicSuffixes.size() > 0)
		name += organicSuffixes.get(System::random(organicSuffixes.size() - 1));

	if(!isOrganic && inorganicSuffixes.size() > 0)
		name += inorganicSuffixes.get(System::random(inorganicSuffixes.size() - 1));

}

void NameManager::test() {
	uint64 start = Time::currentNanoTime();

	int iterations = 1000000;

	for(int i = 0;i < iterations; ++i)
		//System::out << makeResourceName(false) << endl;
		makeResourceName(false);

	for(int i = 0;i < iterations; ++i)
		//System::out << makeResourceName(true) << endl;
		makeResourceName(true);

	for(int i = 0;i < iterations; ++i)
		//System::out << makeCreatureName(true) << endl;
		makeCreatureName(true);

	for(int i = 0;i < iterations; ++i)
		//System::out << makeCreatureName(false) << endl;
		makeCreatureName(false);

	uint64 end = Time::currentNanoTime();

	float nano = (end - start);
	float milli = nano * .000001;
	float seconds = milli / 1000;
	System::out << "Average: " << nano / iterations  << " nanoseconds / " << milli / iterations << " milliseconds" << seconds / iterations  << " seconds" << endl;
	System::out << "Total: " << nano << " nanoseconds / " << milli << " milliseconds" << seconds << " seconds" << endl;
}
