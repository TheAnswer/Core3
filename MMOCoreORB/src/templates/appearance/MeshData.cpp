/*
 * MeshData.h
 *
 *  Created on: 04/22/2016
 *      Author: gslomin
 */

#include "MeshData.h"

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
		//triangleIndexes.add(indexData->getUnsignedShort());
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
