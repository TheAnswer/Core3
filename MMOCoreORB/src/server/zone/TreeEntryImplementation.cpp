/*
Copyright (C) 2007 <SWGEmu>. All rights reserved.
Distribution of this file for usage outside of Core3 is prohibited.
*/

#include <autogen/server/zone/ZoneServer.h>
#include "server/zone/TreeEntry.h"
#include "server/zone/TreeNode.h"
#include "server/zone/objects/scene/SceneObject.h"
#include "server/zone/objects/tangible/TangibleObject.h"

//#define DEBUG_TREE_ENTRY

TreeEntryImplementation::TreeEntryImplementation(TreeNode* n) {
	node = n;
	bounding = false;
	closeobjects = nullptr;

	radius = 0.5f;
	receiverFlags = 0;
}

void TreeEntryImplementation::setNode(TreeNode* n) {
	node = n;
}

void TreeEntryImplementation::addInRangeObject(TreeEntry* obj, bool doNotifyUpdate) {
	if (obj == nullptr) {
		return;
	}

	if (closeobjects != nullptr && closeobjects->put(obj) != -1) {
 		notifyInsert(obj);
	} else if (doNotifyUpdate) {
		notifyPositionUpdate(obj);
	}
}

void TreeEntryImplementation::removeInRangeObject(TreeEntry* obj, bool notifyDisappear) {
	if (obj == nullptr) {
		return;
	}

	if (closeobjects != nullptr && closeobjects->drop(obj) && notifyDisappear) {
		notifyDissapear(obj);
	}
}

bool TreeEntryImplementation::containsPoint(float px, float py) {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::containsPoint";
#endif

#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::containsPoint - checking against RadiusSq: " << (radius * radius);
#endif

	return (((px - getPositionX()) * (px - getPositionX())) + ((py - getPositionY()) * (py - getPositionY())) < radius * radius );
}

bool TreeEntryImplementation::containsPoint(float px, float py, float pz) {

	float deltaX = px - getPositionX();
	float deltaY = py - getPositionY();
	float deltaZ = pz - getPositionZ();
	float radiusSq = radius * radius;

#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::containsPoint - checking against RadiusSq: " << radiusSq << " Entry: " << getObjectID() << " ClassName: " << _className;
#endif


	return ((deltaX * deltaX) + (deltaY * deltaY) + (deltaZ * deltaZ)) < radiusSq;
}

bool TreeEntryImplementation::isInSWArea(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.Logger::console.info(true) << "TreeEntryImplementation::isInSWArea -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY &&
				coordinates.getPositionZ() >= checkNode->minZ && coordinates.getPositionZ() < checkNode->dividerZ;

	else
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY;
}

bool TreeEntryImplementation::isInSEArea(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInSEArea -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY &&
				coordinates.getPositionZ() >= checkNode->minZ && coordinates.getPositionZ() < checkNode->dividerZ;
	else
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY;
}

bool TreeEntryImplementation::isInNWArea(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInNEArea -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < checkNode->maxY &&
				coordinates.getPositionZ() >= checkNode->minZ && coordinates.getPositionZ() < checkNode->dividerZ;
	else
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < checkNode->maxY;
}

bool TreeEntryImplementation::isInNEArea(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInNEArea -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < checkNode->maxY &&
				coordinates.getPositionZ() >= checkNode->minZ && coordinates.getPositionZ() < checkNode->dividerZ;
	else
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < node->maxY;
}

bool TreeEntryImplementation::isInSW2Area(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInSW2Area -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY &&
				coordinates.getPositionZ() >= checkNode->dividerZ && coordinates.getPositionZ() < checkNode->maxZ;
	else
		return 0;
}

bool TreeEntryImplementation::isInSE2Area(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInSE2Area -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->minY && coordinates.getPositionY() < checkNode->dividerY &&
				coordinates.getPositionZ() >= checkNode->dividerZ && coordinates.getPositionZ() < checkNode->maxZ;
	else
		return 0;

}

