/*
 * PetDeedImplementation.cpp
 *
 *  Created on: Aug 17, 2013
 *      Author: washu
 */

#include "server/zone/objects/tangible/deed/pet/PetDeed.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/packets/object/ObjectMenuResponse.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/IngredientSlot.h"
#include "server/zone/objects/manufactureschematic/ingredientslots/ComponentSlot.h"
#include "server/zone/templates/tangible/PetDeedTemplate.h"
#include "server/zone/templates/mobile/CreatureTemplate.h"
#include "server/zone/managers/player/PlayerManager.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/creature/AiAgent.h"
#include "server/zone/objects/creature/Creature.h"
#include "server/zone/templates/tangible/SharedCreatureObjectTemplate.h"
#include "server/zone/objects/tangible/component/genetic/GeneticComponent.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/templates/mobile/CreatureTemplate.h"
#include "server/zone/templates/tangible/PetDeedTemplate.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/customization/CustomizationIdManager.h"
#include "server/zone/objects/scene/variables/CustomizationVariables.h"


void PetDeedImplementation::loadTemplateData(SharedObjectTemplate* templateData) {
	DeedImplementation::loadTemplateData(templateData);
	PetDeedTemplate* deedData = dynamic_cast<PetDeedTemplate*>(templateData);

	if (deedData == NULL)
		return;

	controlDeviceObjectTemplate = deedData->getControlDeviceObjectTemplate();
	mobileTemplate = deedData->getMobileTemplate();
}

void PetDeedImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* object) {
	DeedImplementation::fillAttributeList(alm, object);
	alm->insertAttribute("challenge_level", level);
	alm->insertAttribute("creature_health", health);
	alm->insertAttribute("creature_action", action);
	alm->insertAttribute("creature_mind", mind);
	if (armor == 0)
		alm->insertAttribute("armor_rating", "None");
	else if (armor == 1)
		alm->insertAttribute("armor_rating", "Light");
	else if (armor == 2)
		alm->insertAttribute("armor_rating", "Medium");
	else if (armor == 3)
		alm->insertAttribute("armor_rating", "Heavy");

	if (kinResist < 0)
		alm->insertAttribute("dna_comp_armor_kinetic", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_kinetic", kinResist);

	if (energyResist < 0)
		alm->insertAttribute("dna_comp_armor_energy", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_energy", energyResist);

	if (blastResist < 0)
		alm->insertAttribute("dna_comp_armor_blast", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_blast", blastResist);

	if (heatResist < 0)
		alm->insertAttribute("dna_comp_armor_heat", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_heat", heatResist);

	if (coldResist < 0)
		alm->insertAttribute("dna_comp_armor_cold", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_cold", coldResist);

	if (elecResist < 0)
		alm->insertAttribute("dna_comp_armor_electric", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_electric", elecResist);

	if (acidResist < 0)
		alm->insertAttribute("dna_comp_armor_acid", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_acid", acidResist);

	if (stunResist < 0)
		alm->insertAttribute("dna_comp_armor_stun", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_stun", stunResist);

	if (saberResist < 0)
		alm->insertAttribute("dna_comp_armor_saber", "Vulnerable");
	else
		alm->insertAttribute("dna_comp_armor_saber", saberResist);

	alm->insertAttribute("creature_attack", attackSpeed);
	alm->insertAttribute("creature_tohit", chanceHit);
	alm->insertAttribute("creature_damage", String::valueOf(damageMin) + " - " + String::valueOf(damageMax));

	if (special1 != "none" && special1 != "defaultattack")
		alm->insertAttribute("spec_atk_1", special1);
	else
		alm->insertAttribute("spec_atk_1", "---");

	if (special2 != "none" && special2 != "defaultattack")
		alm->insertAttribute("spec_atk_2", special2);
	else
		alm->insertAttribute("spec_atk_2", "---");

	CreatureTemplateManager* creatureTemplateManager = CreatureTemplateManager::instance();
	ManagedReference<CreatureTemplate*> petTemplate =  creatureTemplateManager->getTemplate( mobileTemplate.hashCode() );
	bool allowRanged = false;
	if (petTemplate != NULL) {
		if (petTemplate->getWeapons().size() > 0) {
			allowRanged = true;
		}
	}
	if(ranged && allowRanged)
		alm->insertAttribute("dna_comp_ranged_attack", "Yes");
	else
		alm->insertAttribute("dna_comp_ranged_attack", "No");
}

void PetDeedImplementation::initializeTransientMembers() {
	DeedImplementation::initializeTransientMembers();

	setLoggingName("PetDeed");
}
CreatureAttackMap* PetDeedImplementation::getAttacks() {
	return &attacks;
}

void PetDeedImplementation::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	ManagedReference<ManufactureSchematic*> manufact = values->getManufactureSchematic();
	for (int i = 0; i < manufact->getSlotCount(); ++i) {
		// Dna Component Slots
		Reference<IngredientSlot* > iSlot = manufact->getSlot(i);
		if (iSlot->isComponentSlot()) {
			ComponentSlot* cSlot = cast<ComponentSlot*>(iSlot.get());
			ManagedReference<TangibleObject*> tano = cSlot->getPrototype();
			ManagedReference<GeneticComponent*> component = cast<GeneticComponent*>( tano.get());
			// Now we can suck in the values
			level = component->getLevel();
			chanceHit = component->getHit();
			attackSpeed = component->getSpeed();
			damageMin = component->getMinDamage();
			damageMax = component->getMaxDamage();
			armor = component->getArmor();
			kinResist = component->getKinetic();
			energyResist = component->getEnergy();
			blastResist = component->getBlast();
			coldResist = component->getCold();
			heatResist = component->getHeat();
			elecResist = component->getElectrical();
			acidResist = component->getAcid();
			stunResist = component->getStun();
			saberResist = component->getSaber();
			health = component->getHealth();
			action = component->getAction();
			mind = component->getMind();
			special1 = component->getSpecial1();
			special2 = component->getSpecial2();
			ranged = component->getRanged();
		}
	}
	CreatureTemplateManager* creatureTemplateManager = CreatureTemplateManager::instance();
	ManagedReference<CreatureTemplate*> petTemplate =  creatureTemplateManager->getTemplate( mobileTemplate.hashCode() );
	if (petTemplate != NULL) {
		// get min CL from the template
		int minCl = petTemplate->getLevel();
		if (level < minCl) {
			level = minCl;
		}
	}
	if (special1 != "none" && special1 != "defaultattack") {
		String args = "";
		if (special1.contains("creature") || special1.contains("poison")){
			attacks.addAttack(special1,args);
		} else if(special1.contains("blind")) {
			attacks.addAttack(special1,"blindChance=50");
		}else if(special1.contains("dizzy")) {
			attacks.addAttack(special1,"dizzyChance=50");
		}else if(special1.contains("intimidation")) {
			attacks.addAttack(special1,"intimidationChance=50");
		}else if(special1.contains("stun")) {
			attacks.addAttack(special1,"stunChance=50");
		}else if(special1.contains("knockdown")) {
			attacks.addAttack(special1,"knockdownChance=50");
		}else if(special1.contains("posturedown")) {
			attacks.addAttack(special1,"postureDownChance=50");
		}else if(special1.contains("postureup")) {
			attacks.addAttack(special1,"postureUpChance=50");
		}
	}
	if (special2 != "none" && special2 != "defaultattack") {
		String args = "";
		if (special2.contains("creature") || special2.contains("poison")){
			attacks.addAttack(special2,args);
		} else if(special2.contains("blind")) {
			attacks.addAttack(special2,"blindChance=50");
		}else if(special2.contains("dizzy")) {
			attacks.addAttack(special2,"dizzyChance=50");
		}else if(special2.contains("intimidation")) {
			attacks.addAttack(special2,"intimidationChance=50");
		}else if(special2.contains("stun")) {
			attacks.addAttack(special2,"stunChance=50");
		}else if(special2.contains("knockdown")) {
			attacks.addAttack(special2,"knockdownChance=50");
		}else if(special2.contains("posturedown")) {
			attacks.addAttack(special2,"postureDownChance=50");
		}else if(special2.contains("postureup")) {
			attacks.addAttack(special2,"postureUpChance=50");
		}
	}
}

void PetDeedImplementation::fillObjectMenuResponse(ObjectMenuResponse* menuResponse, CreatureObject* player) {
	DeedImplementation::fillObjectMenuResponse(menuResponse, player);

	if(isASubChildOf(player))
		menuResponse->addRadialMenuItem(20, 3, "@pet/pet_menu:menu_tame");
}

int PetDeedImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {

	if (selectedID == 20) {

		if (generated || !isASubChildOf(player))
			return 1;

		if (player->isInCombat() || player->getParentRecursively(SceneObjectType::BUILDING) != NULL) {
			player->sendSystemMessage("@pet/pet_menu:cant_call"); //You can only unpack vehicles while Outside and not in Combat.
			return 1;
		}

		ManagedReference<SceneObject*> datapad = player->getSlottedObject("datapad");

		if (datapad == NULL) {
			player->sendSystemMessage("Datapad doesn't exist when trying to call pet");
			return 1;
		}
		if (datapad->getContainerObjectsSize() >= datapad->getContainerVolumeLimit()) {
			player->sendSystemMessage("@faction_recruiter:datapad_full"); // Your datapad is full. You must first free some space.
			return 1;
		}
		// Does the player have room
		ManagedReference<PlayerManager*> playerManager = player->getZoneServer()->getPlayerManager();

		int numberStored = 0;
		int maxStoredPets = playerManager->getBaseStoredCreaturePets() + player->getSkillMod("stored_pets");

		for (int i = 0; i < datapad->getContainerObjectsSize(); ++i) {
			ManagedReference<SceneObject*> object = datapad->getContainerObject(i);

			if (object != NULL && object->isPetControlDevice()) {
				PetControlDevice* device = cast<PetControlDevice*>( object.get());

				if (device->getPetType() == PetManager::CREATUREPET) {
					if (++numberStored >= maxStoredPets) {
						player->sendSystemMessage("@pet/pet_menu:sys_too_many_stored"); // There are too many pets stored in this container. Release some of them to make room for more.
						return 1;
					}

				}
			}
		}
		// Can the player control it
		ManagedReference<PlayerObject*> ghost = player->getPlayerObject();

		int currentlySpawned = 0;
		int spawnedLevel = 0;
		int cLevel = level;
		int maxPets = player->getSkillMod("keep_creature");
		int maxLevelofPets = player->getSkillMod("tame_level");

		for (int i = 0; i < ghost->getActivePetsSize(); ++i) {
			ManagedReference<AiAgent*> object = ghost->getActivePet(i);

			if (object != NULL && object->isCreature()) {
				if (++currentlySpawned >= maxPets) {
					player->sendSystemMessage("@pet/pet_menu:too_many"); // You can't control any more pets. Store one first
					return 1;
				}

				spawnedLevel += object->getLevel();

				if ((spawnedLevel + cLevel) > maxLevelofPets) {
					player->sendSystemMessage("Taming this pet would exceed your control level ability.");
					return 1;
				}
			}
		}

		Reference<CreatureManager*> creatureManager = player->getZone()->getCreatureManager();
		if( creatureManager == NULL )
			return 1;

		CreatureTemplateManager* creatureTemplateManager = CreatureTemplateManager::instance();
		ManagedReference<CreatureTemplate*> petTemplate = creatureTemplateManager->getTemplate( mobileTemplate.hashCode() );

		if (petTemplate == NULL) {
			error("Failed to load template crc");
			return 1;
		}

		bool isVicious = petTemplate->getPvpBitmask() & CreatureFlag::AGGRESSIVE;

		if (level > 10 || isVicious) {
			if (!player->hasSkill("outdoors_creaturehandler_novice") || (level > maxLevelofPets)) {
				player->sendSystemMessage("@pet/pet_menu:sys_lack_skill"); // You lack the skill to be able to tame that creature.
				return 1;
			}

			if (isVicious && player->getSkillMod("tame_aggro") < 1) {
				player->sendSystemMessage("@pet/pet_menu:sys_lack_skill"); // You lack the skill to be able to tame that creature.
				return 1;
			}
		}
		// All checks complete, lets setup the control device and do it.
		ManagedReference<PetControlDevice*> controlDevice = (server->getZoneServer()->createObject(controlDeviceObjectTemplate.hashCode(), 1)).castTo<PetControlDevice*>();
		String templateToSpawn = creatureManager->getTemplateToSpawn(mobileTemplate.hashCode());
		ManagedReference<CreatureObject*> creatureObject = creatureManager->createCreature(templateToSpawn.hashCode(), true, 0 );
		if( creatureObject == NULL ){
			player->sendSystemMessage("wrong pet templates;mobileTemplate=[" + mobileTemplate + "];generatedObjectTemplate=[" + generatedObjectTemplate + "]" );
			return 1;
		}

		ManagedReference<Creature*> pet = creatureObject.castTo<Creature*>();
		if( pet == NULL )
			return 1;
		ObjectManager* objectManager = server->getZoneServer()->getObjectManager();
		pet->setPetDeed(_this.get());
		pet->loadTemplateData( petTemplate );
		pet->createChildObjects();
		// update base stats on the pet now
		// We will store the deed pointer to the aiagent before serialization

		// Copy color customization from deed to pet
		CustomizationVariables* customVars = getCustomizationVariables();
		if( customVars != NULL ){
			for (int i = 0; i < customVars->size(); ++i) {
				uint8 id = customVars->elementAt(i).getKey();
				int16 val = customVars->elementAt(i).getValue();

				String name = CustomizationIdManager::instance()->getCustomizationVariable(id);
				if( name.contains( "color" ) ){
					pet->setCustomizationVariable( name, val, true );
				}
			}
		}
		// then this is complete
		StringId s;
		s.setStringId(pet->getObjectName()->getFullPath());
		controlDevice->setObjectName(s);
		controlDevice->setPetType(PetManager::CREATUREPET);
		controlDevice->setMaxVitality(100);
		controlDevice->setVitality(100);
		controlDevice->setGrowthStage(10);
		controlDevice->updateStatus(1);

		controlDevice->setDefaultCommands();
		controlDevice->setControlledObject(pet);
		datapad->transferObject(controlDevice, -1);

		objectManager->persistSceneObjectsRecursively(pet, 1);

		datapad->broadcastObject(controlDevice, true);
		controlDevice->callObject(player);
		//Remove the deed from it's container. (TEMP disable the removal for testing)
		ManagedReference<SceneObject*> deedContainer = getParent();

		if (deedContainer != NULL) {
			destroyObjectFromWorld(true);
		}

		generated = true;
		player->sendSystemMessage("@pet/pet_menu:device_added"); // "A control device has been added to your datapad."
		return 0;
	}

	return DeedImplementation::handleObjectMenuSelect(player, selectedID);
}

