/*
 * MeshData.h
 *
 *  Created on: 04/22/2016
 *      Author: gslomin
 */

#include "MeshData.h"

AABB MeshData::buildAABB() const {
	float minx = 100000;
	float miny = 100000;
	float minz = 100000;
	
	float maxx = -1000000;
	float maxy = -1000000;
	float maxz = -1000000;
	
	for (int i=0; i<vertices.size(); i++) {
		const Vector3& vert = vertices.get(i);
		const float& x = vert.getX();
		const float& y = vert.getY();
		const float& z = vert.getZ();
		
		if (x < minx)
			minx = x;
		
		if (y < miny)
			miny = y;
		
		if (z < minz)
			minz = z;
		
		if ( x > maxx)
			maxx = x;
		
		if (y > maxy)
			maxy = y;
		
		if (z > maxz)
			maxz = z;
	}
	
	return AABB(Vector3(minx, miny, minz), Vector3(maxx, maxy, maxz));
}

Vector<MeshTriangle>* MeshData::getMeshWithinBounds(AABB& bounds) const {
	float minx = bounds.getXMin();
	float miny = bounds.getYMin();
	float minz = bounds.getZMin();
	
	float maxx = bounds.getXMax();
	float maxy = bounds.getYMax();
	float maxz = bounds.getZMax();
	
	Vector<MeshTriangle>* tris = new Vector<MeshTriangle>();
	
	for (int i=0; i<triangles.size(); i++) {
		const MeshTriangle& tri = triangles.get(i);
		const int* v = tri.getVerts();
		
		for(int i=0; i<3; i++) {
			const Vector3& vert = vertices.get(v[i]);
			const float& x = vert.getX();
			const float& y = vert.getY();
			const float& z = vert.getZ();
			
			if (x >= minx && x <= maxx && y >= miny && y <= maxy && z >= minz && z <= maxz) {
				tris->add(tri);
				break;
			}
		}
	}
	
	return tris;
}

void MeshData::readObject(IffStream* iffStream) {
	iffStream->openForm('VTXA');
	iffStream->openForm('0003');
	iffStream->openChunk('INFO');

	iffStream->getInt(); //unk

	int numVertices = iffStream->getInt();

	iffStream->closeChunk();

	Chunk* vertexDataChunk = iffStream->openChunk('DATA');

	int vertexDataChunkSize = vertexDataChunk->getChunkSize();

	int intBytesPerVertex = vertexDataChunkSize / numVertices;
	
	for (int i = 0; i < numVertices; ++i) {
		float x = iffStream->getFloat();
		float y = iffStream->getFloat();
		float z = iffStream->getFloat();
		
		Vector3 vert(x, y, z);

		vertices.add(vert);

		vertexDataChunk->shiftOffset(intBytesPerVertex - 12);
	}

	iffStream->closeChunk('DATA');
	iffStream->closeForm('0003');
	iffStream->closeForm('VTXA');

	Chunk* indexData = iffStream->openChunk('INDX');

	int indexCount = iffStream->getInt();

	for (int i = 1; i <= indexCount; i += 3) {
		int a = indexData->readShort();
		int b = indexData->readShort();
		int c = indexData->readShort();

		MeshTriangle triangle;
		triangle.verts[0] = a;
		triangle.verts[1] = b;
		triangle.verts[2] = c;

		triangles.add(triangle);
	}

	iffStream->closeChunk();
}
