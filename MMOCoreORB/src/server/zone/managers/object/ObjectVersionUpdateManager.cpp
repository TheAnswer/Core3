/*
 * ObjectVersionUpdateManager.cpp
 *
 *  Created on: Oct 28, 2010
 *      Author: oru
 */

#include "ObjectVersionUpdateManager.h"
#include "ObjectManager.h"
#include "server/zone/objects/player/Races.h"
#include "server/db/ServerDatabase.h"
#include "system/util/SortedVector.h"
#include "server/zone/objects/structure/StructurePermissionList.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "server/zone/managers/templates/TemplateManager.h"
#include "server/zone/managers/templates/TemplateCRCMap.h"
#include "server/zone/templates/SharedTangibleObjectTemplate.h"
#include "server/zone/templates/LootItemTemplate.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"
#include "server/zone/templates/TemplateReference.h"
#include "server/zone/templates/tangible/LootSchematicTemplate.h"
#include "server/zone/templates/tangible/SharedFactoryObjectTemplate.h"
#include "server/zone/managers/loot/LootGroupMap.h"
#include "server/zone/managers/loot/LootManager.h"

#define INITIAL_DATABASE_VERSION 0

ObjectVersionUpdateManager::ObjectVersionUpdateManager() : Logger("ObjectVersionUpdateManager") {
	luaInstance = new Lua();
	luaInstance->init();
}


int ObjectVersionUpdateManager::run() {
	int version = ObjectDatabaseManager::instance()->getCurrentVersion();

	if (version == INITIAL_DATABASE_VERSION ){
		updateResidences();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 1);
		return 0;
	} else if ( version ==  INITIAL_DATABASE_VERSION + 1) {
		updateCityTreasury();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 2);
		return 0;
	} else if (version == INITIAL_DATABASE_VERSION + 2) {
		updateWeaponsDots();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 3);
		return 0;
	} else if (version == INITIAL_DATABASE_VERSION + 3) {
		updateStructurePermissionLists();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 4);
		return 0;
	} else if (version == INITIAL_DATABASE_VERSION + 4) {
		updateCityTreasuryToDouble();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 5);
		return 0;
	} else if (version == INITIAL_DATABASE_VERSION + 5) {
		updateTangibleObjectUseCounts();
		ObjectDatabaseManager::instance()->updateCurrentVersion(INITIAL_DATABASE_VERSION + 6);
		return 0;
	} else {

		info("database on latest version : " + String::valueOf(version), true);
		//verifyResidenceVariables();
		return 1;
	}
/*

	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true);

	ObjectData baseIterator iterator(database);

	ObjectInputStream objectData(2000);
	uint64 objectID = 0;

	info("starting database update", true);

	try {

		while (iterator.getNextKeyAndValue(objectID, &objectData)) {
			bool municipalZone = false;

			try {
				if (!Serializable::getVariable<bool>("ActiveArea.municipalZone", &municipalZone, &objectData)) {
					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}

			//info("updating object", true);

			if (municipalZone) {
				database->deleteData(objectID);

				info("deleting municipal zone 0x" + String::hexvalueOf((int64)objectID), true);
			}

			objectData.clear();
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	ObjectDatabaseManager::instance()->updateCurrentVersion(version);

	info("finished database update", true);

	return 0;
	*/
}



ObjectOutputStream* ObjectVersionUpdateManager::addVariable(String variableName, ObjectInputStream* object, Stream* newVariableData){
	object->reset();

	uint16 oldVariableCount = object->readShort();
	//info("first variable count is " + String::valueOf(oldVariableCount),true);
	ObjectOutputStream* newData = new ObjectOutputStream(object->size());
	object->copy(newData, 0);

	newData->writeShort(0, oldVariableCount + 1);
	newData->setOffset(newData->size());


	uint32 _nameHashCode = variableName.hashCode();
	TypeInfo<uint32>::toBinaryStream(&_nameHashCode, newData);
	newData->writeInt(newVariableData->size());
	newData->writeStream(newVariableData);

	return newData;


}

