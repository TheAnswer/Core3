/*
 * ObjectManager.h
 *
 *  Created on: Sep 5, 2009
 *      Author: theanswer
 */

#ifndef OBJECTMANAGER_H_
#define OBJECTMANAGER_H_

#include "engine/engine.h"
#include "engine/log/Logger.h"

#include "system/lang/UnicodeString.h"
#include "system/platform.h"
#include "system/thread/Mutex.h"

class ObjectMap;
class SceneObject;
class Zone;
namespace engine {
namespace lua {
class Lua;
class LuaObject;
}  // namespace lua
namespace util {
template <typename CtorSignature, typename UniqueIdType> class ObjectFactory;
}  // namespace util
}  // namespace engine

class ObjectManager : public Mutex, public Logger {
	ObjectMap* objectMap;

	static ObjectFactory<SceneObject* (LuaObject*), uint32> objectFactory;
	static Lua* luaInstance;
	static Mutex luaMutex;

	Zone* zone;

	void registerObjectTypes();

public:
	ObjectManager();
	virtual ~ObjectManager();

	SceneObject* createObject(uint32 objectCRC, uint64 objectID);
	void destroyObject(uint64 objectID);

	SceneObject* getObject(uint64 objectID);

	SceneObject* getObject(const UnicodeString& customName);

	inline void setZone(Zone* zn) {
		zone = zn;
	}

	uint32 getObjectMapSize();

	// LUA templates
	void registerFunctions();
	static int includeFile(lua_State* L);
	static int crcString(lua_State* L);
	static int addTemplateCRC(lua_State* L);

};


#endif /* OBJECTMANAGER_H_ */
