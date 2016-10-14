/*
 * SharedObjectTemplate.cpp
 *
 *  Created on: 30/04/2010
 *      Author: victor
 */

#include "SharedObjectTemplate.h"
#include "templates/manager/TemplateManager.h"
#include "templates/slots/SlotDescriptor.h"
#include "templates/slots/ArrangementDescriptor.h"

SharedObjectTemplate::SharedObjectTemplate() {
	portalLayout = NULL;
	appearanceTemplate = NULL;
	loadedPortalLayout = false, loadedAppearanceTemplate = false;

	updatesNavMesh = true;
	snapToTerrain = false;
	containerType = 0;
	containerVolumeLimit = 0;

	totalCellNumber = 0;
	
	collisionMaterialFlags = 0;
	collisionMaterialPassFlags = 0;
	collisionMaterialBlockFlags = 0;
	collisionActionFlags = 0;
	collisionActionPassFlags = 0;
	collisionActionBlockFlags = 0;
	gameObjectType = 0;
	clientGameObjectType = 0;
	sendToClient = false;
	scaleThresholdBeforeExtentTest = 0;
	clearFloraRadius = 0;
	surfaceType = 0;
	noBuildRadius = 0;
	onlyVisibleInTools = 0;
	locationReservationRadius = 0;

	templateType = 0;

	autoRegisterWithPlanetMap = false;

	inheritPermissionsFromParent = false;

	noTrade = false;
}

