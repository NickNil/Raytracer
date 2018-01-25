#ifndef _RAY_H
#define _RAY_H

#include "Vector3d.h"

class Ray
{
	Vector3d origin, direction;

public:

	Ray();

	Ray(Vector3d, Vector3d);

	//method functions
	Vector3d getRayOrigin() { return origin; }
	Vector3d getRayDirection() { return direction; }
};

Ray::Ray()
{
	origin = Vector3d(0,0,0);
	direction = Vector3d(1,0,0);
}

Ray::Ray(Vector3d o, Vector3d d)
{
	origin = o;
	direction = d;
}

#endif