/*
 * PathGraph.cpp
 *
 *  Created on: 03/12/2010
 *      Author: victor
 */

#include "PathGraph.h"
#include "templates/appearance/FloorMesh.h"

uint32 PathNode::getID() const {
	int cellID = pathGraph->getFloorMesh()->getCellID();

	return (cellID << 16) + id;
}

void PathGraph::readObject(IffStream* iffStream) {
	iffStream->openForm('PGRF');
	iffStream->openForm('0001');
	iffStream->openChunk('META');

	type = static_cast<PathGraphType>(iffStream->getInt());

	iffStream->closeChunk('META');

	iffStream->openChunk('PNOD');

	int nodesSize = iffStream->getInt();

	pathNodes.removeAll(nodesSize);

	for (int i = 0; i < nodesSize; ++i) {
		PathNode* pathNode = new PathNode(this);

		pathNode->readObject(iffStream);

		pathNodes.add(pathNode);
	}

	iffStream->closeChunk('PNOD');

	iffStream->openChunk('PEDG');

	int pathEdgeSize = iffStream->getInt();

	pathEdges.removeAll(pathEdgeSize);

	for (int i = 0; i < pathEdgeSize; ++i) {
		PathEdge pathEdge;

		pathEdge.readObject(iffStream);

		pathEdges.add(pathEdge);
	}

	iffStream->closeChunk('PEDG');

	iffStream->openChunk('ECNT');

	int ecntSize = iffStream->getInt();

	edgeCounts.removeAll(ecntSize);

	for (int i = 0; i < ecntSize; ++i) {
		edgeCounts.add(iffStream->getInt());
	}

	iffStream->closeChunk('ECNT');

	iffStream->openChunk('ESTR');

	int estrSize = iffStream->getInt();

	edgeStarts.removeAll(estrSize);

	for (int i = 0; i < estrSize; ++i) {
		edgeStarts.add(iffStream->getInt());
	}

	iffStream->closeChunk('ESTR');
	iffStream->closeForm('0001');
	iffStream->closeForm('PGRF');

	connectNodes(pathEdges);
}

const PathNode* PathGraph::getNode(int globalNumberID) const {
	for (int i = 0; i < pathNodes.size(); ++i) {
		const PathNode* pathNode = pathNodes.getUnsafe(i);

		if (pathNode->getGlobalGraphNodeID() == globalNumberID)
			return pathNode;
	}

	return nullptr;
}

const PathNode* PathGraph::findGlobalNode(int globalNodeID) const {
	for (int i = 0; i < pathNodes.size(); ++i) {
		const PathNode* pathNode = pathNodes.getUnsafe(i);

		if (pathNode->getGlobalGraphNodeID() == globalNodeID)
			return pathNode;
	}

	return nullptr;
}

const PathNode* PathGraph::findNearestGlobalNode(const Vector3& pointAlfa) const {
	float minDistance = 160000000.f;
	const PathNode* node = nullptr;

	for (int i = 0; i < pathNodes.size(); ++i) {
		const PathNode* pathNode = pathNodes.getUnsafe(i);

		if (pathNode->getGlobalGraphNodeID() == -1)
			continue;

		Vector3 point(pathNode->getX(), pathNode->getY(), pathNode->getZ());

		float sqrDistance = pointAlfa.squaredDistanceTo(point);

		if (sqrDistance < minDistance) {
			minDistance = sqrDistance;
			node = pathNode;
		}
	}

	return node;
}

Vector<const PathNode*> PathGraph::getEntrances() const {
	Vector<const PathNode*> vec;
	for (const PathNode *node : pathNodes) {
		if(node->getType() == PathNode::BuildingEntrance) {
			vec.add(node);
		}
	}
	return vec;
}

const PathNode* PathGraph::findNearestNode(const Vector3& pointAlfa) const {
	float minDistance = 160000000.f;
	PathNode* node = nullptr;

	for (int i = 0; i < pathNodes.size(); ++i) {
		PathNode* pathNode = pathNodes.getUnsafe(i);

		Vector3 point(pathNode->getX(), pathNode->getY(), pathNode->getZ());

		float sqrDistance = pointAlfa.squaredDistanceTo(point);

		if (sqrDistance < minDistance) {
			minDistance = sqrDistance;
			node = pathNode;
		}
	}

	return node;
}

void PathGraph::connectNodes(Vector<PathEdge>& pathEdges) {
	for (int i = 0; i < pathEdges.size(); ++i) {
		PathEdge* pathEdge = &pathEdges.get(i);

		int from = pathEdge->getFromConnection();
		int to = pathEdge->getToConnection();

		PathNode* fromNode = pathNodes.get(from);
		PathNode* toNode = pathNodes.get(to);

		/*Vector<PathNode*>* path = AStarAlgorithm<PathGraph, PathNode>::search<uint32>(this, fromNode, toNode);

		if (path != nullptr) {
			System::out << "found path\n";
			delete path;
		} else {
			System::out << "didint find path\n";
		}*/

		fromNode->addChild(toNode);
	}
}