void SharedObjectTemplate::parseVariableData(const String& varName, LuaObject* templateData) {
	TemplateManager* templateManager = TemplateManager::instance();
	lua_State* state = templateData->getLuaState();

	if (varName == "objectName") {
		objectName = Lua::getStringParameter(state);
	} else if (varName == "detailedDescription") {
		detailedDescription = Lua::getStringParameter(state);
	} else if (varName == "lookAtText") {
		lookAtText = Lua::getStringParameter(state);
	} else if (varName == "customName") {
		customName = Lua::getStringParameter(state);
	} else if (varName == "snapToTerrain") {
		snapToTerrain = Lua::getByteParameter(state);
	} else if (varName == "containerType") {
		containerType = Lua::getIntParameter(state);
	} else if (varName == "containerVolumeLimit") {
		containerVolumeLimit = Lua::getIntParameter(state);
	} else if (varName == "tintPallete") {
		tintPallete = Lua::getStringParameter(state);
	} else if (varName == "slotDescriptorFilename") {
		String slotDescriptorFilename = Lua::getStringParameter(state);

		slotDescriptors = templateManager->getSlotDescriptor(slotDescriptorFilename);
	} else if (varName == "arrangementDescriptorFilename") {
		String arrangementDescriptorFilename = Lua::getStringParameter(state);

		arrangementDescriptors = templateManager->getArrangementDescriptor(arrangementDescriptorFilename);
	} else if (varName == "appearanceFilename") {
		appearanceFilename = Lua::getStringParameter(state);
		appearanceTemplate = NULL;
	} else if (varName == "portalLayoutFilename") {
		portalLayoutFilename = Lua::getStringParameter(state);
		portalLayout = NULL;
	} else if (varName == "clientDataFile") {
		clientDataFile = Lua::getStringParameter(state);
	} else if (varName == "collisionMaterialFlags") {
		collisionMaterialFlags = Lua::getIntParameter(state);
	} else if (varName == "collisionMaterialPassFlags") {
		collisionMaterialPassFlags = Lua::getIntParameter(state);
	} else if (varName == "collisionMaterialBlockFlags") {
		collisionMaterialBlockFlags = Lua::getIntParameter(state);
	} else if (varName == "collisionActionFlags") {
		collisionActionFlags = Lua::getIntParameter(state);
	} else if (varName == "collisionActionPassFlags") {
		collisionActionPassFlags = Lua::getIntParameter(state);
	} else if (varName == "collisionActionBlockFlags") {
		collisionActionBlockFlags = Lua::getIntParameter(state);
	} else if (varName == "scale") {
		LuaObject scaleObject(state);// = templateData->getObjectField("scale");

		if (scaleObject.isValidTable()) {
			/*scale.removeAll(2, 0);

			for (int i = 1; i <= scaleObject.getTableSize(); ++i) {
				scale.add(scaleObject.getFloatAt(i));
			}*/

			if (scaleObject.getTableSize() > 1) {
				scale.setMin(scaleObject.getFloatAt(1));
				scale.setMax(scaleObject.getFloatAt(2));
			} else {
				scale = scaleObject.getFloatAt(1);
			}

			scaleObject.pop();
		} else {
			scale = Lua::getFloatParameter(state);
		}
	} else if (varName == "gameObjectType") {
		gameObjectType = Lua::getIntParameter(state);
	} else if (varName == "sendToClient") {
		sendToClient = Lua::getByteParameter(state);
	} else if (varName == "scaleThresholdBeforeExtentTest") {
		scaleThresholdBeforeExtentTest = Lua::getFloatParameter(state);
	} else if (varName == "clearFloraRadius") {
		clearFloraRadius = Lua::getFloatParameter(state);
	} else if (varName == "surfaceType") {
		surfaceType = Lua::getIntParameter(state);
	} else if (varName == "noBuildRadius") {
		noBuildRadius = Lua::getFloatParameter(state);
	} else if (varName == "onlyVisibleInTools") {
		onlyVisibleInTools = Lua::getByteParameter(state);
	} else if (varName == "locationReservationRadius") {
		locationReservationRadius = Lua::getFloatParameter(state);
	} else if (varName == "templateType") {
		templateType = Lua::getIntParameter(state);
	} else if (varName == "planetMapCategory") {
		planetMapCategory = templateManager->getPlanetMapCategoryByName(Lua::getStringParameter(state));
	} else if (varName == "planetMapSubCategory") {
		planetMapSubCategory = templateManager->getPlanetMapCategoryByName(Lua::getStringParameter(state));
	} else if (varName == "autoRegisterWithPlanetMap") {
		autoRegisterWithPlanetMap = (bool) Lua::getByteParameter(state);
	} else if (varName == "childObjects") {
		LuaObject luaItemList(state);// = templateData->getObjectField("childObjects");

		int size = luaItemList.getTableSize();

		lua_State* L = luaItemList.getLuaState();

		for (int i = 0; i < size; ++i) {
			lua_rawgeti(L, -1, i + 1);
			LuaObject a(L);

			ChildObject object;
			object.parseFromLua(&a);

			childObjects.add(object);

			a.pop();
		}

		luaItemList.pop();
	} else if (varName == "zoneComponent") {
		zoneComponent = Lua::getStringParameter(state);
	} else if (varName == "objectMenuComponent") {
		objectMenuComponent = Lua::getStringParameter(state);
	} else if (varName == "attributeListComponent") {
		attributeListComponent = Lua::getStringParameter(state);
	} else if (varName == "containerComponent") {
		containerComponent = Lua::getStringParameter(state);
	} else if (varName == "totalCellNumber") {
		totalCellNumber = Lua::getIntParameter(state);
	} else if (varName == "dataObjectComponent") {
		dataObjectComponent = Lua::getStringParameter(state);
	} else if (varName == "clientGameObjectType") {
		clientGameObjectType = Lua::getIntParameter(state);
	} else if (varName == "inheritPermissionsFromParent") {
		inheritPermissionsFromParent = Lua::getBooleanParameter(state);
	} else if (varName == "noTrade") {
		noTrade = (bool) Lua::getByteParameter(state);
	} else if (varName == "groupPermissions") {
		groupPermissions.removeAll();

		LuaObject obj(state);

		if (obj.isValidTable()) {
			for (int i = 1; i <= obj.getTableSize(); +i) {
				LuaObject group = obj.getObjectAt(i);

				groupPermissions.put(group.getStringAt(1).hashCode(), group.getIntAt(2));

				group.pop();
			}
		}

		obj.pop();
	} else if (varName == "enableNavMeshUpdates") {
		updatesNavMesh = Lua::getBooleanParameter(state);
	} else {
		//Logger::console.error("unknown variable " + varName);
		templateData->pop();
	}

}

