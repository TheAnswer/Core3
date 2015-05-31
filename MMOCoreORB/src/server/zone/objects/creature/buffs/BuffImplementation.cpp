/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "engine/engine.h"

#include "server/zone/objects/creature/buffs/Buff.h"
#include "BuffDurationEvent.h"
#include "BuffList.h"

#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/creature/CreatureAttribute.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/packets/object/Buffs.h"
#include "server/zone/packets/object/ShowFlyText.h"
#include "server/zone/managers/skill/SkillModManager.h"

void BuffImplementation::init() {
	attributeModifiers.setNoDuplicateInsertPlan();
	attributeModifiers.setNullValue(0);
	skillModifiers.setNoDuplicateInsertPlan();
	skillModifiers.setNullValue(0);

	assert(buffDuration >= 0);
}

void BuffImplementation::initializeTransientMembers() {
	ManagedObjectImplementation::initializeTransientMembers();

	attributeModifiers.setNullValue(0);
	skillModifiers.setNullValue(0);
}

void BuffImplementation::loadBuffDurationEvent(CreatureObject* creo) {
	if(nextExecutionTime.getTime() - time(0) > buffDuration) {
		error("Buff timer was f'ed in the a!  Serialized Time:" + String::valueOf((int)(nextExecutionTime.getTime() - time(0))) + " Duration: " + String::valueOf(buffDuration));
		nextExecutionTime = time(0) + (int)buffDuration;
	}

	if (nextExecutionTime.isPast()) {
		buffEvent = new BuffDurationEvent(creo, _this.getReferenceUnsafeStaticCast());
		buffEvent->execute();
		//info("nextExeutionTime.isPast()", true);
	} else {
		buffEvent = new BuffDurationEvent(creo, _this.getReferenceUnsafeStaticCast());
		buffEvent->schedule(nextExecutionTime);

		//info("scheduling buffEvent with nextExecutionTime difference from now" + String::valueOf(nextExecutionTime.miliDifference()), true);
	}
}

void BuffImplementation::notifyLoadFromDatabase() {
/*
	if (buffEvent != NULL && buffEvent->isScheduled()) {
		buffEvent->cancel();
		error("Buff had event scheduled before it was loaded!");
	}

	*/

	//info("initializeTransientMembers() nextExecutionTime difference from now" + String::valueOf(nextExecutionTime.miliDifference()), true);
}

void BuffImplementation::sendTo(CreatureObject* player) {
	if (buffCRC != 0) {
		AddBuffMessage* abm = new AddBuffMessage(player, buffCRC, getTimeLeft());
		player->sendMessage(abm);
	}
}

void BuffImplementation::setBuffEventNull() {
	buffEvent = NULL;
}

void BuffImplementation::sendDestroyTo(CreatureObject* player) {
	if (!player->hasBuff(buffCRC)) {
		RemoveBuffMessage* rbm = new RemoveBuffMessage(player, buffCRC);
		player->sendMessage(rbm);
	}
}

void BuffImplementation::activate(bool applyModifiers) {
	//info("activating buff with crc " + String::hexvalueOf((int)buffCRC), true);
	try {
		if (applyModifiers && !modsApplied) {
			applyAttributeModifiers();
			applySkillModifiers();
			applyStates();
			modsApplied = true;
		}

		scheduleBuffEvent();

		//info("nextExecutionTime miliDifference:" + String::valueOf(nextExecutionTime.miliDifference()), true);

		if (creature.get()->isPlayerCreature())
			sendTo((cast<CreatureObject*>(creature.get().get())));

		if (!startMessage.isEmpty())
			creature.get()->sendSystemMessage(startMessage);

		if (!startFlyFile.isEmpty())
			creature.get()->showFlyText(startFlyFile, startFlyAux, startFlyRed, startFlyGreen, startFlyBlue);

		if (!startSpam.isEmpty()) {
			creature.get()->sendStateCombatSpam(startSpam.getFile(), startSpam.getStringID(), spamColor, 0, broadcastSpam);
		}

	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}
}

void BuffImplementation::deactivate(bool removeModifiers) {
	ManagedReference<CreatureObject*> strongRef = creature.get().get();

	if (strongRef == NULL)
		return;

	try {
		if (removeModifiers && modsApplied) {
			removeAttributeModifiers();
			removeSkillModifiers();
			removeStates();
			modsApplied = false;
		}

		if (creature.get()->isPlayerCreature())
			sendDestroyTo(cast<CreatureObject*>(creature.get().get()));

		if (!endMessage.isEmpty())
			creature.get()->sendSystemMessage(endMessage);

		if (!endFlyFile.isEmpty())
			creature.get()->showFlyText(endFlyFile, endFlyAux, endFlyRed, endFlyGreen, endFlyBlue);

		if (!endSpam.isEmpty()) {
			creature.get()->sendStateCombatSpam(endSpam.getFile(), endSpam.getStringID(), spamColor, 0, broadcastSpam);
		}

		clearBuffEvent();

	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}
}

