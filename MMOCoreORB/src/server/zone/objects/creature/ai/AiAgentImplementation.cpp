/*
 * AiAgentImplementation.cpp
 *
 *  Created on: 13/07/2010
 *      Author: victor
 */

#include <engine/core/ManagedReference.h>
#include <server/zone/CloseObjectsVector.h>
#include <engine/util/u3d/Vector3.h>
#include <system/lang/IllegalArgumentException.h>
#include <system/lang/ref/Reference.h>
#include <system/lang/String.h>
#include <system/lang/StringBuffer.h>
#include <system/lang/System.h>
#include <system/lang/Time.h>
#include <system/thread/Locker.h>
#include <system/thread/Mutex.h>
#include <system/thread/ReadLocker.h>
#include <system/util/SortedVector.h>
#include <system/util/Vector.h>
#include <system/util/VectorMap.h>

#include "server/zone/objects/creature/ai/bt/Behavior.h"
#include "server/zone/managers/objectcontroller/ObjectController.h"
#include "server/zone/managers/creature/CreatureManager.h"
#include "server/zone/managers/creature/PetManager.h"
#include "server/zone/managers/planet/PlanetManager.h"
#include "server/zone/managers/reaction/ReactionManager.h"
#include "server/zone/objects/cell/CellObject.h"
#include "server/zone/objects/creature/ai/AiAgent.h"
#include "server/zone/objects/creature/ai/Creature.h"
#include "server/zone/objects/creature/conversation/ConversationObserver.h"
#include "server/zone/objects/creature/commands/CombatQueueCommand.h"
#include "server/zone/objects/intangible/PetControlDevice.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/tangible/weapon/WeaponObject.h"
#include "server/zone/Zone.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/managers/collision/CollisionManager.h"
#include "server/zone/managers/collision/PathFinderManager.h"
#include "server/zone/managers/combat/CombatManager.h"
#include "server/zone/managers/components/ComponentManager.h"
#include "server/zone/managers/conversation/ConversationManager.h"
#include "server/zone/managers/creature/AiMap.h"
#include "server/zone/managers/creature/CreatureTemplateManager.h"
#include "server/zone/managers/faction/FactionManager.h"
#include "server/zone/managers/gcw/GCWManager.h"
#include "server/zone/managers/name/NameManager.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/packets/object/StartNpcConversation.h"
#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/zone/packets/scene/LightUpdateTransformMessage.h"
#include "server/zone/packets/scene/LightUpdateTransformWithParentMessage.h"
#include "server/zone/packets/scene/UpdateTransformMessage.h"
#include "server/zone/packets/scene/UpdateTransformWithParentMessage.h"
#include "server/zone/objects/creature/ai/CreatureTemplate.h"
#include "templates/mobile/MobileOutfit.h"
#include "templates/mobile/MobileOutfitGroup.h"
#include "templates/SharedObjectTemplate.h"
#include "server/zone/objects/player/FactionStatus.h"
#include "templates/params/ObserverEventType.h"
#include "server/zone/managers/gcw/observers/ImperialChatObserver.h"
#include "server/zone/objects/scene/variables/DeltaVector.h"
#include "server/zone/objects/scene/WorldCoordinates.h"
#include "server/zone/objects/tangible/threat/ThreatMap.h"
#include "templates/params/creature/CreatureAttribute.h"
#include "templates/params/creature/CreatureFlag.h"
#include "templates/params/creature/CreaturePosture.h"
#include "templates/params/creature/CreatureState.h"
#include "server/zone/objects/creature/damageovertime/DamageOverTimeList.h"
#include "server/zone/objects/creature/ai/events/AiBehaviorEvent.h"
#include "server/zone/objects/creature/ai/events/AiRecoveryEvent.h"
#include "server/zone/objects/creature/events/CamoTask.h"
#include "server/zone/objects/creature/events/DespawnCreatureOnPlayerDissappear.h"
#include "server/zone/objects/creature/events/DespawnCreatureTask.h"
#include "server/zone/objects/creature/events/RespawnCreatureTask.h"
#include "server/zone/objects/creature/ai/PatrolPoint.h"
#include "server/zone/objects/creature/ai/PatrolPointsVector.h"
#include "server/zone/objects/creature/ai/variables/CreatureAttackMap.h"
#include "server/zone/objects/creature/ai/variables/CurrentFoundPath.h"
#include "server/zone/managers/creature/SpawnObserver.h"
#include "server/zone/managers/creature/DynamicSpawnObserver.h"
#include "server/zone/packets/ui/CreateClientPathMessage.h"
#include "server/zone/objects/staticobject/StaticObject.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/transaction/TransactionLog.h"
#include "server/chat/ChatManager.h"

//#define DEBUG
//#define DEBUG_PATHING
//#define SHOW_PATH
//#define SHOW_NEXT_POSITION
//#define DEBUG_FINDNEXTPOSITION

void AiAgentImplementation::loadTemplateData(SharedObjectTemplate* templateData) {
	CreatureObjectImplementation::loadTemplateData(templateData);
}

int AiAgentImplementation::calculateAttackMinDamage(int level) {
	int minDmg = Math::max(getDamageMin(), 20 + (level * 5));
	if (petDeed != nullptr) {
		minDmg = petDeed->getMinDamage();
		if (level < petDeed->getLevel()) {
			// reduce by level range
			float percent = (float)level/(float)petDeed->getLevel();
			minDmg *= percent;
		}
	}
	if (isDroidObject()) {
		minDmg = getDamageMin();
	}
	return minDmg;
}

int AiAgentImplementation::calculateAttackMaxDamage(int level) {
	int dmg = Math::max(getDamageMax(), calculateAttackMinDamage(level) * 2);
	if (petDeed != nullptr) {
		dmg = petDeed->getMaxDamage();
		if (level < petDeed->getLevel()) {
			float percent = (float)level/(float)petDeed->getLevel();
			dmg *= percent;
		}
	}
	if (isDroidObject()) {
		dmg = getDamageMax();
	}
	return dmg;
}

float AiAgentImplementation::calculateAttackSpeed(int level) {
	float speed = 3.5f - ((float)level / 100.f);
	return speed;
}

int AiAgentImplementation::getDamageMax() {
	WeaponObject* currentWeapon = getWeapon();

	if (currentWeapon == nullptr)
		return 0;

	return currentWeapon->getMaxDamage();
}

int AiAgentImplementation::getDamageMin() {
	WeaponObject* currentWeapon = getWeapon();

	if (currentWeapon == nullptr)
		return 0;

	return currentWeapon->getMinDamage();
}

void AiAgentImplementation::loadTemplateData(CreatureTemplate* templateData) {
	npcTemplate = templateData;

	setPvpStatusBitmask(npcTemplate->getPvpBitmask());
	if (npcTemplate->getPvpBitmask() == 0)
		closeobjects = nullptr;

	optionsBitmask = npcTemplate->getOptionsBitmask();
	creatureBitmask = npcTemplate->getCreatureBitmask();

	convoTemplateCRC = npcTemplate->getConversationTemplate();

	level = getTemplateLevel();

	planetMapCategory = npcTemplate->getPlanetMapCategory();

	tauntable = npcTemplate->isTauntable();

	if (getHueValue() == -1 && npcTemplate->getTotalHues() > 0) {
		int randHue = npcTemplate->getRandomHue();
		setHue(randHue);
	}

	loadWeaponTemplateData();
	setupAttackMaps();

	int ham = 0;
	baseHAM.removeAll();
	if (petDeed == nullptr) {
		for (int i = 0; i < 9; ++i) {
			if (i % 3 == 0) {
				ham = System::random(getHamMaximum() - getHamBase()) + getHamBase();
				if (isDroidObject() && isPet())
					ham = getHamMaximum();
				baseHAM.add(ham);
			} else
				baseHAM.add(ham/10);
		}
	} else {
		int health = petDeed->getHealth();
		baseHAM.add(health);
		baseHAM.add(health/10);
		baseHAM.add(health/10);
		int action = petDeed->getAction();
		baseHAM.add(action);
		baseHAM.add(action/10);
		baseHAM.add(action/10);
		int mind = petDeed->getMind();
		baseHAM.add(mind);
		baseHAM.add(mind/10);
		baseHAM.add(mind/10);
	}

	hamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		hamList.add(baseHAM.get(i));
	}

	maxHamList.removeAll();
	for (int i = 0; i < 9; ++i) {
		maxHamList.add(baseHAM.get(i));
	}

	objectName = npcTemplate->getObjectName();

	if (npcTemplate->getRandomNameType() != NameManagerType::TAG) {
		NameManager* nm = server->getNameManager();

		int templSpecies = getSpecies();

		if (!npcTemplate->getRandomNameTag()) {
			setCustomObjectName(nm->makeCreatureName(npcTemplate->getRandomNameType(), templSpecies), false);
		} else {
			String newName = nm->makeCreatureName(npcTemplate->getRandomNameType(), templSpecies);
			newName += " (";

			if (objectName == "")
				newName += templateData->getCustomName();
			else
				newName += StringIdManager::instance()->getStringId(objectName.getFullPath().hashCode()).toString();

			newName += ")";
			setCustomObjectName(newName, false);
		}
	} else {
		setCustomObjectName(templateData->getCustomName(), false);
	}

	setHeight(templateData->getScale(), false);

	String currentLogName = getLoggingName();

	if (!currentLogName.contains(npcTemplate->getTemplateName())) {
		StringBuffer logName;
		logName << getLoggingName() << "[" << npcTemplate->getTemplateName() << "]";

		setLoggingName(logName.toString());

		// Check for template level logging setting
		StringBuffer configName;
		configName << "Core3.AiAgent." << npcTemplate->getTemplateName() << ".LogLevel";

		auto templateLogLevel = ConfigManager::instance()->getInt(configName.toString(), -1);

		if (templateLogLevel >= 0) {
			setLogLevel(static_cast<Logger::LogLevel>(templateLogLevel));
		}

		debug() << "loadTemplateData(" << npcTemplate->getTemplateName() << ")";
	}

	String factionString = npcTemplate->getFaction();

	if (!factionString.isEmpty() && (factionString == "imperial" || factionString == "rebel")) {
		setFaction(factionString.hashCode());

		if (faction == Factions::FACTIONIMPERIAL) {
			ImperialChatObserver* chatObserver = new ImperialChatObserver();

			if (chatObserver != nullptr)
				registerObserver(ObserverEventType::FACTIONCHAT, chatObserver);
		}
	}

	if (!loadedOutfit) {
		loadedOutfit = true;
		String outfit = npcTemplate->getOutfit();

		if (!outfit.isEmpty()) {
			MobileOutfitGroup* group = CreatureTemplateManager::instance()->getMobileOutfitGroup(outfit);

			if (group != nullptr) {
				Vector<MobileOutfit>* outfits = group->getOutfits();

				int num = System::random(outfits->size() - 1);

				if (num > -1) {
					MobileOutfit* clothes = &outfits->get(num);

					Vector<OutfitTangibleObject>* objects = clothes->getObjects();

					for (int i = 0; i < objects->size(); ++i) {
						OutfitTangibleObject* obj = &objects->get(i);

						String templ = obj->getObjectTemplate();

						ManagedReference<TangibleObject*> tano = (getZoneServer()->createObject(templ.hashCode(), getPersistenceLevel())).castTo<TangibleObject*>();

						if (tano != nullptr) {
							Locker objLocker(tano);

							VectorMap<String, uint8>* cust = obj->getCustomizationVariables();

							for (int j = 0; j < cust->size(); ++j) {
								tano->setCustomizationVariable(cust->elementAt(j).getKey(), cust->elementAt(j).getValue());
							}

							if (!transferObject(tano, 4, false) && tano->isPersistent()) {
								tano->destroyObjectFromDatabase(true);
							}
						}

					}
				}
			} else {
				error("null outfit group " + outfit);
			}
		}
	}

	String conTemp = npcTemplate->getContainerComponentTemplate();
	if (!conTemp.isEmpty()) {
		containerComponent = cast<ContainerComponent*>(ComponentManager::instance()->getComponent<SceneObjectComponent*>(conTemp));
	}

	if (!isPet() && isNpc() && (getFaction() == 0) && getMoodID() == 0 && System::random(50) < 15) {
		ZoneServer* zoneServer = asAiAgent()->getZoneServer();

		if (zoneServer != nullptr) {
			ChatManager* chatManager = zoneServer->getChatManager();

			if (chatManager != nullptr) {
				unsigned int id = chatManager->getRandomMoodID();

				setMood(id);
			}
		}
	}
}

void AiAgentImplementation::loadWeaponTemplateData() {
	if (npcTemplate == nullptr) {
		Logger::console.info(true) << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": " << *_this.getReferenceUnsafeStaticCast();
		return;
	}

	float minDmg = npcTemplate->getDamageMin();
	float maxDmg = npcTemplate->getDamageMax();
	float speed = calculateAttackSpeed(level);

	float globalSpeedOverride = CreatureTemplateManager::instance()->getGlobalAttackSpeedOverride();
	float customSpeed = npcTemplate->getAttackSpeed();

	if (globalSpeedOverride > 0.0f)
		speed = globalSpeedOverride;
	else if (customSpeed > 0.0f)
		speed = customSpeed;

	bool allowedWeapon = true;

	if (petDeed != nullptr) {
		minDmg = petDeed->getMinDamage();
		maxDmg = petDeed->getMaxDamage();
		allowedWeapon = petDeed->getRanged();
	}

	Reference<WeaponObject*> defaultWeap = asCreatureObject()->getSlottedObject("default_weapon").castTo<WeaponObject*>();

	if (defaultWeap != nullptr) {
		defaultWeapon = defaultWeap;
		// set the damage of the default weapon
		defaultWeapon->setMinDamage(minDmg);
		defaultWeapon->setMaxDamage(maxDmg);

		if (petDeed != nullptr) {
			defaultWeapon->setAttackSpeed(petDeed->getAttackSpeed());
		} else if (isPet()) {
			defaultWeapon->setAttackSpeed(speed);
		}

		primaryWeapon = defaultWeapon;
	}

	String primaryWeap = npcTemplate->getPrimaryWeapon();
	uint32 primaryWeapHash = primaryWeap.hashCode();

	if (primaryWeapHash != STRING_HASHCODE("unarmed") && primaryWeapHash != STRING_HASHCODE("none")) {
		uint32 weaponCRC = 0;

		if (primaryWeap.indexOf(".iff") != -1) {
			weaponCRC = primaryWeap.hashCode();
		} else {
			const Vector<String>& primaryTemplates = CreatureTemplateManager::instance()->getWeapons(primaryWeap);

			if (primaryTemplates.size() > 0) {
				String& weaponTemplate = primaryTemplates.get(System::random(primaryTemplates.size() - 1));
				weaponCRC = weaponTemplate.hashCode();
			}
		}

		if (weaponCRC != 0)
			primaryWeapon = createWeapon(weaponCRC, true);
	}

	String secondaryWeap = npcTemplate->getSecondaryWeapon();
	uint32 secondaryWeapHash = secondaryWeap.hashCode();

	if (secondaryWeapHash == STRING_HASHCODE("unarmed")) {
		secondaryWeapon = defaultWeapon;
	} else if (secondaryWeapHash != STRING_HASHCODE("none")) {
		uint32 weaponCRC = 0;

		if (secondaryWeap.indexOf(".iff") != -1) {
			weaponCRC = primaryWeap.hashCode();
		} else {
			const Vector<String>& secondaryTemplates = CreatureTemplateManager::instance()->getWeapons(secondaryWeapHash);

			if (secondaryTemplates.size() > 0) {
				String& weaponTemplate = secondaryTemplates.get(System::random(secondaryTemplates.size() - 1));
				weaponCRC = weaponTemplate.hashCode();
			}
		}

		if (weaponCRC != 0)
			secondaryWeapon = createWeapon(weaponCRC, false);
	}

	String thrownWeap = npcTemplate->getThrownWeapon();

	if (thrownWeap != "") {
		uint32 weaponCRC = 0;

		const Vector<String>& thrownTemplates = CreatureTemplateManager::instance()->getWeapons(thrownWeap.hashCode());

		if (thrownTemplates.size() > 0) {
			String& weaponTemplate = thrownTemplates.get(System::random(thrownTemplates.size() - 1));
			weaponCRC = weaponTemplate.hashCode();
		}

		if (weaponCRC != 0) {
			thrownWeapon = createWeapon(weaponCRC, false);
		}
	}
}

WeaponObject* AiAgentImplementation::createWeapon(uint32 templateCRC, bool primaryWeapon) {
	ZoneServer* zoneServer = getZoneServer();

	if (zoneServer == nullptr)
		return nullptr;

	ObjectController* objectController = zoneServer->getObjectController();

	if (objectController == nullptr)
		return nullptr;

	SceneObject* inventory = asAiAgent()->getSlottedObject("inventory");

	if (inventory == nullptr)
		return nullptr;

	WeaponObject* newWeapon = nullptr;

	if (templateCRC == STRING_HASHCODE("none")) {
		if (primaryWeapon) {
			newWeapon = getDefaultWeapon();
		} else {
			return nullptr;
		}
	} else if (templateCRC == STRING_HASHCODE("unarmed")) {
		newWeapon = getDefaultWeapon();
	} else {
		newWeapon = (zoneServer->createObject(templateCRC, getPersistenceLevel())).castTo<WeaponObject*>();
	}

	float minDmg = npcTemplate->getDamageMin();
	float maxDmg = npcTemplate->getDamageMax();
	float speed = calculateAttackSpeed(level);

	float globalSpeedOverride = CreatureTemplateManager::instance()->getGlobalAttackSpeedOverride();
	float customSpeed = npcTemplate->getAttackSpeed();

	if (globalSpeedOverride > 0.0f)
		speed = globalSpeedOverride;
	else if (customSpeed > 0.0f)
		speed = customSpeed;

	bool allowedWeapon = true;

	if (petDeed != nullptr) {
		minDmg = petDeed->getMinDamage();
		maxDmg = petDeed->getMaxDamage();
	}

	if (newWeapon != nullptr) {
		float mod = 1 - (0.1 * newWeapon->getArmorPiercing());
		newWeapon->setMinDamage(minDmg * mod);
		newWeapon->setMaxDamage(maxDmg * mod);

		SharedWeaponObjectTemplate* weaoTemp = cast<SharedWeaponObjectTemplate*>(newWeapon->getObjectTemplate());

		if (weaoTemp != nullptr && weaoTemp->getPlayerRaces()->size() > 0) {
			newWeapon->setAttackSpeed(speed);
		} else if (petDeed != nullptr) {
			newWeapon->setAttackSpeed(petDeed->getAttackSpeed());
		}

		int lightsaberColor = npcTemplate->getLightsaberColor();

		if (newWeapon->isJediWeapon() && lightsaberColor > 0) {
			Locker weaplock(newWeapon);

			newWeapon->setBladeColor(lightsaberColor);
			newWeapon->setCustomizationVariable("/private/index_color_blade", lightsaberColor, true);
		}

		if (newWeapon != defaultWeapon) {
			if (inventory->transferObject(newWeapon, -1, false, true))
				inventory->broadcastObject(newWeapon, true);
		}
	}

	return newWeapon;
}

