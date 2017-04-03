
#ifndef UNLOADCONTAINERTASK_H_
#define UNLOADCONTAINERTASK_H_

#include "engine/engine.h"
#include "server/zone/objects/scene/SceneObject.h"

namespace server {
 namespace zone {
  namespace objects {
   namespace scene {

class UnloadContainerTask : public Task {
	ManagedWeakReference<SceneObject*> container;

	public:
		UnloadContainerTask(SceneObject* obj) {
			container = obj;
		}

		void run() {
			ManagedReference<SceneObject*> obj = container;

			if (obj == NULL)
				return;

			if (obj->getLastContainerAccess() < 900000) {
				reschedule(1800000); // 30 minutes
				return;
			}

			obj->unloadContainerObjects();
		}
	};

   }
  }
 }
}

#endif /* UNLOADCONTAINERTASK_H_ */
