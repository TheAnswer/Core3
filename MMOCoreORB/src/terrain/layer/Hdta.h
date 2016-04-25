/*
 * Hdta.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef HDTA_H_
#define HDTA_H_


#include "../TemplateVariable.h"
#include "Segment.h"

class Hdta : public TemplateVariable<'HDTA'> {
	Vector<Segment*> sgmts;


public:
	Hdta() : IffTemplateVariable() {

	}

	~Hdta() {
		for (int i = 0; i < sgmts.size(); ++i)
			delete sgmts.get(i);
	}
	
	Point3D* getPoint(int segment, int index) {
		sgmts.get(segment)->getPoint(index);
	}
	
	void readObject(engine::util::IffStream* iffStream) {
		uint32 formType = iffStream->getNextFormType();
		switch(formType) {
			case 'HDTA':
			case 'ROAD':
				iffStream->openForm(formType);
				parseFromIffStream(iffStream);
				iffStream->closeForm(formType);
				break;
			default:
				throw Exception("Incorrect form type " + String::valueOf(formType));
		}
	}

	void parseFromIffStream(engine::util::IffStream* iffStream) {
		uint32 version = iffStream->getNextFormType();

		iffStream->openForm(version);

		switch (version) {
		case '0001':
			parseFromIffStream(iffStream, Version<'0001'>());
			break;
		default:
			System::out << "unknown HDTA version " << version << endl;
			break;
		}

		iffStream->closeForm(version);
	}

	void parseFromIffStream(engine::util::IffStream* iffStream, Version<'0001'>) {
		int number = iffStream->getSubChunksNumber();

		for (int i = 0; i < number; i++) {
			Segment* sgmt = new Segment();
			sgmt->readObject(iffStream);

			sgmts.add(sgmt);
		}
	}
	
	float segmentFind(Vector3 position, int idx) {
		return sgmts.get(idx)->find(position);
	}
	
	int getNumPoints(int idx) {
		return sgmts.get(idx)->getNumPoints();
	}


};



#endif /* HDTA_H_ */
