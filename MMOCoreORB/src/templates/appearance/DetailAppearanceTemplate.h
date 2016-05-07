/*
 * DetailAppearanceTemplate.h
 *
 *  Created on: 10/12/2010
 *      Author: victor
 */

#ifndef DETAILAPPEARANCETEMPLATE_H_
#define DETAILAPPEARANCETEMPLATE_H_

#include "engine/engine.h"
#include "templates/appearance/AppearanceTemplate.h"

class DetailAppearanceTemplate : public AppearanceTemplate {
	AppearanceTemplate* firstMesh;

public:
	virtual uint32 getType() {
		return 'DTAL';
	}
	
	DetailAppearanceTemplate() {
		firstMesh = NULL;
	}

	~DetailAppearanceTemplate() {
	}

	void readObject(IffStream* templateData) {
		parse(templateData);
	}

	void parse(IffStream* iffStream);

	AppearanceTemplate* getFirstMesh() {
		return firstMesh;
	}
	
	virtual bool testCollide(const Sphere& testsphere) const {
		firstMesh->testCollide(testsphere);
	}
	
	/**
	 * Checks for intersection against ray, stops on any intersection
	 * @return intersectionDistance, triangle which it intersects
	 */
	virtual bool intersects(const Ray& ray, float distance, float& intersectionDistance, Triangle*& triangle, bool checkPrimitives = false) {
		firstMesh->intersects(ray, distance, intersectionDistance, triangle, checkPrimitives);
	}
	
	/**
	 * Checks for all intersections
	 */
	virtual int intersects(const Ray& ray, float maxDistance, SortedVector<IntersectionResult>& result) {
		firstMesh->intersects(ray, maxDistance, result);
	}

};

#endif /* DETAILAPPEARANCETEMPLATE_H_ */