void AiAgentImplementation::setupAttackMaps() {
	primaryAttackMap = nullptr;
	secondaryAttackMap = nullptr;
	defaultAttackMap = nullptr;

	ZoneServer* zoneServer = getZoneServer();

	if (zoneServer == nullptr)
		return;

	ObjectController* objectController = zoneServer->getObjectController();

	if (objectController == nullptr)
		return;

	const CreatureAttackMap* attackMap;

	if (petDeed != nullptr)
		attackMap = petDeed->getAttacks();
	else
		attackMap = npcTemplate->getPrimaryAttacks();

	Reference<WeaponObject*> defaultWeapon = asAiAgent()->getDefaultWeapon();
	defaultAttackMap = new CreatureAttackMap();
	primaryAttackMap = new CreatureAttackMap();
	secondaryAttackMap = new CreatureAttackMap();

	for (int i = 0; i < attackMap->size(); i++) {
		const CombatQueueCommand* attack = cast<const CombatQueueCommand*>(objectController->getQueueCommand(attackMap->getCommand(i)));

		if (attack == nullptr)
			continue;

		if (primaryWeapon != nullptr && (attack->getWeaponType() & primaryWeapon->getWeaponBitmask())) {
			primaryAttackMap->add(attackMap->get(i));
		}

		if (defaultWeapon != nullptr && (attack->getWeaponType() & defaultWeapon->getWeaponBitmask())) {
			defaultAttackMap->add(attackMap->get(i));
		}
	}

	if (petDeed == nullptr) {
		attackMap = npcTemplate->getSecondaryAttacks();

		for (int i = 0; i < attackMap->size(); i++) {
			const CombatQueueCommand* attack = cast<const CombatQueueCommand*>(objectController->getQueueCommand(attackMap->getCommand(i)));

			if (attack == nullptr)
				continue;

			if (secondaryWeapon != nullptr && (attack->getWeaponType() & secondaryWeapon->getWeaponBitmask())) {
				secondaryAttackMap->add(attackMap->get(i));

			}

			if (defaultWeapon != nullptr && (attack->getWeaponType() & defaultWeapon->getWeaponBitmask())) {
				defaultAttackMap->add(attackMap->get(i));
			}
		}
	}

	// if we didn't get any attacks or the weapon is nullptr, drop the reference to the attack maps
	if (defaultAttackMap != nullptr && defaultAttackMap->isEmpty())
		defaultAttackMap = nullptr;

	if (primaryAttackMap != nullptr && primaryAttackMap->isEmpty())
		primaryAttackMap = nullptr;

	if (secondaryAttackMap != nullptr && secondaryAttackMap->isEmpty())
		secondaryAttackMap = nullptr;

	attackMap = nullptr;
}

void AiAgentImplementation::unequipWeapons() {
	if (currentWeapon == nullptr) {
		currentWeapon = defaultWeapon;
		return;
	}

	SceneObject* inventory = asAiAgent()->getSlottedObject("inventory");

	if (inventory == nullptr)
		return;

	if (currentWeapon == defaultWeapon)
		return;

	ZoneServer* zoneServer = getZoneServer();

	if (zoneServer == nullptr)
		return;

	ObjectController* objectController = zoneServer->getObjectController();

	if (objectController == nullptr)
		return;

	objectController->transferObject(currentWeapon, inventory, -1, true, true);

	currentWeapon = defaultWeapon;
}

void AiAgentImplementation::equipPrimaryWeapon() {
	if (primaryWeapon == nullptr || currentWeapon == primaryWeapon)
		return;

	unequipWeapons();

	if (primaryWeapon == defaultWeapon) {
		currentWeapon = defaultWeapon;
	} else {
		transferObject(primaryWeapon, 4, false);
		broadcastObject(primaryWeapon, false);
		currentWeapon = primaryWeapon;
	}
}

void AiAgentImplementation::equipSecondaryWeapon() {
	if (secondaryWeapon == nullptr || currentWeapon == secondaryWeapon)
		return;

	unequipWeapons();

	if (secondaryWeapon == defaultWeapon) {
		currentWeapon = defaultWeapon;
	} else {
		transferObject(secondaryWeapon, 4, false);
		broadcastObject(secondaryWeapon, false);
		currentWeapon = secondaryWeapon;
	}
}

void AiAgentImplementation::setLevel(int lvl, bool randomHam) {
	if (lvl <= 0)
		return;

	CreatureObjectImplementation::setLevel(lvl);

	level = lvl;

	if (npcTemplate == nullptr) {
		return;
	}

	int baseLevel = getTemplateLevel();

	if (baseLevel == lvl)
		return;

	float minDmg = calculateAttackMinDamage(baseLevel);
	float maxDmg = calculateAttackMaxDamage(baseLevel);
	float speed = calculateAttackSpeed(lvl);

	float globalSpeedOverride = CreatureTemplateManager::instance()->getGlobalAttackSpeedOverride();
	float customSpeed = npcTemplate->getAttackSpeed();

	if (globalSpeedOverride > 0.0f)
		speed = globalSpeedOverride;
	else if (customSpeed > 0.0f)
		speed = customSpeed;

	float ratio = ((float)lvl) / (float)baseLevel;

	minDmg *= ratio;
	maxDmg *= ratio;

	if (primaryWeapon != nullptr) {
		float mod = 1 - 0.1*primaryWeapon->getArmorPiercing();
		primaryWeapon->setMinDamage(minDmg * mod);
		primaryWeapon->setMaxDamage(maxDmg * mod);

		SharedWeaponObjectTemplate* weaoTemp = cast<SharedWeaponObjectTemplate*>(primaryWeapon->getObjectTemplate());
		if (weaoTemp != nullptr && weaoTemp->getPlayerRaces()->size() > 0) {
			primaryWeapon->setAttackSpeed(speed);
		} else if (petDeed != nullptr) {
			primaryWeapon->setAttackSpeed(petDeed->getAttackSpeed());
		}
	}

	if (secondaryWeapon != nullptr) {
		float mod = 1 - 0.1*secondaryWeapon->getArmorPiercing();
		secondaryWeapon->setMinDamage(minDmg * mod);
		secondaryWeapon->setMaxDamage(maxDmg * mod);

		SharedWeaponObjectTemplate* weaoTemp = cast<SharedWeaponObjectTemplate*>(secondaryWeapon->getObjectTemplate());
		if (weaoTemp != nullptr && weaoTemp->getPlayerRaces()->size() > 0) {
			secondaryWeapon->setAttackSpeed(speed);
		} else if (petDeed != nullptr) {
			secondaryWeapon->setAttackSpeed(petDeed->getAttackSpeed());
		}
	}

	Reference<WeaponObject*> defaultWeapon = asAiAgent()->getDefaultWeapon();
	if (defaultWeapon != nullptr) {
		defaultWeapon->setMinDamage(minDmg);
		defaultWeapon->setMaxDamage(maxDmg);
		if(petDeed != nullptr)
			defaultWeapon->setAttackSpeed(petDeed->getAttackSpeed());
		else if(isPet())
			defaultWeapon->setAttackSpeed(speed);
	}

	int baseHamMax = ((float)getHamMaximum()) * ratio;
	int baseHam = ((float)getHamBase()) * ratio;

	int ham = 0;

	for (int i = 0; i < 9; ++i) {
		if (i % 3 == 0) {
			if (randomHam)
				ham = System::random(baseHamMax - baseHam) + baseHam;
			else
				ham = ((baseHamMax - baseHam) / 2) + baseHam;

			setBaseHAM(i, ham);
		} else
			setBaseHAM(i, ham / 100);
	}

	for (int i = 0; i < 9; ++i) {
		setHAM(i, baseHAM.get(i));
	}

	for (int i = 0; i < 9; ++i) {
		setMaxHAM(i, baseHAM.get(i));
	}
}

void AiAgentImplementation::initializeTransientMembers() {
	CreatureObjectImplementation::initializeTransientMembers();

	auto aiLogLevel = ConfigManager::instance()->getInt("Core3.AiAgent.LogLevel", LogLevel::WARNING);

	if (aiLogLevel >= 0) {
		// Files should end up in: log/ai/YYYY-MM-DD/HH-MM/oid/AiAgent-{timestamp}-{oid}.log
		Time now;
		StringBuffer logFilename;
		logFilename << "log/ai/"
			<< now.getFormattedTime("%Y-%m-%d/%H-%M")
			<< "/" << getObjectID()
			<< "/AiAgent-" << now.getTime() << "-" << getObjectID() << ".log";

		setFileLogger(logFilename.toString(), false, false);
		setLogSynchronized(true);
		setLogToConsole(false);
		setGlobalLogging(false);
		setLogLevel(static_cast<Logger::LogLevel>(aiLogLevel));
	} else {
		setLogLevel(LogLevel::ERROR);
		setGlobalLogging(true);
	}

	setLoggingName("AiAgent");

	// Handling of old pets on new AI
	if (controlDevice != nullptr) {
		if (npcTemplate == nullptr) {
			// Old pets with no templates have to be destroyed
			TransactionLog trx(TrxCode::SERVERDESTROYOBJECT, asAiAgent(), controlDevice.get());

			ManagedReference<CreatureObject*> linkedCreature = getLinkedCreature().get();
			if (linkedCreature != nullptr) {
				trx.addState("ownerName", linkedCreature->getFirstName());
				trx.addState("ownerID", linkedCreature->getObjectID());
			}

			trx.commit();
			destroyObjectFromDatabase();
			controlDevice.get()->destroyObjectFromDatabase();
			return;
		} else if (!(getOptionsBitmask() & OptionBitmask::AIENABLED)) {
			Logger::console.info(true) << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << ": " << *_this.getReferenceUnsafeStaticCast();
			loadTemplateData(npcTemplate);
		}

		if (defaultWeapon == nullptr)
			loadWeaponTemplateData();
	}

	setAITemplate();

	if (npcTemplate != nullptr)
		setupAttackMaps();
}

void AiAgentImplementation::notifyLoadFromDatabase() {
	CreatureObjectImplementation::notifyLoadFromDatabase();

	auto strongControlDevice = controlDevice.get();

	if (strongControlDevice != nullptr) {
		auto strongLinkedCreature = linkedCreature.get();

		if (strongLinkedCreature != nullptr && strongLinkedCreature->isPlayerCreature() && !strongLinkedCreature->isOnline()) {
			info() << "Storing because linked creature " << strongLinkedCreature->getObjectID() << " is offline.";
			Locker clock(strongLinkedCreature, _this.getReferenceUnsafeStaticCast());
			Locker lock(strongControlDevice);
			strongControlDevice->storeObject(strongLinkedCreature, true);
		}
	}

	if (npcTemplate != nullptr && convoTemplateCRC != 0) {
		ConversationTemplate* conversationTemplate = CreatureTemplateManager::instance()->getConversationTemplate(convoTemplateCRC);

		if (conversationTemplate == nullptr) {
			uint64 tempCRC = npcTemplate->getConversationTemplate();

			if (convoTemplateCRC != tempCRC)
				convoTemplateCRC = tempCRC;
		}
	}
}

void AiAgentImplementation::notifyPositionUpdate(QuadTreeEntry* entry) {
	CreatureObjectImplementation::notifyPositionUpdate(entry);

	SceneObject* object = static_cast<SceneObject*>(entry);

	CreatureObject* creo = object->asCreatureObject();
}

void AiAgentImplementation::doRecovery(int latency) {
	try {
		if (isDead() || getZoneUnsafe() == nullptr) {
			blackboard.removeAll();
			return;
		}

		activateHAMRegeneration(latency);
		activateStateRecovery();
		activatePostureRecovery();

		// we only want to activate recovery if we need to -- it will restart when we need it
		if (defenderList.size() > 0 || damageOverTimeList.hasDot()
				|| getHAM(CreatureAttribute::HEALTH) < getMaxHAM(CreatureAttribute::HEALTH)
				|| getHAM(CreatureAttribute::ACTION) < getMaxHAM(CreatureAttribute::ACTION)
				|| getHAM(CreatureAttribute::MIND) < getMaxHAM(CreatureAttribute::MIND)
				|| getWounds(CreatureAttribute::CONSTITUTION) > 0 || getWounds(CreatureAttribute::STRENGTH) > 0
				|| getWounds(CreatureAttribute::QUICKNESS) > 0 || getWounds(CreatureAttribute::STAMINA) > 0
				|| getWounds(CreatureAttribute::FOCUS) > 0 || getWounds(CreatureAttribute::WILLPOWER) > 0)
			activateRecovery();
	} catch (const Exception& ex) {
		handleException(ex, __FUNCTION__);
	}
}

bool AiAgentImplementation::selectSpecialAttack() {
	Reference<WeaponObject*> strongWeapon = thrownWeapon;

	if (strongWeapon != nullptr && System::random(100) > 95) {
		Locker locker(strongWeapon);

		Reference<SceneObject*> followCopy = getFollowObject().get();

		if (followCopy != nullptr) {
			auto targetID = followCopy->getObjectID();
			Reference<AiAgent*> strongAiAgent = asAiAgent();

			Core::getTaskManager()->executeTask([strongAiAgent, targetID, strongWeapon] () {
				Locker lock(strongAiAgent);

				if (strongAiAgent->getThrownWeapon() != nullptr) {
					strongAiAgent->enqueueCommand(STRING_HASHCODE("throwgrenade"), 0, targetID, String::valueOf(strongWeapon->getObjectID()), QueueCommand::NORMAL);

					Locker locker(strongWeapon);

					if (strongWeapon->getUseCount() < 1) {
						strongWeapon->destroyObjectFromWorld(true);
						strongAiAgent->clearThrownWeapon();
					}
				} else {
					strongAiAgent->error() << "AiAgentThrowGrenadeLambda: thrownWeapon changed to nullptr";
				}
			}, "AiAgentThrowGrenadeLambda");

		}
	}

	const CreatureAttackMap* attackMap = getAttackMap();

	if (attackMap == nullptr) {
		selectDefaultAttack();
		return true;
	}

	return selectSpecialAttack(attackMap->getRandomAttackNumber());
}

bool AiAgentImplementation::selectSpecialAttack(int attackNum) {
	const CreatureAttackMap* attackMap = getAttackMap();

	if (attackMap == nullptr) {
#ifdef DEBUG_AI
		if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
			info("attackMap == nullptr", true);
#endif // DEBUG_AI
		return false;
	}

	if (attackNum < 0) {
		return selectSpecialAttack();
	}

	if (attackNum >= attackMap->size()) {
#ifdef DEBUG_AI
		if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
			info("attackNum >= attackMap->size()", true);
#endif // DEBUG_AI
		return false;
	}

	String cmd = attackMap->getCommand(attackNum);

	if (cmd.isEmpty()) {
#ifdef DEBUG_AI
		if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
			info("cmd.isEmpty()", true);
#endif // DEBUG_AI
		return false;
	}

	nextActionCRC = cmd.hashCode();
	nextActionArgs = attackMap->getArguments(attackNum);

	ZoneServer* zoneServer = getZoneServer();
	if (zoneServer == nullptr)
		return false;

	ObjectController* objectController = zoneServer->getObjectController();
	if (objectController == nullptr)
		return false;

	const QueueCommand* queueCommand = objectController->getQueueCommand(nextActionCRC);
	ManagedReference<SceneObject*> followCopy = getFollowObject().get();

	if (queueCommand == nullptr || followCopy == nullptr)
		return false;

	return true;
}

bool AiAgentImplementation::selectDefaultAttack() {
	if (npcTemplate == nullptr)
		nextActionCRC = STRING_HASHCODE("defaultattack");
	else
		nextActionCRC = npcTemplate->getDefaultAttack().hashCode();

	nextActionArgs = "";

	return true;
}

const QueueCommand* AiAgentImplementation::getNextAction() {
	if (getZoneServer() == nullptr || getZoneServer()->getObjectController() == nullptr)
		return nullptr;

	return getZoneServer()->getObjectController()->getQueueCommand(nextActionCRC);
}

int AiAgentImplementation::enqueueAttack(int priority) {
	ManagedReference<SceneObject*> followCopy = getFollowObject().get();

	if (followCopy != nullptr) {
		enqueueCommand(nextActionCRC, 0, followCopy->getObjectID(), nextActionArgs, priority);
		nextActionCRC = 0;
		nextActionArgs = "";

		return 0;
	}

	return 1;
}

bool AiAgentImplementation::validateStateAttack() {
	ManagedReference<SceneObject*> followCopy = getFollowObject().get();

	if (followCopy == nullptr)
		return false;

	if (!followCopy->isCreatureObject()) {
		// Do not attempt to apply States to Non-CreO Targets
		return true;
	}

	return validateStateAttack(followCopy->asCreatureObject(), nextActionCRC);
}

