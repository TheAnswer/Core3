/*
 * VendorCreatureTemplate.h
 *
 *  Created on: Mar 22, 2011
 *      Author: polonel
 */

#ifndef VENDORCREATURETEMPLATE_H_
#define VENDORCREATURETEMPLATE_H_

#include "server/zone/templates/tangible/SharedCreatureObjectTemplate.h"

class VendorCreatureTemplate : public SharedCreatureObjectTemplate {
	Vector<String> outfits;
	Vector<String> hairFile;
	Vector<String> customizationStringNames;
	Vector<Vector<int> > customizationValues;

public:
	VendorCreatureTemplate() {

	}

	~VendorCreatureTemplate() {

	}

	void readObject(LuaObject* templateData) {
		SharedCreatureObjectTemplate::readObject(templateData);

		LuaObject hairTemplate = templateData->getObjectField("hair");

		hairFile.removeAll();
		for (int i = 1; i <= hairTemplate.getTableSize(); ++i) {
			hairFile.add(hairTemplate.getStringAt(i));
		}

		hairTemplate.pop();

		LuaObject clothesTemplate = templateData->getObjectField("clothing");

		outfits.removeAll();
		for (int i = 1; i <= clothesTemplate.getTableSize(); ++i) {
			outfits.add(clothesTemplate.getStringAt(i));
		}

		clothesTemplate.pop();

		LuaObject customizationStringNamesList = templateData->getObjectField("customizationStringNames");

		for (int i = 1; i <= customizationStringNamesList.getTableSize(); ++i) {
			customizationStringNames.add(customizationStringNamesList.getStringAt(i));
		}

		customizationStringNamesList.pop();

		LuaObject custValues = templateData->getObjectField("customizationValues");

		for (int i = 1; i <= custValues.getTableSize(); ++i) {
			lua_rawgeti(templateData->getLuaState(), -1, i);

			LuaObject values(templateData->getLuaState());

			Vector<int> valuesVector;

			for (int j = 1; j <= values.getTableSize(); ++j) {
				valuesVector.add(values.getIntAt(j));
			}

			customizationValues.add(valuesVector);

			values.pop();
		}

		custValues.pop();

    }

	inline String getHairFile(int idx) {
		if (idx < 0 || idx >= hairFile.size())
			return "";

		return hairFile.get(idx);
	}

	inline String getOutfitName(int idx) {
		if (idx < 0 || idx >= outfits.size())
			return "";

		return outfits.get(idx);
	}

	inline int getCustomizationStringNamesSize() {
		return customizationStringNames.size();
	}

	inline String getCustomizationStringName(int idx) {
		if (idx < 0 || idx >= customizationStringNames.size())
			return "";

		return customizationStringNames.get(idx);
	}

	inline int getCustomizationValuesSize(){
		return customizationValues.size();
	}

	inline Vector<int> getCustomizationValues( int idx ) {
		return customizationValues.get(idx);
	}

	inline int getOutfitsSize() {
		return outfits.size();
	}

	inline int getHairSize() {
		return hairFile.size();
	}

	bool isVendorCreatureTemplate() {
		return true;
	}

};

#endif /* VENDORCREATURETEMPLATE_H_ */
