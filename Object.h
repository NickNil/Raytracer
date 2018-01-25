#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vector3d.h"
#include "Color.h"

class Object
{
public:

	Object();

	//method functions
	virtual Color getColor() { return Color(0,0,0,0); }
	virtual Vector3d getNormalAt(Vector3d point){ return Vector3d(0,0,0); }


	virtual double findIntersection(Ray ray) 
	{
		return 0;
	}
};

Object::Object() { }

#endif