SceneObject* AiAgentImplementation::getTargetFromMap() {
	TangibleObject* target = getThreatMap()->getHighestThreatAttacker();

	if (target != nullptr && !defenderList.contains(target) && target->getDistanceTo(asAiAgent()) < 128.f && target->isAttackableBy(asAiAgent()) && lastDamageReceived.miliDifference() < 20000) {
		if (target->isCreatureObject()) {
			CreatureObject* creoTarget = target->asCreatureObject();

			if (creoTarget != nullptr && !creoTarget->isDead() && !creoTarget->isIncapacitated()) {
				addDefender(target);
			}
		} else {
			addDefender(target);
		}
	} else if (target != nullptr && defenderList.contains(target) && (!target->isInRange(asAiAgent(), 128) || !target->isAttackableBy(asAiAgent()))) {
		if (target->isCreatureObject()) {
			CreatureObject* tarCreo = target->asCreatureObject();

			if (tarCreo->isDead() || tarCreo->isIncapacitated()) {
				removeDefender(target);
				target = nullptr;
			}
		} else {
			removeDefender(target);
			target = nullptr;
		}
	}

	return target;
}

SceneObject* AiAgentImplementation::getTargetFromDefenders() {
	SceneObject* target = nullptr;

	if (defenderList.size() > 0) {
		for (int i = 0; i < defenderList.size(); ++i) {
			SceneObject* tarObj = defenderList.get(i);

			if (tarObj != nullptr && tarObj->isCreatureObject()) {
				CreatureObject* targetCreature = tarObj->asCreatureObject();

				if (!targetCreature->isDead() && !targetCreature->isIncapacitated() && targetCreature->getDistanceTo(asAiAgent()) < 128.f && targetCreature->isAttackableBy(asAiAgent())) {
					target = targetCreature;

					break;
				} else {
					// if the object on the defender list is no longer attackable, remove it
					removeDefender(targetCreature);
				}
			} else if (tarObj != nullptr && tarObj->isTangibleObject()) {
				TangibleObject* targetTano = tarObj->asTangibleObject();

				if (!targetTano->isDestroyed() && targetTano->getDistanceTo(asAiAgent()) < 128.f && targetTano->isAttackableBy(asAiAgent())) {
					target = targetTano;
					break;
				} else {
					removeDefender(targetTano);
				}
			}
		}
	}

	return target;
}

SceneObject* AiAgentImplementation::getTargetFromTargetsMap(TangibleObject* target) {
	if (target == nullptr) {
		return nullptr;
	}

	Locker threatLock(target, asAiAgent());
	ManagedReference<SceneObject*> prospect = target->getThreatMap()->getHighestThreatAttacker();

	return prospect;
}

SceneObject* AiAgentImplementation::getTargetFromTargetsDefenders() {
	SceneObject* newTarget = nullptr;
	ManagedReference<CreatureObject*> followCopy = getFollowObject().get().castTo<CreatureObject*>();

	if (followCopy == nullptr)
		return nullptr;

	Locker clocker(followCopy, asAiAgent());

	const DeltaVector<ManagedReference<SceneObject*> >* defenders = followCopy->getDefenderList();

	if (defenders->size() > 0) {
		for (int i = 0; i < defenders->size(); ++i) {
			SceneObject* tarObj = defenders->get(i);

			if (tarObj != nullptr && tarObj->isCreatureObject()) {
				CreatureObject* targetCreature = tarObj->asCreatureObject();

				if (!targetCreature->isDead() && !targetCreature->isIncapacitated() && targetCreature->isInRange(asAiAgent(), 128.f) && targetCreature->isAttackableBy(asAiAgent())) {
					newTarget = targetCreature;

					break;
				}
			} else if (tarObj != nullptr && tarObj->isTangibleObject()) {
				TangibleObject* targetTano = tarObj->asTangibleObject();

				if (!targetTano->isDestroyed() && targetTano->getDistanceTo(asAiAgent()) < 128.f && targetTano->isAttackableBy(asAiAgent())) {
					newTarget = targetTano;
					break;
				}
			}
		}
	}

	return newTarget;
}

bool AiAgentImplementation::validateTarget() {
	ManagedReference<SceneObject*> followCopy = getFollowObject().get();

	return validateTarget(followCopy);
}

bool AiAgentImplementation::validateTarget(SceneObject* target) {
	if (target == nullptr) {
		// info("validateTarget target == nullptr", true);
		return false;
	}

	if (!target->isInRange(asAiAgent(), 128)) {
		// info("validateTarget not in range", true);
		return false;
	}

	if (target->isCreatureObject()) {
		CreatureObject* targetCreO = target->asCreatureObject();

		if (targetCreO == nullptr || targetCreO->isInvisible()) {
			// info("validateTarget - target is null or invisible", true);
			return false;
		}

		if (targetCreO->isIncapacitated() || !targetCreO->isAttackableBy(asAiAgent())) {
			// info("validateTarget - target is incapacitated or failed isAttackable check", true);
			return false;
		}

		// Must have permission to walk into cell in order to attack
		if (getParentID() != 0 && getParentID() != targetCreO->getParentID()) {
			Reference<CellObject*> curCell = getParent().get().castTo<CellObject*>();

			if (curCell != nullptr) {
				auto perms = curCell->getContainerPermissions();

				if (!perms->hasInheritPermissionsFromParent()) {
					if (!curCell->checkContainerPermission(targetCreO, ContainerPermissions::WALKIN)) {
						return false;
					}
				}
			}
		}
	} else if (target->isTangibleObject()) {
		TangibleObject* targetTangibleObject = target->asTangibleObject();

		if (targetTangibleObject == nullptr || !targetTangibleObject->isAttackableBy(asAiAgent()) || targetTangibleObject->isDestroyed()) {
			// info("validateTarget failed TANO checks", true);
			return false;
		}
	} else {
		// info("validateTarget returning false as non tano or creature", true);
		return false;
	}

	// info("validateTarget returning true", true);
	return true;
}

int AiAgentImplementation::notifyAttack(Observable* observable) {
	// TODO: add reaction attacks
	return 0;
}

int AiAgentImplementation::handleObjectMenuSelect(CreatureObject* player, byte selectedID) {
	if (isDead() && !isPet()) {
		switch (selectedID) {
		case 35:
			player->executeObjectControllerAction(STRING_HASHCODE("loot"), getObjectID(), "");

			return 0;
		case 36:
			player->executeObjectControllerAction(STRING_HASHCODE("loot"), getObjectID(), "all");

			return 0;
		}
	}

	return CreatureObjectImplementation::handleObjectMenuSelect(player, selectedID);
}

bool AiAgentImplementation::validateStateAttack(CreatureObject* target, unsigned int actionCRC) {
	ManagedReference<ObjectController*> objectController = getZoneServer()->getObjectController();
	const CombatQueueCommand* queueCommand = cast<const CombatQueueCommand*>(objectController->getQueueCommand(actionCRC));

	const VectorMap<uint8, StateEffect>* effects = queueCommand->getStateEffects();

	if (effects->size() == 0) {
		return true;
	}

	for (int i = 0; i < effects->size(); i++) {
		uint8 effectType = effects->elementAt(i).getKey();

		switch (effectType) {
		case CommandEffect::BLIND:
			if (!target->hasState(CreatureState::BLINDED)) {
				return true;
			}
			break;
		case CommandEffect::DIZZY:
			if (!target->hasState(CreatureState::DIZZY)) {
				return true;
			}
			break;
		case CommandEffect::INTIMIDATE:
			if (!target->hasState(CreatureState::INTIMIDATED)) {
				return true;
			}
			break;
		case CommandEffect::STUN:
			if (!target->hasState(CreatureState::STUNNED)) {
				return true;
			}
			break;
		case CommandEffect::KNOCKDOWN:
			if (!target->isKnockedDown() && target->checkKnockdownRecovery()) {
				return true;
			}
			break;
		case CommandEffect::POSTUREUP:
			if (!target->isStanding() && target->checkPostureUpRecovery()) {
				return true;
			}
			break;
		case CommandEffect::POSTUREDOWN:
			if (!target->isProne() && target->checkPostureDownRecovery()) {
				return true;
			}
			break;
		case CommandEffect::NEXTATTACKDELAY:
			if (!target->hasAttackDelay()) {
				return true;
			}
			break;
		default:
			break;
		}
	}

	return false;
}

void AiAgentImplementation::setDespawnOnNoPlayerInRange(bool val) {
	if (despawnOnNoPlayerInRange == val)
		return;

	despawnOnNoPlayerInRange = val;

	if (val && numberOfPlayersInRange <= 0) {
		if (despawnEvent == nullptr) {
			despawnEvent = new DespawnCreatureOnPlayerDissappear(asAiAgent());
		}

		if (!despawnEvent->isScheduled())
			despawnEvent->schedule(30000);
	}
}

void AiAgentImplementation::runAway(CreatureObject* target, float range, bool random = false) {
	if (getParent().get() != nullptr || getParentID() > 0 || target == nullptr || target->getParent().get() != nullptr || getZoneUnsafe() == nullptr) {
		return;
	}

	setTargetObject(target);

	notifyObservers(ObserverEventType::FLEEING, target);
	sendReactionChat(target, ReactionManager::FLEE);

	setMovementState(AiAgent::FLEEING);

	Vector3 runTrajectory;
	Vector3 agentPosition = getWorldPosition();
	Vector3 creaturePosition = target->getWorldPosition();

	if (random) {
		runTrajectory.setX((creaturePosition.getX() + System::random(20)) - (agentPosition.getX() + System::random(20)));
		runTrajectory.setY((creaturePosition.getY() + System::random(20)) - (agentPosition.getY() + System::random(20)));
	} else {
		runTrajectory.setX(agentPosition.getX() - creaturePosition.getX());
		runTrajectory.setY(agentPosition.getY() - creaturePosition.getY());
	}

	float directionAngle = atan2(runTrajectory.getX(), runTrajectory.getY());

	directionAngle = M_PI / 2 - directionAngle;

	if (directionAngle < 0) {
		float a = M_PI + directionAngle;
		directionAngle = M_PI + a;
	}

	float distance = Math::max(1.0f, getWorldPosition().distanceTo(target->getWorldPosition()));

	runTrajectory = agentPosition + (range * (runTrajectory / distance));

	if (runTrajectory.getX() <= coordinateMin || runTrajectory.getX() >= coordinateMax || runTrajectory.getY() <= coordinateMin || runTrajectory.getY() >= coordinateMax) {
		/* info(true) << " Agent trying to flee out of bounds to -- X = " << runTrajectory.getX() << "  Y = " << runTrajectory.getY() << "    With a Direction Angle of " << directionAngle << "  Distance from target to flee " << distance
		<< "  A Range of " << range; */

		return;
	}

	if (isInNavMesh()) {
		Vector3 runPoint(runTrajectory.getX(), runTrajectory.getY(), runTrajectory.getZ());
		Sphere sphere(runPoint, 5.f);
		Vector3 result;

		if (PathFinderManager::instance()->getSpawnPointInArea(sphere, zone, result, true)) {
			runTrajectory = result;
		} else {
			return;
		}
	}

	stopWaiting();
	clearPatrolPoints();
	currentFoundPath = nullptr;

	setNextPosition(runTrajectory.getX(), getZoneUnsafe()->getHeight(runTrajectory.getX(), runTrajectory.getY()), runTrajectory.getY(), getParent().get().castTo<CellObject*>());
}

void AiAgentImplementation::leash(bool forcePeace) {
	Locker locker(&targetMutex);

	if (!forcePeace && getFollowObject() != nullptr)
		removeDefender(getFollowObject().get());

	clearPatrolPoints();
	currentFoundPath = nullptr;
	setFollowObject(nullptr);
	storeFollowObject();

	homeLocation.setReached(false);
	setMovementState(AiAgent::LEASHING);

	eraseBlackboard("targetProspect");

	clearQueueActions(true);
	clearDots();

	if (forcePeace)
		CombatManager::instance()->forcePeace(asAiAgent());
}

void AiAgentImplementation::setDefender(SceneObject* defender) {
	if (defender == nullptr)
		return;

	CreatureObjectImplementation::setDefender(defender);

	setFollowObject(defender);
	setMovementState(AiAgent::FOLLOWING);
	activateRecovery();
}

bool AiAgentImplementation::killPlayer(SceneObject* prospect) {
	if (prospect == nullptr || !prospect->isCreatureObject()) {
		setFollowObject(nullptr);
		return false;
	}

	faceObject(prospect, true);

	PatrolPoint point = prospect->getPosition();
	setNextPosition(point.getPositionX(), point.getPositionZ(), point.getPositionY(), prospect->getParent().get().castTo<CellObject*>());
	activateAiBehavior();

	if (prospect->isInRange(asAiAgent(), 6.f)) {
		ZoneServer* zoneServer = getZoneServer();

		if (zoneServer == nullptr) {
			return false;
		}

		PlayerManager* playerMan = zoneServer->getPlayerManager();
		CreatureObject* prospectCreo = prospect->asCreatureObject();

		if (playerMan != nullptr && prospectCreo != nullptr) {
			Locker klock(prospectCreo);
			playerMan->killPlayer(asCreatureObject(), prospectCreo, 0, false);

			return true;
		}
	}

	return false;
}

bool AiAgentImplementation::stalkProspect(SceneObject* prospect) {
	if (prospect == nullptr || !prospect->isCreatureObject())
		return false;

	CreatureObject* creature = prospect->asCreatureObject();

	if (creature != nullptr && creature->isPlayerCreature() && creature->hasSkill("outdoors_ranger_novice")) {
		StringIdChatParameter param;
		param.setStringId("@skl_use:notify_stalked"); // "It seems that you are being stalked by %TO."
		param.setTO(getObjectName());
		creature->sendSystemMessage(param);
	}

	setStalkObject(prospect);

	PatrolPoint point = prospect->getPosition();
	setNextPosition(point.getPositionX(), point.getPositionZ(), point.getPositionY(), prospect->getParent().get().castTo<CellObject*>());

	return true;
}

void AiAgentImplementation::healTarget(CreatureObject* healTarget) {
	if (healTarget == nullptr || healTarget->isDead()) {
		return;
	}

#ifdef DEBUG_AIHEAL
	ZoneServer* zoneServer = getZoneServer();

	ChatManager* chatManager = nullptr;

	if (zoneServer != nullptr)
		chatManager = zoneServer->getChatManager();
#endif

	uint32 socialGroup = getSocialGroup().toLowerCase().hashCode();

	// Types are: force & normal
	uint32 healerType = getHealerType().toLowerCase().hashCode();
	uint32 typeForce = STRING_HASHCODE("force");

	if (healerType == typeForce || socialGroup == STRING_HASHCODE("nightsister") || socialGroup == STRING_HASHCODE("mtn_clan") || socialGroup == STRING_HASHCODE("force") || socialGroup == STRING_HASHCODE("spider_nightsister")) {
		if (healTarget == asAiAgent()) {
			healTarget->playEffect("clienteffect/pl_force_heal_self.cef");

#ifdef DEBUG_AIHEAL
			if (chatManager != nullptr)
				chatManager->broadcastChatMessage(asAiAgent(), "Force Healing myself!", 0, 0, asAiAgent()->getMoodID());
#endif

		} else {
			doCombatAnimation(healTarget, STRING_HASHCODE("force_healing_1"), 0, 0xFF);

#ifdef DEBUG_AIHEAL
			if (chatManager != nullptr)
				chatManager->broadcastChatMessage(asAiAgent(), "Force Healing target!", 0, 0, asAiAgent()->getMoodID());
#endif
		}
	} else {
		if (healTarget == asAiAgent()) {
			doAnimation("heal_self");

#ifdef DEBUG_AIHEAL
			if (chatManager != nullptr)
				chatManager->broadcastChatMessage(asAiAgent(), "Healing myself!", 0, 0, asAiAgent()->getMoodID());
#endif

		} else {
			doAnimation("heal_other");

#ifdef DEBUG_AIHEAL
			if (chatManager != nullptr)
				chatManager->broadcastChatMessage(asAiAgent(), "Healing target!", 0, 0, asAiAgent()->getMoodID());
#endif
		}

		healTarget->playEffect("clienteffect/healing_healdamage.cef");
	}

	Locker healLock(healTarget);

	int healthMax = healTarget->getMaxHAM(CreatureAttribute::HEALTH) - healTarget->getWounds(CreatureAttribute::HEALTH);
	int actionMax = healTarget->getMaxHAM(CreatureAttribute::ACTION) - healTarget->getWounds(CreatureAttribute::ACTION);
	int mindMax = healTarget->getMaxHAM(CreatureAttribute::MIND) - healTarget->getWounds(CreatureAttribute::MIND);

	int healthDam = healthMax - healTarget->getHAM(CreatureAttribute::HEALTH);
	int actionDam = actionMax - healTarget->getHAM(CreatureAttribute::ACTION);
	int mindDam = mindMax - healTarget->getHAM(CreatureAttribute::MIND);

	int healAmount = getLevel() * 20;

	if (healAmount > healthDam) {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::HEALTH, healthMax, true, false);
	} else {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::HEALTH, healAmount, true, false);
	}

	if (healAmount > actionDam) {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::ACTION, actionMax, true, false);
	} else {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::ACTION, healAmount, true, false);
	}

	if (healAmount > mindDam) {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::MIND, mindMax, true, false);
	} else {
		healTarget->healDamage(asAiAgent(), CreatureAttribute::MIND, healAmount, true, false);
	}
}

void AiAgentImplementation::queueDizzyFallEvent() {
	if (isNonPlayerCreatureObject())
		CreatureObjectImplementation::queueDizzyFallEvent();
}

void AiAgentImplementation::addDefender(SceneObject* defender) {
	if (defender == nullptr) {
		return;
	}

	CreatureObjectImplementation::addDefender(defender);

	if (defender->isTangibleObject() && threatMap != nullptr) {
		threatMap->addAggro(defender->asTangibleObject(), 1);
	}

	notifyPackMobs(defender);
}

void AiAgentImplementation::removeDefender(SceneObject* defender) {
	CreatureObjectImplementation::removeDefender(defender);

	if (defender == nullptr)
		return;

	if (defender->isTangibleObject()) {
		getThreatMap()->dropDamage(defender->asTangibleObject());
	}

	if (getFollowObject().get() == defender) {
		TangibleObject* target = getThreatMap()->getHighestThreatAttacker();

		if (target == nullptr && defenderList.size() > 0) {
			SceneObject* tarObj = defenderList.get(0);

			if (tarObj != nullptr && tarObj->isTangibleObject()) {
				target = tarObj->asTangibleObject();
			}
		}

		if (target != nullptr)
			setDefender(target);
		else
			restoreFollowObject();
	}

	activateRecovery();
}

