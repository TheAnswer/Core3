/*
 * ClientBuilding.h
 *
 *  Created on: 4/10/2016
 *      Author: gslomin
 */

#ifndef CLIENTBUILDING_H_
#define CLIENTBUILDING_H_

#include "engine/engine.h"
#include "../scene/SceneObject.h"
class ClientCell;
class MeshData;
class InteriorLayoutTemplate;

class ClientBuilding : public SceneObject {
	PathGraph *pathGraph;
	osg::ref_ptr<osg::Group> mainGroup;
	VectorMap<String, Reference<ClientCell*> > cellMap;
	Vector<Reference<ClientCell*> > cells;
	InteriorLayoutTemplate *interiorLayoutTemplate;
	
public:
	ClientBuilding(SharedObjectTemplate *tmpl);
	void initializePortalLayout(PortalLayout *layout);
	void addToScene();
	uint32 getCellCount() {
		return cells.size();
	}
	
	bool isBuildingObject() {
		return true;
	}
	
	virtual void createChildObjects();
	
	Vector<Reference<MeshData*> > getTransformedMeshData();
};


#endif /* CLIENTCELL_H_ */
