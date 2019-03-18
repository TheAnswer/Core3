/*
 * SkillBoxMap.cpp
 *
 *  Created on: 27/12/2009
 *      Author: victor
 */

#include "SkillList.h"

#include "Skill.h"
#include "server/zone/managers/skill/SkillManager.h"

bool SkillList::containsSkill(const String& skillBox) {
	String low = skillBox.toLowerCase();

	ReadLocker readLocker(getLock());

	for (int i = 0; i < vector.size(); ++i) {
		Reference<Skill*> skill = vector.get(i);

		if (skill == NULL)
			continue;

		String name = skill->getSkillName().toLowerCase();

		if (name == low)
			return true;
	}

	SkillManager* skillManager = SkillManager::instance();

	Reference<Skill*> box = skillManager->getSkill(skillBox);

	if (box != NULL) {
		return find(box) != -1;
	}

	return false;
}

void SkillList::getStringList(Vector<String>& skills) const {
	for (int i = 0; i < vector.size(); ++i) {
		const Reference<Skill*>& skill = vector.getUnsafe(i);

		if (skill == NULL)
			continue;

		const String& name = skill->getSkillName();

		skills.emplace(name);
	}
}

bool SkillList::toBinaryStream(ObjectOutputStream* stream) {
#ifdef ODB_SERIALIZATION
	skills.toBinaryStream(stream);
#else
	Vector<String> names;
	getStringList(names);

	TypeInfo<uint32>::toBinaryStream(&updateCounter, stream);
	names.toBinaryStream(stream);
#endif
	return true;
}

void to_json(nlohmann::json& j, const SkillList& s) {
#ifdef ODB_SERIALIZATION
	j = s.skills;
#else
	Vector<String> names;
	s.getStringList(names);

	j = names;
#endif
}

bool SkillList::parseFromBinaryStream(ObjectInputStream* stream) {
#ifdef ODB_SERIALIZATION
	skills.parseFromBinaryStream(stream);
#else
	Vector<String> skills;

	TypeInfo<uint32>::parseFromBinaryStream(&updateCounter, stream);
	skills.parseFromBinaryStream(stream);

	loadFromNames(skills);
#endif

	return true;
}

void SkillList::loadFromNames(Vector<String>& skillBoxes) {
	vector.removeAll();

	SkillManager* skillManager = SkillManager::instance();

	for (int i = 0; i < skillBoxes.size(); ++i) {
		String& name = skillBoxes.get(i);

		Reference<Skill*> box = skillManager->getSkill(name);

		if (box == NULL)
			continue;

		vector.add(box);
	}
}

bool SkillList::add(Skill* skill, DeltaMessage* message) {
	bool val = vector.emplace(skill);

	if (message != NULL && skill != NULL) {
		message->startList(1, ++updateCounter);

		message->insertByte(1);

		String name = skill->getSkillName();
		name.toBinaryStream(message);
	}

	return val;
}

void SkillList::remove(Skill* skill, DeltaMessage* message) {
	int index = find(skill);

	Reference<Skill*> skillObject = vector.remove(index);

	if (message != NULL  && skill != NULL) {
		message->startList(1, ++updateCounter);

		message->insertByte(0);
		String name = skillObject->getSkillName();
		name.toBinaryStream(message);
	}
}

void SkillList::insertToMessage(BaseMessage* msg) {
	msg->insertInt(vector.size());
	msg->insertInt(updateCounter);

	for (int i = 0; i < vector.size(); ++i) {
		const Reference<Skill*>& skill = get(i);

		if (skill == NULL)
			continue;

		msg->insertAscii(skill->getSkillName());
	}
}
