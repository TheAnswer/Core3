/*
 * SphereAreaShapeImplementation.cpp
 *
 * Created: 12/30/2023
 * Author: Hakry
 */

#include "server/zone/objects/area/areashapes/SphereAreaShape.h"
#include "server/zone/objects/area/areashapes/CuboidAreaShape.h"

//#define DEBUG_POSITION
//#define DEBUG_SPHERE

bool SphereAreaShapeImplementation::containsPoint(float x, float z, float y) const {
	Vector3 point;

	point.setX(x);
	point.setZ(z);
	point.setY(y);

	bool containsTest = containsPoint(point);

#ifdef DEBUG_SPHERE
	StringBuffer msg;

	msg << "containsPoint --- for Sphere: " << (containsTest ? "TRUE" : "FALSE");
	msg << " Point to Check- X = " << x << " Z = " << z << " Y = " << y << " Radius = " << getRadius() << "\n";
	msg << "Sphere Location: X = " << areaCenter.getX() << " Z = " << areaCenter.getZ() << " Y = " << areaCenter.getY();

	info(true) << msg.toString();
#endif

	return containsTest;
}

bool SphereAreaShapeImplementation::containsPoint(const Vector3& point) const {
	float distSq = areaCenter.squaredDistanceTo(point);

#ifdef DEBUG_SPHERE
	info(true) << "Distance Squared: " << distSq << " Radius2 = " << radius2;
#endif

	return distSq < radius2;
}

Vector3 SphereAreaShapeImplementation::getRandomPosition() const {
	float distance = System::random((int)radius);
	float angle = System::random(360) * Math::DEG2RAD;

	Vector3 position;
	position.set(areaCenter.getX() + distance * Math::cos(angle), 0, areaCenter.getY() + distance * Math::sin(angle));

	return position;
}

Vector3 SphereAreaShapeImplementation::getRandomPosition(const Vector3& origin, float minDistance, float maxDistance) const {
#ifdef DEBUG_POSITION
	info(true) << "Cuboid - getRandomPosition called";
#endif // DEBUG_POSITION

	Vector3 position;
	bool found = false;
	int retries = 10;

	while (!found && retries-- > 0) {
		// Generate random spherical coordinates
		float theta = 2.0 * Math::PI * System::frandom(1.0);
		float phi = acos(1.0 - 2.0 * System::frandom(1.0));

		// Generate a random distance within the specified range
		float distance = minDistance + (maxDistance - minDistance) * System::frandom(1.0);

		// Convert spherical coordinates to Cartesian coordinates
		float x = areaCenter.getX() + distance * sin(phi) * cos(theta);
		float y = areaCenter.getY() + distance * sin(phi) * sin(theta);
		float z = areaCenter.getZ() + distance * cos(phi);

		position.set(x, y, z);

#ifdef DEBUG_POSITION
		info(true) << " X Calc = " << xCalc << " Y Calc = " << yCalc << " Spawn Distance Delta = " << spawnDistanceDelta;
		info(true) << "Checking Position: " << position.toString() << "   Squared Distance: " << areaCenter.squaredDistanceTo(position) << "  Squared Radius = " << radius2;
#endif // DEBUG_POSITION

		found = containsPoint(position);
	}

	if (!found) {
#ifdef DEBUG_POSITION
		info(true) << "Cuboid - Position not found !!!";
#endif // DEBUG_POSITION

		return areaCenter;
	}

	return position;
}

bool SphereAreaShapeImplementation::intersectsWith(AreaShape* areaShape) const {
	if (areaShape == nullptr) {
		return false;
	}

	if (areaShape->isSphereAreaShape()) {
		auto sphere = dynamic_cast<SphereAreaShape*>(areaShape);

		if (sphere != nullptr)
			return intersectsWithSphere(sphere);
	} else if (areaShape->isCuboidAreaShape()) {
		auto cuboid = dynamic_cast<CuboidAreaShape*>(areaShape);

		if (cuboid != nullptr)
			return intersectsWithCuboid(cuboid);
	}

	return false;
}

bool SphereAreaShapeImplementation::intersectsWithSphere(SphereAreaShape* sphere) const {
	//float squaredRadiusSum = radius2 + circle->getRadius2();
	//float squaredCenterDistance = areaCenter.squaredDistanceTo(circle->getAreaCenter());

	return true;
}

bool SphereAreaShapeImplementation::intersectsWithCuboid(CuboidAreaShape* cuboid) const {
	//float squaredRadiusSum = radius2 + circle->getRadius2();
	//float squaredCenterDistance = areaCenter.squaredDistanceTo(circle->getAreaCenter());

	return true;
}
