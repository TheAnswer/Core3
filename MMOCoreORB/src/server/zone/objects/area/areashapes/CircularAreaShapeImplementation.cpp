/*
 * CircularAreaShapeImplementation.cpp
 *
 *  Created on: 31/10/2012
 *      Author: loshult
 */

#include "CircularAreaShape.h"
#include "RectangularAreaShape.h"
#include "RingAreaShape.h"

bool CircularAreaShapeImplementation::containsPoint(float x, float y) {
	Vector3 position;
	position.set(x, 0, y);

	return (areaCenter.squaredDistanceTo(position) <= radius2);
}

bool CircularAreaShapeImplementation::containsPoint(const Vector3& point) {
	return (areaCenter.squaredDistanceTo(point) <= radius2);
}

Vector3 CircularAreaShapeImplementation::getRandomPosition() {
	float distance = System::random((int)radius);
	float angle = System::random(360) * Math::DEG2RAD;

	Vector3 position;
	position.set(areaCenter.getX() + distance * Math::cos(angle), 0, areaCenter.getY() + distance * Math::sin(angle));

	return position;
}

Vector3 CircularAreaShapeImplementation::getRandomPosition(const Vector3& origin, float minDistance, float maxDistance) {
	float distance = System::random((int)(maxDistance - minDistance)) + minDistance;
	float angle = System::random(360) * Math::DEG2RAD;

	Vector3 position;
	position.set(origin.getX() + distance * Math::cos(angle), 0, origin.getY() + distance * Math::sin(angle));

	return position;
}

bool CircularAreaShapeImplementation::intersectsWith(AreaShape* areaShape) {
	if (areaShape == NULL) {
		return false;
	}

	if (areaShape->isCircularAreaShape()) {
		ManagedReference<CircularAreaShape*> circle = cast<CircularAreaShape*>(areaShape);
		return intersectsWithCircle(circle);
	} else if (areaShape->isRectangularAreaShape()) {
		ManagedReference<RectangularAreaShape*> rectangle = cast<RectangularAreaShape*>(areaShape);
		return rectangle->intersectsWith(_this.get());
	} else if (areaShape->isRingAreaShape()) {
		ManagedReference<RingAreaShape*> ring = cast<RingAreaShape*>(areaShape);
		return ring->intersectsWith(_this.get());
	} else {
		return false;
	}
}

bool CircularAreaShapeImplementation::intersectsWithCircle(CircularAreaShape* circle) {
	float squaredRadiusSum = radius2 + circle->getRadius2();
	float squaredCenterDistance = areaCenter.squaredDistanceTo(circle->getAreaCenter());

	return (squaredCenterDistance <= squaredRadiusSum);
}

float CircularAreaShapeImplementation::getArea() {
	return Math::PI * radius2;
}
