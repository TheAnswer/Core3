/*
 * IntangibleObject.h
 *
 *  Created on: Sep 6, 2009
 *      Author: theanswer
 */

#ifndef INTANGIBLEOBJECT_H_
#define INTANGIBLEOBJECT_H_

#include "engine/engine.h"
#include "../scene/SceneObject.h"

class IntangibleObject : public SceneObject {
public:
	IntangibleObject(SharedObjectTemplate* templateData);
};

#endif /* INTANGIBLEOBJECT_H_ */