/**
 * Clears the combat state
 * @pre { this object is locked }
 * @post { this object is locked, this object is not in a combat state }
 * @param clearDefenders if true the defender vector will be emptied
 */
void AiAgentImplementation::clearCombatState(bool clearDefenders) {
	CreatureObjectImplementation::clearCombatState(clearDefenders);

	if (threatMap != nullptr)
		threatMap->removeAll();

	notifyObservers(ObserverEventType::PEACE);
	sendReactionChat(nullptr, ReactionManager::CALM);
}

void AiAgentImplementation::notifyInsert(QuadTreeEntry* entry) {
	CreatureObjectImplementation::notifyInsert(entry);

	SceneObject* scno = static_cast<SceneObject*>(entry);

	if (scno == nullptr)
		return;

	if (scno == asAiAgent() || !scno->isCreatureObject())
		return;

	CreatureObject* creo = scno->asCreatureObject();

	if (creo != nullptr && !creo->isInvisible() && creo->isPlayerCreature()) {
		int newValue = (int) numberOfPlayersInRange.increment();
		activateAiBehavior();
	}
}

void AiAgentImplementation::clearDespawnEvent() {
	despawnEvent = nullptr;
}

void AiAgentImplementation::respawn(Zone* zone, int level) {
	if (getZoneUnsafe() != nullptr)
		return;

	blackboard.removeAll();
	CreatureManager* creatureManager = zone->getCreatureManager();

	if (npcTemplate != nullptr && creatureManager != nullptr && isCreature()) {
		int chance = 2000;
		int babiesSpawned = 0;

		ManagedReference<SceneObject*> home = homeObject.get();

		if (home != nullptr) {
			SortedVector<ManagedReference<Observer*> > observers = home->getObservers(ObserverEventType::CREATUREDESPAWNED);
			DynamicSpawnObserver* observer = nullptr;

			for (int i = 0; i < observers.size(); i++) {
				observer = observers.get(i).castTo<DynamicSpawnObserver*>();

				if (observer != nullptr) {
					break;
				}
			}

			if (observer != nullptr) {
				chance = 500;
				babiesSpawned = observer->getBabiesSpawned();
			}
		}

		if (creatureManager->checkSpawnAsBaby(npcTemplate->getTame(), babiesSpawned, chance)) {
			Creature* creature = cast<Creature*>(asAiAgent());

			if (creature) {
				creature->loadTemplateDataForBaby(npcTemplate);
			} else {
				error("object is not a Creature but returned true to spawn as a baby");
			}
		}
	} else {
		setLevel(level);
	}

	clearRunningChain();
	clearCombatState(true);

	setFollowObject(nullptr);
	storeFollowObject();

	setPosture(CreaturePosture::UPRIGHT, false);

	initializePosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY());

	CellObject* cell = homeLocation.getCell();

	Locker zoneLocker(zone);

	if (cell != nullptr)
		cell->transferObject(asAiAgent(), -1);
	else
		zone->transferObject(asAiAgent(), -1, true);

	setNextPosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY(), cell);
	currentFoundPath = nullptr;

	respawnCounter++;

	activateAiBehavior();
}

void AiAgentImplementation::sendBaselinesTo(SceneObject* player) {
	ReadLocker locker(&despawnMutex);

	CreatureObjectImplementation::sendBaselinesTo(player);
}

void AiAgentImplementation::notifyDespawn(Zone* zone) {
	cancelBehaviorEvent();
	cancelRecoveryEvent();

#ifdef SHOW_NEXT_POSITION
	for (int i = 0; i < movementMarkers.size(); ++i) {
		ManagedReference<SceneObject*> marker = movementMarkers.get(i);
		Locker clocker(marker, asAiAgent());
		marker->destroyObjectFromWorld(false);
	}
#endif

	SceneObject* creatureInventory = asAiAgent()->getSlottedObject("inventory");

	if (creatureInventory != nullptr) {
		Locker clocker(creatureInventory, asAiAgent());
		creatureInventory->setContainerOwnerID(0);
	}

	if (npcTemplate == nullptr)
		return;

	int oldLevel = level;

	Locker locker(&despawnMutex);

	loadTemplateData(templateObject);
	loadTemplateData(npcTemplate);

	if (oldLevel != level)
		setLevel(level);

	stateBitmask = 0;

	shockWounds = 0;
	unmitigatedDamage = 0;

	if (threatMap != nullptr)
		threatMap->removeAll();

	locker.release();

	//Delete all loot out of inventory
	ManagedReference<SceneObject*> inventory = asAiAgent()->getSlottedObject("inventory");

	if (inventory != nullptr) {
		while (inventory->getContainerObjectsSize() > 0) {
			ManagedReference<SceneObject*> obj = inventory->getContainerObject(0);
			inventory->removeFromContainerObjects(0);
			obj->destroyObjectFromWorld(false);
			obj->destroyObjectFromDatabase(true);
		}
	}

	setTargetObject(nullptr);
	setFollowObject(nullptr);

	ManagedReference<SceneObject*> home = homeObject.get();

	if (home != nullptr) {
		home->notifyObservers(ObserverEventType::CREATUREDESPAWNED, asAiAgent());
		return;
	}

	notifyObservers(ObserverEventType::CREATUREDESPAWNED);

	//printReferenceHolders();
	//info(true) << "ID: " << getObjectID() << " Reference Count: " << getReferenceCount();

	if (respawnTimer <= 0) {
		return;
	}

	float respawn = respawnTimer * 1000;

	if (randomRespawn) {
		respawn = System::random(respawn) + (respawn / 2.f);
	}

	Reference<Task*> task = new RespawnCreatureTask(asAiAgent(), zone, level);
	task->schedule(respawn);
}

void AiAgentImplementation::scheduleDespawn(int timeToDespawn) {
	if (getPendingTask("despawn") != nullptr)
		return;

	Reference<DespawnCreatureTask*> despawn = new DespawnCreatureTask(asAiAgent());
	addPendingTask("despawn", despawn, timeToDespawn * 1000);
}

void AiAgentImplementation::notifyDissapear(QuadTreeEntry* entry) {
	CreatureObjectImplementation::notifyDissapear(entry);

	SceneObject* scno = static_cast<SceneObject*>( entry);

	if (scno == asAiAgent())
		return;

	if (scno == getFollowObject().get()) {
			ManagedReference<AiAgent*> ai = asAiAgent();
			ManagedReference<SceneObject*> sceno = scno;

			Core::getTaskManager()->executeTask([=] () {
				Locker locker(ai);
				Locker clocker(sceno, ai);

				if (sceno == ai->getFollowObject().get()) {
					ai->restoreFollowObject();
				}
			}, "RestoreFollowObjectLambda");
	}

	if (scno->isPlayerCreature()) {
		CreatureObject* creo = scno->asCreatureObject();

		if (!creo->isInvisible()) {
			int32 newValue = (int32) numberOfPlayersInRange.decrement();

			if (newValue < 0) {
				int oldValue;

				do {
					oldValue = (int)numberOfPlayersInRange.get();

					newValue = oldValue;

					if (newValue < 0)
						newValue = 0;
				} while (!numberOfPlayersInRange.compareAndSet((uint32)oldValue, (uint32)newValue));
			}

			if (newValue == 0) {
				if (despawnOnNoPlayerInRange && (despawnEvent == nullptr) && !isPet()) {
					despawnEvent = new DespawnCreatureOnPlayerDissappear(asAiAgent());
					despawnEvent->schedule(30000);
				}

				if (isCreature()) {
					Creature* creature = cast<Creature*>(asAiAgent());

					if (creature != nullptr && creature->hasBeenMilked()) {
						Locker clock(creature);

						creature->setMilkState(CreatureManager::NOTMILKED);
					}
				}
			} else if (newValue < 0) {
				error("numberOfPlayersInRange below 0");
			}
		}
	}

	activateAiBehavior();
}

void AiAgentImplementation::activateRecovery() {
	ZoneServer* zoneServer = getZoneServer();

	if (zoneServer != nullptr && zoneServer->isServerShuttingDown()) {
		cancelRecoveryEvent();

		return;
	}

	Locker tLock(&recoveryEventMutex);

	if (recoveryEvent == nullptr) {
		recoveryEvent = new AiRecoveryEvent(asAiAgent());

		recoveryEvent->schedule(2000);
	} else if (!recoveryEvent->isScheduled())
		recoveryEvent->schedule(2000);
}

void AiAgentImplementation::activatePostureRecovery() {
	// Handle AI being Knocked down
	if (isKnockedDown() && !hasPostureChangeDelay() && (!isNpc() || System::random(100) < 40)) {
		enqueueCommand(STRING_HASHCODE("stand"), 0, 0, "");
		return;
	}

	// Prone and kneeling, used for ranged NPC AI
	if (isProne() || isKneeling()) {
		if (isInCombat()) {
			if (System::random(100) > 75 && postureSet.miliDifference() > 0) {
				enqueueCommand(STRING_HASHCODE("stand"), 0, 0, "");
				return;
			}

			ManagedReference<SceneObject*> followCopy = getFollowObject().get();

			if (followCopy != nullptr) {
				float sqrDist = getPosition().squaredDistanceTo(followCopy->getPosition());

				if ((weapon != nullptr && Math::sqrt(sqrDist) > weapon->getMaxRange()) || !checkLineOfSight(followCopy)) {
					enqueueCommand(STRING_HASHCODE("stand"), 0, 0, "");
				}
			}
		} else {
			enqueueCommand(STRING_HASHCODE("stand"), 0, 0, "");
		}
	}

	// Lying down and sitting, should clear when in combat or if their movement state changes
	if ((isLyingDown() || isSitting()) && (getMovementState() != AiAgent::RESTING || isInCombat())) {
		eraseBlackboard("restingTime");
		restDelay.updateToCurrentTime();

		enqueueCommand(STRING_HASHCODE("stand"), 0, 0, "");
	}
}

void AiAgentImplementation::activateHAMRegeneration(int latency) {
	if (isIncapacitated() || isDead() || isInCombat() || isHamRegenDisabled())
		return;

	uint32 healthTick = (uint32) Math::max(1.f, (float) ceil(getMaxHAM(CreatureAttribute::HEALTH) / 300000.f * latency));
	uint32 actionTick = (uint32) Math::max(1.f, (float) ceil(getMaxHAM(CreatureAttribute::ACTION) / 300000.f * latency));
	uint32 mindTick   = (uint32) Math::max(1.f, (float) ceil(getMaxHAM(CreatureAttribute::MIND) / 300000.f * latency));

	healDamage(asCreatureObject(), CreatureAttribute::HEALTH, healthTick, true, false);
	healDamage(asCreatureObject(), CreatureAttribute::ACTION, actionTick, true, false);
	healDamage(asCreatureObject(), CreatureAttribute::MIND,   mindTick,   true, false);

	activatePassiveWoundRegeneration();
}

void AiAgentImplementation::updateCurrentPosition(PatrolPoint* nextPosition) {
	if (nextPosition == nullptr)
		return;

	setPosition(nextPosition->getPositionX(), nextPosition->getPositionZ(), nextPosition->getPositionY());

	CellObject* cell = nextPosition->getCell();

	/*StringBuffer reachedPosition;
	reachedPosition << "(" << nextPosition->getPositionX() << ", " << nextPosition->getPositionY() << ")";
	info("reached " + reachedPosition.toString(), true);*/

	if (getZoneUnsafe() == nullptr)
		return;

	if (cell != nullptr && cell->getParent().get() != nullptr)
		updateZoneWithParent(cell, false, false);
	else
		updateZone(false, false);

	removeOutOfRangeObjects();
	broadcastNextPositionUpdate(nextPosition);
}

void AiAgentImplementation::updatePetSwimmingState() {
	if (parent != nullptr) {
		return;
	}

	Zone* zone = getZoneUnsafe();

	if (zone == nullptr) {
		return;
	}

	PlanetManager* planetManager = zone->getPlanetManager();

	if (planetManager == nullptr) {
		return;
	}

	TerrainManager* terrainManager = planetManager->getTerrainManager();

	if (terrainManager == nullptr) {
		return;
	}

	float waterHeight;
	bool waterIsDefined = terrainManager->getWaterHeight(getPositionX(), getPositionY(), waterHeight);
	float petHeight = getPositionZ();
	float swimVar = waterHeight - swimHeight;

#ifdef DEBUG_PETSWIMMING
	String waterDef = waterIsDefined ? " waterIsDefined == true " : " waterIsDefined == FALSE ";
	info(true) << waterDef;
	info(true) << "Pet Height = " << petHeight << "   Water Height =  " << waterHeight << "   Swim Height = " << swimHeight << " Swim Var = " << swimVar;
#endif

	if (waterIsDefined && ((petHeight <= swimVar + 0.2f) || (petHeight <= swimVar - 0.2f))) {
		// Pet is in the water.
		setState(CreatureState::SWIMMING, true);
	} else {
		// Terrain is above water level.
		clearState(CreatureState::SWIMMING, true);
	}
}

void AiAgentImplementation::updateHomeDirection() {
	float dirDiff = fabs(getDirectionAngle() - homeLocation.getDirection());

	if (dirDiff >= 0.5) {
		setDirection(Math::deg2rad(homeLocation.getDirection()));
		broadcastNextPositionUpdate(&homeLocation);
	}
}

void AiAgentImplementation::checkNewAngle() {
	ManagedReference<SceneObject*> followCopy = getFollowObject().get();

	if (followCopy == nullptr)
		return;

	// info("Check new angle called ", true);

	faceObject(followCopy, true);
}

