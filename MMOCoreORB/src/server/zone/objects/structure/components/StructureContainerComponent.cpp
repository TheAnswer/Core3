/*
 * StructureContainerComponent.cpp
 *
 *  Created on: 09/02/2012
 *      Author: victor
 */

#include "StructureContainerComponent.h"
#include "server/zone/objects/building/BuildingObject.h"
#include "server/zone/objects/creature/CreatureObject.h"

bool StructureContainerComponent::checkContainerPermission(SceneObject* sceneObject, CreatureObject* creature, uint16 permission) const {
	StructureObject* building = dynamic_cast<StructureObject*>(sceneObject);

	if (building != NULL) {
		if (permission == ContainerPermissions::MOVEVENDOR) {
			if (building->isPublicStructure() && (building->isOnPermissionList("VENDOR", creature) || building->isOnAdminList(creature)))
				return true;
			else {
				if (building->isPrivateStructure())
					creature->sendSystemMessage("@player_structure:vendor_public_only"); //
				else
					creature->sendSystemMessage("@player_structure:drop_npc_vendor_perm"); // You do not have vendor permission in this building

				return false;
			}
		}

		if (building->isOwnerOf(creature) || building->isOnAdminList(creature))
			return true;

		if ((permission == ContainerPermissions::OPEN)  && building->isClientObject())
			return true;

		if (building->isBuildingObject()) {
			BuildingObject* buio = cast<BuildingObject*>(building);

			if (!buio->isAllowedEntry(creature))
				return false;
		}
	}

	return ContainerComponent::checkContainerPermission(sceneObject, creature, permission);
}