void SharedObjectTemplate::parseVariableData(const String& varName, Chunk* data) {
	TemplateManager* templateManager = TemplateManager::instance();

	if (varName == "objectName") {
		objectName.parse(data);
	} else if (varName == "detailedDescription") {
		detailedDescription.parse(data);
	} else if (varName == "lookAtText") {
		lookAtText.parse(data);
	} else if (varName == "snapToTerrain") {
		snapToTerrain.parse(data);
	} else if (varName == "containerType") {
		containerType.parse(data);
	} else if (varName == "containerVolumeLimit") {
		containerVolumeLimit.parse(data);
	} else if (varName == "tintPallete") {
		tintPallete.parse(data);
	} else if (varName == "slotDescriptorFilename") {
		StringParam slotDescriptorFilename;
		if (slotDescriptorFilename.parse(data))
			slotDescriptors = templateManager->getSlotDescriptor(slotDescriptorFilename.get());
	} else if (varName == "arrangementDescriptorFilename") {
		StringParam arrangementDescriptorFilename;
		
		if (arrangementDescriptorFilename.parse(data))
			arrangementDescriptors = templateManager->getArrangementDescriptor(arrangementDescriptorFilename.get());
	} else if (varName == "appearanceFilename") {
		appearanceFilename.parse(data);
	} else if (varName == "portalLayoutFilename") {
		portalLayoutFilename.parse(data);
	} else if (varName == "clientDataFile") {
		clientDataFile.parse(data);
	} else if (varName == "collisionMaterialFlags") {
		collisionMaterialFlags.parse(data);
	} else if (varName == "collisionMaterialPassFlags") {
		collisionMaterialPassFlags.parse(data);
	} else if (varName == "collisionMaterialBlockFlags") {
		collisionMaterialBlockFlags.parse(data);
	} else if (varName == "collisionActionFlags") {
		collisionActionFlags.parse(data);
	} else if (varName == "collisionActionPassFlags") {
		collisionActionPassFlags.parse(data);
	} else if (varName == "collisionActionBlockFlags") {
		collisionActionBlockFlags.parse(data);
	} else if (varName == "scale") {
		scale.parse(data);
	} else if (varName == "gameObjectType") {
		if (gameObjectType.parse(data))
			clientGameObjectType = gameObjectType.get();
	} else if (varName == "sendToClient") {
		sendToClient.parse(data);
	} else if (varName == "scaleThresholdBeforeExtentTest") {
		scaleThresholdBeforeExtentTest.parse(data);
	} else if (varName == "clearFloraRadius") {
		clearFloraRadius.parse(data);
	} else if (varName == "surfaceType") {
		surfaceType.parse(data);
	} else if (varName == "noBuildRadius") {
		noBuildRadius.parse(data);
	} else if (varName == "onlyVisibleInTools") {
		onlyVisibleInTools.parse(data);
	} else if (varName == "locationReservationRadius") {
		locationReservationRadius.parse(data);
	}
}

void SharedObjectTemplate::parseFileData(IffStream* iffStream) {
	iffStream->openChunk('PCNT');

	int variableCount = iffStream->getInt();

	iffStream->closeChunk('PCNT');

	for (int i = 0; i < variableCount; ++i) {
	//while (iffStream->getRemainingSubChunksNumber() > 0) {
		Chunk* chunk = iffStream->openChunk('XXXX');

		if (chunk == NULL)
			continue;

		String varName;
		iffStream->getString(varName);

		//std::cout << "parsing wtf shit:[" << varName.toStdString() << "]\n";
		parseVariableData(varName, chunk);

		iffStream->closeChunk();
	}
}

