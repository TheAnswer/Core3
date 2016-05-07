/*
 * BaseBoundingVolume.h
 *
 *  Created on: 03/20/2016
 *      Author: gslomin
 */

#ifndef BASEBOUNDINGVOLUME_H_
#define BASEBOUNDINGVOLUME_H_

#include "engine/engine.h"
#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgGA/StateSetManipulator>

class BaseBoundingVolume : public Logger {
protected:
	Sphere bsphere; // All volumes have an enclosing sphere
public:
	BaseBoundingVolume() : bsphere(Vector3(0, 0, 0), 0) { }
	virtual const AABB& getBoundingBox() = 0;
	virtual const Sphere& getBoundingSphere() {
		return bsphere;
	}
	
	//virtual osg::ref_ptr<osg::Node> draw() const = 0;
	
	virtual bool isBoundingBox() { return false; }
	virtual bool isCollisionMesh() { return false; }
	virtual bool isCompositeVolume() { return false; }
	virtual bool isComponentVolume() { return false; }
	virtual bool isDetailVolume() { return false; }
	
	virtual void read(IffStream *iff);
};

#endif /* BASEBOUNDINGVOLUME_H_ */
