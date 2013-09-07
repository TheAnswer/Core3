/*
 * Boundary.h
 *
 *  Created on: 01/02/2010
 *      Author: victor
 */

#ifndef BOUNDARY_H_
#define BOUNDARY_H_

class ProceduralTerrainAppearance;

class Boundary {
protected:
	int featheringType;
	float featheringAmount;

public:
	virtual ~Boundary() {
		featheringType = 0;
	}

	virtual void executeRule(ProceduralTerrainAppearance* generator) {

	}

	virtual bool containsPoint(float x, float y) {
		return false;
	}

	virtual float getLocalWaterTableHeight() {
		return -16000;
	}

	virtual float process(float x, float y) {
		return 0;
	}

	inline int getFeatheringType() {
		return featheringType;
	}

	virtual bool isEnabled() {
		return false;
	}

	virtual float getMinX() = 0;
	virtual float getMaxX() = 0;
	virtual float getMinY() = 0;
	virtual float getMaxY() = 0;

	virtual void translateBoundary(float x, float y) = 0;
};


#endif /* BOUNDARY_H_ */
