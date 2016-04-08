/*
 * FilterHeight.h
 *
 *  Created on: 01/02/2010
 *      Author: victor
 */

#ifndef FILTERHEIGHT_H_
#define FILTERHEIGHT_H_


#include "../ProceduralRule.h"
#include "FilterProceduralRule.h"

class FilterHeight : public ProceduralRule<'FHGT'>, public FilterProceduralRule {
	float minHeight;
	float maxHeight;

public:
	FilterHeight() : FilterProceduralRule(0), minHeight(0), maxHeight(0) {
		filterType = HEIGHTTYPE;
	}

	void parseFromIffStream(engine::util::IffStream* iffStream) {
		uint32 version = iffStream->getNextFormType();

		iffStream->openForm(version);

		switch (version) {
		case '0002':
			parseFromIffStream(iffStream, Version<'0002'>());
			break;
		default:
			System::out << "unknown FilterHeight version 0x" << hex << version << endl;
			break;
		}

		iffStream->closeForm(version);
	}

	void parseFromIffStream(engine::util::IffStream* iffStream, Version<'0002'>) {
		informationHeader.readObject(iffStream);

		iffStream->openChunk('DATA');

		minHeight = iffStream->getFloat();
		maxHeight = iffStream->getFloat();
		featheringType = iffStream->getInt();
		featheringAmount = iffStream->getFloat();

		iffStream->closeChunk('DATA');
	}

	float process(float x, float y, float transformValue, float& baseValue, TerrainGenerator* terrainGenerator, FilterRectangle* rect) {
		float result;

		if ((baseValue > minHeight) && (baseValue < maxHeight)) {
			float v9 = (maxHeight - minHeight) * featheringAmount * 0.5;

			if (minHeight + v9 <= baseValue) {
				if (maxHeight - v9 >= baseValue) {
					result = 1.0;
				} else
					result = (maxHeight - baseValue) / v9;

			} else {
				result = (baseValue - minHeight) / v9;
			}
		} else
			result = 0;

		return result;
	}

	bool isEnabled() {
		return informationHeader.isEnabled();
	}
};

#endif /* FILTERHEIGHT_H_ */
