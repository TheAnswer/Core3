/*
 * SharedStructureObjectTemplate.cpp
 *
 *  Created on: May 22, 2010
 *      Author: crush
 */


#include "SharedStructureObjectTemplate.h"


void SharedStructureObjectTemplate::readObject(LuaObject* templateData) {
	SharedTangibleObjectTemplate::readObject(templateData);

	lotSize = templateData->getByteField("lotSize");

	baseMaintenanceRate = templateData->getIntField("baseMaintenanceRate");

	basePowerRate = templateData->getIntField("basePowerRate");

	LuaObject allowzones = templateData->getObjectField("allowedZones");
	allowedZones.removeAll(); //Make sure it's empty...

	for (int i = 1; i <= allowzones.getTableSize(); ++i) {
		allowedZones.put(allowzones.getStringAt(i));
	}

	allowzones.pop();

	cityRankRequired = templateData->getByteField("cityRankRequired");

	constructionMarkerTemplate = templateData->getStringField("constructionMarker");

	abilityRequired = templateData->getStringField("abilityRequired");

	uniqueStructure = templateData->getBooleanField("uniqueStructure");

	cityMaintenanceBase = templateData->getIntField("cityMaintenanceBase");

	cityMaintenanceRate = templateData->getIntField("cityMaintenanceRate");
}
