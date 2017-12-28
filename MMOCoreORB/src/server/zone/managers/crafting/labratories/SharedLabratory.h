/*
 * SharedLabratory.h
 *
 *  Created on: Aug 5, 2013
 *      Author: swgemu
 */

#ifndef SHAREDLABRATORY_H_
#define SHAREDLABRATORY_H_

#include "engine/core/ManagedReference.h"
#include "engine/log/Logger.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"
#include "server/zone/objects/manufactureschematic/craftingvalues/CraftingValues.h"
#include "server/zone/objects/tangible/TangibleObject.h"
#include "system/lang/Object.h"

namespace server {
namespace zone {
namespace objects {
namespace creature {
class CreatureObject;
}  // namespace creature
namespace draftschematic {
class DraftSchematic;
}  // namespace draftschematic
namespace manufactureschematic {
class ManufactureSchematic;
namespace craftingvalues {
class CraftingValues;
}  // namespace craftingvalues
}  // namespace manufactureschematic
namespace tangible {
class TangibleObject;
}  // namespace tangible
}  // namespace objects
}  // namespace zone
}  // namespace server

namespace server {
namespace zone {
namespace managers {
namespace crafting {
namespace labratories {

class SharedLabratory :  public Object, public Logger{
public:
	SharedLabratory();
	virtual ~SharedLabratory();
	// setup the initial combine values
	virtual void setInitialCraftingValues(TangibleObject* prototype, ManufactureSchematic* manufactureSchematic, int assemblySuccess) = 0;
	// allow each lab to handle the experimation results. You can callback to the crafting manage to determine changes if you wish
	virtual void experimentRow(CraftingValues* craftingValues,int rowEffected, int pointsAttempted, float failure, int experimentationResult) = 0;
	virtual void initialize(ZoneServer* server);
	virtual float getWeightedValue(ManufactureSchematic* manufactureSchematic, int type);
	virtual int getCreationCount(ManufactureSchematic* manufactureSchematic) = 0;
	virtual int calculateAssemblySuccess(CreatureObject* player,DraftSchematic* draftSchematic, float effectiveness);
protected:
	ManagedReference<ZoneServer*> zoneServer;
	float calculateExperimentationValueModifier(int experimentationResult, int pointsAttempted);
	float calculateAssemblyValueModifier(int assemblyResult);
	float getAssemblyPercentage(float value);
};

}
}
}
}
}
using namespace server::zone::managers::crafting::labratories;

#endif /* SHAREDLABRATORY_H_ */
