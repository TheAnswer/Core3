/*
 * InRangeObjectsVector.h
 *
 *  Created on: Aug 24, 2015
 *      Author: TheAnswer
 */

#ifndef SRC_SERVER_ZONE_INRANGEOBJECTSVECTOR_H_
#define SRC_SERVER_ZONE_INRANGEOBJECTSVECTOR_H_

#include "engine/engine.h"

#include "server/zone/QuadTreeEntry.h"

typedef SortedVector<server::zone::QuadTreeEntry*> InRangeObjectsVector;

#endif /* SRC_SERVER_ZONE_INRANGEOBJECTSVECTOR_H_ */