void BuffImplementation::parseAttributeModifierString(const String& modifierstring) {
	StringTokenizer tokenizer(modifierstring);
	tokenizer.setDelimeter(";");

	while (tokenizer.hasMoreTokens()) {
		String token;
		tokenizer.getStringToken(token);

		int tokpos = token.indexOf("=");

		if (tokpos == -1)
			continue;

		uint8 attribute = CreatureAttribute::getAttribute(token.subString(0, tokpos));
		int value = Integer::valueOf(token.subString(tokpos + 1, token.length()));

		attributeModifiers.put(attribute, value);
	}
}

void BuffImplementation::parseSkillModifierString(const String& modifierstring) {
	StringTokenizer tokenizer(modifierstring);
	tokenizer.setDelimeter(";");

	while (tokenizer.hasMoreTokens()) {
		String token;
		tokenizer.getStringToken(token);

		int tokpos = token.indexOf("=");

		if (tokpos == -1)
			continue;

		String modname = token.subString(0, tokpos);
		float value = Float::valueOf(token.subString(tokpos + 1, token.length()));

		skillModifiers.put(modname, (int) value);
	}
}

String BuffImplementation::getAttributeModifierString() {
	if (attributeModifiers.size() == 0)
		return String("none");

	String retString = "";

	for (int i = 0; i < attributeModifiers.size(); i++) {
		VectorMapEntry<byte, int> entry = attributeModifiers.elementAt(i);
		retString += CreatureAttribute::getName(entry.getKey()) + " +" + String::valueOf(entry.getValue()) + ";";
	}

	return retString;
}

String BuffImplementation::getSkillModifierString() {
	if (skillModifiers.size() == 0)
		return String("none");

	String retString = "";

	for (int i = 0; i < skillModifiers.size(); i++) {
		VectorMapEntry<String, int> entry = skillModifiers.elementAt(i);
		retString += entry.getKey() + " +" + String::valueOf(entry.getValue()) + "; ";
	}

	return retString;
}

void BuffImplementation::scheduleBuffEvent() {
	buffEvent = new BuffDurationEvent(creature.get(), _this.getReferenceUnsafeStaticCast());
	buffEvent->schedule((int) (buffDuration * 1000));
	Core::getTaskManager()->getNextExecutionTime(buffEvent, nextExecutionTime);
}

float BuffImplementation::getTimeLeft() {
	if (buffEvent == NULL || !buffEvent->isScheduled()) {
		//info("buffEvent == NULL || !buffEvent->isScheduled()", true);
		return 0.0f;
	}

	Time next;

	Core::getTaskManager()->getNextExecutionTime(buffEvent, next);

	float timeleft = round(Time().miliDifference(next) / 1000.0f);

	//info("timeLeft = " + String::valueOf(timeleft), true);

	return MAX(0.0f, timeleft);
}


void BuffImplementation::applyAttributeModifiers() {
	if (creature.get() == NULL)
		return;

	int size = attributeModifiers.size();

	if (size <= 0)
		return;

	for (int i = 0; i < size; ++i) {
		VectorMapEntry<uint8, int>* entry = &attributeModifiers.elementAt(i);

		uint8 attribute = entry->getKey();
		int value = entry->getValue();

		if (value == 0)
			continue;

		try {
			int attributemax = creature.get()->getMaxHAM(attribute) + value;
			int attributeval = MAX(attributemax, creature.get()->getHAM(attribute) + value);

			creature.get()->setMaxHAM(attribute, attributemax);

			if (!creature.get()->isDead() && !creature.get()->isIncapacitated()) {
				if (fillAttributesOnBuff) {
					//creature.get()->setHAM(attribute, attributeval - creature.get()->getWounds(attribute));
					creature.get()->healDamage(creature.get(), attribute, attributeval, true);
				} else if (value >= 0)
					creature.get()->healDamage(creature.get(), attribute, value);
			}
		} catch (Exception& e) {
			error(e.getMessage());
			e.printStackTrace();
		}
	}

}