bool TreeEntryImplementation::isInNE2Area(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInNE2Area -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->dividerX && coordinates.getPositionX() < checkNode->maxX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < checkNode->maxY &&
				coordinates.getPositionZ() >= checkNode->dividerZ && coordinates.getPositionZ() < checkNode->maxZ;
	else
		return 0;
}

bool TreeEntryImplementation::isInNW2Area(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInNW2Area -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1)
		return coordinates.getPositionX() >= checkNode->minX && coordinates.getPositionX() < checkNode->dividerX &&
			coordinates.getPositionY() >= checkNode->dividerY && coordinates.getPositionY() < checkNode->maxY &&
				coordinates.getPositionZ() >= checkNode->dividerZ && coordinates.getPositionZ() < checkNode->maxZ;
	else
		return 0;
}

bool TreeEntryImplementation::isInArea(TreeNode* checkNode) const {
#ifdef DEBUG_TREE_ENTRY
	Logger::console.info(true) << "TreeEntryImplementation::isInArea -- Divider Z: " << checkNode->dividerZ;
#endif

	if (checkNode->dividerZ != -1) {
		return ((coordinates.getPositionX() + radius > checkNode->dividerX && coordinates.getPositionX() - radius < checkNode->dividerX) ||
			(coordinates.getPositionY() + radius > checkNode->dividerY && coordinates.getPositionY() - radius < checkNode->dividerY)) &&
			(coordinates.getPositionZ() + radius > checkNode->dividerZ && coordinates.getPositionZ() - radius < checkNode->dividerZ);
	} else {
		return (coordinates.getPositionX() + radius > checkNode->dividerX && coordinates.getPositionX() - radius < checkNode->dividerX) ||
			(coordinates.getPositionY() + radius > checkNode->dividerY && coordinates.getPositionY() - radius < checkNode->dividerY);
	}
}

uint64 TreeEntryImplementation::getObjectID() {
	return _this.getReferenceUnsafeStaticCast()->_getObjectID();
}

TreeEntry* TreeEntryImplementation::getRootParent() {
	ManagedReference<TreeEntry*> grandParent = getParent();
	ManagedReference<TreeEntry*> tempParent = nullptr;

	if (grandParent == nullptr)
		return nullptr;

	while ((tempParent = grandParent->getParent()) != nullptr)
		grandParent = tempParent;

	return grandParent;
}

TreeEntry* TreeEntryImplementation::getParentUnsafe() {
	return parent.getReferenceUnsafe();
}

TreeEntry* TreeEntryImplementation::getRootParentUnsafe() {
	TreeEntry* parent = this->parent.getReferenceUnsafe();

	if (parent == nullptr)
		return nullptr;

	TreeEntry* grandParent = parent;
	TreeEntry* temp = nullptr;

	while ((temp = grandParent->getParentUnsafe()) != nullptr)
		grandParent = temp;

	return grandParent;
}

int TreeEntry::compareTo(TreeEntry* obj) {
	if (getDirtyObjectID() < obj->getDirtyObjectID())
		return 1;
	else if (getDirtyObjectID() > obj->getDirtyObjectID())
		return -1;
	else
		return 0;
}

int TreeEntryImplementation::compareTo(TreeEntry* obj) {
	if (getObjectID() < obj->getObjectID())
		return 1;
	else if (getObjectID() > obj->getObjectID())
		return -1;
	else
		return 0;
}

uint64 TreeEntry::getDirtyObjectID() {
	return _getObjectID();
}

uint64 TreeEntry::getObjectID() {
	return _getObjectID();
}

uint64 TreeEntryImplementation::getDirtyObjectID() {
	return _this.getReferenceUnsafeStaticCast()->_getObjectID();
}

float TreeEntryImplementation::getOutOfRangeDistance() {
	auto thisNode = node.get();
	float closeRange = ZoneServer::CLOSEOBJECTRANGE;

	if (thisNode != nullptr && thisNode->dividerZ != -1) {
		closeRange = ZoneServer::SPACEOBJECTRANGE;
	}

	if (radius > (closeRange * 0.5f)) {
		return closeRange + radius;
	}

	return closeRange;
}

