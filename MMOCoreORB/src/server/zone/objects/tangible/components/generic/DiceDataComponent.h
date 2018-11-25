/*
 * DataObjectComponent.h
 *
 *  Created on: 18/03/2012
 *      Author: victor
 */

#ifndef DICEDATACOMPONENT_H_
#define DICEDATACOMPONENT_H_

#include "server/zone/objects/scene/components/DataObjectComponent.h"

class DiceDataComponent : public DataObjectComponent {
protected:
	byte sides;
public:
	DiceDataComponent() {
		sides = 0;
		addSerializableVariables();
	}

	virtual ~DiceDataComponent() {

	}

	void writeJSON(nlohmann::json& j) const {
		DataObjectComponent::writeJSON(j);

		SERIALIZE_JSON_MEMBER(sides);
	}

	void setSides(byte count) {
		sides = count;
	}

	byte getSides() const {
		return sides;
	}

	bool isDiceData() {
		return true;
	}

private:
	void addSerializableVariables() {
		addSerializableVariable("sides", &sides);
	}
};


#endif /* DICEDATACOMPONENT_H_ */