void BuffImplementation::applySkillModifiers() {
	if (creature.get() == NULL)
		return;

	int size = skillModifiers.size();

	for (int i = 0; i < size; ++i) {
		VectorMapEntry<String, int>* entry = &skillModifiers.elementAt(i);

		String key = entry->getKey();
		int value = entry->getValue();

		creature.get()->addSkillMod(SkillModManager::BUFF, key, value, true);
	}

	// if there was a speed or acceleration mod change, this will take care of immediately setting them.
	// the checks for if they haven't changed are in these methods
	creature.get()->updateSpeedAndAccelerationMods();
	creature.get()->updateTerrainNegotiation();
}

void BuffImplementation::applyStates() {
	if (creature.get() == NULL)
		return;

	int size = states.size();

	for (int i = 0; i < size; ++i) {

		creature.get()->setState(states.get(i), true);
	}
}

void BuffImplementation::removeAttributeModifiers() {
	if (creature.get() == NULL)
		return;

	int size = attributeModifiers.size();

	if (size <= 0)
		return;

	for (int i = 0; i < size; ++i) {
		VectorMapEntry<uint8, int>* entry = &attributeModifiers.elementAt(i);

		uint8 attribute = entry->getKey();
		int value = entry->getValue();

		if (value == 0)
			continue;

		try {

			int attributemax = creature.get()->getMaxHAM(attribute) - value;

			int currentVal = creature.get()->getHAM(attribute);

			creature.get()->setMaxHAM(attribute, attributemax);

			if (currentVal >= attributemax) {
				//creature.get()->inflictDamage(creature.get(), attribute, currentVal - attributemax, isSpiceBuff());

				if (attribute % 3 == 0) {
					creature.get()->inflictDamage(creature.get(), attribute, currentVal - attributemax, false);
				} // else setMaxHam sets secondaries to max
			}

		} catch (Exception& e) {
			error(e.getMessage());
			e.printStackTrace();
		}


		/*int attributeval = MIN(attributemax, creature.get()->getHAM(attribute) - value);

		creature.get()->setHAM(attribute, attributeval);*/
	}

}

void BuffImplementation::removeSkillModifiers() {
	if (creature.get() == NULL)
		return;

	int size = skillModifiers.size();

	for (int i = 0; i < size; ++i) {
		VectorMapEntry<String, int>* entry = &skillModifiers.elementAt(i);

		String key = entry->getKey();
		int value = entry->getValue();

		creature.get()->addSkillMod(SkillModManager::BUFF, key, -value, true);

	}

	// if there was a speed or acceleration mod change, this will take care of immediately setting them.
	// the checks for if they haven't changed are in these methods
	creature.get()->updateSpeedAndAccelerationMods();
	creature.get()->updateTerrainNegotiation();
}

void BuffImplementation::removeStates() {
	if (creature.get() == NULL)
		return;

	int size = states.size();

	for (int i = 0; i < size; ++i) {
		creature.get()->clearState(states.get(i), true);
	}
}

void BuffImplementation::clearBuffEvent() {
	if (buffEvent != NULL) {
		if (buffEvent->isScheduled())
			buffEvent->cancel();

		buffEvent->setBuffObject(NULL);
		buffEvent = NULL;
		nextExecutionTime.updateToCurrentTime();
	}
}

bool BuffImplementation::isActive() {
	return (buffEvent != NULL && buffEvent->isScheduled());
}

void BuffImplementation::setStartMessage(StringIdChatParameter& start) {
	startMessage = start;
}

void BuffImplementation::setEndMessage(StringIdChatParameter& end) {
	endMessage = end;
}

void BuffImplementation::setStartSpam(StringIdChatParameter& start) {
	startSpam = start;
}

void BuffImplementation::setEndSpam(StringIdChatParameter& end) {
	endSpam = end;
}

void BuffImplementation::setSpamColor(uint8 color) {
	spamColor = color;
}

void BuffImplementation::setBroadcastSpam(bool value) {
	broadcastSpam = value;
}

void BuffImplementation::setStartFlyText(const String& file, const String& aux, uint8 red, uint8 green, uint8 blue) {
	startFlyFile = file;
	startFlyAux = aux;
	startFlyRed = red;
	startFlyGreen = green;
	startFlyBlue = blue;
}

void BuffImplementation::setEndFlyText(const String& file, const String& aux, uint8 red, uint8 green, uint8 blue) {
	endFlyFile = file;
	endFlyAux = aux;
	endFlyRed = red;
	endFlyGreen = green;
	endFlyBlue = blue;
}

