/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#ifndef STRUCTUREMANAGER_H_
#define STRUCTUREMANAGER_H_

#include "engine/engine.h"
#include "templates/manager/TemplateManager.h"

namespace server {
namespace zone {
	class ZoneServer;
namespace objects {
namespace scene {
	class SceneObject;
}
namespace creature {
	class CreatureObject;
}
namespace structure {
	class StructureObject;
}
namespace tangible {
	class TangibleObject;
namespace deed {
namespace structure {
	class StructureDeed;
}
}
}
}
}
}

using namespace server::zone;
using namespace server::zone::objects::scene;
using namespace server::zone::objects::creature;
using namespace server::zone::objects::structure;
using namespace server::zone::objects::tangible;
using namespace server::zone::objects::tangible::deed::structure;

class StructureManager : public Singleton<StructureManager>, public Logger, public Object {
	ZoneServer* server;
	TemplateManager* templateManager;

public:
	StructureManager() : Logger("StructureManager") {
		server = NULL;
		templateManager = TemplateManager::instance();

		setGlobalLogging(true);
		setLogging(false);
	}

	void setZoneServer(ZoneServer* zoneServer) {
		server = zoneServer;
	}

	void loadPlayerStructures(const String& zoneName);

	int placeStructureFromDeed(CreatureObject* creature, StructureDeed* deed, float x, float y, int angle);

	/**
	 * Simply creates and places a structure at the provided coordinates.
	 */
	StructureObject* placeStructure(CreatureObject* creature, const String& structureTemplatePath, float x, float y, int angle, int persistenceLevel = 1);

	/**
	 * Destroys the structure after the confirmation of the player.
	 * All items still in the structure will be deleted from the database. If the maintenance requirements are met, it is to be redeeded.
	 * pre: structure locked
	 * post: structure deleted*
	 * @param structure The structure that is being destroyed.
	 */
	int destroyStructure(StructureObject* structureObject);

	/**
	 * Redeeds the structure, returning it to the player in deed form with any surplus maintenance and power attached.
	 * This should occur after a destroy confirm, but before the structure has been completely destroyed.
	 * pre: player locked, building locked
	 * post: player locked, building locked
	 * @param creature The creatureObject who is redeeding the structure.
	 * @param destroy Should the structure be destroyed after the deed is retrieved?
	 */
	int redeedStructure(CreatureObject* creature);

	int declareResidence(CreatureObject* player, StructureObject* structureObject);

	/**
	 * Converts seconds remaining into days, hours, minutes timestamp
	 */
	String getTimeString(uint32 timestamp);

	/**
	 * Returns the closest parking garage to the obj, or NULL if one can't be found within the required radius.
	 * @pre { obj locked, zone unlocked }
	 * @post { obj locked, zone unlocked }
	 * @param obj The object to use as reference for searching for the nearest parking garage.
	 * @return Returns a SceneObject representing the nearest parking garage, or NULL if one was not found in the required range.
	 */
	Reference<SceneObject*> getInRangeParkingGarage(SceneObject* obj, int range = 64);

	/**
	 * Sends a sui list box containing information about the structure.
	 * @param creature The creature receiving the report.
	 * @param structure The structure the report is about.
	 */
	void reportStructureStatus(CreatureObject* creature, StructureObject* structure);

	/**
	 * Sends a Sui prompt to the player asking them to enter a name for the structure.
	 * @param creature The player receiving the prompt.
	 * @param structure The structure that will be named.
	 * @param object Optional terminal to be used for sui
	 */
	void promptNameStructure(CreatureObject* creature, StructureObject* structure, TangibleObject* object);

	/**
	 * Sends a Sui prompt to the player asking if they wish to delete all the items in their house.
	 * @param creature The player receiving the prompt.
	 * @param building The building that will have all items deleted.
	 */
	void promptDeleteAllItems(CreatureObject* creature, StructureObject* structure);

	/**
	 * Sends a Sui prompt to the player asking if they want to move the first item in the house to their feet.
	 * @param creature The creature who the item will be moved to.
	 * @param structure The structure which holds the items.
	 */
	void promptFindLostItems(CreatureObject* creature, StructureObject* structure);

	/**
	 * Moves the first object in the building to the creature's feet.
	 * @param creature The creature to whom's feet the item will be moved.
	 * @param structure The structure containing the item.
	 */
	void moveFirstItemTo(CreatureObject* creature, StructureObject* structure);

	/**
	 * Sends a Sui prompt to the player asking if they want to pay the
	 * required maintenance and fee to un-condemn the structure.
	 * @param creature The creature who owns the structure.
	 * @param structure The condemned structure.
	 */
	void promptPayUncondemnMaintenance(CreatureObject* creature, StructureObject* structure);

	/**
	 * Sends the transfer box prompting for maintenance to be paid.
	 * @param structure The structure being paid maintenance to.
	 * @param creature The creature attempting to pay maintenance.
	 * @param terminal The terminal used in the process. Defaults to null.
	 */
	void promptPayMaintenance(StructureObject* structure, CreatureObject* creature, SceneObject* terminal = NULL);

	/**
	 * Sends the transfer box prompting for maintenance to be withdrawn.
	 * @param structure The structure having maintenance withdrawn from.
	 * @param creature The creature attempting to withdraw maintenance.
	 */
	void promptWithdrawMaintenance(StructureObject* structure, CreatureObject* creature);

	/**
	 * Sends the transfer box prompting for selection of sign
	 * @param structure The structure having a sign added
	 * @param player The player attempting to choose sign
	 */
	void promptSelectSign(StructureObject* structure, CreatureObject* player);

	/**
	 * Attempts to set the requested sign on the structure and notifies the creature.
	 * @param structure The structure to be signed.
	 * @param creature The creature attempting to set sign.
	 * @param signSuiItem suiItem string selected
	 */
	void setSign(StructureObject* structure, CreatureObject* creature, String signSuiItem);

	/**
	 * Attempts to pay the requested amount to the structure, and notifies the creature.
	 * @param structure The structure being paid maintenance.
	 * @param creature The creature attempting to pay maintenance.
	 * @param amount The amount to pay maintenance.
	 */
	void payMaintenance(StructureObject* structure, CreatureObject* creature, int amount);

	/**
	 * Attempts to withdraw the requested amount from the structure, and notifies the creature.
	 * @param structure The structure having maintenance withdrawn from.
	 * @param creature The creature attempting to withdraw maintenance.
	 * @param amount The amount of maintenance to withdraw.
	 */
	void withdrawMaintenance(StructureObject* structure, CreatureObject* creature, int amount);

	int getStructureFootprint(SharedObjectTemplate* objectTemplate, int angle, float& l0, float& w0, float& l1, float& w1);

	bool isInStructureFootprint(StructureObject* structure, float positionX, float positionY, int extraFootprintMargin);

	void promptMaintenanceDroid(StructureObject* structure, CreatureObject* creature);
};

#endif /*STRUCTUREMANAGER_H_*/