/* This function determines whether or not an AI should try to continue
 * finding positions next tick. If true, the AI has not reached the first
 * patrolPoint in their queue. If false, the point is either out of movement
 * range or they have reached their maxDistance to the point.
*/
bool AiAgentImplementation::findNextPosition(float maxDistance, bool walk) {
	/*
	 * SETUP: Check speed and posture before attempting to find a path
	 */

#ifdef DEBUG_AI
	if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
		info("findNextPosition(" + String::valueOf(maxDistance) + ", " + String::valueOf(walk) + ")", true);
#endif // DEBUG_AI

	Locker locker(&targetMutex);

	if (isDead() || getPatrolPointSize() <= 0)
		return false;

	int posture = getPosture();
	int movementState = getMovementState();

	if (posture == CreaturePosture::CROUCHED)
		return false;

	float newSpeed = runSpeed;

	if (movementState == AiAgent::FLEEING)
		newSpeed *= 0.7f;

	if ((walk && movementState != AiAgent::FLEEING) || posture == CreaturePosture::PRONE)
		newSpeed = walkSpeed;

	if (hasState(CreatureState::IMMOBILIZED))
		newSpeed = newSpeed / 2.f;

	if (hasState(CreatureState::FROZEN))
		newSpeed = 0.01f;

	float updateTicks = float(BEHAVIORINTERVAL) / 1000.f;
	float maxSpeed = newSpeed * updateTicks; // maxSpeed is the distance able to travel in time updateTicks

	Vector3 currentPosition = getPosition();
	Vector3 currentWorldPos = getWorldPosition();
	PatrolPoint endMovementPosition = getNextPosition();

	Vector3 endDistDiff(currentWorldPos - endMovementPosition.getWorldPosition());
	float endDistanceSq = (endDistDiff.getX() * endDistDiff.getX() + endDistDiff.getY() * endDistDiff.getY());
	float maxSquared = Math::max(0.1f, maxDistance * maxDistance);

	float endDistZSq = endDistDiff.getZ() * endDistDiff.getZ();
	endDistZSq = Math::getPrecision(endDistZSq, 2);

#ifdef DEBUG_FINDNEXTPOSITION
	info(true) << "findNextPosition -- ID: " <<  getObjectID() << " endDistSquared = " << endDistanceSq << "  maxSquared = " << maxSquared << " endDistDiff Z = " << endDistZSq << " Max Distance = " << maxDistance;
#endif

	if (endDistanceSq <= maxSquared && fabs(endDistZSq) < (maxDistance + 1.f)) {
		currentFoundPath = nullptr;

		if (patrolPoints.size() > 0)
			patrolPoints.remove(0);

		if (movementState != AiAgent::FOLLOWING)
			notifyObservers(ObserverEventType::DESTINATIONREACHED);

		setCurrentSpeed(0.f);
		updateLocomotion();

		return false;
	}

	// Handle speed up and slow down
	if ((((currentSpeed * currentSpeed) * maxSquared) > endDistanceSq) && newSpeed > 0.4f) {
		newSpeed = Math::max(0.2f, (currentSpeed - 0.4f));
	} else if (currentSpeed < newSpeed) {
		float speedDiff = newSpeed - currentSpeed;

		if (speedDiff > 0.4f)
			newSpeed = currentSpeed + 0.4f;
	}

	setCurrentSpeed(newSpeed);
	updateLocomotion();

#ifdef DEBUG_FINDNEXTPOSITION
	printf("--- !!!!    findNextPosition -- Start -- !!!! ----- \n");

	printf("Patrol Points Size = %i \n", patrolPoints.size());

	printf("Current World Position x = %f , ", currentWorldPos.getX());
	printf(" z = %f \n", currentWorldPos.getZ());
	printf(" y = %f \n", currentWorldPos.getY());

	printf("End Movement Position x = %f , ", endMovementPosition.getWorldPosition().getX());
	printf(" z = %f \n", endMovementPosition.getWorldPosition().getZ());
	printf(" y = %f \n", endMovementPosition.getWorldPosition().getY());

	printf("endDistanceSq = %f \n", endDistanceSq);
	printf("maxSquared = %f \n", maxSquared);
	printf("max distance = %f \n", maxDistance);
#endif

	PathFinderManager* pathFinder = PathFinderManager::instance();

	if (pathFinder == nullptr)
		return false;

	/*
	*	STEP 1: If we do not already have a path referenced, find a new path
	*/

	Reference<Vector<WorldCoordinates>* > path;
	ManagedReference<SceneObject*> currentParent = getParent().get();

	PatrolPoint currentPoint(currentPosition);
	const WorldCoordinates endMovementCoords = endMovementPosition.getCoordinates();
	CellObject* endMovementCell = endMovementPosition.getCell();

	if (currentFoundPath == nullptr) {
		// No prior path or path is null, find new path
		if (currentParent != nullptr && currentParent->isCellObject()) {
			currentPoint.setCell(currentParent.castTo<CellObject*>());
		}

		path = currentFoundPath = static_cast<CurrentFoundPath*>(pathFinder->findPath(currentPoint.getCoordinates(), endMovementCoords, getZoneUnsafe()));
	} else {
		if (currentParent != nullptr && !currentParent->isCellObject())
			currentParent = nullptr;

		if ((movementState == AiAgent::FOLLOWING || movementState == AiAgent::PATHING_HOME || movementState == AiAgent::NOTIFY_ALLY || movementState == AiAgent::MOVING_TO_HEAL || movementState == AiAgent::WATCHING)
			&& endMovementCell == nullptr && currentParent == nullptr && currentFoundPath->get(currentFoundPath->size() - 1).getWorldPosition().squaredDistanceTo(endMovementCoords.getWorldPosition()) > 4 * 4) {

			path = currentFoundPath = static_cast<CurrentFoundPath*>(pathFinder->findPath(currentPoint.getCoordinates(), endMovementPosition.getCoordinates(), getZoneUnsafe()));
		} else {
			currentFoundPath->set(0, WorldCoordinates(currentPosition, currentParent.castTo<CellObject*>()));
			path = currentFoundPath;
		}
	}

	if (path == nullptr || path->size() < 2) {
		currentFoundPath = nullptr;
		return false;
	}

#ifdef SHOW_PATH
	CreateClientPathMessage* pathMessage = new CreateClientPathMessage();
	if (getParent() == nullptr && pathMessage != nullptr) {
		pathMessage->addCoordinate(currentPosition.getX(), currentPosition.getZ(), currentPosition.getY());
	}
#endif

	// Filter out duplicate path points
	if (currentParent != nullptr && endMovementCell != nullptr)
		pathFinder->filterPastPoints(path, asAiAgent());

	// the farthest we will move is one point in the path, and the movement update time will change to reflect that
	WorldCoordinates nextMovementPosition;

	nextMovementPosition = path->get(1);

	if (nextMovementPosition.getX() == currentPosition.getX() && nextMovementPosition.getY() == currentPosition.getY()) {
		path->remove(1);

		if (path->size() >= 2) {
			nextMovementPosition = path->get(1);
		} else {
			path = nullptr;
			currentFoundPath = nullptr;
			return false;
		}
	}

	CellObject* nextMovementCell = nextMovementPosition.getCell();
	uint64 currentParentID = currentParent != nullptr ? currentParent->getObjectID() : 0;
	uint64 nextParentID = nextMovementCell != nullptr ? nextMovementCell->getObjectID() : 0;

	if (currentParentID != nextParentID && nextParentID > 0)
		currentPosition = PathFinderManager::transformToModelSpace(currentPosition, nextMovementCell->getParent().get());

	Vector3 movementDiff(currentWorldPos - nextMovementPosition.getWorldPosition());

	// Determine the distance to the next point excluding the Z coordnate
	float nextMovementDistance = Math::sqrt(movementDiff.getX() * movementDiff.getX() + movementDiff.getY() * movementDiff.getY());
	float maxDist = maxSpeed;

	if (endDistanceSq > maxSquared) {
		// this is the actual "distance we can travel" calculation. We only want to
		// go to the edge of the maxDistance radius and stop, so select the minimum
		// of either our max travel distance (maxSpeed) or the distance from the
		// maxDistance radius
		maxDist = Math::min(maxSpeed, endDistanceSq - maxSquared + 0.1f);
	}

#ifdef DEBUG_AI
	if (nextMovementDistance <= 0) {
		/*info(true) << "findNextPosition -- ID: " <<  getObjectID() << " endDistSquared = " << endDistanceSq << "  maxSquared = " << maxSquared << "   For:  " << getObjectID();
		info(true) << " ----- >>>>>>>> nextMovementDistance = " << nextMovementDistance << "   For: " << getObjectID() << " Movement State = " << movementState << " Path size: " << path->size() << "  Patrol points total = " << getPatrolPointSize() << "  Location: " << currentPosition.toString() << "    Next Position = " << nextMovementPosition.getWorldPosition().toString();*/
	}
#endif

#ifdef DEBUG_PATHING
	printf("findNextPosition - Path Size = %i ---  \n", path->size());

	printf("max speed = %f \n", maxSpeed);
	printf("max distance = %f \n", maxDist);
	printf("Current Position x = %f , ", currentPosition.getX());
	printf(" z = %f \n", currentPosition.getZ());
	printf(" y = %f \n", currentPosition.getY());

	printf("Next Movement Position X = %f , ", nextMovementPosition.getX());
	printf(" Z = %f \n", nextMovementPosition.getZ());
	printf(" Y = %f \n", nextMovementPosition.getY());
	printf("nextMovementDistance = %f \n", nextMovementDistance);

	/*printf(" - Current Path Points - \n");

	for (int i = 0; i < path->size(); ++i) {
		WorldCoordinates pos = path->get(i);

		printf("Point # %i ", i);
		printf(" X = %f , ", pos.getX());
		printf(" Z = %f ", pos.getZ());
		printf(" Y = %f \n", pos.getY());

		if (pos.getCell() == nullptr)
			printf(" Cell is null \n");
	}*/
#endif
	Vector3 newPosition;

	if (nextMovementDistance > maxDist && currentParentID == nextParentID) {
		// nextMovementPosition is further then the maxDist and both points are in the same parent or in the zone
		// Calculate the distance we can go and set the new nextMovementPosition
		float dx = nextMovementPosition.getX() - currentPosition.getX();
		float dy = nextMovementPosition.getY() - currentPosition.getY();

		newPosition.setX(currentPosition.getX() + (maxDist * (dx / nextMovementDistance)));
		newPosition.setY(currentPosition.getY() + (maxDist * (dy / nextMovementDistance)));
	} else {
		newPosition.setX(nextMovementPosition.getX());
		newPosition.setY(nextMovementPosition.getY());

		path->remove(1);
	}

	// Handle next Z coordinate
	if (!isInNavMesh() && currentParent == nullptr) {
		newPosition.setZ(getWorldZ(newPosition));
	} else {
		newPosition.setZ(nextMovementPosition.getZ());
	}

	nextMovementPosition.setX(newPosition.getX());
	nextMovementPosition.setY(newPosition.getY());
	nextMovementPosition.setZ(newPosition.getZ());

#ifdef SHOW_PATH
		for (int i = 1; i < path->size(); ++i) { // i = 0 is our position
			const WorldCoordinates& nextPositionDebug = path->get(i);

			Vector3 nextWorldPos = nextPositionDebug.getWorldPosition();

			if (nextPositionDebug.getCell() == nullptr)
				pathMessage->addCoordinate(nextWorldPos.getX(), currentPosition.getZ(), nextWorldPos.getY());
		}

		broadcastMessage(pathMessage, false);
#endif

#ifdef SHOW_NEXT_POSITION
		for (int i = 0; i < movementMarkers.size(); ++i) {
			ManagedReference<SceneObject*> marker = movementMarkers.get(i);
			Locker clocker(marker, asAiAgent());
			marker->destroyObjectFromWorld(false);
		}

		movementMarkers.removeAll();

		for (int i = 1; i < path->size(); ++i) { // i = 0 is our position
			const WorldCoordinates& nextPositionDebug = path->get(i);

			Vector3 nextWorldPos = nextPositionDebug.getWorldPosition();

			Reference<SceneObject*> movementMarker = getZoneServer()->createObject(STRING_HASHCODE("object/path_waypoint/path_waypoint.iff"), 0);

			Locker clocker(movementMarker, asAiAgent());

			movementMarker->initializePosition(nextPositionDebug.getX(), nextPositionDebug.getZ(), nextPositionDebug.getY());
			StringBuffer msg;
			msg << "Next Position: path distance: " << nextPositionDebug.getWorldPosition().distanceTo(getWorldPosition()) << " maxDist:" << maxDist;
			movementMarker->setCustomObjectName(msg.toString(), false);

			CellObject* cellObject = nextPositionDebug.getCell();

			if (cellObject != nullptr) {
				cellObject->transferObject(movementMarker, -1, true);
			} else {
				getZone()->transferObject(movementMarker, -1, true);
			}

			movementMarkers.add(movementMarker);
		}
#endif

	/*
	* STEP 3: Send the movement updates
	*/

	// Set the next place we will be if we are to move
	nextStepPosition.setPosition(nextMovementPosition.getX(), nextMovementPosition.getZ(), nextMovementPosition.getY());
	nextStepPosition.setCell(nextMovementCell);

	float dx = nextMovementPosition.getX() - getPositionX();
	float dy = nextMovementPosition.getY() - getPositionY();

	float directionAngle = atan2(dy, dx);

	directionAngle = M_PI / 2 - directionAngle;

	if (directionAngle < 0) {
		float a = M_PI + directionAngle;
		directionAngle = M_PI + a;
	}

	float error = fabs(directionAngle - direction.getRadians());

	if (error >= 0.05) {
		setDirection(directionAngle);
	}

	auto interval = BEHAVIORINTERVAL;
	nextBehaviorInterval = Math::min((int)((Math::min(nextMovementDistance, maxDist) / newSpeed) * 1000 + 0.5), interval);
	currentSpeed = newSpeed;

	updateCurrentPosition(&nextStepPosition);

	if (isPet()) {
		updatePetSwimmingState();
	}

#ifdef DEBUG_AI
	if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
		info("findNextPosition - complete returning true", true);
#endif // DEBUG_AI

#ifdef DEBUG_FINDNEXTPOSITION
	printf("----   !!!!   findNextPosition -- End --   !!!! -----\n");
#endif

	return true;
}

bool AiAgentImplementation::checkLineOfSight(SceneObject* obj) {
	return CollisionManager::checkLineOfSight(asAiAgent(), obj);
}

float AiAgentImplementation::getWorldZ(const Vector3& position) {
	float zCoord = 0.f;

	Zone* zone = getZoneUnsafe();

	if (zone == nullptr)
		return zCoord;

	IntersectionResults intersections;

	if (closeobjects != nullptr) {
		Vector<QuadTreeEntry*> closeObjects(closeobjects->size(), 10);

		closeobjects->safeCopyReceiversTo(closeObjects, CloseObjectsVector::COLLIDABLETYPE);
		CollisionManager::getWorldFloorCollisions(position.getX(), position.getY(), zone, &intersections, closeObjects);

		PlanetManager* planetMan = zone->getPlanetManager();

		if (planetMan != nullptr)
			zCoord = planetMan->findClosestWorldFloor(position.getX(), position.getY(), position.getZ(), swimHeight, &intersections, nullptr);
	} else {
		SortedVector<ManagedReference<QuadTreeEntry*> > closeObjects;

#ifdef COV_DEBUG
		zone->info("Null closeobjects vector in AiAgentImplementation::getWorldZ", true);
#endif

		Vector3 worldPosition = getWorldPosition();
		zone->getInRangeObjects(worldPosition.getX(), worldPosition.getY(), 128, &closeObjects, true);

		CollisionManager::getWorldFloorCollisions(position.getX(), position.getY(), zone, &intersections, closeObjects);

		PlanetManager* planetMan = zone->getPlanetManager();

		if (planetMan != nullptr)
			zCoord = planetMan->findClosestWorldFloor(position.getX(), position.getY(), position.getZ(), swimHeight, &intersections, nullptr);
	}

	return zCoord;
}

void AiAgentImplementation::runBehaviorTree() {
	try {
		if (getZoneUnsafe() == nullptr || !(getOptionsBitmask() & OptionBitmask::AIENABLED))
			return;

#ifdef DEBUG_AI
		bool alwaysActive = ConfigManager::instance()->getAiAgentLoadTesting();
#else // DEBUG_AI
		bool alwaysActive = false;
#endif // DEBUG_AI

		ZoneServer* zoneServer = getZoneServer();

		if ((!alwaysActive && numberOfPlayersInRange.get() <= 0 && getFollowObject().get() == nullptr && !isRetreating()) || zoneServer == nullptr || zoneServer->isServerLoading() || zoneServer->isServerShuttingDown()) {
			cancelBehaviorEvent();
			setFollowObject(nullptr);
			return;
		}

		Reference<Behavior*> rootBehavior = getBehaviorTree(BehaviorTreeSlot::NONE);
		assert(rootBehavior != nullptr);

#ifdef DEBUG_AI
		Time startTime;
		startTime.updateToCurrentTime();

		if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
			info("Performing root behavior: " + rootBehavior->print(), true);
#endif // DEBUG_AI

		// activate AI
		Behavior::Status actionStatus = rootBehavior->doAction(asAiAgent());

		if (actionStatus == Behavior::RUNNING)
			popRunningChain(); // don't keep root in the running chain

#ifdef DEBUG_AI
		if (peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true)
			info("rootBehavior->doAction() took " + String::valueOf((int)startTime.miliDifference()) + "ms to complete.", true);
#endif // DEBUG_AI

		activateAiBehavior(true);
	} catch (Exception& ex) {
		cancelBehaviorEvent();
		handleException(ex, __FUNCTION__);
	}
}

void AiAgentImplementation::setAIDebug(bool flag) {
#ifdef DEBUG_AI
	writeBlackboard("aiDebug", flag);
#endif // DEBUG_AI
	info() << "setAIDebug(" << flag << ")";

	if (flag) {
		setLogLevel(LogLevel::DEBUG);
		debug() << "setAIDebug(" << flag << ")";
		debug() << "behaviorEvent->isScheduled = " << (behaviorEvent != nullptr ? behaviorEvent->isScheduled() : -1);
		debug() << "recoveryEvent->isScheduled = " << (recoveryEvent != nullptr ? recoveryEvent->isScheduled() : -1);
		debug() << "primaryAttackMap.size = " << (primaryAttackMap != nullptr ? primaryAttackMap->size() : -1);
		debug() << "secondaryAttackMap.size = " << (secondaryAttackMap != nullptr ? secondaryAttackMap->size() : -1);
		debug() << "defaultAttackMap.size = " << (defaultAttackMap != nullptr ? defaultAttackMap->size() : -1);
		debug() << "lastDamageReceived = " << lastDamageReceived.miliDifference();
		debug() << "lastPackNotify = " << lastPackNotify.miliDifference();
		debug() << "lastCallForHelp = " << lastCallForHelp.miliDifference();
		debug() << "fleeDelay = " << fleeDelay.miliDifference();
		debug() << "postureSet = " << postureSet.miliDifference();
		debug() << "healDelay = " << healDelay.miliDifference();
		debug() << "alertedTime = " << alertedTime.miliDifference();
		debug() << "aggroDelay = " << aggroDelay.miliDifference();
		debug() << "numberOfPlayersInRange = " << numberOfPlayersInRange.get();
	} else {
		auto aiLogLevel = ConfigManager::instance()->getInt("Core3.AiAgent.LogLevel", -1);

		if (aiLogLevel >= 0) {
			setLogLevel(static_cast<Logger::LogLevel>(aiLogLevel));
		} else {
			setLogLevel(Logger::LogLevel::ERROR);
		}
	}
}

bool AiAgentImplementation::getAIDebug() {
#ifdef DEBUG_AI
	return peekBlackboard("aiDebug") && readBlackboard("aiDebug") == true;
#else // DEBUG_AI
	return getLogLevel() >= LogLevel::DEBUG;
#endif // DEBUG_AI
}

bool AiAgentImplementation::isRunningBehavior(unsigned int id) {
	return runningChain.contains(id);
}

void AiAgentImplementation::addRunningID(unsigned int id) {
	runningChain.add(id); // this adds to the back
}

void AiAgentImplementation::popRunningChain() {
	// pop_back
	runningChain.remove(runningChain.size() - 1);
}

uint32 AiAgentImplementation::peekRunningChain() {
	return runningChain.get(runningChain.size() - 1);
}

void AiAgentImplementation::clearRunningChain() {
	runningChain.removeAll();
}

void AiAgentImplementation::setAITemplate() {
	btreeMap.removeAll();
	blackboard.removeAll();

	uint64 customMap = customAiMap;

	if (npcTemplate.get() != nullptr && customAiMap == 0)
		customMap = npcTemplate.get()->getCustomAiMap();

	for (const auto& slot : BehaviorTreeSlot()) {
		Behavior* btree = AiMap::instance()->getTemplate(creatureBitmask, slot, customMap);

		if (btree == nullptr)
			continue;

		setTree(btree, slot);
	}

	activateAiBehavior();
}

Behavior* AiAgentImplementation::getBehaviorTree(const BehaviorTreeSlot& slot) {
	return btreeMap.get(slot).get();
}

void AiAgentImplementation::setTree(Behavior* subRoot, const BehaviorTreeSlot& slot) {
	if (subRoot == nullptr) {
		btreeMap.drop(slot);
		return;
	}

	btreeMap.put(slot, subRoot);
}

void AiAgentImplementation::removeTree(const BehaviorTreeSlot& slot) {
	setTree(NULL, slot);
}

void AiAgentImplementation::loadCreatureBitmask() {
}

void AiAgentImplementation::unloadCreatureBitmask() {
}

