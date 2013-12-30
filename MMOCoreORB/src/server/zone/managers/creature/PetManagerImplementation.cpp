
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/creature/ValidMountScaleRange.h"
#include "server/zone/managers/name/NameManager.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/objects/creature/AiAgent.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/objects/intangible/tasks/EnqueuePetCommand.h"
#include "server/zone/templates/datatables/DataTableIff.h"
#include "server/zone/templates/datatables/DataTableRow.h"
#include "server/zone/templates/params/primitives/StringParam.h"


void PetManagerImplementation::loadValidMountScaleRanges() {
	IffStream* iffStream = TemplateManager::instance()->openIffFile("datatables/mount/valid_scale_range.iff");

	if (iffStream == NULL) {
		error("Could not load valid mount scales.");
		return;
	}

	DataTableIff dtiff;
	dtiff.readObject(iffStream);

	delete iffStream;

	for (int i = 0; i < dtiff.getTotalRows(); ++i) {
		DataTableRow* row = dtiff.getRow(i);

		Reference<ValidMountScaleRange*> validMountScale = new ValidMountScaleRange();
		validMountScale->parseDataTableRow(row);

		validMountScaleRanges.add(validMountScale);
	}

	info("Loaded " + String::valueOf(validMountScaleRanges.size()) + " mount scale ranges.", true);
}

short PetManagerImplementation::isValidMountScale(const String& appearanceFilename, int saddleCapacity, float scale) {
	for (int i = 0; i < validMountScaleRanges.size(); i++) {
		ValidMountScaleRange* range = validMountScaleRanges.get(i);

		if (range == NULL)
			continue;

		if (appearanceFilename == range->getAppearanceFilename() && saddleCapacity == range->getSaddleCapacity()) {

			if (scale < range->getValidScaleMin())
				return TOOSMALL;
			else if (scale > range->getValidScaleMax())
				return TOOLARGE;
			else
				return CANBEMOUNTTRAINED;
		}
	}

	return INVALIDCREATURE;
}

short PetManagerImplementation::checkMountEligibility(PetControlDevice* petControlDevice, float height) {
	ManagedReference<TangibleObject*> controlledObject = petControlDevice->getControlledObject();
	if (controlledObject == NULL || !controlledObject->isAiAgent())
		return PetManager::INVALIDCREATURE;

	ManagedReference<AiAgent*> pet = cast<AiAgent*>(controlledObject.get());
	if( pet == NULL )
		return PetManager::INVALIDCREATURE;

	//Check if the pet's species is able to be trained as a mount
	if (!pet->hasSlotDescriptor("rider"))
		return PetManager::INVALIDCREATURE;

	SharedObjectTemplate* objectTemplate = petControlDevice->getObjectTemplate();
	if (objectTemplate == NULL)
		return PetManager::INVALIDCREATURE;

	short result;

	if (height == -1)
		result = isValidMountScale(objectTemplate->getAppearanceFilename(), 1, pet->getHeight());
	else
		result = isValidMountScale(objectTemplate->getAppearanceFilename(), 1, height);

	return result;
}

