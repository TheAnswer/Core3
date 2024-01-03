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
#include "server/zone/TreeEntry.h"

class TurretZoneComponent : public GroundZoneComponent {

public:
	void notifyInsertToZone(SceneObject* sceneObject, Zone* zne) const;

	void notifyInsert(SceneObject* sceneObject, TreeEntry* entry) const;

	void notifyDissapear(SceneObject* sceneObject, TreeEntry* entry) const;
};

#endif /* TURRETZONECOMPONENT_H_ */
