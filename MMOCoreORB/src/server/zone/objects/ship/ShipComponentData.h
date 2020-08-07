/*
 * ShipComponent.h
 *
 *  Created on: 18/10/2013
 *      Author: victor
 */

#ifndef SHIPCOMPONENTDATA_H_
#define SHIPCOMPONENTDATA_H_

#include "engine/engine.h"
#include "templates/datatables/DataTableRow.h"

class ShipComponentData : public Object {
protected:
	String name;
	String objectTemplate;
	String sharedObjectTemplate;
	String componentType;
	String compatibility;
public:
	ShipComponentData() {

	}

	void readObject(DataTableRow* row) {
		row->getCell(0)->getValue(name);
		row->getCell(1)->getValue(objectTemplate);
		row->getCell(2)->getValue(sharedObjectTemplate);
		row->getCell(3)->getValue(componentType);
		row->getCell(4)->getValue(compatibility);
	}

	const String& getCompatibility() const {
		return compatibility;
	}

	const String& getComponentType() const {
		return componentType;
	}

	const String& getName() const {
		return name;
	}

	const String& getObjectTemplate() const {
		return objectTemplate;
	}

	const String& getSharedObjectTemplate() const {
		return sharedObjectTemplate;
	}
};



#endif /* SHIPCOMPONENT_H_ */
