/*
 * DataObjectComponent.h
 *
 *  Created on: 18/03/2012
 *      Author: victor
 */

#ifndef DATAOBJECTCOMPONENT_H_
#define DATAOBJECTCOMPONENT_H_

#include "SceneObjectComponent.h"

#include "engine/util/json_utils.h"

#define SERIALIZE_JSON_MEMBER(x) j[#x] = x;

namespace server {
namespace zone {
namespace objects {
namespace scene {

class SceneObject;

} // namespace scene
} // namespace objects
} // namespace zone
} // namespace server

using namespace server::zone::objects::scene;

class DataObjectComponent : public SceneObjectComponent, public Serializable {
protected:
	ManagedWeakReference<SceneObject*> parent;

public:

	DataObjectComponent();

	~DataObjectComponent();

	friend void to_json(nlohmann::json& j, const DataObjectComponent& comp);

	virtual void writeJSON(nlohmann::json& j) const {
		j["parent"] = parent;
	}

	virtual void initializeTransientMembers();

	virtual void notifyObjectDestroyingFromDatabase();

	void setParent(SceneObject* object);

	SceneObject* getParent();

	virtual bool isDiceData() {
		return false;
	}

	virtual bool isFireworkShowData() {
		return false;
	}

	virtual bool isVendorData() {
		return false;
	}

	virtual bool isAuctionTerminalData() {
		return false;
	}

	virtual bool isCreatureHabitatData() {
		return false;
	}

	virtual bool isBuildingData(){
		return false;
	}

	virtual bool isDestructibleBuildingData(){
		return false;
	}

	virtual bool isTurretData(){
		return false;
	}

	virtual bool isDetectorData(){
		return false;
	}

	virtual bool isGCWBaseData(){
		return false;
	}

	virtual bool isSecurityTerminalData(){
		return false;
	}

	virtual bool isMinefieldData(){
		return false;
	}

	virtual bool isTurretControlTerminalData(){
		return false;
	}

	virtual bool isDecoration(){
		return false;
	}

	virtual bool isCoaMessageData(){
		return false;
	}

	virtual bool isEventPerkData() {
		return false;
	}

	virtual bool isFlagGameData() {
		return false;
	}

	virtual bool isDataStorageUnitData() {
		return false;
	}

	virtual bool isDroidModuleData() {
		return false;
	}

	virtual bool isGamblingTerminalData() {
		return false;
	}

};

#endif /* DATAOBJECTCOMPONENT_H_ */