bool AiAgentImplementation::generatePatrol(int num, float dist) {
	// info(true) << "ID: " << getObjectID() << "  generatePatrol called with a state of " << getMovementState() << " and point size of = " << getPatrolPointSize();

	Zone* zone = getZoneUnsafe();

	if (zone == nullptr)
		return false;

	// save movementState in case point generation fails
	uint32 savedState = getMovementState();

	if (savedState == AiAgent::LEASHING || savedState == AiAgent::PATHING_HOME)
		return false;

	if (savedState != AiAgent::PATROLLING) {
		// this clears patrol points
		setMovementState(AiAgent::PATROLLING);

		clearSavedPatrolPoints();
	}

	Vector3 currentPosition = getPosition();
	dist += getTemplateRadius();

	if (isInNavMesh()) {
		Vector3 homeCoords;
		homeCoords.setX(homeLocation.getPositionX());
		homeCoords.setY(homeLocation.getPositionY());
		homeCoords.setZ(homeLocation.getPositionZ());

		Sphere sphere(homeCoords, dist);
		Vector3 result;

		for (int i = 0; i < num; i++) {
			if (PathFinderManager::instance()->getSpawnPointInArea(sphere, zone, result, true)) {
				PatrolPoint point(result);

				if (point.getPositionX() == currentPosition.getX() && point.getPositionY() == currentPosition.getY())
					continue;

				addPatrolPoint(point);
			}
		}
	} else {
		SortedVector<QuadTreeEntry*> closeObjects;

		if (closeobjects != nullptr) {
			closeobjects->safeCopyReceiversTo(closeObjects, CloseObjectsVector::COLLIDABLETYPE);
		} else {
#ifdef COV_DEBUG
			zone->info("Null closeobjects vector in AiAgentImplementation::generatePatrol", true);
#endif

			Vector3 worldPosition = getWorldPosition();
			zone->getInRangeObjects(worldPosition.getX(), worldPosition.getY(), 128, &closeObjects, true);
		}

		for (int i = 0; i < num; i++) {
			PatrolPoint newPoint;
			newPoint.setPositionX(homeLocation.getPositionX() + (-1 * dist + (float)System::random((unsigned int)dist * 2)));
			newPoint.setPositionY(homeLocation.getPositionY() + (-1 * dist + (float)System::random((unsigned int)dist * 2)));
			newPoint.setPositionZ(homeLocation.getPositionZ());

			if (newPoint.getPositionX() == currentPosition.getX() && newPoint.getPositionY() == currentPosition.getY())
				continue;

			ManagedReference<SceneObject*> strongParent = getParent().get();
			if (strongParent != nullptr && strongParent->isCellObject()) {
				newPoint.setCell(strongParent.castTo<CellObject*>());
			}

			if (newPoint.getCell() == nullptr && zone != nullptr) {
				PlanetManager* planetManager = zone->getPlanetManager();
				IntersectionResults intersections;

				CollisionManager::getWorldFloorCollisions(newPoint.getPositionX(), newPoint.getPositionY(), zone, &intersections, closeObjects);

				newPoint.setPositionZ(planetManager->findClosestWorldFloor(newPoint.getPositionX(), newPoint.getPositionY(), newPoint.getPositionZ(), this->getSwimHeight(), &intersections, (CloseObjectsVector*) this->getCloseObjects()));
			}

			addPatrolPoint(newPoint);
		}
	}

	// info(true) << "ID: " << getObjectID() << " Finished - generatePatrol with a state of " << getMovementState() << " and point size of = " << getPatrolPointSize();

	if (getPatrolPointSize() > 0)
		return true;

	setMovementState(savedState);
	return false;
}

float AiAgentImplementation::getMaxDistance() {
	if (isRetreating() || isFleeing())
		return 0.1f;

	ManagedReference<SceneObject*> followCopy = getFollowObject().get();
	unsigned int stateCopy = getMovementState();

	// info("getmaxDistance - stateCopy: " + String::valueOf(stateCopy) /*+ " Max Distance: " + String::valueOf(maxDistance)*/, true);
	switch (stateCopy) {
		case AiAgent::OBLIVIOUS:
			return 2.0f;
		case AiAgent::WATCHING:
			return 1.5f;
		case AiAgent::PATROLLING:
			return 0.1f;
		case AiAgent::LEASHING:
			return 0.1f;
		case AiAgent::STALKING: {
			int stalkRad = 0;
			if (peekBlackboard("stalkRadius"))
				stalkRad = readBlackboard("stalkRadius").get<int>() / 5;

			return stalkRad > 0 ? stalkRad : 10;
		}
		case AiAgent::FOLLOWING:
			if (followCopy == nullptr)
				return 0.1f;

			if (!CollisionManager::checkLineOfSight(asAiAgent(), followCopy)) {
				return 1.0f;
			} else if (!isInCombat()) {
				if (peekBlackboard("formationOffset")) {
					if (isPet()) {
						return 0.1f;
					} else {
						return 1.0f;
					}
				} else {
					if (isPet() && isDroid())
						return 2.0f;

					return 4.0f;
				}
			} else if (getWeapon() != nullptr) {
				WeaponObject* currentWeapon = getWeapon();
				float weapMaxRange = 1.0f;

				if (currentWeapon != nullptr) {
					weapMaxRange = Math::min(currentWeapon->getIdealRange(), currentWeapon->getMaxRange());

					if (currentWeapon->isMeleeWeapon() && weapMaxRange > 8) {
						weapMaxRange = 3.f;
					}

					weapMaxRange = Math::max(1.0f, weapMaxRange + getTemplateRadius() + followCopy->getTemplateRadius());
				}

				return weapMaxRange;
			} else {
				return 1 + getTemplateRadius() + followCopy->getTemplateRadius();
			}
			break;
		case AiAgent::PATHING_HOME:
			return 0.1f;
			break;
		case AiAgent::MOVING_TO_HEAL: {
			float healRange = 5.0f;
			uint32 healerType = getHealerType().toLowerCase().hashCode();

			if (healerType == STRING_HASHCODE("force")) {
				// Force heals can be executed at 32m but we will move AI closer
				healRange = 20.f;
			}

			return healRange;
		}
		case AiAgent::NOTIFY_ALLY:
			return 1.0f;
		case AiAgent::CRACKDOWN_SCANNING:
			return 1.0f;
		case AiAgent::HARVESTING:
			return 1.0f;
	}

	return 5.f;
}

int AiAgentImplementation::setDestination() {
	//info("setDestination start", true);

	ManagedReference<SceneObject*> followCopy = getFollowObject().get();
	unsigned int stateCopy = getMovementState();

	// info(true) << "ID: " << getObjectID() << "  setDestination - stateCopy: " << String::valueOf(stateCopy) << "  Patrol Point Size:" << getPatrolPointSize();
	// info("homeLocation: " + homeLocation.toString(), true);

	if (patrolPoints.size() > 20) {
		info() << "Patrol points have overflowed. Total points: " << patrolPoints.size();
		clearPatrolPoints();
	}

	switch (stateCopy) {
	case AiAgent::OBLIVIOUS:
		if (!(creatureBitmask & CreatureFlag::EVENTCONTROL) && !(creatureBitmask & CreatureFlag::STATIONARY) && !homeLocation.isInRange(asAiAgent(), 1.0f)) {
			homeLocation.setReached(false);
			setMovementState(AiAgent::PATHING_HOME);
		}
		break;
	case AiAgent::FLEEING: {
		int64 fleeDiff = (fleeDelay.miliDifference() / 4) * -1;

		if (fleeDiff < 1500) {
			eraseBlackboard("fleeRange");
			setMovementState(AiAgent::FOLLOWING);
			return stateCopy;
		}

		break;
	}
	case AiAgent::LEASHING:
		clearPatrolPoints();

		if (!homeLocation.isInRange(asAiAgent(), 4.0f)) {
			homeLocation.setReached(false);
			setNextPosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY(), homeLocation.getCell());
		} else {
			updateHomeDirection();
			homeLocation.setReached(true);
			setOblivious();
		}

		break;
	case AiAgent::PATROLLING:
		// info(true) << " ID: " << getObjectID() << " Patrolling - Patrol points size = " << getPatrolPointSize();

		if (getPatrolPointSize() == 0) {
			setPatrolPoints(savedPatrolPoints);
			clearSavedPatrolPoints();
		}

		break;
	case AiAgent::WATCHING:
		if ((getCreatureBitmask() & CreatureFlag::ESCORT) && followCopy != nullptr)
			setNextPosition(followCopy->getPositionX(), followCopy->getPositionZ(), followCopy->getPositionY(), followCopy->getParent().get().castTo<CellObject*>());

		break;
	case AiAgent::STALKING:
		if (followCopy == nullptr || !followCopy->isInRange(asAiAgent(), 128)) {
			setMovementState(AiAgent::OBLIVIOUS);
			break;
		}

		setNextPosition(followCopy->getPositionX(), followCopy->getPositionZ(), followCopy->getPositionY(), followCopy->getParent().get().castTo<CellObject*>());
		break;
	case AiAgent::FOLLOWING: {
		clearPatrolPoints();

		if (followCopy == nullptr) {
			setMovementState(AiAgent::PATHING_HOME);
			break;
		}

		if (!isPet() && !checkLineOfSight(followCopy) && !homeLocation.isInRange(asAiAgent(), AiAgent::MAX_OOS_RANGE)) {
			if (++outOfSightCounter > AiAgent::MAX_OOS_COUNT && System::random(100) <= AiAgent::MAX_OOS_PERCENT) {
			    leash();
			    return setDestination();
			}
		} else if (outOfSightCounter > 0) {
			--outOfSightCounter;
		}

		if (!isPet() && followCopy->getParent().get() != nullptr) {
			ManagedReference<SceneObject*> rootParent = followCopy->getRootParent();

			if (rootParent != nullptr && rootParent->isBuildingObject()) {
				BuildingObject* rootBuilding = rootParent.castTo<BuildingObject*>();

				if (rootBuilding != nullptr && rootBuilding->isPrivateStructure()) {
					leash();
					return setDestination();
				}
			}
		}

		PatrolPoint nextPos = followCopy->getPosition();

		if (peekBlackboard("formationOffset") && !isInCombat()) {
			Vector3 formationOffset = readBlackboard("formationOffset").get<Vector3>();
			float directionAngle = followCopy->getDirection()->getRadians();
			float xRotated = (formationOffset.getX() * Math::cos(directionAngle) + formationOffset.getY() * Math::sin(directionAngle));
			float yRotated = (-formationOffset.getX() * Math::sin(directionAngle) + formationOffset.getY() * Math::cos(directionAngle));

			nextPos.setPositionX(nextPos.getPositionX() + xRotated);
			nextPos.setPositionY(nextPos.getPositionY() + yRotated);
		} else {
			checkNewAngle();
		}

		setNextPosition(nextPos.getPositionX(), nextPos.getPositionZ(), nextPos.getPositionY(), followCopy->getParent().get().castTo<CellObject*>());
		break;
	}
	case AiAgent::EVADING:
		if (followCopy == nullptr || getPatrolPointSize() == 0) {
			setMovementState(AiAgent::PATHING_HOME);
			return setDestination();
		}

		break;
	case AiAgent::PATHING_HOME: {
		if (isInCombat()) {
			setMovementState(AiAgent::FOLLOWING);
			break;
		}

		clearPatrolPoints();

		if (!homeLocation.isInRange(asAiAgent(), 1.0f)) {
			homeLocation.setReached(false);

			setNextPosition(homeLocation.getPositionX(), homeLocation.getPositionZ(), homeLocation.getPositionY(), homeLocation.getCell());
		} else {
			updateHomeDirection();
			setOblivious();
			homeLocation.setReached(true);
		}

		break;
	}
	case AiAgent::MOVING_TO_HEAL: {
		if (!peekBlackboard("healTarget")) {
			if (!isWaiting()) {
				if (followCopy != nullptr) {
					setMovementState(AiAgent::FOLLOWING);
				} else {
					setMovementState(AiAgent::PATHING_HOME);
				}
			}
		} else {
			ManagedReference<CreatureObject*> healTarget = readBlackboard("healTarget").get<ManagedReference<CreatureObject*> >().get();

			if (healTarget != nullptr) {
				clearPatrolPoints();
				Vector3 targetPos = healTarget->getPosition();
				setNextPosition(targetPos.getX(), targetPos.getZ(), targetPos.getY(), healTarget->getParent().get().castTo<CellObject*>());
			}
		}
		break;
	}
	case AiAgent::NOTIFY_ALLY: {
		break;
	}
	case AiAgent::CRACKDOWN_SCANNING: {
		clearPatrolPoints();

		if (followCopy == nullptr) {
			break;
		}

		PatrolPoint nextPos = followCopy->getPosition();

		setNextPosition(nextPos.getPositionX(), nextPos.getPositionZ(), nextPos.getPositionY(), followCopy->getParent().get().castTo<CellObject*>());
		break;
	}
	case AiAgent::HARVESTING: {
		break;
	}
	case AiAgent::RESTING: {
		break;
	}
	case AiAgent::CONVERSING: {
		break;
	}
	default:
		if (creatureBitmask & CreatureFlag::STATIC || homeLocation.getCell() != nullptr) {
			setMovementState(AiAgent::PATHING_HOME);
		} else if (followCopy == nullptr) {
			setMovementState(AiAgent::PATROLLING);
		}
		break;
	}

	//info("setDestination end " + String::valueOf(getPatrolPointSize()), true);

	return getPatrolPointSize();
}

void AiAgentImplementation::setWait(int wait) {
	cooldownTimerMap->updateToCurrentAndAddMili("waitTimer", wait);
}

void AiAgentImplementation::stopWaiting() {
	if (peekBlackboard("isWaiting"))
		eraseBlackboard("isWaiting");
	cooldownTimerMap->updateToCurrentTime("waitTimer");
}

bool AiAgentImplementation::isWaiting() const {
	return !cooldownTimerMap->isPast("waitTimer");
}

bool AiAgentImplementation::isCamouflaged(CreatureObject* creature) {
	if (creature == nullptr)
		return false;

	Locker locker(&targetMutex);

	CreatureObject* effectiveTarget = creature;

	if (creature->isVehicleObject() || creature->isMount()) {
		effectiveTarget = creature->getSlottedObject("rider").castTo<CreatureObject*>();
	}

	if (effectiveTarget == nullptr || !effectiveTarget->isPlayerCreature())
		return false;

	uint32 concealCRC = STRING_HASHCODE("skill_buff_mask_scent");

	bool concealed = effectiveTarget->hasBuff(concealCRC);

	if ((!concealed && !isMonster()) || isPet() || isDroid() || isAndroid())
		return false;

	bool scentMasked = effectiveTarget->hasBuff(STRING_HASHCODE("skill_buff_mask_scent_self"));
	uint64 effectiveTargetID = effectiveTarget->getObjectID();

	if (!concealed && !scentMasked) {
		camouflagedObjects.drop(effectiveTargetID);
		return false;
	}

	// Don't check if it's already been checked
	if (camouflagedObjects.contains(effectiveTargetID))
		return true;

	StringBuffer camoDebug;

	int concealMod = 0;

	if (concealed) {
		ConcealBuff* concealBuff = cast<ConcealBuff*>(effectiveTarget->getBuff(concealCRC));

		if (concealBuff == nullptr)
			return false;

		String buffPlanet = concealBuff->getPlanetName();

		if (buffPlanet != getZoneUnsafe()->getZoneName())
			return false;

		concealMod = effectiveTarget->getSkillModFromBuffs("private_conceal");

		camoDebug << "Concealed = true. Conceal Mod = " << concealMod <<". ";
	}

	int mod = 0;

	if (concealed) {
		mod = concealMod;

		if (concealMod > 0) {
			mod += effectiveTarget->getSkillMod("camouflage") / 2;
		}
	} else {
		mod = effectiveTarget->getSkillMod("mask_scent");

		if (mod > 0)
			mod /= 2;
	}

	camoDebug << " Initial Mod = " << mod << ". ";

	if (mod < 5)
		return false;

	// Player is masked or concealed, do not roll chance to break on non-aggressive Mobs
	if (!isStalker() && !isAggressiveTo(effectiveTarget))
		return true;

	PlayerManager* playerMan = server->getPlayerManager();

	if (playerMan == nullptr)
		return false;

	int playerLevel = playerMan->calculatePlayerLevel(effectiveTarget);
	int creatureLevel = getLevel();

	if (effectiveTarget->isGrouped()) {
		Reference<GroupObject*> group = effectiveTarget->getGroup();

		if (group != nullptr) {
			Locker locker(group);
			playerLevel += group->getGroupLevel(false);
		}
	}

	camoDebug << " Player Level = " << playerLevel << ".  Creature Level = " << creatureLevel << ". ";

	if ((playerLevel - creatureLevel) >= 10) {
#ifdef DEBUG_CAMOUFLAGE
		effectiveTarget->sendSystemMessage(camoDebug.toString());
#endif
		return true;
	}

	float roll = (100 + mod) - creatureLevel;

	camoDebug << " Initial Roll = " << roll << ". ";

	if (effectiveTarget->getPosture() == CreaturePosture::PRONE) {
		roll += 15;
	}

	if (effectiveTarget->isRunning()) {
		roll -= 10;
	} else if (effectiveTarget->isWalking()) {
		roll -= 5;
	}

	if (creatureLevel > (2 * mod))
		roll -= 20;

	if (concealed) {
		if (roll > 98)
			roll = 98;
	} else {
		if (roll > 95)
			roll = 95;
	}

	if (!isMonster())
		roll -= 5;

	camoDebug << "Roll after calcs = " << roll << ". ";

	bool success = (System::random(100) +1) < roll ? true : false;

	if (success) {
		camouflagedObjects.put(effectiveTargetID);

		camoDebug << "Success";
	} else {
		camouflagedObjects.drop(effectiveTargetID);

		camoDebug << "Failure";
	}

#ifdef DEBUG_CAMOUFLAGE
	effectiveTarget->sendSystemMessage(camoDebug.toString());
#endif

	Reference<Task*> camoTask = new CamoTask(effectiveTarget, asAiAgent(), !concealed, success);
	camoTask->execute();

	return success;
}

