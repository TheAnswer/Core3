/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "StringId.h"

StringId::StringId() : Object() {
	file = "";
	filler = 0;
	stringID = "";
}

StringId::StringId(const StringId& id) : Object() {
	file = id.file;
	filler = 0;
	stringID = id.stringID;
}

StringId::StringId(const String& fullPath) : Object() {
	setStringId(fullPath);
	filler = 0;
}

StringId::StringId(const char * fullPath) : Object() {
	setStringId(String(fullPath));
	filler = 0;
}

StringId::StringId(const String& fil, const String& stringId) : Object() {
	file = fil;
	stringID = stringId;
	filler = 0;
}

#ifdef CXX11_COMPILER
StringId::StringId(StringId&& id) : Object(), file(std::move(id.file)), filler(id.filler),
		stringID(std::move(id.stringID)) {

}
#endif

void StringId::clear() {
	file = "";
	stringID = "";
	filler = 0;
}

void StringId::setStringId(const String& fullPath) {
	if (fullPath.isEmpty())
		return;

	if (fullPath.charAt(0) == '@') {
		StringTokenizer tokenizer(fullPath.subString(1));
		tokenizer.setDelimeter(":");

		tokenizer.getStringToken(file);
		tokenizer.getStringToken(stringID);
	}
}

void server::zone::objects::scene::variables::to_json(nlohmann::json& j, const server::zone::objects::scene::variables::StringId& str) {
	j["file"] = str.getFile();
	j["filler"] = str.getFiller();
	j["stringID"] = str.getStringID();
}
