/*
 * ObjectControllerImplementation.cpp
 *
 *  Created on: 11/08/2009
 *      Author: victor
 */

#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/objectcontroller/command/CommandConfigManager.h"
#include "server/zone/managers/objectcontroller/command/CommandList.h"

#include "server/zone/managers/skill/SkillModManager.h"

#include "server/zone/objects/creature/LuaCreatureObject.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/player/PlayerObject.h"

#include "server/db/ServerDatabase.h"

#include "server/zone/Zone.h"

void ObjectControllerImplementation::loadCommands() {
	configManager = new CommandConfigManager(server);
	queueCommands = new CommandList();

	info("loading queue commands...", true);
	configManager->registerSpecialCommands(queueCommands);
	configManager->loadSlashCommandsFile();

	StringBuffer infoMsg;
	infoMsg << "loaded " << queueCommands->size() << " commands";
	info(infoMsg.toString(), true);

	adminLog.setLoggingName("AdminCommands");
	StringBuffer fileName;
	fileName << "log/admin/admin.log";
	adminLog.setFileLogger(fileName.toString(), true);
	adminLog.setLogging(true);

	// LUA
	/*init();
	Luna<LuaCreatureObject>::Register(L);

	runFile("scripts/testscript.lua");*/
}

void ObjectControllerImplementation::finalize() {
	info("deleting object controller", true);

	delete configManager;
	configManager = NULL;

	CommandConfigManager::slashCommands = NULL;

	delete queueCommands;
	queueCommands = NULL;
}

bool ObjectControllerImplementation::transferObject(SceneObject* objectToTransfer, SceneObject* destinationObject, int containmentType, bool notifyClient, bool allowOverflow) {
	ManagedReference<SceneObject*> parent = objectToTransfer->getParent();

	if (parent == NULL) {
		error("objectToTransfer parent is NULL in ObjectManager::transferObject");
		return false;
	}

	uint32 oldContainmentType = objectToTransfer->getContainmentType();

	if (!destinationObject->transferObject(objectToTransfer, containmentType, notifyClient, allowOverflow)) {
		error("could not add objectToTransfer to destinationObject in ObjectManager::transferObject");
		parent->transferObject(objectToTransfer, oldContainmentType);

		return false;
	}

	/*if (destinationObject->getZone() != NULL && objectToTransfer->getZone() == NULL)
		destinationObject->broadcastObject(objectToTransfer, false);

	uint32 oldContainmentType = objectToTransfer->getContainmentType();

	//What about nested containers inside of the inventory...
	SceneObject* playerParent = objectToTransfer->getParentRecursively(SceneObjectType::PLAYERCREATURE);

	if (playerParent != NULL && destinationObject->isCellObject())
		objectToTransfer->sendDestroyTo(playerParent);

	if (parent->isCellObject()) {
		objectToTransfer->removeFromZone();
		parent->removeObject(objectToTransfer, false);
	} else {
		if (!parent->removeObject(objectToTransfer)) {
			error("could not remove objectToTransfer from parent in ObjectManager::transferObject");
			return false;
		}
	}

	if (parent->isCellObject()) {
		objectToTransfer->sendTo(destinationObject->getParent(), true);
	}

	if (!destinationObject->transferObject(objectToTransfer, containmentType, notifyClient)) {
		error("could not add objectToTransfer to destinationObject in ObjectManager::transferObject");
		parent->transferObject(objectToTransfer, oldContainmentType);

		if (parent->isCellObject()) {
			Zone* zne = destinationObject->getParent()->getZone();

			//objectToTransfer->insertToZone(zne);
			zne->transferObject(objectToTransfer, -1, true);
		}

		return false;
	}

	if (destinationObject->isCellObject()) {
		if (destinationObject->getParent() != NULL) {
			Zone* zne = destinationObject->getParent()->getZone();

			if (zne != NULL) {
				//objectToTransfer->insertToZone(zne);
				zne->transferObject(objectToTransfer, -1, true);
				//System::out << "Inserted to zone" << endl;
			}
		}
	}

	if (parent->isContainerObject() && parent->getGameObjectType() == SceneObjectType::STATICLOOTCONTAINER) {
		objectToTransfer->sendTo(destinationObject, true);
	}

	if (objectToTransfer->isContainerObject()) {
		objectToTransfer->sendTo(destinationObject, true);
	}

	if (destinationObject->isPlayerCreature() && parent->isCellObject())
		objectToTransfer->sendTo(destinationObject, true);

	parent->updateToDatabase();
	//destinationObject->updateToDatabaseAllObjects(false);
	*/
	return true;
}

