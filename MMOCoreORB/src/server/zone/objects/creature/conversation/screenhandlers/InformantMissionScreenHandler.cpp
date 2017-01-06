/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "InformantMissionScreenHandler.h"
#include "server/zone/objects/mission/BountyMissionObjective.h"

const String InformantMissionScreenHandler::STARTSCREENHANDLERID = "convoscreenstart";

MissionObject* InformantMissionScreenHandler::getBountyMissionObject(CreatureObject* player) {
	if (player == NULL) {
		return NULL;
	}

	SceneObject* datapad = player->getSlottedObject("datapad");

	if (datapad == NULL) {
		return NULL;
	}

	int datapadSize = datapad->getContainerObjectsSize();

	for (int i = 0; i < datapadSize; ++i) {
		if (datapad->getContainerObject(i)->isMissionObject()) {
			Reference<MissionObject*> mission = datapad->getContainerObject(i).castTo<MissionObject*>();

			if (mission != NULL && mission->getTypeCRC() == MissionTypes::BOUNTY) {
				BountyMissionObjective* objective = cast<BountyMissionObjective*>(mission->getMissionObjective());
				if (objective != NULL) {
					return mission;
				}
			}
		}
	}

	//No relevant mission found.
	return NULL;
}

ConversationScreen* InformantMissionScreenHandler::handleScreen(CreatureObject* conversingPlayer, CreatureObject* conversingNPC, int selectedOption, ConversationScreen* conversationScreen) {
	//Get informant level.
	int informantLevel;
	if (conversationScreen->getOptionLink(0) == "1") {
		informantLevel = 1;
	} else if (conversationScreen->getOptionLink(0) == "2") {
		informantLevel = 2;
	} else if (conversationScreen->getOptionLink(0) == "3") {
		informantLevel = 3;
	} else {
		error("Informant level incorrect.");
		return NULL;
	}

	//Check if player is bounty hunter.
	if (!conversingPlayer->hasSkill("combat_bountyhunter_novice")) {
		conversationScreen->setDialogText(String("@mission/mission_generic:informant_not_bounty_hunter"));
	} else {
		//Get bounty mission object if it exists.
		MissionObject* mission = getBountyMissionObject(conversingPlayer);

		if (mission == NULL) {
			//Player has no bounty mission.
			conversationScreen->setDialogText(String("@mission/mission_generic:informant_no_bounty_mission"));
		} else {
			//Check mission level.
			if (mission->getMissionLevel() < informantLevel) {
				//Incorrect informant level.
				conversationScreen->setDialogText(String("@mission/mission_bounty_informant:informant_find_easier"));
			} else if (mission->getMissionLevel() > informantLevel) {
				//Incorrect informant level.
				conversationScreen->setDialogText(String("@mission/mission_bounty_informant:informant_find_harder"));
			} else {
				//Player has bounty mission.
				BountyMissionObjective* objective = cast<BountyMissionObjective*>(mission->getMissionObjective());
				if (objective != NULL) {
					//Run mission logic.
					if (objective->getObjectiveStatus() == BountyMissionObjective::INITSTATUS) {
						objective->updateMissionStatus(informantLevel);
						if (objective->getObjectiveStatus() == BountyMissionObjective::HASBIOSIGNATURESTATUS) {
							//Player received info about target position.
							int randomStringValue = System::random(4) + 1;
							conversationScreen->setDialogText("@mission/mission_bounty_informant:target_easy_" + String::valueOf(randomStringValue));
						} else {
							//Should never happen.
							error("Bounty mission update failed.");
						}
					} else {
						//Player has already got the target position.
						conversationScreen->setDialogText(String("@mission/mission_generic:informant_no_bounty_mission"));
					}
				} else {
					//Player has already got the target position.
					conversationScreen->setDialogText(String("@mission/mission_generic:informant_no_bounty_mission"));
				}
			}
		}
	}
	return conversationScreen;
}
