 /*
 * SharedTangibleObjectTemplate.h
 *
 *  Created on: 30/04/2010
 *      Author: victor
 */

#ifndef SHAREDTANGIBLEOBJECTTEMPLATE_H_
#define SHAREDTANGIBLEOBJECTTEMPLATE_H_

#include <algorithm>

#include "system/lang/String.h"
#include "system/lang/ref/Reference.h"
#include "system/platform.h"
#include "system/util/Vector.h"
#include "system/util/VectorMap.h"
#include "templates/SharedObjectTemplate.h"
#include "templates/crafting/resourceweight/ResourceWeight.h"
#include "templates/params/primitives/BoolParam.h"

class ResourceWeight;
class StructureFootprint;
namespace engine {
namespace lua {
class LuaObject;
}  // namespace lua
namespace util {
class Chunk;
class IffStream;
}  // namespace util
}  // namespace engine

class SharedTangibleObjectTemplate : public SharedObjectTemplate {
protected:
	/*PaletteColorCustomizationVariables paletteColorCustomizationVariables;
	RangedIntCustomizationVariables rangedIntCustomizationVariables;

	SocketDestinations socketDestinations;*/

	Reference<StructureFootprint*> structureFootprint;

	BoolParam targetable;

	uint16 playerUseMask;

	int level;

	Vector<String> certificationsRequired;

	int maxCondition;

	uint32 optionsBitmask;
	uint32 pvpStatusBitmask;

	int useCount;

	bool sliceable;

	bool invisible;

	unsigned int faction;

	int junkDealerNeeded;
	int junkValue;

	VectorMap<String, int> skillMods;

	Vector<short>* numberExperimentalProperties;
	Vector<String>* experimentalProperties;
	Vector<short>* experimentalWeights;
	Vector<String>* experimentalGroupTitles;
	Vector<String>* experimentalSubGroupTitles;
	Vector<float>* experimentalMin;
	Vector<float>* experimentalMax;
	Vector<short>* experimentalPrecision;
	Vector<short>* experimentalCombineType;
	Vector<uint32>* playerRaces;

	Vector<Reference<ResourceWeight* > >* resourceWeights;
	//CustomizationVariableMapping customizationVariableMapping;

public:
	SharedTangibleObjectTemplate();

	~SharedTangibleObjectTemplate();

	void readObject(LuaObject* templateData);
	void readObject(IffStream* iffStream);

	void parseFileData(IffStream* str);

	void parseVariableData(const String& varName, Chunk* data);
	void parseVariableData(const String& varName, LuaObject* data);

	inline int getMaxCondition() const {
		return maxCondition;
	}

	inline uint32 getOptionsBitmask() const {
		return optionsBitmask;
	}

	inline uint32 getPvpStatusBitmask() const {
		return pvpStatusBitmask;
	}

	void setMaxCondition(int maxCondition) {
		this->maxCondition = maxCondition;
	}

	void setOptionsBitmask(uint32 optionsBitmask) {
		this->optionsBitmask = optionsBitmask;
	}

	void setPvpStatusBitmask(uint32 pvpStatusBitmask) {
		this->pvpStatusBitmask = pvpStatusBitmask;
	}

	inline const Vector<String>& getCertificationsRequired() const {
		return certificationsRequired;
	}

	inline int getLevel() const {
		return level;
	}

	inline bool isInvisible() {
		return invisible;
	}

	inline int getUseCount() const {
		return useCount;
	}

    inline uint16 getPlayerUseMask() const {
		return playerUseMask;
	}

    inline unsigned int getFaction() const {
    	return faction;
    }

    inline int getJunkDealerNeeded() const {
    	return junkDealerNeeded;
    }

    inline int getJunkValue() const {
    	return junkValue;
    }

	inline StructureFootprint* getStructureFootprint() const {
		return structureFootprint;
	}

	inline bool getTargetable() const {
		return targetable;
	}

	void setCertificationsRequired(Vector<String> certificationsRequired) {
		this->certificationsRequired = certificationsRequired;
	}

	void setPlayerUseMask(uint16 playerUseMask) {
		this->playerUseMask = playerUseMask;
	}

	void setTargetable(bool targetable) {
		this->targetable = targetable;
	}

	inline bool getSliceable() const {
		return sliceable;
	}

    Vector<short >* getNumberExperimentalProperties() {
        return numberExperimentalProperties;
    }

    Vector<String>* getExperimentalProperties() {
        return experimentalProperties;
    }

    Vector<short >* getExperimentalWeights() {
        return experimentalWeights;
    }

    Vector<String>* getExperimentalGroupTitles() {
        return experimentalGroupTitles;
    }

    Vector<String>* getExperimentalSubGroupTitles() {
        return experimentalSubGroupTitles;
    }

    Vector<float>* getExperimentalMin() {
        return experimentalMin;
    }

    Vector<float>* getExperimentalMax() {
        return experimentalMax;
    }

    Vector<short >* getExperimentalPrecision() {
        return experimentalPrecision;
    }

    Vector<uint32>* getPlayerRaces() {
    	return playerRaces;
    }

    Vector<Reference<ResourceWeight* > >* getResourceWeights() {
    	return resourceWeights;
    }

    VectorMap<String, int>* getSkillMods() {
    	return &skillMods;
    }

    int getSkillMod(const String& mod) {
    	return skillMods.get(mod);
    }

    bool isSharedTangibleObjectTemplate() {
    	return true;
    }
};



#endif /* SHAREDTANGIBLEOBJECTTEMPLATE_H_ */
