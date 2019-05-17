/*
 * RectangularAreaShapeImplementation.cpp
 *
 *  Created on: 31/10/2012
 *      Author: loshult
 */

#include "server/zone/objects/area/areashapes/RectangularAreaShape.h"
#include "server/zone/objects/area/areashapes/RingAreaShape.h"
#include "engine/util/u3d/Segment.h"

bool RectangularAreaShapeImplementation::containsPoint(float x, float y) {
	return (x >= blX) && (x <= urX) && (y >= blY) && (y <= urY);
}

bool RectangularAreaShapeImplementation::containsPoint(const Vector3& point) {
	return containsPoint(point.getX(), point.getY());
}

Vector3 RectangularAreaShapeImplementation::getRandomPosition() {
	float width = getWidth();
	float height = getHeight();
	int x = System::random(width);
	int y = System::random(height);
	Vector3 position;

	position.set(blX + x, 0, blY + y);

	return position;
}

Vector3 RectangularAreaShapeImplementation::getRandomPosition(const Vector3& origin, float minDistance, float maxDistance) {
	bool found = false;
	Vector3 position;
	int retries = 5;

	while (!found && retries-- > 0) {
		int distance = System::random((int)(maxDistance - minDistance)) + minDistance;
		int angle = System::random(360) * Math::DEG2RAD;
		position.set(origin.getX() + distance * Math::cos(angle), 0, origin.getY() + distance * Math::sin(angle));

		found = containsPoint(position);
	}

	if (!found)
		return getRandomPosition();

	return position;
}

bool RectangularAreaShapeImplementation::intersectsWith(AreaShape* areaShape) {
	if (areaShape->isRingAreaShape()) {
		ManagedReference<RingAreaShape*> ring = cast<RingAreaShape*>(areaShape);
		Vector3 center = ring->getAreaCenter();

		if (ring->getOuterRadius2() < center.squaredDistanceTo(getClosestPoint(center))) // wholly outside the ring
			return false;
		else if (ring->getInnerRadius2() > center.squaredDistanceTo(getFarthestPoint(center))) // wholly inside the ring's inner circle
			return false;
		else
			return true;
	} else
		return areaShape->containsPoint(getClosestPoint(areaShape->getAreaCenter()));
}

Vector3 RectangularAreaShapeImplementation::getClosestPoint(const Vector3& position) {
	// Calculate corners.
	Vector3 topLeft, topRight, bottomLeft, bottomRight;
	topLeft.set(blX, 0, urY);
	topRight.set(urX, 0, urY);
	bottomLeft.set(blX, 0, blY);
	bottomRight.set(urX, 0, blY);

	// Find closest point on each side.
	Segment topSegment(topLeft, topRight);
	Segment rightSegment(topRight, bottomRight);
	Segment bottomSegment(bottomRight, bottomLeft);
	Segment leftSegment(bottomLeft, topLeft);

	Vector3 top, right, bottom, left;
	top = topSegment.getClosestPointTo(position);
	right = rightSegment.getClosestPointTo(position);
	bottom = bottomSegment.getClosestPointTo(position);
	left = leftSegment.getClosestPointTo(position);

	// Find the closes of the four side points.
	Vector3 point = top;
	if (point.distanceTo(position) > right.distanceTo(position)) {
		point = right;
	}
	if (point.distanceTo(position) > bottom.distanceTo(position)) {
		point = bottom;
	}
	if (point.distanceTo(position) > left.distanceTo(position)) {
		point = left;
	}

	return point;
}

Vector3 RectangularAreaShapeImplementation::getFarthestPoint(const Vector3& position) {
	// Calculate corners.
	Vector3 topLeft, topRight, bottomLeft, bottomRight;
	topLeft.set(blX, 0, urY);
	topRight.set(urX, 0, urY);
	bottomLeft.set(blX, 0, blY);
	bottomRight.set(urX, 0, blY);

	// Find the farthest of the four corners.
	Vector3 point = topLeft;
	if (point.distanceTo(position) < topRight.distanceTo(position)) {
		point = topRight;
	}
	if (point.distanceTo(position) < bottomLeft.distanceTo(position)) {
		point = bottomLeft;
	}
	if (point.distanceTo(position) < bottomRight.distanceTo(position)) {
		point = bottomRight;
	}

	return point;
}
