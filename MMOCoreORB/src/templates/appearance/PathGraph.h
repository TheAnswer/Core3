/*
 * PathGraph.h
 *
 *  Created on: 02/12/2010
 *      Author: victor
 */

#ifndef PATHGRAPH_H_
#define PATHGRAPH_H_

#include "templates/appearance/PathNode.h"
#include "templates/IffTemplate.h"

class FloorMesh;

class PathGraph : public IffTemplate {
	enum PathGraphType
	{
		Cell,
		Building,
		City,

		None,
	};


	Vector<PathNode*> pathNodes;
	Vector<PathEdge> pathEdges;

	PathGraphType type;

	Vector<int> edgeCounts;
	Vector<int> edgeStarts;

	FloorMesh* floorMesh;

protected:
	void connectNodes(Vector<PathEdge>& pathEdges);

public:
	PathGraph(FloorMesh* floor) {
		floorMesh = floor;
		type = None;
	}

	~PathGraph() {
		while (pathNodes.size() > 0)
			delete pathNodes.remove(0);
	}

	void readObject(IffStream* iffStream);

	float calculateManhattanDistance(PathNode* node1, PathNode* node2) {
		/*return abs(node1->getX() - node2->getX()) + abs(node1->getY() - node2->getY())
				+ abs(node1->getZ() - node2->getZ());*/

		return node1->getPosition().squaredDistanceTo(node2->getPosition());
	}

	PathNode* getNode(int globalNumberID);

	inline PathNode* findNearestNode(float x, float z, float y) {
		return findNearestNode(Vector3(x, y, z));
	}

	PathNode* findNearestNode(const Vector3& pointAlfa);
	Vector<const PathNode*> getEntrances();
	PathNode* findNearestGlobalNode(const Vector3& pointAlfa);
	PathNode* findGlobalNode(int globalNodeID);

	inline void addPathNode(PathNode* pathNode) {
		pathNodes.add(pathNode);
	}

	inline Vector<PathEdge>* getPathEdges() {
		return &pathEdges;
	}

	inline Vector<PathNode*>* getPathNodes() {
		return &pathNodes;
	}

	inline Vector<PathNode*> getGlobalNodes() {
		Vector<PathNode*> nodes;

		for (int i = 0; i < pathNodes.size(); ++i) {
			if (pathNodes.get(i)->getGlobalGraphNodeID() != -1)
				nodes.add(pathNodes.get(i));
		}

		return nodes;
	}

	inline FloorMesh* getFloorMesh() {
		return floorMesh;
	}

	inline PathGraphType getType() const {
		return type;
	}

};


#endif /* PATHGRAPH_H_ */