void TreeEntryImplementation::setParent(TreeEntry* value) {
	parent = value;
	updateWorldPosition(false);
}

void TreeEntryImplementation::initializePosition(const Vector3& value) {
	coordinates.initializePosition(value);
	updateWorldPosition(true);
}

void TreeEntryImplementation::initializePosition(float x, float z, float y) {
	coordinates.initializePosition(x, z, y);
	updateWorldPosition(true);
}

void TreeEntryImplementation::setPosition(const Vector3& value) {
	coordinates.setPosition(value);
	updateWorldPosition(false);
}

void TreeEntryImplementation::setPosition(float x, float z, float y) {
	coordinates.setPosition(x, z, y);
	updateWorldPosition(false);
}

void TreeEntryImplementation::updateWorldPosition(bool initialize) {
#ifdef DEBUG_WORLD_POSITION
	auto sceneO = static_cast<SceneObject*>(_this.getReferenceUnsafeStaticCast());
#endif // DEBUG_WORLD_POSITION

	auto root = static_cast<SceneObject*>(getRootParentUnsafe());

	Vector3 worldPosition = getPosition();

	if (root != nullptr) {
		if (root->isBuildingObject() || root->isPobShip()) {
			float rootRad = -root->getDirection()->getRadians();
			float rootCos = cos(rootRad);
			float rootSin = sin(rootRad);

			float localX = getPositionX();
			float localY = getPositionY();
			float localZ = getPositionZ();

			float rotatedX = (localX * rootCos) - (localY * rootSin);
			float rotatedY = (localX * rootSin) + (localY * rootCos);

			float worldX = root->getPositionX() + rotatedX;
			float worldY = root->getPositionY() + rotatedY;
			float worldZ = root->getPositionZ() + localZ;

#ifdef DEBUG_WORLD_POSITION
			if (sceneO != nullptr && sceneO->isPlayerCreature())
				Logger::console.info(true) << sceneO->getDisplayedName() << " -- Coordinates are using root parent to calculate";
#endif // DEBUG_WORLD_POSITION

			worldPosition = Vector3(worldX, worldY, worldZ);
		} else {
			worldPosition = root->getPosition();
		}
	}

	if (initialize) {
#ifdef DEBUG_WORLD_POSITION
		if (sceneO != nullptr && sceneO->isPlayerCreature()) {
			Logger::console.info(true) << sceneO->getDisplayedName() << " -- INITIALIZING - World Coordinates to " << worldPosition.toString();
		}
#endif // DEBUG_WORLD_POSITION

		worldCoordinates.initializePosition(worldPosition.getX(), worldPosition.getZ(), worldPosition.getY());
	} else {
#ifdef DEBUG_WORLD_POSITION
		if (sceneO != nullptr && sceneO->isPlayerCreature()) {
			Logger::console.info(true) << sceneO->getDisplayedName() << " -- UPDATING - World Coordinates to " << worldPosition.toString();
		}
#endif // DEBUG_WORLD_POSITION

		worldCoordinates.setPosition(worldPosition.getX(), worldPosition.getZ(), worldPosition.getY());
	}
}

Vector3 TreeEntryImplementation::getWorldPosition() {
#ifdef DEBUG_WORLD_POSITION
	auto sceneO = static_cast<SceneObject*>(_this.getReferenceUnsafeStaticCast());
#endif // DEBUG_WORLD_POSITION

	auto currentWorld = worldCoordinates.getPosition();

#ifdef DEBUG_WORLD_POSITION
	if (sceneO != nullptr && sceneO->isPlayerCreature()) {
		Logger::console.info(true) << sceneO->getDisplayedName() << " -- TreeEntryImplementation::getWorldPosition() is returning  " << currentWorld.toString();
	}
#endif // DEBUG_WORLD_POSITION

	return currentWorld;
}

float TreeEntryImplementation::getWorldPositionX() const {
	return worldCoordinates.getPositionX();
}

float TreeEntryImplementation::getWorldPositionY() const {
	return worldCoordinates.getPositionY();
}

float TreeEntryImplementation::getWorldPositionZ() const {
	return worldCoordinates.getPositionZ();
}

