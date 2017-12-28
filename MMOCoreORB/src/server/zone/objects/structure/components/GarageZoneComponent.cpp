/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "GarageZoneComponent.h"

#include <stddef.h>
#include <algorithm>

#include "GarageDataComponent.h"
#include "engine/core/ManagedReference.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/scene/components/DataObjectComponentReference.h"
#include "system/lang/ref/Reference.h"
#include "system/platform.h"

namespace server {
namespace zone {
class QuadTreeEntry;
}  // namespace zone
}  // namespace server

void GarageZoneComponent::notifyPositionUpdate(SceneObject* sceneObject, QuadTreeEntry* entry) const {
	ManagedReference<SceneObject*> obj = cast<SceneObject*>(entry);

	if (obj == NULL || !obj->isPlayerCreature())
		return;

	GarageDataComponent* data = cast<GarageDataComponent*>(sceneObject->getDataObjectComponent()->get());

	if (data == NULL)
		return;

	uint64 objID = obj->getObjectID();

	float deltaX = sceneObject->getPositionX() - obj->getPositionX();
	float deltaY = sceneObject->getPositionY() - obj->getPositionY();
	float rangeSq = deltaX * deltaX + deltaY * deltaY;

	if (rangeSq > 4096) { // 64^2
		if (data->hasNotifiedPlayer(objID))
			data->removeNotifiedPlayer(objID);

	} else {
		ManagedReference<SceneObject*> rootParent = obj->getRootParent();

		if (rootParent == NULL || !rootParent->isVehicleObject())
			return;

		if (!data->hasNotifiedPlayer(objID)) {
			data->addNotifiedPlayer(objID);

			CreatureObject* player = obj->asCreatureObject();
			player->sendSystemMessage("@pet/pet_menu:garage_proximity"); //You have entered into the proximity of a vehicle garage.
		}
	}
}

void GarageZoneComponent::notifyDissapear(SceneObject* sceneObject, QuadTreeEntry* entry) const {
	StructureZoneComponent::notifyDissapear(sceneObject, entry);

	ManagedReference<SceneObject*> obj = cast<SceneObject*>(entry);

	if (obj == NULL || !obj->isPlayerCreature())
		return;

	GarageDataComponent* data = cast<GarageDataComponent*>(sceneObject->getDataObjectComponent()->get());

	if (data == NULL)
		return;

	if (data->hasNotifiedPlayer(obj->getObjectID()))
		data->removeNotifiedPlayer(obj->getObjectID());
}
