#ifndef NAVMESHREGION_H_
#define NAVMESHREGION_H_
#include "engine/engine.h"
#include "templates/appearance/MeshData.h"
#include "pathfinding/RecastNavMesh.h"

class NavmeshRegion : public Object {
public:
	String name;
	Vector<Sphere> regionBounds;
	Vector<Reference<MeshData*> > sceneData;
	RecastNavMesh *navMesh;
};

#endif