void AiAgentImplementation::activateAiBehavior(bool reschedule) {
	if (getZoneUnsafe() == nullptr || !(getOptionsBitmask() & OptionBitmask::AIENABLED))
		return;

#ifdef DEBUG_AI
	bool alwaysActive = ConfigManager::instance()->getAiAgentLoadTesting();
#else // DEBUG_AI
	bool alwaysActive = false;
#endif // DEBUG_AI

	ZoneServer* zoneServer = getZoneServer();

	if ((!alwaysActive && numberOfPlayersInRange.get() <= 0 && getFollowObject().get() == nullptr && !isRetreating()) || zoneServer == nullptr || zoneServer->isServerLoading() || zoneServer->isServerShuttingDown()) {
		cancelBehaviorEvent();
		return;
	}

	Locker locker(&behaviorEventMutex);

	if (behaviorEvent == nullptr) {
		behaviorEvent = new AiBehaviorEvent(asAiAgent());
		behaviorEvent->schedule(Math::max(10, nextBehaviorInterval));
	} else {
		if (reschedule) {
			try {
				if (!behaviorEvent->isScheduled())
					behaviorEvent->schedule(Math::max(10, nextBehaviorInterval));
			} catch (IllegalArgumentException& e) {
			}
		}
	}

	nextBehaviorInterval = BEHAVIORINTERVAL;
}

void AiAgentImplementation::cancelBehaviorEvent() {
	Locker locker(&behaviorEventMutex);

	if (behaviorEvent == nullptr) {
		return;
	}

	if (behaviorEvent->isScheduled())
		behaviorEvent->cancel();

	behaviorEvent->clearCreatureObject();
	behaviorEvent = nullptr;
}

void AiAgentImplementation::cancelRecoveryEvent() {
	Locker locker(&recoveryEventMutex);

	if (recoveryEvent == nullptr) {
		return;
	}

	if (recoveryEvent->isScheduled())
		recoveryEvent->cancel();

	recoveryEvent->clearAgentObject();
	recoveryEvent = nullptr;
}

void AiAgentImplementation::setNextPosition(float x, float z, float y, CellObject* cell) {
	//info("setNextPosition(" + String::valueOf(x) + ", " + String::valueOf(z) + ", " + String::valueOf(y) + ")", true);

	Locker locker(&targetMutex);

	PatrolPoint point(x, z, y, cell);

	patrolPoints.add(0, point);
}

void AiAgentImplementation::setNextStepPosition(float x, float z, float y, CellObject* cell) {
	Locker locker(&targetMutex);

	PatrolPoint point(x, z, y, cell);

	nextStepPosition = point;
}

void AiAgentImplementation::broadcastNextPositionUpdate(PatrolPoint* point) {
	BasePacket* msg = nullptr;
	++movementCounter;

	if (point == nullptr) {
		if (parent.get() != nullptr)
			msg = new UpdateTransformWithParentMessage(asAiAgent());
		else
			msg = new UpdateTransformMessage(asAiAgent());
	} else {
		if (point->getCell() != nullptr) {
			msg = new LightUpdateTransformWithParentMessage(asAiAgent(), point->getPositionX(), point->getPositionZ(), point->getPositionY(), point->getCell()->getObjectID());
		} else {
			msg = new LightUpdateTransformMessage(asAiAgent(), point->getPositionX(), point->getPositionZ(), point->getPositionY());
		}
	}

 	broadcastMessage(msg, false);
}

int AiAgentImplementation::notifyObjectDestructionObservers(TangibleObject* attacker, int condition, bool isCombatAction) {
	sendReactionChat(attacker, ReactionManager::DEATH);

	if (isPet()) {
		PetManager* petManager = getZoneServer()->getPetManager();

		petManager->notifyDestruction(attacker, asAiAgent(), condition, isCombatAction);

	} else {
		if (getZoneUnsafe() != nullptr) {
			CreatureManager* creatureManager = getZoneUnsafe()->getCreatureManager();

			creatureManager->notifyDestruction(attacker, asAiAgent(), condition, isCombatAction);
		}
	}

	return CreatureObjectImplementation::notifyObjectDestructionObservers(attacker, condition, isCombatAction);
}

int AiAgentImplementation::notifyConverseObservers(CreatureObject* converser) {
	notifyObservers(ObserverEventType::CONVERSE, converser);

	return 1;
}

int AiAgentImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, bool notifyClient, bool isCombatAction) {
	lastDamageReceived.updateToCurrentTime();

	activateRecovery();

	if (damage > 0) {
		// This damage is DOT or other types of non direct combat damage, it should not count towards loot and thus not be added to the threat map damage.
		// Adding aggro should still be done.
		getThreatMap()->addAggro(attacker, 1);
	}

	notifyObservers(ObserverEventType::DAMAGERECEIVED, attacker);
	return CreatureObjectImplementation::inflictDamage(attacker, damageType, damage, destroy, notifyClient, isCombatAction);
}

int AiAgentImplementation::inflictDamage(TangibleObject* attacker, int damageType, float damage, bool destroy, const String& xp, bool notifyClient, bool isCombatAction) {
	lastDamageReceived.updateToCurrentTime();

	activateRecovery();

	if (damage > 0) {
		getThreatMap()->addDamage(attacker, damage, xp);
	}

	notifyObservers(ObserverEventType::DAMAGERECEIVED, attacker);

	return CreatureObjectImplementation::inflictDamage(attacker, damageType, damage, destroy, notifyClient, isCombatAction);
}

void AiAgentImplementation::notifyPackMobs(SceneObject* attacker) {
	auto closeObjectsVector = getCloseObjects();

	if (!lastPackNotify.isPast() || closeObjectsVector == nullptr)
		return;

	lastPackNotify.updateToCurrentTime();
	lastPackNotify.addMiliTime(30000);

	Vector<QuadTreeEntry*> closeObjects(closeObjectsVector->size(), 10);
	closeObjectsVector->safeCopyReceiversTo(closeObjects, CloseObjectsVector::CREOTYPE);
	uint32 socialGroup = getSocialGroup().toLowerCase().hashCode();

	for (int i = 0; i < closeObjects.size(); ++i) {
		SceneObject* object = static_cast<SceneObject*>(closeObjects.get(i));

		if (!object->isCreatureObject())
			continue;

		CreatureObject* creo = object->asCreatureObject();

		if (creo == nullptr || creo->isDead() || creo == asAiAgent() || creo->isPlayerCreature() || creo->isInCombat())
			continue;

		if (creo->getParentID() != getParentID())
			continue;

		if (!(creo->getPvpStatusBitmask() & CreatureFlag::ATTACKABLE))
			continue;

		AiAgent* agent = creo->asAiAgent();

		if (agent == nullptr || !(agent->getCreatureBitmask() & CreatureFlag::PACK) || agent->getMovementState() == AiAgent::LEASHING)
			continue;

		String targetSocialGroup = agent->getSocialGroup().toLowerCase();

		if (targetSocialGroup.isEmpty() || targetSocialGroup.hashCode() != socialGroup)
			continue;

		float packRange = 20.f + (getLevel() / 100.f);

		if (getPvpStatusBitmask() & CreatureFlag::AGGRESSIVE)
			packRange += 5.f;

		if (!agent->isInRange(asAiAgent(), packRange))
			continue;

		if (!CollisionManager::checkLineOfSight(asAiAgent(), creo))
			continue;

		Reference<AiAgent*> agentRef = agent;
		Reference<SceneObject*> attackerRef = attacker;

		Core::getTaskManager()->executeTask([agentRef, attackerRef] () {
			Locker locker(agentRef);
			Locker clocker(attackerRef, agentRef);

			Time* lastNotify = agentRef->getLastPackNotify();

			if (lastNotify != nullptr) {
				lastNotify->updateToCurrentTime();
				lastNotify->addMiliTime(30000);
			}

			agentRef->showFlyText("npc_reaction/flytext", "threaten", 0xFF, 0, 0);
			agentRef->setDefender(attackerRef);

		}, "PackAttackLambda");
	}
}

