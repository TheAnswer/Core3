/*
 * TurretZoneComponent.h
 *
 *  Created on: Nov 20, 2012
 *      Author: root
 */

#ifndef TURRETZONECOMPONENT_H_
#define TURRETZONECOMPONENT_H_

#include "engine/engine.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/scene/components/SceneObjectComponent.h"
#include "server/zone/objects/structure/components/StructureZoneComponent.h"

#include "engine/util/u3d/QuadTreeEntry.h"
#include "server/zone/objects/tangible/TangibleObject.h"

class TurretZoneComponent : public ZoneComponent {

public:
	void notifyInsertToZone(SceneObject* sceneObject, Zone* zne);

	void notifyInsert(SceneObject* sceneObject, QuadTreeEntry* entry);

	void notifyDissapear(SceneObject* sceneObject, QuadTreeEntry* entry);
};

#endif /* TURRETZONECOMPONENT_H_ */
