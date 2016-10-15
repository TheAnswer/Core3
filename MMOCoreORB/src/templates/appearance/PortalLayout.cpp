	/*
 * PortalLayout.cpp
 *
 *  Created on: 03/12/2010
 *      Author: victor
 */

#include "PortalLayout.h"
#include "templates/appearance/MeshAppearanceTemplate.h"
#include "templates/appearance/PathNode.h"
#include "templates/appearance/FloorMesh.h"
#include "engine/util/u3d/AStarAlgorithm.h"

void PortalLayout::readPortalGeometry(IffStream *iff, int numPortals) {

	for(int i=0; i<numPortals; i++) {
		iff->openChunk('PRTL');
		uint32 size = iff->getUnsignedInt();

		Reference<PortalGeometry*> portal = new PortalGeometry();
		Reference<MeshData*> mesh = portal->getGeometry();
		Vector<Vector3> *verts = mesh->getVerts();
		Vector<MeshTriangle> *tris = mesh->getTriangles();

		Vector3 min(50000, 50000, 50000);
		Vector3 max(-50000, -50000, -50000);

		for (int i=0; i<size; i++) {
			float x = iff->getFloat();
			float y = iff->getFloat();
			float z = iff->getFloat();

			if(x < min.getX())
				min.setX(x);

			if(x > max.getX())
				max.setX(x);

			if(y < min.getY())
				min.setY(y);

			if (y > max.getY())
				max.setY(y);

			if (z < min.getZ())
				min.setZ(z);

			if (z > max.getZ())
				max.setZ(z);

			Vector3 vert(x, y, z);
			verts->add(vert);
		}

		iff->closeChunk('PRTL');
		portal->setBoundingBox(AABB(min, max));
		Vector3 center = portal->getBoundingBox().center();

		for (int i=0; i<size; i++) {
			Vector3 &vert = verts->get(i);

			vert = center + ((vert - center) * 1.1);
			
			// Triangle fan
			if ( i >= 2) {
				MeshTriangle triA;
				triA.set(2, 0);
				triA.set(1, i-1);
				triA.set(0, i);
				tris->add(triA);

				MeshTriangle triB;
				triB.set(0, 0);
				triB.set(1, i-1);
				triB.set(2, i);
				tris->add(triB);
			}
		}
		portalGeometry.add(portal);
	}
}

PortalLayout::PortalLayout() {
	pathGraph = NULL;

	setLoggingName("PortalLayout");
}

PortalLayout::~PortalLayout() {
	delete pathGraph;
	pathGraph = NULL;
}

void PortalLayout::parse(IffStream* iffStream) {
	try {
		iffStream->openForm('PRTO');

		uint32 type = iffStream->getNextFormType();

		if (type != '0003' && type != '0004') {
			StringBuffer stream;
			stream << "invalid PROTO type 0x" << hex << type;
			error(stream);

			return;
		}

		iffStream->openForm(type);

		Chunk* data = iffStream->openChunk('DATA');

		uint32 numPortals = iffStream->getInt();
		uint32 numCells = iffStream->getInt();

		iffStream->closeChunk('DATA');

		iffStream->openForm('PRTS');
		readPortalGeometry(iffStream, numPortals);
		iffStream->closeForm('PRTS');

		//open CELS form
		parseCELSForm(iffStream, numCells);

		//path graph

		uint32 nextType = iffStream->getNextFormType();

		if (nextType == 'PGRF') {
			pathGraph = new PathGraph(NULL);
			pathGraph->readObject(iffStream);
		}

		iffStream->closeForm(type);

		iffStream->closeForm('PRTO');
	} catch (Exception& e) {
		String err = "unable to parse file ";
		err += iffStream->getFileName();
		error(err);
	}

	connectFloorMeshGraphs();
}

int PortalLayout::getCellID(const String& cellName) {
	for (int i = 0; i < cellProperties.size(); ++i) {
		CellProperty& cell = cellProperties.get(i);

		if (cell.getName() == cellName)
			return i;
	}

	return -1;
}

void PortalLayout::connectFloorMeshGraphs() {
	for (int i = 0; i < cellProperties.size(); ++i) {
		FloorMesh* floorMesh = getFloorMesh(i);

		if (floorMesh == NULL)
			continue;

		PathGraph* pathGraph = floorMesh->getPathGraph();

		if (pathGraph == NULL)
			continue;

		Vector<PathNode*> globalNodes = pathGraph->getGlobalNodes();

		for (int j = 0; j < globalNodes.size(); ++j) {
			PathNode* node = globalNodes.get(j);

			int globalID = node->getGlobalGraphNodeID();

			for (int k = 0; k < cellProperties.size(); ++k) {
				if (i != k) {
					FloorMesh* newMesh = getFloorMesh(k);

					if (newMesh != NULL) {
						PathGraph* newPathGraph = newMesh->getPathGraph();

						if (newPathGraph != NULL) {
							Vector<PathNode*> newGlobalNodes = newPathGraph->getGlobalNodes();

							for (int l = 0; l < newGlobalNodes.size(); ++l) {
								PathNode* newNode = newGlobalNodes.get(l);

								int newGlobalID = newNode->getGlobalGraphNodeID();

								if (globalID == newGlobalID)
									node->addChild(newNode);
							}
						}
					}
				}
			}
		}
	}

}

int PortalLayout::getFloorMeshID(int globalNodeID, int floorMeshToExclude) {
	for (int i = 0; i < cellProperties.size(); ++i) {
		if (i == floorMeshToExclude)
			continue;

		FloorMesh* floorMesh = getFloorMesh(i);
		PathNode* node = floorMesh->getGlobalNode(globalNodeID);

		if (node != NULL)
			return i;
	}

	return -1;
}

void PortalLayout::parseCELSForm(IffStream* iffStream, int numCells) {
	try {
		iffStream->openForm('CELS');

		uint32 nextType;

		for (int i=0; i<numCells; i++) {
			CellProperty cell(cellProperties.size());
			cell.readObject(iffStream);
			cellProperties.add(cell);
		}

		iffStream->closeForm('CELS');

	} catch (Exception& e) {
		error(e.getMessage());
		error("parsing CELS for " + iffStream->getFileName());
	} catch (...) {
		//error("parsing CELS for " + iffStream->getFileName());
		throw;
	}
}

Vector<PathNode*>* PortalLayout::getPath(PathNode* node1, PathNode* node2) {
	return AStarAlgorithm<PathGraph, PathNode>::search<uint32>(node1->getPathGraph(), node1, node2);
}

uint32 PortalLayout::loadCRC(IffStream* iffStream) {
	uint32 crc = 0;
	try {
		iffStream->openForm('PRTO');
		uint32 type = iffStream->getNextFormType();
		iffStream->openForm(type);

		Chunk *chunk = iffStream->openChunk();
		while(chunk != NULL && chunk->getChunkID() != 'CRC ') // Yes the space is intentional
		{
			iffStream->closeChunk();
			chunk = iffStream->openChunk();
		}

		if(chunk->getChunkID() == 'CRC ')
			crc = iffStream->getUnsignedInt();

		iffStream->closeChunk('CRC ');
		iffStream->closeForm(type);
		iffStream->closeForm('PRTO');
	} catch (Exception& e) {
		String err = "unable to parse portal crc ";
		err += iffStream->getFileName();
		static Logger logger;
		logger.error(err);
	}

	return crc;
}
