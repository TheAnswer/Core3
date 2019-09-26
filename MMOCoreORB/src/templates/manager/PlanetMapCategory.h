/*
 * PlanetMapCategory.h
 *
 *  Created on: Apr 24, 2011
 *      Author: polonel
 */

#ifndef PLANETMAPCATEGORY_H_
#define PLANETMAPCATEGORY_H_

#include "templates/datatables/DataTableRow.h"

class PlanetMapCategory : public Object {
	String name;
	int nameCRC;
	int index;
	bool category;
	bool subCategory;
	bool activatable;
	String faction;
	bool factionVisibleOnly;

public:
	PlanetMapCategory() {
		nameCRC = 0;
		index = 0;
		category = false;
		subCategory = false;
		activatable = false;
		factionVisibleOnly = false;
	}

	PlanetMapCategory(const PlanetMapCategory& pmc) : Object() {
		name = pmc.name;
		nameCRC = pmc.nameCRC;
		index =  pmc.index;
		category = pmc.category;
		subCategory = pmc.subCategory;
		activatable = pmc.activatable;
		faction = pmc.faction;
		factionVisibleOnly = pmc.factionVisibleOnly;
	}

	PlanetMapCategory& operator= (const PlanetMapCategory& pmc) {
		if (this == &pmc)
			return *this;

		name = pmc.name;
		nameCRC = pmc.nameCRC;
		index =  pmc.index;
		category = pmc.category;
		subCategory = pmc.subCategory;
		activatable = pmc.activatable;
		faction = pmc.faction;
		factionVisibleOnly = pmc.factionVisibleOnly;

		return *this;
	}

	inline ~PlanetMapCategory() {

	}

	int compareTo(const PlanetMapCategory& pmc) const {
		return name.compareTo(pmc.name);
	}

	void parseFromDataTableRow(DataTableRow* row) {
		try {
			row->getValue(0, name);
			nameCRC = name.hashCode();
			row->getValue(1, index);
			row->getValue(2, category);
			row->getValue(3, subCategory);
			row->getValue(4, activatable);
			row->getValue(5, faction);
			row->getValue(6, factionVisibleOnly);

		} catch (Exception& ex) {
			System::out << "Error parsing PlanetMapCategory: " + ex.getMessage() << endl;
		}

	}

	inline const String& getName() const {
		return name;
	}

	inline int getCrc() const {
		return nameCRC;
	}

	inline int getIndex() const {
		return index;
	}

	inline bool isCategory() const {
		return category;
	}

	inline bool isSubCategory() const {
		return subCategory;
	}

	inline bool canBeActive() const {
		return activatable;
	}

	inline const String& getFaction() const {
		return faction;
	}

	inline bool isFactionVisibleOnly() const {
		return factionVisibleOnly;
	}

};

#endif /* PLANETMAPCATEGORY_H_ */
