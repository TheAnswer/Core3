/*
 * MeshAppearanceTemplate.h
 *
 *  Created on: 07/12/2010
 *      Author: victor
 */

#ifndef MESHAPPEARANCETEMPLATE_H_
#define MESHAPPEARANCETEMPLATE_H_

#include "engine/engine.h"
#include "templates/appearance/AppearanceTemplate.h"
#include "templates/appearance/MeshData.h"

class MeshAppearanceTemplate : public AppearanceTemplate {
	Vector<Reference<MeshData*> > meshes;
	//Vector<Triangle> triangles;

	AABBTree* aabbTree;
	Sphere* boundingSphere;
	//String file;

public:
	virtual uint32 getType() {
		return 'MESH';
	}
	MeshAppearanceTemplate() {
		aabbTree = NULL;
		meshes = NULL;
		boundingSphere = NULL;
	}

	~MeshAppearanceTemplate() {
		delete aabbTree;
		aabbTree = NULL;

		delete boundingSphere;
		boundingSphere = NULL;
	}

	void createAABB();

	void readObject(IffStream* templateData) {
		parse(templateData);
	}

	void parse(IffStream* iffStream);
	void parseSPS(IffStream* iffStream);
	void parseVertexData(IffStream* iffStream, int idx);

	bool testCollide(float x, float z, float y, float radius);

	AppearanceTemplate* getFirstMesh() {
		return this;
	}

	inline void getTriangles(Vector<Triangle*>& triangles) {
		if (aabbTree != NULL)
			aabbTree->getTriangles(triangles);
	}

	/*inline String& getFileName() {
		return file;
	}*/

	inline AABBTree* getAABBTree() {
		return aabbTree;
	}

	inline Sphere* getBoundingSphere() {
		return boundingSphere;
	}
	
	Vector<Reference<MeshData*> > getMeshes() {
		return meshes;
	}

};

#endif /* MESHAPPEARANCETEMPLATE_H_ */