float ObjectControllerImplementation::activateCommand(CreatureObject* object, unsigned int actionCRC, unsigned int actionCount, uint64 targetID, const UnicodeString& arguments) {
	// Pre: object is wlocked
	// Post: object is wlocked

	QueueCommand* queueCommand = getQueueCommand(actionCRC);

	float durationTime = 0.f;

	if (queueCommand == NULL) {
		StringBuffer msg;
		msg << "unregistered queue command 0x" << hex << actionCRC << " arguments: " << arguments.toString();
		object->error(msg.toString());

		return 0.f;
	}

	/*StringBuffer infoMsg;
	infoMsg << "activating queue command 0x" << hex << actionCRC << " " << queueCommand->getQueueCommandName() << " arguments='" << arguments.toString() << "'";
	object->info(infoMsg.toString(), true);*/

	String characterAbility = queueCommand->getCharacterAbility();

	if (characterAbility.length() > 1) {
		object->info("activating characterAbility " + characterAbility);

		if (object->isPlayerCreature()) {
			Reference<PlayerObject*> playerObject =  object->getSlottedObject("ghost").castTo<PlayerObject*>();

			if (!playerObject->hasAbility(characterAbility)) {
				object->clearQueueAction(actionCount, 0, 2);

				return 0.f;
			}
		}
	}

	uint32 commandGroup = queueCommand->getCommandGroup();

	if (commandGroup != 0) {
		if (commandGroup == 0xe1c9a54a && queueCommand->getQueueCommandName() != "attack") {
			if (!object->isAiAgent()) {
				object->clearQueueAction(actionCount, 0, 2);

				return 0.f;
			}
		}
	}


	if(queueCommand->requiresAdmin()) {

		try {

			if(object->isPlayerCreature()) {
				Reference<PlayerObject*> ghost =  object->getSlottedObject("ghost").castTo<PlayerObject*>();
				if (ghost == NULL || !ghost->isPrivileged() || !ghost->hasAbility(queueCommand->getQueueCommandName())) {

					StringBuffer logEntry;
					logEntry << object->getDisplayedName() << " attempted to use the '/" << queueCommand->getQueueCommandName()
							<< "' command without permissions";
					adminLog.warning(logEntry.toString());
					object->sendSystemMessage("@error_message:insufficient_permissions");
					object->clearQueueAction(actionCount, 0, 2);
					return 0.f;
				}
			} else {
				return 0.f;
			}


			String name = "unknown";

			Reference<SceneObject*> targetObject = Core::getObjectBroker()->lookUp(targetID).castTo<SceneObject*>();
			if(targetObject != NULL) {
				name = targetObject->getDisplayedName();
				if(targetObject->isPlayerCreature())
					name += "(Player)";
				else
					name += "(NPC)";
			} else {
				name = "(null)";
			}

			StringBuffer logEntry;
			logEntry << object->getDisplayedName() << " used '/" << queueCommand->getQueueCommandName()
					<< "' on " << name << " with params '" << arguments.toString() << "'";
			adminLog.info(logEntry.toString());
		} catch (Exception& e) {
			Logger::error("Unhandled Exception logging admin commands" + e.getMessage());
		}
	}

	/// Add Skillmods if any
	for(int i = 0; i < queueCommand->getSkillModSize(); ++i) {
		String skillMod;
		int value = queueCommand->getSkillMod(i, skillMod);
		object->addSkillMod(SkillModManager::ABILITYBONUS, skillMod, value, false);
	}


	int errorNumber = queueCommand->doQueueCommand(object, targetID, arguments);

	/// Remove Skillmods if any
	for(int i = 0; i < queueCommand->getSkillModSize(); ++i) {
		String skillMod;
		int value = queueCommand->getSkillMod(i, skillMod);
		object->addSkillMod(SkillModManager::ABILITYBONUS, skillMod, -value, false);
	}

	//onFail onComplete must clear the action from client queue
	if (errorNumber != QueueCommand::SUCCESS)
		queueCommand->onFail(actionCount, object, errorNumber);
	else {
		if (queueCommand->getDefaultPriority() != QueueCommand::IMMEDIATE)
			durationTime = queueCommand->getCommandDuration(object, arguments);

		queueCommand->onComplete(actionCount, object, durationTime);
	}

	/*if (actionCRC == STRING_HASHCODE("stand")) {
		Time start;

		for (int i = 0; i < 10000; ++i) {
			LuaFunction func(L, "runScript", 0);
			func << object;

			callFunction(&func);
			//int cred = object->getBankCredits();
		}

		info("time elapsed " + String::valueOf(start.miliDifference()), true);
	}*/

	return durationTime;
}

void ObjectControllerImplementation::addQueueCommand(QueueCommand* command) {
	queueCommands->put(command);
}

QueueCommand* ObjectControllerImplementation::getQueueCommand(const String& name) {
	return queueCommands->getSlashCommand(name);
}

QueueCommand* ObjectControllerImplementation::getQueueCommand(uint32 crc) {
	return queueCommands->getSlashCommand(crc);
}
