#ifndef SCAVENGERCHESTCONTAINERCOMPONENT_H_
#define SCAVENGERCHESTCONTAINERCOMPONENT_H_
#include "server/zone/objects/scene/components/ContainerComponent.h"

class ScavengerChestContainerComponent: public ContainerComponent {
public:
	bool checkContainerPermission(SceneObject* container, CreatureObject* creature, uint16 permission);
	int notifyObjectRemoved(SceneObject* container, SceneObject* item, SceneObject* destination);
};

#endif /* SCAVENGERCHESTCONTAINERCOMPONENT_H_ */