void SharedObjectTemplate::loadDerv(IffStream* stream) {
	stream->openForm('DERV');

	Chunk* chunk = stream->openChunk('XXXX');

	String file;
	chunk->readString(file);

	//mainWindow->printToConsole(file);

	if (loadedDerivedFiles.contains(file)) {
		stream->closeChunk();

		stream->closeForm('DERV');

		return;
	}

	loadedDerivedFiles.put(file);

	IffStream* dervStream = TemplateManager::instance()->openIffFile(file);

	if (dervStream != NULL) {
		readObject(dervStream);

		delete dervStream;
	} else {
		Logger::console.warning("could not open derv: " + file);
	}

	// now server lua

	String serverTemplate = file.replaceAll("shared_", "");

	LuaObject* luaObject = TemplateManager::instance()->getLuaObject(serverTemplate);

	if (luaObject != NULL) {
		//Logger::console.info("loading derv from " + serverTemplate, true);

		readObject(luaObject);

		luaObject->pop();

		delete luaObject;
	} else {
		Logger::console.warning("could not open lua derv: " + serverTemplate);
	}

	stream->closeChunk();

	stream->closeForm('DERV');
}

String SharedObjectTemplate::getType(int type) {
	//char* chars = (char*)&type;
	int reversed = htonl(type);
	char chars[5];
	memcpy(chars, &reversed, 4);
	chars[4] = 0;

	return String(chars);
}

void SharedObjectTemplate::readObject(IffStream* iffStream) {
	uint32 nextType = iffStream->getNextFormType();

	if (nextType != 'SHOT') {
		Logger::console.warning("expecting SHOT got " + getType(nextType) + " in file: " + iffStream->getFileName());

		iffStream->openForm(nextType);
		iffStream->closeForm(nextType);
		return;
	}

	iffStream->openForm('SHOT');

	uint32 derv = iffStream->getNextFormType();

	if (derv == 'DERV') {
		loadDerv(iffStream);

		derv = iffStream->getNextFormType();
	}

	iffStream->openForm(derv);

	try {
		parseFileData(iffStream);
	} catch (Exception& e) {
		String msg;
		msg += "exception caught parsing file data ->";
		msg += e.getMessage();

		Logger::console.error(msg);
	}

	iffStream->closeForm(derv);

	iffStream->closeForm('SHOT');

}

void SharedObjectTemplate::readObject(LuaObject* templateData) {
	lua_State* L = templateData->getLuaState();

	if (!templateData->isValidTable())
		return;

	//int tableSize = templateData->getTableSize();

	int i = 0;

	//int tablePos = lua_gettop(L);

	//lua_pushvalue(L, -1); //push table again
	
	lua_pushnil(L);  
	while (lua_next(L, -2) != 0) {
		// 'key' is at index -2 and 'value' at index -1 
		//printf("%s - %s\n",
		//		lua_tostring(L, -2), lua_typename(L, lua_type(L, -1)));

		int type = lua_type(L, -2);

		if (type == LUA_TSTRING) {
			size_t len = 0;
			const char* varName = lua_tolstring(L, -2, &len);

			parseVariableData(varName, templateData);
		} else
			lua_pop(L, 1);
		

		++i;
	}

	clientTemplateFileName = templateData->getStringField("clientTemplateFileName");
	
	return;
}

PortalLayout* SharedObjectTemplate::getPortalLayout() {
	if (portalLayout != NULL)
		return portalLayout;
	else if (!loadedPortalLayout) {
		loadedPortalLayout = true;

		if (portalLayoutFilename.get().length() > 1) {
			try {
				portalLayout = TemplateManager::instance()->getPortalLayout(portalLayoutFilename);
			} catch(Exception& e) {
				e.printStackTrace();
			}
		}
	}

	return portalLayout;
}

AppearanceTemplate* SharedObjectTemplate::getAppearanceTemplate() {
	if (appearanceTemplate != NULL)
		return appearanceTemplate;
	else if (!loadedAppearanceTemplate) {
		loadedAppearanceTemplate = true;

		if (appearanceFilename.get().length() > 1) {
			try {
				appearanceTemplate = TemplateManager::instance()->getAppearanceTemplate(appearanceFilename);
			} catch(Exception& e) {
				e.printStackTrace();
			}
		}
	}

	return appearanceTemplate;
}

bool SharedObjectTemplate::isDerivedFrom(const String& iffPath, bool includeSelf) {

	if(includeSelf && iffPath == fullTemplateString) {
		return true;
	}

	if(loadedDerivedFiles.contains(iffPath)) {
		return true;
	}

	return false;
}
