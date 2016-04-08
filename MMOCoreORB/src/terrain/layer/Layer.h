/*
 * Layer.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef LAYER_H_
#define LAYER_H_

#include "../TemplateVariable.h"
#include "InformationHeader.h"

/*class Boundary;
class AffectorProceduralRule;
class FilterProceduralRule;*/

#include "boundaries/Boundary.h"
#include "affectors/AffectorProceduralRule.h"
#include "filters/FilterProceduralRule.h"

class Layer : public TemplateVariable<'LAYR'> {
	InformationHeader infoHeader;

	Vector<Layer*> children;
	//Vector<TerrainRule*> rules;

	Vector<Boundary*> boundaries;
	Vector<AffectorProceduralRule*> affectors;
	Vector<FilterProceduralRule*> filters;

	//Vector<AffectorProceduralRule*> heightAffectors;
	//Vector<AffectorProceduralRule*> environmentAffectors;

	Layer* parent;

	int boundariesFlag;
	int filterFlag;

public:
	Layer(Layer* par = NULL) {
		parent = par;
		boundariesFlag = 0;
		filterFlag = 0;
	}

	~Layer();

	void parseFromIffStream(engine::util::IffStream* iffStream);
	void parseFromIffStream(engine::util::IffStream* iffStream, Version<'0003'>);

	IffTemplateVariable* parseAffector(IffStream* iffStream);
	IffTemplateVariable* parseBoundary(IffStream* iffStream);
	IffTemplateVariable* parseFilter(IffStream* iffStream);

	Vector<Layer*>* getChildren() {
		return &children;
	}

	Vector<Boundary*>* getBoundaries() {
		return &boundaries;
	}

	Vector<AffectorProceduralRule*>* getAffectors() {
		return &affectors;
	}

	/*Vector<AffectorProceduralRule*>* getHeightAffectors() {
		return &heightAffectors;
	}*/

	Vector<FilterProceduralRule*>* getFilters() {
		return &filters;
	}

	inline Layer* getParent() {
		return parent;
	}

	inline bool invertBoundaries() {
		return boundariesFlag != 0;
	}

	inline bool invertFilters() {
		return filterFlag != 0;
	}

	inline bool isEnabled() {
		return infoHeader.isEnabled();
	}

	inline String& getDescription() {
		return infoHeader.getDescription();
	}
};


#endif /* LAYER_H_ */