int ObjectVersionUpdateManager::getVariableDataOffset(const uint32& variableHashCode, ObjectInputStream* stream) {
	uint16 dataSize = stream->readShort();
	int offset;

	for (int i = 0; i < dataSize; ++i) {
		uint32 namehashCode = stream->readInt();

		uint32 varSize = stream->readInt();

		offset = stream->getOffset();

		if (namehashCode != variableHashCode)
			stream->shiftOffset(varSize);
		else {
			stream->reset();
			return offset;
		}
	}

	offset = -1;

	stream->reset();

	return offset;
}

ObjectOutputStream* ObjectVersionUpdateManager::changeVariableData(const uint32& variableHashCode, ObjectInputStream* object, Stream* newVariableData) {
	int offset = getVariableDataOffset(variableHashCode, object);

	if (offset == -1)
		return NULL;

	ObjectOutputStream* newData = new ObjectOutputStream(object->size());
	object->copy(newData);

	object->reset();
	newData->reset();
	object->shiftOffset(offset - 4);
	uint32 dataSize = object->readInt();

	newData->shiftOffset(offset); //we go data length

	if (dataSize > 0)
		newData->removeRange(offset, offset + dataSize);

	newData->writeInt(offset - 4, newVariableData->size());
	newData->insertStream(newVariableData, newVariableData->size(), offset);

	object->reset();
	//newData->reset();

	return newData;
}

