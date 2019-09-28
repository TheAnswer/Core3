/*
 * ContainerObjectsMap.h
 *
 *  Created on: 13/10/2012
 *      Author: victor
 */

#ifndef CONTAINEROBJECTSMAP_H_
#define CONTAINEROBJECTSMAP_H_

#include "engine/engine.h"
#include "system/thread/atomic/AtomicTime.h"

namespace server {
 namespace zone {
  namespace objects {
   namespace scene {
   	   class SceneObject;
   	   class UnloadContainerTask;

   	class ContainerObjectsMap : public Variable {
   		int operationMode;

   		VectorMap<uint64, ManagedReference<SceneObject*> > containerObjects;
   		AtomicReference<VectorMap<uint64, uint64>*> oids;

   		AtomicTime lastAccess;

   		ManagedWeakReference<SceneObject*> container;

   		mutable ReadWriteLock* containerLock;

   		Reference<UnloadContainerTask*> unloadTask;

   	public:
   		enum {NORMAL_LOAD = 0, DELAYED_LOAD };

   	private:
   		void copyData(const ContainerObjectsMap& c);
   		void scheduleContainerUnload();

   	public:
   		ContainerObjectsMap();
   		~ContainerObjectsMap();
   		ContainerObjectsMap(const ContainerObjectsMap& c);

   		ContainerObjectsMap& operator=(const ContainerObjectsMap& c);

   		VectorMap<uint64, ManagedReference<SceneObject*> >* getContainerObjects();

   		bool toBinaryStream(ObjectOutputStream* stream);
   		bool parseFromBinaryStream(ObjectInputStream* stream);

   		void notifyLoadFromDatabase();

   		void loadObjects();
   		void unloadObjects();

   		void removeAll();
   		void removeElementAt(int index);

   		bool contains(uint64 oid) const;
   		int size();

   		ManagedReference<SceneObject*> get(int index);
   		ManagedReference<SceneObject*> get(uint64 oid);

   		void put(uint64 oid, SceneObject* object);
   		void drop(uint64 oid);

   		void setContainer(SceneObject* obj);

   		void setDelayedLoadOperationMode() {
   			operationMode = DELAYED_LOAD;
   		}

   		void setNormalLoadOperationMode() {
   			operationMode = NORMAL_LOAD;
   		}

   		bool hasDelayedLoadOperationMode() const {
   			return operationMode == DELAYED_LOAD;
   		}

   		bool isLoaded(bool readLock = true) const {
   			if (readLock) {
   				ReadLocker locker(containerLock);

   				return operationMode == NORMAL_LOAD || oids == nullptr;
   			} else {
   				return operationMode == NORMAL_LOAD || oids == nullptr;
   			}
   		}

   		const AtomicTime* getLastAccess() const {
   			return &lastAccess;
   		}

   		ManagedWeakReference<SceneObject*> getContainer() const {
   			return container;
   		}

   		void cancelUnloadTask();

		VectorMap<uint64, uint64>* getOids() const {
			return oids.get();
		}

		const VectorMap<uint64, ManagedReference<SceneObject*> >* getContainerObjects() const {
			return &containerObjects;
		}


   	};

	void to_json(nlohmann::json& k, const server::zone::objects::scene::ContainerObjectsMap& map);
   }
  }
 }
}

using namespace server::zone::objects::scene;

#endif /* CONTAINEROBJECTSMAP_H_ */
