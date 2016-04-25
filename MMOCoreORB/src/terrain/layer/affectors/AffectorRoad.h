/*
 * AffectorRoad.h
 *
 *  Created on: 01/02/2010
 *      Author: victor
 */

#ifndef AFFECTORROAD_H_
#define AFFECTORROAD_H_


#include "../ProceduralRule.h"
#include "../Hdta.h"
#include "AffectorRiver.h"

class AffectorRoad : public ProceduralRule<'AROA'>, public AffectorProceduralRule, public Logger {
	Hdta hdta;

	int var1;

	Vector<Point2D*> positions;

	float width;
	int var3;
	int featheringType;
	float featheringAmount;
	int var6;
	float var7;
	
	float minX, minY, maxX, maxY;

public:
	AffectorRoad() : Logger("AffectorRoad"), var1(0), width(0), var3(0), featheringType(0), featheringAmount(0), var6(0), var7(0) {
		affectorType = HEIGHTTYPE;
	}

	~AffectorRoad() {
		for (int i = 0; i < positions.size(); ++i)
			delete positions.get(i);
	}
	
	bool isEnabled() {
		return true;
	}
	
	bool isWithinBounds(float x, float y) {
		if ( x >= minX && x <= maxX) {
			if( y >= minY && y <= maxY) {
				return true;
			}
		}
		
		return false;
	}
	
	void process(float x, float y, float transformValue, float& baseValue, TerrainGenerator* terrainGenerator) {
		
		if (!isEnabled ())
			return;
		
		if (transformValue> 0.f)
		{
			if (isWithinBounds(x, y))
			{
				float  halfWidth = width/2.0f;
				
				if(x == 6716 && y == -5470) {
					info("What the fuck", true);
				}
				//
				float originalHeight = baseValue;
				float dist = 0.0f;
				float height = 0.0f;
				if (getHeight (x, y, halfWidth, dist, height))
				{
					if(dist < 0.0f)
						dist *= -1.0f;
					
					
					if(isnan(height)) {
						info("WTF M8 1", true);
					}
					
					float featherThreshold = (1.0f - featheringAmount) * halfWidth;
					
					if ( featherThreshold >= 0.0f && featherThreshold <= dist) {
						baseValue = height;
						if(isnan(baseValue)) {
							info("WTF M8 2");
						}
					}
					else {
						baseValue = height + (originalHeight - height) * (dist / halfWidth);
						if(isnan(baseValue)) {
							info("WTF M8 3");
						}
					}
				}
			}
		}

		if(isnan(baseValue))
			info("[" + String::valueOf(baseValue) + " ] <" + String::valueOf(x) + ", " + String::valueOf(y) + ">", true);
	}

	void parseFromIffStream(engine::util::IffStream* iffStream) {
		uint32 version = iffStream->getNextFormType();

		iffStream->openForm(version);

		switch (version) {
		case '0005':
			parseFromIffStream(iffStream, Version<'0005'>());
			break;
		default:
			System::out << "unknown AffectorRoad version 0x" << hex << version << endl;
			break;
		}

		iffStream->closeForm(version);
	}

	void parseFromIffStream(engine::util::IffStream* iffStream, Version<'0005'>) {
		informationHeader.readObject(iffStream);

		iffStream->openForm('DATA');

		uint32 type = iffStream->getNextFormType();

		switch (type) {
		case ('ROAD'):
		case ('HDTA'):
			hdta.readObject(iffStream);
			break;
		default:
			System::out << "Unknown type in AffectorRoad, expecting ROAD or HDTA!\n";
			break;
		}

		iffStream->openChunk('DATA');

		var1 = iffStream->getInt();

		for (int i = 0; i < var1; i++) {
			Point2D* pos = new Point2D();
			pos->x = iffStream->getFloat();
			pos->y = iffStream->getFloat();
			positions.add(pos);
		}

		width = iffStream->getFloat();
	
		var3 = iffStream->getInt();
		featheringType = iffStream->getInt();
		featheringAmount = iffStream->getFloat();
		var6 = iffStream->getInt();
		var7 = iffStream->getFloat();
		
		calculateBoundary();

		iffStream->closeChunk('DATA');

		iffStream->closeForm('DATA');
	}
	
	void calculateBoundary() {
		minX = 800000000;
		minY = 800000000;
		
		maxX = -80000000;
		maxY = -80000000;
		
		for(int i = 0; i < positions.size(); ++i) {
			Point2D* point = positions.get(i);
			
			if (point->x < minX)
				minX = point->x;
			
			if (point->y < minY)
				minY = point->y;
			
			if (point->x > maxX)
				maxX = point->x;
			
			if (point->y > maxY)
				maxY = point->y;
		}
		
		minX = minX - width;
		maxX = maxX + width;
		minY = minY - width;
		maxY = maxY + width;
	}
	
	bool getHeight(float x, float y, const float width, float &dist, float &height)
	{
		if (positions.size() == 0)
			return false;
		
		double v16 = width * width;
		double v21 = 0;
		
		double v31 = v16;
		double v20, v19;
		
		double result = 0;

		for (int i=0; i < positions.size(); ++i) {
			Point2D* point = positions.get(i);
			
			v20 = y - point->y;
			v19 = v20 * v20 + (x - point->x) * (x - point->x);
			if ( v19 < v16 ) {
				v21 = v19;
				v16 = v21;
				
				//result.t        = m_lengthTotals [i];
				height   = (i != positions.size () - 1) ? hdta.getPoint (i, 0)->y : hdta.getPoint (i - 1, hdta.getNumPoints(i - 1) - 1)->y;
			}
		}
		
		double v33, v35, v36, v26, v27, v25, v24, v28;
		int index = 0;
		for (int i = 0; i < positions.size() - 1; i++) {
			const Point2D* point = positions.get(i);
			const Point2D* point2 = positions.get(i + 1);
			
			v35 = point2->x - point->x;
			v24 = point2->y - point->y;
			v33 = v24;
			v36 = ((y -  point->y) * v33 + (x - point->x) * v35) / (v24 * v24 + v35 * v35);
			
			if ( v36 >= 0.0 ) {
				if ( v36 <= 1.0 ) {
					v26 = x - (v35 * v36 + point->x); //lerpx
					v27 = y - (v33 * v36 +  point->y); //lerpy
					v25 = v27 * v27 + v26 * v26; //dist
					
					if ( v25 < v16 ) {
						v16 = v25; // dist
						index = i;
						//result.t         = m_lengthTotals [i] + (v36 * m_lengths [i + 1]);
					}
				}
			}
			
		}
		
		if ( v16 < v31 ) {
			dist = static_cast<float> (sqrt (v16));
			//result.length           = m_lengthTotals.back ();
			if(isnan(height)) {
				info("WTF M8 4", true);
			}
			height = hdta.segmentFind(Vector3(v26, 0, v27), index);
			
			if(isnan(height)) {
				info("WTF M8 5", true);
			}
			return true;
		}
		
		return false;
	}
};

#endif /* AFFECTORROAD_H_ */