void ObjectVersionUpdateManager::updateWeaponsDots() {
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true);

	ObjectDatabaseIterator iterator(database);
	ObjectInputStream objectData(2000);
	uint64 objectID = 0;

	info("update database weapon dots", true);

	int count = 0;
	uint32 classNameHashCode = STRING_HASHCODE("_className");
	try {

		while (iterator.getNextKeyAndValue(objectID, &objectData)) {
			String className;
			int oldType = 0;

			try {
				if (!Serializable::getVariable<String>(classNameHashCode, &className, &objectData) ||
						!Serializable::getVariable<int>(STRING_HASHCODE("WeaponObject.dotType"), &oldType, &objectData)) {

					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}

			if (className == "WeaponObject") {
				Vector<int> dots;

				ObjectOutputStream newDotsValue;
				TypeInfo<Vector<int> >::toBinaryStream(&dots, &newDotsValue);

				//	info("we found a Player " + String::valueOf(objectID) + " with residence " + String::valueOf(residence),true);
				ObjectOutputStream* newData = changeVariableData(STRING_HASHCODE("WeaponObject.dotType"), &objectData, &newDotsValue);
				newData->reset();

				ObjectInputStream* inputStream = new ObjectInputStream(newData->getBuffer(), newData->size());
				delete newData;

				ObjectOutputStream* newNextData = changeVariableData(STRING_HASHCODE("WeaponObject.dotAttribute"), inputStream, &newDotsValue);
				newNextData->reset();

				delete inputStream;
				inputStream = new ObjectInputStream(newNextData->getBuffer(), newNextData->size());
				delete newNextData;

				newData = changeVariableData(STRING_HASHCODE("WeaponObject.dotStrength"), inputStream, &newDotsValue);
				newData->reset();

				delete inputStream;
				inputStream = new ObjectInputStream(newData->getBuffer(), newData->size());
				delete newData;

				newNextData = changeVariableData(STRING_HASHCODE("WeaponObject.dotDuration"), inputStream, &newDotsValue);
				newNextData->reset();

				delete inputStream;
				inputStream = new ObjectInputStream(newNextData->getBuffer(), newNextData->size());
				delete newNextData;

				newData = changeVariableData(STRING_HASHCODE("WeaponObject.dotPotency"), inputStream, &newDotsValue);
				newData->reset();

				delete inputStream;
				inputStream = new ObjectInputStream(newData->getBuffer(), newData->size());
				delete newData;

				newNextData = changeVariableData(STRING_HASHCODE("WeaponObject.dotUses"), inputStream, &newDotsValue);
				newNextData->reset();

				delete inputStream;

				database->putData(objectID, newNextData, NULL);
			}

			objectData.clear();
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	info("done updating databse weapon dots\n", true);
}

void ObjectVersionUpdateManager::updateTangibleObjectUseCounts() {
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true);

	ObjectDatabaseIterator iterator(database);
	ObjectInputStream objectData(2000);
	uint64 objectID = 0;
	TemplateCRCMap& templateMap = TemplateManager::instance()->getTemplateCRCMap();
	HashTableIterator<uint32, TemplateReference<SharedObjectTemplate*> > iter = templateMap.iterator();

	SortedVector<uint32> templateKeys;
	templateKeys.setInsertPlan(SortedVector<uint32>::NO_DUPLICATE);


	info("Building tangible object template list for migration", true);
	while(iter.hasNext()) {
		TemplateReference<SharedObjectTemplate*> tmpl;
		uint32 key = 0;
		iter.getNextKeyAndValue(key, tmpl);

		if(tmpl->isSharedTangibleObjectTemplate()) {
			SharedTangibleObjectTemplate *tanotmp = tmpl.castTo<SharedTangibleObjectTemplate*>();
			LootSchematicTemplate *lootSchem = tmpl.castTo<LootSchematicTemplate*>();

			// Add all templates that still have a use Count > 1
			// Also add all loot schematic templates as well as factory crates
			if((tanotmp != NULL && tanotmp->getUseCount() > 0) ||
			   (lootSchem != NULL && lootSchem->getTargetUseCount() > 0) ||
			    tmpl->getGameObjectType() == SceneObjectType::FACTORYCRATE) {
				templateKeys.put(key);
				info("Adding Tangible Template: " + tanotmp->getTemplateFileName(), false);
			}
		}
	}

	Reference<LootGroupMap*> lootMap =  LootGroupMap::instance();
	lootMap->initialize();

	HashTable<String, Reference<LootItemTemplate*> > &itemTemplates = lootMap->itemTemplates;
	HashTableIterator<String, Reference<LootItemTemplate*> > lootIter = itemTemplates.iterator();

	while(lootIter.hasNext()) {

		Reference<LootItemTemplate*> lootTmpl = lootIter.next();

		CraftingValues craftingValues = lootTmpl->getCraftingValuesCopy();

		for (int i = 0; i < craftingValues.getExperimentalPropertySubtitleSize(); ++i) {

			String subtitle = craftingValues.getExperimentalPropertySubtitle(i);

			// if a loot template contains any of the following subtitles then it will be generated in stacks
			// add the base template to the exclusion list
			if (subtitle == "useCount" ||
				subtitle == "quantity" ||
				subtitle == "charges" ||
				subtitle == "uses" ||
				subtitle == "charge") {

				uint32 hash = lootTmpl->getDirectObjectTemplate().hashCode();

				SharedObjectTemplate *tmpl = templateMap.get(hash);

				if(tmpl == NULL) {
					info("Null shared object template from loot template" + lootTmpl->getDirectObjectTemplate(), false);
					continue;
				}

				templateKeys.put(hash);
				info("Adding Loot Template: " + tmpl->getTemplateFileName(), false);
			}
		}
	}


	info("Migrating tangible objects based on :" + String::valueOf(templateKeys.size()) + " templates.", true);
	int count = 0;
	uint32 classNameHashCode = STRING_HASHCODE("_className");
	try {

		while (iterator.getNextKeyAndValue(objectID, &objectData)) {

			int useCount = 0;
			uint objCRC = 0;


			try {
				if (!Serializable::getVariable<int>(STRING_HASHCODE("TangibleObject.useCount"), &useCount, &objectData) ||
						!Serializable::getVariable<uint>(STRING_HASHCODE("SceneObject.serverObjectCRC"), &objCRC, &objectData)) {
					objectData.clear();
					continue;
				}
			} catch (Exception& e) {
				info(e.getMessage(), true);
				objectData.clear();
				continue;
			}

			if(useCount == 1) { // We're moving 1->0, we don't need to do anything else
				if(templateKeys.contains(objCRC)) {
					info("Skipping: [" + templateMap.get(objCRC)->getTemplateFileName() + "] useCount: " + String::valueOf(useCount));
					continue;
				} else {
					// change useCount to 0
					info("Found tangible object to migrate: " + templateMap.get(objCRC)->getTemplateFileName());
					ObjectOutputStream data;
					data.writeInt(0);
					ObjectOutputStream* test = changeVariableData(STRING_HASHCODE("TangibleObject.useCount"), &objectData, &data);
					test->reset();
					database->putData(objectID, test, NULL);

				}
			}

			objectData.clear();
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

	info("done tangible object use coutns\n", true);
}


void ObjectVersionUpdateManager::updateStructurePermissionLists() {
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("playerstructures", true);

	ObjectDatabaseIterator iterator(database);

	ObjectInputStream objectData(2000);
	uint64 objectID = 0;
	int count = 0;

	info("Setting owner on structure permission lists",true);

	try {

		while (iterator.getNextKeyAndValue(objectID, &objectData)) {

			String className;

			try {
				if (!Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {
					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}

			if (className == "BuildingObject" || className == "InstallationObject" || className == "GarageInstallation" || className == "ShuttleInstallation") {
				uint64 ownerID = 0;
				String ownerName;
				count ++;
				printf("\r\tUpdating structure owners [%d] / [?]\t", count);

				if( Serializable::getVariable<uint64>(STRING_HASHCODE("StructureObject.ownerObjectID"), &ownerID, &objectData)) {
					StructurePermissionList permissionList;

					if ( Serializable::getVariable<StructurePermissionList>(STRING_HASHCODE("StructureObject.structurePermissionList"), &permissionList, &objectData)){
						ObjectOutputStream newOutputStream;
						permissionList.setOwner(ownerID);
						permissionList.toBinaryStream(&newOutputStream);

						ObjectOutputStream* test = changeVariableData(STRING_HASHCODE("StructureObject.structurePermissionList"), &objectData, &newOutputStream);
						test->reset();
						database->putData(objectID, test, NULL);
					} else {
						info("ERROR unable to get structurePermissionList for structure " + String::valueOf(objectID),true);
					}

				} else {
					info("ERROR unable to get ownerObjectID for structure " + String::valueOf(objectID),true);
				}
			}
			objectData.clear();
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}
	info("Done updating owner on structure permission lists\n",true);

}

void ObjectVersionUpdateManager::updateResidences(){
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("sceneobjects", true);

	ObjectDatabaseIterator iterator(database);

	ObjectInputStream objectData(2000);
	uint64 objectID = 0;
	info("---------------Setting residences---------------------",true);
	info("Setting residence values for all active player residences ", true);

	int count = 0;
	try {

		while (iterator.getNextKeyAndValue(objectID, &objectData)) {

			String className;
			uint64 residence = 0;

			try {
				if (!Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData) ||
						!Serializable::getVariable<uint64>(STRING_HASHCODE("PlayerObject.declaredResidence"), &residence, &objectData)) {

					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}

			if (className == "PlayerObject") {
			//	info("we found a Player " + String::valueOf(objectID) + " with residence " + String::valueOf(residence),true);
				SortedVector<unsigned long long> structureList;
				uint64 residence = 0;
				count++;
				printf("\r\tUpdating player residence [%d] / [?]\t", count);
				if( Serializable::getVariable< SortedVector<unsigned long long> >(STRING_HASHCODE("PlayerObject.ownedStructures"), &structureList, &objectData) &&
						Serializable::getVariable<uint64>(STRING_HASHCODE("PlayerObject.declaredResidence"), &residence, &objectData)){

					for(int i = 0; i < structureList.size(); i++){
						bool isRes = (structureList.get(i) == residence);
						//info("Owned structure " + String::valueOf(structureList.get(i)) + " residence =  " + String::valueOf(isRes),true);
						setResidence(structureList.get(i),isRes);
					}
				} else {
					info("ERROR unable to get ownedStructures for player " + String::valueOf(objectID),true);
				}

			}

			objectData.clear();
		}
	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}
	info("\n",true);

}

void ObjectVersionUpdateManager::setResidence(uint64 buildingID, bool isResidence){
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("playerstructures", true);
	ObjectInputStream objectData(2000);

	bool res  = isResidence;
	String className;

	ObjectOutputStream newResidenceValue;
	TypeInfo<bool>::toBinaryStream(&res, &newResidenceValue );

	if(!database->getData(buildingID,&objectData)){


		if ( Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)){

			if(className == "BuildingObject"){
				if ( !Serializable::getVariable<bool>(STRING_HASHCODE("BuildingObject.isOwnerResidence"), &isResidence, &objectData)){
					//info("setResidence() adding the variable",true);
					ObjectOutputStream* newVariableData = addVariable("BuildingObject.isOwnerResidence", &objectData, &newResidenceValue);
					database->putData(buildingID, newVariableData, NULL);

				} else {
					//info("setResidence() has variable and value = " + String::valueOf(isResidence) +  " ... changing it",true);
					ObjectOutputStream* test = changeVariableData(STRING_HASHCODE("BuildingObject.isOwnerResidence"), &objectData, &newResidenceValue);
					test->reset();
					database->putData(buildingID, test, NULL);
				}

			}
		} else {

			info("ERROR couldn't get object " + String::valueOf(buildingID),true);

		}


	}
}

void ObjectVersionUpdateManager::updateCityTreasury(){

	info("---------------MOdifying City Treasury---------------------",true);
	info("Converting treasury to float for all cities ", true);
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("cityregions", true);
	ObjectInputStream objectData(2000);

	String className;
	ObjectDatabaseIterator iterator(database);

	uint64 objectID = 0;
	int count = 0;
	try {
		while (iterator.getNextKeyAndValue(objectID, &objectData)) {

			String className;
			try {

				if (!Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {

					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}
			int funds;
			float floatFunds = 5;

			if (className == "CityRegion") {
				count++;
				printf("\r\tUpdating city treasury [%d] / [?]\t", count);
				if ( Serializable::getVariable<int>(STRING_HASHCODE("CityRegion.cityTreasury"), &funds, &objectData)){

					floatFunds = funds;
					ObjectOutputStream newFunds;
					TypeInfo<float>::toBinaryStream(&floatFunds, &newFunds );
					ObjectOutputStream* test = changeVariableData(STRING_HASHCODE("CityRegion.cityTreasury"), &objectData, &newFunds);
					test->reset();
					database->putData(objectID, test, NULL);


				} else {
					info("Error... city " + String::valueOf(objectID) + " doesn't have cityTreasury variable",true);
				}
			}

			objectData.clear();


		}

	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

}

void ObjectVersionUpdateManager::updateCityTreasuryToDouble(){

	info("---------------MOdifying City Treasury---------------------",true);
	info("Converting treasury to double for all cities ", true);
	ObjectDatabase* database = ObjectDatabaseManager::instance()->loadObjectDatabase("cityregions", true);
	ObjectInputStream objectData(2000);

	String className;
	ObjectDatabaseIterator iterator(database);

	uint64 objectID = 0;
	int count = 0;
	try {
		while (iterator.getNextKeyAndValue(objectID, &objectData)) {

			String className;
			try {

				if (!Serializable::getVariable<String>(STRING_HASHCODE("_className"), &className, &objectData)) {

					objectData.clear();
					continue;
				}
			} catch (...) {
				objectData.clear();
				continue;
			}
			float funds;
			double floatFunds = 5;

			if (className == "CityRegion") {
				count++;
				printf("\r\tUpdating city treasury [%d] / [?]\t", count);
				if ( Serializable::getVariable<float>(STRING_HASHCODE("CityRegion.cityTreasury"), &funds, &objectData)){

					floatFunds = funds;
					ObjectOutputStream newFunds;
					TypeInfo<double>::toBinaryStream(&floatFunds, &newFunds );
					ObjectOutputStream* test = changeVariableData(STRING_HASHCODE("CityRegion.cityTreasury"), &objectData, &newFunds);
					test->reset();
					database->putData(objectID, test, NULL);


				} else {
					info("Error... city " + String::valueOf(objectID) + " doesn't have cityTreasury variable",true);
				}
			}

			objectData.clear();


		}

	} catch (Exception& e) {
		error(e.getMessage());
		e.printStackTrace();
	}

}
