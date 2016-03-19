/*
 * DroidDatapadContainerComponent.h
 */

#ifndef DROIDDATAPADCONTAINERCOMPONENT_H_
#define DROIDDATAPADCONTAINERCOMPONENT_H_

#include "server/zone/objects/scene/components/ContainerComponent.h"

class DroidDatapadContainerComponent : public ContainerComponent {
public:
	bool checkContainerPermission(SceneObject* sceneObject, CreatureObject* creature, uint16 permission) const;

	int canAddObject(SceneObject* sceneObject, SceneObject* object, int containmentType, String& errorDescription) const;

	inline bool isDataPadContainer() const {
		return true;
	}
};

#endif /* DROIDDATAPADCONTAINERCOMPONENT_H_ */
