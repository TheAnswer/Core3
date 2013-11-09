/*
 * DroidDeedTemplate.h
 *
 *  Created on: October 23, 2013
 *      Author: Klivian
 */

#ifndef DROIDDEEDTEMPLATE_H_
#define DROIDDEEDTEMPLATE_H_

#include "DeedTemplate.h"

class DroidDeedTemplate : public DeedTemplate {

	String controlDeviceObjectTemplate;
	String mobileTemplate;

public:
	DroidDeedTemplate() {

	}

	~DroidDeedTemplate() {

	}

	void readObject(LuaObject* templateData) {
		DeedTemplate::readObject(templateData);
		controlDeviceObjectTemplate = templateData->getStringField("controlDeviceObjectTemplate");
		mobileTemplate = templateData->getStringField("mobileTemplate");
    }

	String getControlDeviceObjectTemplate()
	{
		return controlDeviceObjectTemplate;
	}

	String getMobileTemplate()
	{
		return mobileTemplate;
	}
};


#endif /* DROIDDEEDTEMPLATE_H_ */
