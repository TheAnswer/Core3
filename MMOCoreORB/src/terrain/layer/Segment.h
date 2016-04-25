/*
 * Sgmt.h
 *
 *  Created on: 31/01/2010
 *      Author: victor
 */

#ifndef SGMT_H_
#define SGMT_H_
#include "engine/util/u3d/Vector3.h"
#include "../TemplateVariable.h"
#include <vector>

template<class T>inline const T clamp(const T &minT, const T &t, const T &maxT) {
	return (t < minT) ? minT : ((t > maxT) ? maxT : t);
}
class Point3D {
public:
	float x, z, y;
};

class Segment : public TemplateVariable<'SGMT'> {
	Vector<Point3D*> positions;

public:
	~Segment() {
		for (int i = 0; i < positions.size(); ++i)
			delete positions.get(i);
	}
	
	Point3D* getPoint(int index) {
		return positions.get(index);
	}
	
	int getNumPoints() {
		return positions.size();
	}

	void readObject(engine::util::IffStream* iffStream) {
		iffStream->openChunk('SGMT');

		int count = iffStream->getDataSize() / 12;

		for (int i = 0; i < count; i++) {
			Point3D* position = new Point3D();
			position->x = iffStream->getFloat();
			position->y = iffStream->getFloat();
			position->z = iffStream->getFloat();
			
			positions.add(position);
			
			if ( i > 2) {
				Point3D* a = positions.get(i-2);
				Point3D* b = positions.get(i-1);
				Point3D* c = position;
				a->y = (a->y + b->y + c->y) / 3.0f;
			}
		}

		iffStream->closeChunk('SGMT');
	}
	float linearInterpolate(float a, float b, float c) {
		return a + (b-a) * c;
	}
	float find (Vector3 position)
	{
		
		std::vector<Point3D*> m_pointList;
		
		for (int i=0; i<positions.size(); i++) {
			m_pointList.push_back(positions.get(i));
		}
		
#if 1
		{
			const float x0 = MIN((*m_pointList.begin())->x, m_pointList.back()->x);
			const float z0 = MIN((*m_pointList.begin())->z, m_pointList.back()->z);
			const float x1 = MAX((*m_pointList.begin())->x, m_pointList.back()->x);
			const float z1 = MAX((*m_pointList.begin())->z, m_pointList.back()->z);
			//		DEBUG_FATAL (position.x < x0 || position.x > x1 || position.y < z0 || position.y > z1, ("position out of range"));
			position.setX(clamp(x0, position.getX(), x1));
			position.setY(clamp(z0, position.getY(), z1));
			
			// clamp position to edges of AABB
		}
#endif
		
		float result;
		const float width  = m_pointList.back()->x - (*m_pointList.begin())->x;
		const float height = m_pointList.back()->z - (*m_pointList.begin())->z;
		
		if (fabsf (width) >= fabsf (height))
		{
			if (width >= 0)
			{
				//-- go from -x to +x
				std::vector<Point3D*>::const_iterator current  = m_pointList.begin ();
				std::vector<Point3D*>::const_iterator previous = current;
				
				for (; current != m_pointList.end () && (*current)->x < position.getX(); previous = current, ++current);
				
				const float t = (position.getX() - (*previous)->x) / ((*current)->x - (*previous)->x);
				result = linearInterpolate ((*previous)->y, (*current)->y, t);
			}
			else
			{
				//-- go from +x to -x
				std::vector<Point3D*>::const_reverse_iterator current  = m_pointList.rbegin ();
				std::vector<Point3D*>::const_reverse_iterator previous = current;
				
				for (; current != m_pointList.rend () && (*current)->x < position.getX(); previous = current, ++current);
				
				const float t = (position.getX() - (*previous)->x) / ((*current)->x - (*previous)->x);
				result = linearInterpolate ((*previous)->y, (*current)->y, t);
			}
		}
		else
		{
			if (height >= 0)
			{
				//-- go from -z to +z
				std::vector<Point3D*>::const_iterator current  = m_pointList.begin ();
				std::vector<Point3D*>::const_iterator previous = current;
				
				for (; current != m_pointList.end () && (*current)->z < position.getY(); previous = current, ++current);
				
				const float t = (position.getY() - (*previous)->z) / ((*current)->z - (*previous)->z);
				result = linearInterpolate ((*previous)->y, (*current)->y, t);
			}
			else
			{
				//-- go from +z to -z
				std::vector<Point3D*>::const_reverse_iterator current  = m_pointList.rbegin ();
				std::vector<Point3D*>::const_reverse_iterator previous = current;
				
				for (; current != m_pointList.rend () && (*current)->z < position.getY(); previous = current, ++current);
				
				const float t = (position.getY() - (*previous)->z) / ((*current)->z - (*previous)->z);
				result = linearInterpolate ((*previous)->y, (*current)->y, t);
			}
		}
		
		if(isnan(result)) {
			Logger logger;
			logger.info("WHAT THE FUCK", true);
		}
		
		return result;
	}
};


#endif /* SGMT_H_ */
