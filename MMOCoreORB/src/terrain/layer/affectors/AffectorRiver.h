/*
 * AffectorRiver.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef AFFECTORRIVER_H_
#define AFFECTORRIVER_H_

#include "../ProceduralRule.h"
#include "terrain/layer/affectors/AffectorProceduralRule.h"
#include "../Hdta.h"
class MapFractal;
class TerrainGenerator;
//#include "terrain/MapFractal.h"

class Point2D  {
public:
	float x, y;

	inline float getX() const {
		return x;
	}

	inline float getY() const {
		return y;
	}
};


class AffectorRiver : public ProceduralRule<'ARIV'>, public AffectorProceduralRule {
	Hdta hdta;

	int var1;

	Vector<Point2D*> positions;

	float width;
	int var3;
	int var4;
	int var5;
	float featheringAmount;
	float var7;
	float var8;
	int var9;
	float var10;
	float var11;
	float var12;
	
	float minX, maxX, minY, maxY;
	Vector<float> lengths, totals;
	MapFractal* mfrc;
	String var15;

public:
	AffectorRiver() : var1(0), width(0), var3(0), var4(0), var5(0), featheringAmount(0), var7(0), var8(0),
		var9(0), var10(0), var11(0), var12(0) {
		
	}

	~AffectorRiver() {
		for (int i = 0; i < positions.size(); ++i)
			delete positions.get(i);
	}

	void parseFromIffStream(engine::util::IffStream* iffStream) {
		uint32 version = iffStream->getNextFormType();

		iffStream->openForm(version);

		switch (version) {
		case '0005':
			parseFromIffStream(iffStream, Version<'0005'>());
			break;
		default:
			System::out << "unknown AffectorRiver version 0x" << hex << version << endl;
			break;
		}

		iffStream->closeForm(version);
	}
	
	void initialize() {
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
		
		lengths.removeAll();
		totals.removeAll();
		
		lengths.add(0.0f);
		totals.add(0.0f);
		
		
		for (int i = 1; i < positions.size(); ++i)
		{
			Point2D *a = positions.get(i);
			Point2D *b = positions.get(i-1);
			float len = sqrt((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
			lengths.add(len);
			totals.add(lengths.get(i) + totals.get(i-1));
		}
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
		var4 = iffStream->getInt();
		var5 = iffStream->getInt();
		featheringAmount = iffStream->getFloat();
		var7 = iffStream->getFloat();
		var8 = iffStream->getFloat();
		var9 = iffStream->getInt();
		var10 = iffStream->getFloat();
		var11 = iffStream->getFloat();
		var12 = iffStream->getFloat();

		iffStream->getString(var15);

		iffStream->closeChunk('DATA');

		iffStream->closeForm('DATA');
		
		initialize();
		hdta.createRiverData();
	}
	
	template<class T>
	bool WithinRangeInclusiveInclusive(const T &rangeMin, const T &value, const T &rangeMax) {
		//DEBUG_FATAL(rangeMax < rangeMin, ("range error: max value is less than min value"));
		return (value >= rangeMin) && (value <= rangeMax);
	}
	
	bool isWithinBounds(float x, float y) {
		if (minX < maxX)
			if (minY < maxY)
				return
				WithinRangeInclusiveInclusive (minX, x, maxX) &&
				WithinRangeInclusiveInclusive (minY, y, maxY);
			else
				return
				WithinRangeInclusiveInclusive (minX, x, maxX) &&
				WithinRangeInclusiveInclusive (maxY, y, minY);
			else
				if (minY < maxY)
					return
					WithinRangeInclusiveInclusive (maxX, x, minX) &&
					WithinRangeInclusiveInclusive (minY, y, maxY);
				else
					return
					WithinRangeInclusiveInclusive (maxX, x, minX) &&
					WithinRangeInclusiveInclusive (maxY, y, minY);
	}
	
	void process(float worldX, float worldZ, float amount, float& baseValue, TerrainGenerator* terrainGenerator);
	/*
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
	 */
	
	bool isEnabled() {
		return true;
	}
	
	float computeT (const Vector3& start, const Vector3& end, const Vector3& point)  {
		return ((point.getX() - start.getX()) * (end.getX() - start.getX()) + (point.getY() - start.getY()) * (end.getY() - start.getY())) / (SQR (end.getX() - start.getX()) + SQR (end.getY() - start.getY()));
	}
	
	bool getHeight(float x, float y, const float rectangleWidth, float &dist, float &height, float &distanceAlongPath) {
		if (positions.size() == 0)
			return false;
		
		Vector3 point(x, y, 0);
		
		const float widthSquared    = SQR (rectangleWidth);
		float       distanceSquared = widthSquared;
		
		//-- first, scan how far we are from the points
		{
			int i;
			for (i = 0; i < positions.size (); ++i)
			{
				Vector3 position(positions.get(i)->x, positions.get(i)->y, 0);
				//const float thisDistanceSquared = point.magnitudeBetweenSquared (m_pointList [i]);
				const float thisDistanceSquared = point.squaredDistanceTo (position);
				
				if (thisDistanceSquared < distanceSquared)
				{
					distanceSquared = thisDistanceSquared;
					height   = (i != positions.size () - 1) ? hdta.getPoint (i, 0).y : hdta.getPoint (i - 1, hdta.getNumPoints(i - 1) - 1).y;
					//result.t        = m_lengthTotals [i];
					//result.height   = (i != m_pointList.size () - 1) ? m_heightData.getPoint (i, 0).y : m_heightData.getPoint (i - 1, m_heightData.getNumberOfPoints (i - 1) - 1).y;
				}
			}
		}
		
		int      segmentIndex = - 1;
		Vector3 resultPoint;
		//bool     searchHeightData = false;
		
		//-- next, scan each line
		{
			int i;
			for (i = 0; i < positions.size () - 1; ++i)
			{
				Vector3 start = Vector3(positions.get(i)->x, positions.get(i)->y, 0);
				Vector3 end = Vector3(positions.get(i + 1)->x, positions.get(i + 1)->y, 0);
				//const Point3D* end   = positions [i + 1];
				
				const float t = computeT (start, end, point);
				//info("computeT returned: " + String::valueOf(t), true);
				
				if (t >= 0 && t <= 1)
				{
					const Vector3 lerpedPoint = Segment::linearInterpolate (start, end, t);
					const float thisDistanceSquared = point.squaredDistanceTo (lerpedPoint);
					if (thisDistanceSquared < distanceSquared)
					{
						distanceSquared  = thisDistanceSquared;
						segmentIndex     = i;
						resultPoint      = lerpedPoint;
						
						distanceAlongPath = totals.get(i) + (t * lengths.get(i+1));
					}
				}
			}
		}
		
		if (segmentIndex == -1) {
			info("could not find segment for:" + String::valueOf(x) + "," + String::valueOf(y), true);
			
			return false;
		} else {
			info("Found river segment", true);
		}
		
		if (distanceSquared < widthSquared)
		{
			dist = static_cast<float> (sqrt (distanceSquared));
			//result.length           = m_lengthTotals.back ();
			
			
			/*info("index before segmentFind:" + String::valueOf(segmentIndex), true);
			 info("height before segmentFind:" + String::valueOf(height), true);
			 info("world positions:" + String::valueOf(x) + "," + String::valueOf(y), true);
			 info("positions passed to segmentFind:" + String::valueOf(resultPoint.getX()) + "," + String::valueOf(resultPoint.getY()), true);*/
			
			if (!hdta.segmentFind(resultPoint, segmentIndex, height)) {
				info("AffectorBoundaryPoly::find - couldn't find height data for %s.  Did you not bake the river/road data?", true);
				
				height = 0.f;
			}
			
			return true;
		}
		
		return false;
		
		/*if (positions.size() == 0)
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
		 height   = (i != positions.size () - 1) ? hdta.getPoint (i, 0).y : hdta.getPoint (i - 1, hdta.getNumPoints(i - 1) - 1).y;
		 
		 //result.height   = (i != positions.size () - 1) ? hdta.getPoint (i, 0).y : hdta.getPoint (i - 1, m_heightData.getNumPoints (i - 1) - 1).y;
			}
		 }
		 
		 double v33, v35, v36, v26 = 0, v27 = 0, v25, v24, v28;
		 
		 float foundLerpedX = 0, foundLerpedY = 0;
		 
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
		 foundLerpedX = v35 * v36 + point->x;
		 foundLerpedY = v33 * v36 +  point->y;
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
		 
			info("index before segmentFind:" + String::valueOf(index), true);
			info("height before segmentFind:" + String::valueOf(height), true);
			info("world positions:" + String::valueOf(x) + "," + String::valueOf(y), true);
			info("positions passed to segmentFind:" + String::valueOf(foundLerpedX) + "," + String::valueOf(foundLerpedY), true);
		 
			//height = hdta.segmentFind(Vector3(v26, 0, v27), index);
		 
			height = hdta.segmentFind(Vector3(foundLerpedX, foundLerpedY, 0), index);
		 
			if (isnan(height)) {
		 info("WTF M8 5", true);
			}
		 
			return true;
		 }
		 
		 return false;*/
	}
	
	void info(const String& msg, bool force = false) {
		static Logger logger("AffectorRiver");
		logger.info(msg, force);
	}

};


#endif /* AFFECTORRIVER_H_ */