void PetManagerImplementation::handleChat(CreatureObject* speaker, AiAgent* pet, const String& message){

	if( speaker == NULL || pet == NULL )
		return;

	if( message.isEmpty() )
		return;

	ManagedReference<PetControlDevice*> pcd = pet->getControlDevice().get().castTo<PetControlDevice*>();

	if( pcd == NULL )
		return;

	// Handle command training
	if( pcd->getTrainingCommand() > 0 ){
		bool ownerChat = handleCommandTraining( speaker, pet, message );
		if (ownerChat)
			pcd->setTrainingCommand(0); // no longer training

		return;
	}

	ManagedWeakReference< CreatureObject*> linkedCreature = pet->getLinkedCreature();
	if( linkedCreature == NULL )
		return;

	// Check if speaker has permission to command pet
	// TODO: Add check for players other than owner that are on pet's friend list
	if( linkedCreature != speaker)
		return;

	// Handle trained command
	if( pcd->hasTrainedCommand(STAY) && pcd->getTrainedCommand(STAY) == message ){
		enqueuePetCommand(speaker, pet, String("petStay").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(FOLLOW) && pcd->getTrainedCommand(FOLLOW) == message ){
		enqueuePetCommand(speaker, pet, String("petFollow").toLowerCase().hashCode(), "", true);
	}
	else if( pcd->hasTrainedCommand(STORE) && pcd->getTrainedCommand(STORE) == message ){
		enqueueOwnerOnlyPetCommand(speaker, pet, String("petStore").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(ATTACK) && pcd->getTrainedCommand(ATTACK) == message ){
		enqueuePetCommand(speaker, pet, String("petAttack").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(GUARD) && pcd->getTrainedCommand(GUARD) == message ){
		speaker->sendSystemMessage("GUARD pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(FRIEND) && pcd->getTrainedCommand(FRIEND) == message ){
		speaker->sendSystemMessage("FRIEND pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(FOLLOWOTHER) && pcd->getTrainedCommand(FOLLOWOTHER) == message ){
		enqueuePetCommand(speaker, pet, String("petFollow").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(TRICK1) && pcd->getTrainedCommand(TRICK1) == message ){
		enqueuePetCommand(speaker, pet, String("petTrick").toLowerCase().hashCode(), "1");
	}
	else if( pcd->hasTrainedCommand(TRICK2) && pcd->getTrainedCommand(TRICK2) == message ){
		enqueuePetCommand(speaker, pet, String("petTrick").toLowerCase().hashCode(), "2");
	}
	else if( pcd->hasTrainedCommand(PATROL) && pcd->getTrainedCommand(PATROL) == message ){
		speaker->sendSystemMessage("PATROL pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(FORMATION1) && pcd->getTrainedCommand(FORMATION1) == message ){
		speaker->sendSystemMessage("FORMATION2 pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(FORMATION2) && pcd->getTrainedCommand(FORMATION2) == message ){
		speaker->sendSystemMessage("FORMATION2 pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(SPECIAL_ATTACK1) && pcd->getTrainedCommand(SPECIAL_ATTACK1) == message ){
		speaker->sendSystemMessage("SPECIAL_ATTACK1 pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(SPECIAL_ATTACK2) && pcd->getTrainedCommand(SPECIAL_ATTACK2) == message ){
		speaker->sendSystemMessage("SPECIAL_ATTACK2 pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(RANGED_ATTACK) && pcd->getTrainedCommand(RANGED_ATTACK) == message ){
		speaker->sendSystemMessage("RANGED_ATTACK pet command is not yet implemented.");
	}
	else if( pcd->hasTrainedCommand(GROUP) && pcd->getTrainedCommand(GROUP) == message ){
		enqueueOwnerOnlyPetCommand(speaker, pet, String("petGroup").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(RECHARGEOTHER) && pcd->getTrainedCommand(RECHARGEOTHER) == message ){
		enqueuePetCommand(speaker, pet, String("petRechargeOther").toLowerCase().hashCode(), "");
	}
	else if( pcd->hasTrainedCommand(TRANSFER) && pcd->getTrainedCommand(TRANSFER) == message ){
		enqueueOwnerOnlyPetCommand(speaker, pet, String("petTransfer").toLowerCase().hashCode(), "");
	}

}

bool PetManagerImplementation::handleCommandTraining(CreatureObject* speaker, AiAgent* pet, const String& message){

	if( speaker == NULL || pet == NULL )
		return false;

	if( message.isEmpty() )
		return false;

	ManagedWeakReference< CreatureObject*> linkedCreature = pet->getLinkedCreature();
	if( linkedCreature == NULL )
		return false;

	// Only owner may train
	if( linkedCreature != speaker)
		return false;

	ManagedReference<PetControlDevice*> pcd = pet->getControlDevice().get().castTo<PetControlDevice*>();

	if( pcd == NULL )
		return false;

	if( pcd->hasTrainedCommandString(message) ){
		pet->showFlyText("npc_reaction/flytext","confused", 204, 0, 0);  // "?!!?!?!"
		return true;
	}

	unsigned int trainingCommand = pcd->getTrainingCommand();
	int petType = pcd->getPetType();

	// Train command
	if (petType == CREATUREPET) {
		bool alreadyTrained = pcd->hasTrainedCommand(trainingCommand);

		if (!alreadyTrained) {
			bool success = false;

			int skill = speaker->getSkillMod("tame_level");
			int roll = System::random(skill + 30);

			if (skill > roll)
				success = true;

			if (!success) {
				pet->showFlyText("npc_reaction/flytext","confused", 204, 0, 0);  // "?!!?!?!"
				speaker->sendSystemMessage("@pet/pet_menu:pet_nolearn"); // Your pet doesn't seem to understand you.
				return true;
			}
		}

		// Success
		pcd->addTrainedCommand( trainingCommand, message );
		pet->showFlyText("npc_reaction/flytext","threaten", 204, 0, 0);  // "!"
		speaker->sendSystemMessage("@pet/pet_menu:pet_learn"); // You teach your pet a new command.

		if (!alreadyTrained) {
			CreatureTemplate* creatureTemplate = pet->getCreatureTemplate();

			if (creatureTemplate == NULL)
				return true;

			PlayerManager* playerManager = zoneServer->getPlayerManager();
			playerManager->awardExperience(speaker, "creaturehandler", 10 * creatureTemplate->getLevel());
		}
	}
	else{
		pcd->addTrainedCommand( trainingCommand, message );
		pet->showFlyText("npc_reaction/flytext","threaten", 204, 0, 0);  // "!"
		speaker->sendSystemMessage("@pet/pet_menu:pet_learn"); // You teach your pet a new command.
	}

	// No renaming of faction pets
	if (petType == FACTIONPET)
		return true;

	// Check for naming string
	StringTokenizer tokenizer(message);
	tokenizer.setDelimeter(" ");
	String parsedName = "";
	int numberOfSubStrings = 0;

	while (tokenizer.hasMoreTokens()) {
		numberOfSubStrings++;

		if (!parsedName.isEmpty())
			break;

		tokenizer.getStringToken(parsedName);
	}

	String futureName = pcd->getFutureName();
	unsigned int namingProgress = pcd->getNamingProgress();

	// Validate and check name
	if (numberOfSubStrings > 1) {
		ZoneProcessServer* zps = pet->getZoneProcessServer();
		NameManager* nameManager = zps->getNameManager();

		if (nameManager->validateName(parsedName, -1) != NameManagerResult::ACCEPTED) {
			return true;
		}

		if (futureName == parsedName)
			pcd->incrementNamingProgress();
		else {
			pcd->resetNamingProgress();
			pcd->incrementNamingProgress();
			pcd->setFutureName(parsedName);
			return true;
		}
	} else {
		pcd->resetNamingProgress();
		parsedName = "";
		pcd->setFutureName(parsedName);
		return true;
	}

	// Set name, if applicable
	if (pcd->getNamingProgress() == 4) {
		UnicodeString newName = "(" + futureName + ")";
		pcd->setCustomObjectName(newName, true);
		pet->setCustomObjectName(newName, true);
	}

	return true;
}

void PetManagerImplementation::enqueuePetCommand(CreatureObject* player, AiAgent* pet, uint32 command, const String& args, bool selfTarget){

	uint64 targetID;
	if (selfTarget)
		targetID = player->getObjectID();
	else
		targetID = player->getTargetID();

	//CreatureObject* pet, uint32 command, const String& args, uint64 target, int priority = -1
	EnqueuePetCommand* enqueueCommand = new EnqueuePetCommand(pet, command, args, targetID);
	enqueueCommand->execute();
}

void PetManagerImplementation::enqueueOwnerOnlyPetCommand(CreatureObject* player, AiAgent* pet, uint32 command, const String& args){

	ManagedReference< CreatureObject*> linkedCreature = pet->getLinkedCreature().get();
	if( linkedCreature == NULL )
		return;

	// Player must be pet's owner
	if( linkedCreature != player)
		return;

	//CreatureObject* pet, uint32 command, const String& args, uint64 target, int priority = -1
	EnqueuePetCommand* enqueueCommand = new EnqueuePetCommand(pet, command, args, player->getTargetID());
	enqueueCommand->execute();

}
