/*
 * ShuttleInstallationZoneComponent.h
 *
 *  Created on: Apr 7, 2012
 *      Author: TragD
 */

#ifndef SHUTTLEINSTALLATIONZONECOMPONENT_H_
#define SHUTTLEINSTALLATIONZONECOMPONENT_H_

#include "engine/engine.h"

#include "server/zone/objects/scene/components/SceneObjectComponent.h"
#include "server/zone/objects/structure/components/StructureZoneComponent.h"


namespace server {
	namespace zone {
		namespace objects {
			namespace scene {
				class SceneObject;
			}
		}
		class Zone;
	}
}

using namespace server::zone::objects::scene;
using namespace server::zone;

#include "engine/util/u3d/QuadTreeEntry.h"

class ShuttleInstallationZoneComponent : public StructureZoneComponent {

public:
	void notifyInsertToZone(SceneObject* sceneObject, Zone* zone);
	void notifyRemoveFromZone(SceneObject* sceneObject);
	void destroyObjectFromWorld(SceneObject* sceneObject, bool sendSelfDestroy);
};

#endif /* SHUTTLEINSTALLATIONZONECOMPONENT_H_ */