void AiAgentImplementation::fillAttributeList(AttributeListMessage* alm, CreatureObject* player) {

	if (isDead()) {
		return;
	}

	if (getArmor() == 0)
		alm->insertAttribute("armorrating", "None");
	else if (getArmor() == 1)
		alm->insertAttribute("armorrating", "Light");
	else if (getArmor() == 2)
		alm->insertAttribute("armorrating", "Medium");
	else if (getArmor() == 3)
		alm->insertAttribute("armorrating", "Heavy");

	if (isSpecialProtection(SharedWeaponObjectTemplate::KINETIC)) {
		StringBuffer txt;
		txt << Math::getPrecision(getKinetic(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_kinetic", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::ENERGY)) {
		StringBuffer txt;
		txt << Math::getPrecision(getEnergy(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_energy", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::ELECTRICITY)) {
		StringBuffer txt;
		txt << Math::getPrecision(getElectricity(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_electrical", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::STUN)) {
		StringBuffer txt;
		txt << Math::getPrecision(getStun(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_stun", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::BLAST)) {
		StringBuffer txt;
		txt << Math::getPrecision(getBlast(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_blast", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::HEAT)) {
		StringBuffer txt;
		txt << Math::getPrecision(getHeat(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_heat", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::COLD)) {
		StringBuffer txt;
		txt << Math::getPrecision(getCold(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_cold", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::ACID)) {
		StringBuffer txt;
		txt << Math::getPrecision(getAcid(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_elemental_acid", txt.toString());
	}

	if (isSpecialProtection(SharedWeaponObjectTemplate::LIGHTSABER)) {
		StringBuffer txt;
		txt << Math::getPrecision(getLightSaber(), 1) << "%";
		alm->insertAttribute("cat_armor_special_protection.armor_eff_restraint", txt.toString());
	}

	if (getKinetic() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::KINETIC)) {
		StringBuffer txt;
		txt << Math::getPrecision(getKinetic(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_kinetic", txt.toString());
	}

	if (getEnergy() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::ENERGY)) {
		StringBuffer txt;
		txt << Math::getPrecision(getEnergy(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_energy", txt.toString());
	}

	if (getElectricity() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::ELECTRICITY)) {
		StringBuffer txt;
		txt << Math::getPrecision(getElectricity(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_electrical", txt.toString());
	}

	if (getStun() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::STUN)) {
		StringBuffer txt;
		txt << Math::getPrecision(getStun(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_stun", txt.toString());
	}

	if (getBlast() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::BLAST)) {
		StringBuffer txt;
		txt << Math::getPrecision(getBlast(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_blast", txt.toString());
	}

	if (getHeat() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::HEAT)) {
		StringBuffer txt;
		txt << Math::getPrecision(getHeat(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_heat", txt.toString());
	}

	if (getCold() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::COLD)) {
		StringBuffer txt;
		txt << Math::getPrecision(getCold(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_cold", txt.toString());
	}

	if (getAcid() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::ACID)) {
		StringBuffer txt;
		txt << Math::getPrecision(getAcid(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_elemental_acid", txt.toString());
	}

	if (getLightSaber() > 0 && !isSpecialProtection(SharedWeaponObjectTemplate::LIGHTSABER)) {
		StringBuffer txt;
		txt << Math::getPrecision(getLightSaber(), 1) << "%";
		alm->insertAttribute("cat_armor_effectiveness.armor_eff_restraint", txt.toString());
	}

	if (getKinetic() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_kinetic", "-");

	if (getEnergy() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_energy", "-");

	if (getElectricity() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_electrical", "-");

	if (getStun() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_stun", "-");

	if (getBlast() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_blast", "-");

	if (getHeat() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_heat", "-");

	if (getCold() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_cold", "-");

	if (getAcid() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_elemental_acid", "-");

	if (getLightSaber() < 0)
		alm->insertAttribute("cat_armor_vulnerability.armor_eff_restraint", "-");

	if (isPet())
	{
		ManagedReference<CreatureObject*> owner = getLinkedCreature().get();

		if (owner != nullptr)
		{
			StringBuffer fullName;
			fullName << owner->getFirstName();
			if(!owner->getLastName().isEmpty())
				fullName << " " << owner->getLastName();

			alm->insertAttribute("@obj_attr_n:owner", fullName.toString());
		}
	}

	if (player != nullptr && player->getPlayerObject() && player->getPlayerObject()->hasGodMode()) {
		ManagedReference<SceneObject*> home = homeObject.get();

		if (home != nullptr) {
			int type = 0;
			if (home->getObserverCount(ObserverEventType::OBJECTDESTRUCTION) > 0)
				type = ObserverEventType::OBJECTDESTRUCTION;
			else if (home->getObserverCount(ObserverEventType::CREATUREDESPAWNED) > 0)
				type = ObserverEventType::CREATUREDESPAWNED;

			if (type != 0) {
				ManagedReference<SpawnObserver*> spawnObserver = nullptr;
				SortedVector<ManagedReference<Observer*> > observers = home->getObservers(type);

				for (int i = 0; i < observers.size(); i++) {
					spawnObserver = cast<SpawnObserver*>(observers.get(i).get());
					if (spawnObserver != nullptr)
						break;
				}

				if (spawnObserver) {
					String name = spawnObserver->getLairTemplateName();
					alm->insertAttribute("blank_entry" , "");
					alm->insertAttribute("object_type" , name);
					alm->insertAttribute("blank_entry" , "");
				}
			}
		}
	}
}

bool AiAgentImplementation::sendConversationStartTo(SceneObject* player) {
	if (!player->isPlayerCreature() || isDead() || convoTemplateCRC == 0)
		return false;

	setMovementState(AiAgent::CONVERSING);
	clearPatrolPoints();

	// Face player.
	faceObject(player);

	PatrolPoint current(coordinates.getPosition(), getParent().get().castTo<CellObject*>());

	broadcastNextPositionUpdate(&current);

	CreatureObject* playerCreature = cast<CreatureObject*>(player);

	if (playerCreature == nullptr)
		return false;

	ConversationTemplate* conversationTemplate = CreatureTemplateManager::instance()->getConversationTemplate(convoTemplateCRC);

	if (conversationTemplate == nullptr) {
		error("Could not find the conversation template.");
		return false;
	}

	if (conversationTemplate->getConversationTemplateType() == ConversationTemplate::ConversationTemplateTypeLua && conversationTemplate->getLuaClassHandler() == "trainerConvHandler") {
		ManagedReference<CityRegion*> city = player->getCityRegion().get();

		if (city != nullptr && !city->isClientRegion() && city->isBanned(player->getObjectID())) {
			playerCreature->sendSystemMessage("@city/city:banned_services"); // You are banned from using this city's services.
			return false;
		}
	}

	StartNpcConversation* conv = new StartNpcConversation(playerCreature, getObjectID(), "");
	player->sendMessage(conv);

	SortedVector<ManagedReference<Observer*> > observers = getObservers(ObserverEventType::STARTCONVERSATION);

	for (int i = 0;  i < observers.size(); ++i) {
		if (dynamic_cast<ConversationObserver*>(observers.get(i).get()) != nullptr)
			return true;
	}

	//Create conversation observer.
	ConversationObserver* conversationObserver = ConversationManager::instance()->getConversationObserver(convoTemplateCRC);

	if (conversationObserver != nullptr) {
		//Register observers.
		registerObserver(ObserverEventType::CONVERSE, conversationObserver);
		registerObserver(ObserverEventType::STARTCONVERSATION, conversationObserver);
		registerObserver(ObserverEventType::SELECTCONVERSATION, conversationObserver);
		registerObserver(ObserverEventType::STOPCONVERSATION, conversationObserver);
	} else {
		error("Could not create conversation observer.");
		return false;
	}

	return true;
}

bool AiAgentImplementation::stopConversation() {
	setMovementState(AiAgent::OBLIVIOUS);

	return true;
}

bool AiAgentImplementation::isAggressiveTo(CreatureObject* target) {
	if (target == nullptr || asAiAgent() == target)
		return false;

	// info(true) << "AiAgent isAggressiveTo called for ID: " << getObjectID() << " towards creature: " << target->getObjectID();

	if (isAggressive(target)) {
		// info(true) << "AiAgent isAggressiveTo check returned true";
		return true;
	}

	// info(true) << "AiAgent isAggressiveTo check returned FALSE";
	return false;
}

bool AiAgentImplementation::isAggressive(CreatureObject* target) {
	if (target == nullptr)
		return false;

	if (target->isInvisible())
		return false;

	bool targetIsPlayer = target->isPlayerCreature();
	bool targetIsAgent = target->isAiAgent();

	if (targetIsAgent && target->isPet() && !target->asAiAgent()->isMindTricked()) {
		ManagedReference<PetControlDevice*> pcd = target->getControlDevice().get().castTo<PetControlDevice*>();

		if (pcd != nullptr && pcd->getPetType() == PetManager::FACTIONPET && isNeutral())
			return false;

		ManagedReference<CreatureObject*> owner = target->getLinkedCreature().get();

		if (owner == nullptr)
			return false;

		return isAggressiveTo(owner);
	}

	if (isPet() && !isMindTricked()) {
		ManagedReference<PetControlDevice*> pcd = getControlDevice().get().castTo<PetControlDevice*>();

		if (pcd != nullptr && pcd->getPetType() == PetManager::FACTIONPET && target->isNeutral()) {
			return false;
		}

		ManagedReference<CreatureObject*> owner = getLinkedCreature().get();

		if (owner == nullptr || target == owner)
			return false;

		return owner->isAggressiveTo(target);
	}

	// Get factions
	uint32 thisFaction = getFaction();
	uint32 targetFaction = target->getFaction();

	//GCW Faction Checks -- Both the agent and attcking CreO have GCW Factions and they are different
	if (thisFaction != 0 && targetFaction != 0 && thisFaction != targetFaction) {
		// Target is an AiAgent
		if (targetIsAgent) {
			return true;
		}

		if (targetIsPlayer) {
			bool covertOvert = ConfigManager::instance()->useCovertOvertSystem();

			if (covertOvert) {
				PlayerObject* ghost = target->getPlayerObject();

				if (ghost == nullptr)
					return false;

				uint32 targetStatus = target->getFactionStatus();
				bool gcwTef = ghost->hasGcwTef();

				if (!gcwTef && targetStatus == FactionStatus::COVERT)
					return false;

				if (targetStatus == FactionStatus::OVERT || gcwTef) {
					return true;
				}
			} else {
				// this is the same thing, but ensures that if the target is a player, that they aren't on leave
				if (target->getFactionStatus() != FactionStatus::ONLEAVE) {
					return true;
				}
			}
		}
	}

	AiAgent* tarAgent = nullptr;

	if (targetIsAgent) {
		tarAgent = target->asAiAgent();

		if (tarAgent != nullptr) {
			if (isCarnivore() && tarAgent->isHerbivore())
				return true;

			uint32 socialGroup = getSocialGroup().toLowerCase().hashCode();

			if (socialGroup == STRING_HASHCODE("poacher") && tarAgent->isMonster())
				return true;
		}
	}

	// Get this agents faction string agents faction string (which could include imp/reb)
	String factionString = getFactionString();

	// info(true) << " faction string == " << factionString;

	if (!factionString.isEmpty()) {
		if (targetIsAgent && tarAgent != nullptr) {
			String tarAgentFaction = tarAgent->getFactionString();

			// AI can check the enemy strings directly vs other AI (since they don't have a standing)
			if (FactionManager::instance()->isEnemy(factionString, tarAgentFaction)) {
				return true;
			} else if (factionString.hashCode() == tarAgentFaction.hashCode()) {
				return false;
			}
		}

		if (targetIsPlayer) {
			PlayerObject* tarGhost = target->getPlayerObject();

			if (tarGhost == nullptr) {
				return false;
			}

			/* for players, we are only an enemy if the standing is less than -3000, but we are forced to non-aggressive
			*  status if the standing is over 3000, otherwise use the pvpStatusBitmask to determine aggressiveness
			*/
			if (!(getOptionsBitmask() & CreatureFlag::IGNORE_FACTION_STANDING)) {
				int minFactionStanding = -3000;
				float targetsStanding = tarGhost->getFactionStanding(factionString);

				// info(true) << " faction standing: " << targetsStanding;

				if (targetsStanding <= minFactionStanding) {
					return true;
				} else if (targetsStanding >= 3000)
					return false;
			}
		}
	}

	// Agent is not aggressive due to faction or standing, remaining aggressive check based on pvpStatusBitmask
	return pvpStatusBitmask & CreatureFlag::AGGRESSIVE;
}

bool AiAgentImplementation::isAttackableBy(TangibleObject* object) {
	/*
	* This function should return true if this creature is attackable by the passed Tangible Object
	*/

	if (object == nullptr)
		return false;

	if (object->isCreatureObject())
		return isAttackableBy(object->asCreatureObject());

	// info(true) << "AiAgent::isAttackableBy TangibleObject Check -- Object ID = " << getObjectID() << " by attacking TanO ID = " << object->getObjectID();

	if (movementState == AiAgent::LEASHING || isDead() || isIncapacitated())
		return false;

	if (isPet() && !isMindTricked()) {
		ManagedReference<PetControlDevice*> pcd = getControlDevice().get().castTo<PetControlDevice*>();

		if (pcd != nullptr && pcd->getPetType() == PetManager::FACTIONPET && object->isNeutral())
			return false;

		ManagedReference<CreatureObject*> owner = getLinkedCreature().get();

		if (owner == nullptr)
			return false;

		return owner->isAttackableBy(object, true);
	}

	// Initial this creature has an attackable pvpStatusBitmask
	if (!(pvpStatusBitmask & CreatureFlag::ATTACKABLE) || optionsBitmask & OptionBitmask::INVULNERABLE)
		return false;

	if (eventArea.get() != nullptr) {
		if (!object->hasActiveArea(eventArea.get())) {
			return false;
		}
	}

	if (isInNoCombatArea())
		return false;

	// Get factions
	uint32 thisFaction = getFaction();
	uint32 tanoFaction = object->getFaction();

	if (thisFaction != 0 || tanoFaction != 0) {
		if (thisFaction == tanoFaction) {
			return false;
		}

		if (thisFaction == 0 && tanoFaction != 0) {
			return false;
		}
	}

	// info(true) << "AiAgent::isAttackableBy TangibleObject Check returned true";

	return true;
}

bool AiAgentImplementation::isAttackableBy(CreatureObject* creature) {
	if (creature == nullptr || asAiAgent() == creature)
		return false;

	// info(true) << "AiAgent::isAttackableBy Creature Check -- Object ID = " << getObjectID() << " by attacking Creature ID = " << creature->getObjectID();

	if (movementState == AiAgent::LEASHING || isDead() || isIncapacitated())
		return false;

	// Handle Pets - Check against owner
	if (isPet() && !isMindTricked()) {
		ManagedReference<PetControlDevice*> pcd = getControlDevice().get().castTo<PetControlDevice*>();
		if (pcd != nullptr && pcd->getPetType() == PetManager::FACTIONPET && creature->isNeutral()) {
			return false;
		}

		ManagedReference<CreatureObject*> owner = getLinkedCreature().get();

		if (owner == nullptr) {
			return false;
		}

		return owner->isAttackableBy(creature, true);
	}

	if (pvpStatusBitmask == 0 || !(pvpStatusBitmask & CreatureFlag::ATTACKABLE))
		return false;

	if (creature->isPet()) {
		ManagedReference<PetControlDevice*> controlDevice = creature->getControlDevice().get().castTo<PetControlDevice*>();

		if (controlDevice != nullptr && controlDevice->getPetType() == PetManager::FACTIONPET && isNeutral())
			return false;

		ManagedReference<CreatureObject*> owner = creature->getLinkedCreature().get();

		if (owner == nullptr)
			return false;

		return isAttackableBy(owner);
	}

	// Check against Vehicle Object Owner
	if (creature->isVehicleObject()) {
		ManagedReference<CreatureObject*> owner = creature->getLinkedCreature().get();

		if (owner == nullptr)
			return false;

		return isAttackableBy(owner);
	}

	if (eventArea.get() != nullptr) {
		if (!creature->hasActiveArea(eventArea.get())) {
			return false;
		}
	}

	if (isInNoCombatArea())
		return false;

	bool creatureIsAgent = creature->isAiAgent();
	bool creatureIsPlayer = creature->isPlayerCreature();

	// Get factions
	uint32 thisFaction = getFaction();
	uint32 creatureFaction = creature->getFaction();

	// Faction Checks
	if (thisFaction != 0) {
		if (creatureIsPlayer) {
			Reference<PlayerObject*> ghost = creature->getPlayerObject();

			if (ghost != nullptr && ghost->hasCrackdownTefTowards(thisFaction)) {
				return true;
			}
		}

		// Attacking creature has no faction
		if (creatureFaction == 0)
			return false;

		// This faction and attacking creature are same faction
		if (thisFaction == creatureFaction)
			return false;

		// Attack creature is not an AiAgent && their faction status is OnLeave
		if (creatureIsPlayer && creature->getFactionStatus() < FactionStatus::COVERT)
			return false;
	}

	// Ai vs Ai Checks
	if (creatureIsAgent) {
		if ((getCreatureBitmask() & CreatureFlag::NOAIAGGRO) && !creature->isPet())
			return false;

		// Faction check stricly for AI vs AI. Faction AI attamepting to attack non-faction AI
		if (thisFaction == 0 && creatureFaction > 0)
			return false;

		AiAgent* creatureAgent = creature->asAiAgent();

		if (creatureAgent == nullptr)
			return false;

		const CreatureTemplate* thisTemplate = getCreatureTemplate();
		const CreatureTemplate* creoTemplate = creatureAgent->getCreatureTemplate();

		if ((thisTemplate != nullptr && creoTemplate != nullptr) && (thisTemplate->getTemplateName() == creoTemplate->getTemplateName()))
			return false;

		uint32 thisSocialGroup = getSocialGroup().toLowerCase().hashCode();
		uint32 targetSocialGroup = creatureAgent->getSocialGroup().toLowerCase().hashCode();

		// AI from same social group
		if ((targetSocialGroup != 0) && (targetSocialGroup != STRING_HASHCODE("self")) && (thisSocialGroup == targetSocialGroup))
			return false;

		uint32 thisLairTemplateCRC = getLairTemplateCRC();
		uint32 targetLairTemplateCRC = creatureAgent->getLairTemplateCRC();

		// AI from the same lair type
		if ((targetLairTemplateCRC != 0) && (targetLairTemplateCRC == thisLairTemplateCRC))
			return false;
	}

	// info(true) << "AiAgent::isAttackableBy Creature # "<< creature->getObjectID() << " Check returning true";

	return true;
}

bool AiAgentImplementation::hasLoot(){
	SceneObject* inventory = asAiAgent()->getSlottedObject("inventory");

	if(inventory == nullptr)
		return false;

	return inventory->getContainerObjectsSize() > 0;
}

void AiAgentImplementation::sendDefaultConversationTo(SceneObject* player) {
}

void AiAgentImplementation::selectConversationOption(int option, SceneObject* obj) {
}

bool AiAgentImplementation::isEventMob() const {
	if (getDisplayedName().contains(" (event)"))
		return true;

	return false;
}

void AiAgentImplementation::setCombatState() {
	CreatureObjectImplementation::setCombatState();

	ManagedReference<SceneObject*> home = homeObject.get();

	if (home != nullptr)
		home->notifyObservers(ObserverEventType::AIMESSAGE, asAiAgent(), ObserverEventType::STARTCOMBAT);

	notifyObservers(ObserverEventType::STARTCOMBAT, asAiAgent());

	ManagedReference<SceneObject*> followCopy = getFollowObject().get();
	if (followCopy != nullptr && followCopy->isTangibleObject()) {
		ManagedReference<TangibleObject*> target = cast<TangibleObject*>(followCopy.get());
		ManagedReference<AiAgent*> ai = asAiAgent();

		Core::getTaskManager()->executeTask([=] () {
			Locker locker(ai);
			Locker clocker(target, ai);

			if (target->hasDefender(ai))
				ai->sendReactionChat(followCopy, ReactionManager::ATTACKED);
		}, "SendAttackedChatLambda");
	}
}

bool AiAgentImplementation::hasRangedWeapon() {
	if (primaryWeapon != nullptr && primaryWeapon->isRangedWeapon()) {
		return true;
	}

	if (secondaryWeapon != nullptr && secondaryWeapon->isRangedWeapon()) {
		return true;
	}

	return false;
}

bool AiAgentImplementation::hasMeleeWeapon() {
	if (primaryWeapon != nullptr && (primaryWeapon->isMeleeWeapon() || primaryWeapon->isUnarmedWeapon())) {
		return true;
	}

	if (secondaryWeapon != nullptr && (secondaryWeapon->isMeleeWeapon() || secondaryWeapon->isUnarmedWeapon())) {
		return true;
	}

	return false;
}

bool AiAgentImplementation::getUseRanged() {
	if (isPet()) {
		ManagedReference<PetControlDevice*> pcd = getControlDevice().get().castTo<PetControlDevice*>();

		if (pcd != nullptr && pcd->getUseRanged()) {
			return true;
		}
	}

	return false;
}

bool AiAgentImplementation::hasSpecialAttack(int num) {
	const CreatureAttackMap* attackMap = getAttackMap();
	if (attackMap == nullptr) {
		return false;
	}

	if (num == 0 || (num > attackMap->size())) {
		return false;
	}

	String cmd = attackMap->getCommand(num - 1);

	if (cmd.isEmpty()) {
		return false;
	}

	return true;
}

void AiAgentImplementation::restoreFollowObject() {
	Locker locker(&targetMutex);
	ManagedReference<SceneObject*> obj = followStore.get();
	locker.release();
	if (obj == nullptr) {
		setMovementState(AiAgent::PATHING_HOME);
		return;
	} else if (getCloseObjects() != nullptr && !getCloseObjects()->contains(obj.get())) {
		setMovementState(AiAgent::PATHING_HOME);
		return;
	} else if (obj->isCreatureObject() && obj->asCreatureObject()->isInvisible()) {
		setMovementState(AiAgent::PATHING_HOME);
		return;
	} else {
		setFollowObject(obj);
	}
}
String AiAgentImplementation::getPersonalityStf() {
	if (npcTemplate == nullptr)
		return "";
	else
		return npcTemplate->getPersonalityStf();
}

void AiAgentImplementation::sendReactionChat(SceneObject* object, int type, int state, bool force) {
	if (object == nullptr || (!getCooldownTimerMap()->isPast("reaction_chat") && !force) || getZoneUnsafe() == nullptr || !isNpc() || isDead()) {
		return;
	}

	ReactionManager* reactionManager = getZoneServer()->getReactionManager();

	if (reactionManager != nullptr)
		reactionManager->sendChatReaction(asAiAgent(), object, type, state, force);
}

void AiAgentImplementation::setMaxHAM(int type, int value, bool notifyClient) {
		CreatureObjectImplementation::setMaxHAM(type, value, notifyClient);
		activateRecovery();
	}

float AiAgentImplementation::getEffectiveResist() {
	if (!isSpecialProtection(SharedWeaponObjectTemplate::ACID) && getAcid() > 0)
		return getAcid();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::BLAST) && getBlast() > 0)
		return getBlast();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::COLD) && getCold() > 0)
		return getCold();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::ELECTRICITY) && getElectricity() > 0)
		return getElectricity();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::ENERGY) && getEnergy() > 0)
		return getEnergy();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::HEAT) && getHeat() > 0)
		return getHeat();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::KINETIC) && getKinetic() > 0)
		return getKinetic();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::LIGHTSABER) && getLightSaber() > 0)
		return getLightSaber();
	if (!isSpecialProtection(SharedWeaponObjectTemplate::STUN) && getStun() > 0)
		return getStun();
	return 0;
}

float AiAgentImplementation::getReducedResist(float value) {
	if (value == -1) {
		return value;
	}

	float newValue = value;

	if (isPet()) {
		float shockWounds = getShockWounds();

		if (shockWounds <= 500) {
			return newValue;
		}

		newValue = value - (value * (shockWounds - 500) * 0.001);

		return newValue;
	}

	int totalHAM = 0;
	int i = 0;

	// Get total of max HAM pools
	while (i <= 6) {
		totalHAM += getMaxHAM(i);
		i += 3;
	}

	// Total damage that was not resisted by armor
	int unmitigatedDamage = getUnmitigatedDamage();

	// Damage not prevented by armor resists compared to totalHAM
	float percentUnmitigated = unmitigatedDamage / (float)totalHAM;

#ifdef DEBUG_RESIST_DECAY
	info (true) << " Value of HAM mitigated = " << mitigatedAmount;
#endif

	// Decay resists when mitigated damage is greater than 25% totalHAM
	if (percentUnmitigated > 0.25f) {
		// Reduce resists 2% for every 1% of damage mitigated by armor valued greater than 25% of totalHAM.
		// Reduction Range is from 75% to 50% of totalHAM. totaling a max 50% reduction of resists
		float reduction = (percentUnmitigated - 0.25f) * 2.f;

		// Resists never drop below 50%
		reduction = 1.f - (reduction > 0.50f ? 0.50f : reduction);

		newValue = (value * reduction);

#ifdef DEBUG_RESIST_DECAY
		info(true) << "getReducedResist: totalHAM = " << totalHAM << " Resist Mitigation = " << unmitigatedDamage << " Start value: " << value << " New Value = " << newValue << " Reduction percent = " << reduction;
#endif
	}

	return newValue;
}

void AiAgentImplementation::setPatrolPoints(PatrolPointsVector& pVector) {
	Locker locker(&targetMutex);
	patrolPoints = pVector;
}

AiAgent* AiAgentImplementation::asAiAgent() {
	return _this.getReferenceUnsafeStaticCast();
}

AiAgent* AiAgent::asAiAgent() {
	return this;
}

void AiAgentImplementation::reloadTemplate() {
	clearBuffs(false, false);
	loadTemplateData(npcTemplate);

	if (isMount()) {
		setOptionBit(0x1000);
	}
}

void AiAgentImplementation::setConvoTemplate(const String& templateString) {
	uint32 templateCRC = templateString.hashCode();

	ConversationTemplate* conversationTemplate = CreatureTemplateManager::instance()->getConversationTemplate(templateCRC);

	if (conversationTemplate == nullptr) {
		error("Unable to find conversation template " + templateString);
		return;
	}

	convoTemplateCRC = templateCRC;
}

void AiAgentImplementation::setCreatureBit(uint32 option) {
	if (!(creatureBitmask & option)) {
		creatureBitmask = creatureBitmask | option;
	}
}

void AiAgentImplementation::clearCreatureBit(uint32 option) {
	if (creatureBitmask & option) {
		creatureBitmask = creatureBitmask & ~option;
	}
}

bool AiAgentImplementation::isPet() const {
	return (getControlDevice() != nullptr);
}

void AiAgentImplementation::writeBlackboard(const String& key, const BlackboardData& data) {
	blackboard.drop(key);
	blackboard.put(key, data);
}

String AiAgentImplementation::getErrorContext() {
	auto zone = getZone();

	StringBuffer msg;

	msg << "AiAgentImplementation" << ": " << getObjectID() << "; '" << getDisplayedName() << "';";

	msg << " Health: " << getHAM(CreatureAttribute::HEALTH) << ";";
	msg << " Action: " << getHAM(CreatureAttribute::ACTION) << ";";
	msg << " Mind: " << getHAM(CreatureAttribute::MIND) << ";";
	msg << " posture: " << CreaturePosture::postureToString(posture) << ";";
	msg << " Zone: " << (zone != nullptr ? zone->getZoneName() : "nullptr") << ";";
	msg << " at: " << getWorldPosition().toString();

	return msg.toString();
}

void AiAgentImplementation::handleException(const Exception& ex, const String& context) {
	auto numExceptions = AiMap::instance()->countExceptions.increment();
	auto consoleToo = (numExceptions - 1) % ConfigManager::instance()->getAiAgentConsoleThrottle() == 0;

	StringBuffer msg;

	msg << "Unhandled AiAgent exception from " << context << ": " << ex.getMessage()
		<< "; AiMap::countExceptions=" << numExceptions
		<< "; AiAgentID=" << getObjectID()
		;

	if (consoleToo) {
		auto fileName = getLogFileName();

		Logger::console.error()
			<< msg << (fileName.isEmpty() ? "" : "; logFileName=" + fileName);
	}

	auto trace = ex.getStackTrace();
	error() << msg << endl << trace.toStringData() << endl << *this;
}

void AiAgentImplementation::addCreatureFlag(unsigned int flag) {
	if (!(creatureBitmask & flag))
		creatureBitmask |= flag;
}

void AiAgentImplementation::removeCreatureFlag(unsigned int flag) {
	if (creatureBitmask & flag)
		creatureBitmask &= ~flag;
}
