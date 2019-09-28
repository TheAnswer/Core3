#include "ScavengerChestContainerComponent.h"
#include "server/zone/objects/tangible/eventperk/ScavengerChest.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/objects/tangible/components/EventPerkDataComponent.h"

bool ScavengerChestContainerComponent::checkContainerPermission(SceneObject* container, CreatureObject* creature, uint16 permission) const {
	if(!container->isEventPerkItem())
		return false;

	ManagedReference<ScavengerChest*> chest = cast<ScavengerChest*>(container);

	if (chest == nullptr)
		return false;

	Locker guard(chest);

	EventPerkDataComponent* gameData = cast<EventPerkDataComponent*>(chest->getDataObjectComponent()->get());

	if (gameData == nullptr)
		return false;

	EventPerkDeed* deed = gameData->getDeed();

	if (deed == nullptr)
		return false;

	ManagedReference<CreatureObject*> owner = deed->getOwner().get();

	if (permission == ContainerPermissions::MOVEIN) {
		if (creature == owner) {
			return true;
		} else {
			creature->sendSystemMessage("@event_perk:chest_can_not_add");
			return false;
		}
	} else if (permission == ContainerPermissions::MOVEOUT) {
		if (creature == owner) {
			return true;
		} else if (chest->isOnLootedList(creature->getObjectID())) {
			creature->sendSystemMessage("@event_perk:chest_only_one_item");
			return false;
		}

		return true;
	} else if (permission == ContainerPermissions::OPEN) {
		return true;
	}

	return false;
}

int ScavengerChestContainerComponent::canAddObject(SceneObject* sceneObject, SceneObject* object, int containmentType, String& errorDescription) const {

	if (object->isNoTrade()) {
		errorDescription = "@container_error_message:container28"; //You cannot put this item into this container.
		return TransferErrorCode::INVALIDTYPE;
	}

	return ContainerComponent::canAddObject(sceneObject, object, containmentType, errorDescription);
}

int ScavengerChestContainerComponent::notifyObjectRemoved(SceneObject* container, SceneObject*, SceneObject* destination) const {
	if (destination == nullptr)
		return 0;

	ManagedReference<SceneObject*> rootParent = destination->getParent().get();

	if (rootParent != nullptr && rootParent->isCreatureObject()) {
		CreatureObject* creature = cast<CreatureObject*>(rootParent.get());

		if(!container->isEventPerkItem())
			return 0;

		ManagedReference<ScavengerChest*> chest = cast<ScavengerChest*>(container);

		if (chest == nullptr)
			return 0;

		EventPerkDataComponent* gameData = cast<EventPerkDataComponent*>(chest->getDataObjectComponent()->get());

		if (gameData == nullptr)
			return 0;

		EventPerkDeed* deed = gameData->getDeed();

		if (deed == nullptr)
			return 0;

		ManagedReference<CreatureObject*> owner = deed->getOwner().get();

		Locker guard(chest);

		if (creature != nullptr && creature != owner)
			chest->addtoLootedList(creature->getObjectID());
	}

	return 0;
}


