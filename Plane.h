#ifndef _PLANE_H
#define _PLANE_H

#include "Math.h"
#include "Object.h"
#include "Vector3d.h"
#include "Color.h"

class Plane : public Object
{
	Vector3d normal;
	double distance;
	Color color;

public:

	Plane();

	Plane(Vector3d, double, Color);

	//method functions
	Vector3d getPlaneNormal() { return normal; }
	double getPlaneDistance() { return distance; }
	Color getColor() { return color; }

	Vector3d getNormalAt(Vector3d point)
	{
		return normal;
	}

	double findIntersection(Ray ray) //distance from ray origin to intersection point
	{
		//normal*distance = plane position
		//distance = -((ray origin-plane position) dot normal)/(ray direction dot normal)
		Vector3d rayDirection = ray.getRayDirection();

		double a = rayDirection.dotProduct(normal); 

		if (a == 0) //ray is paralell
		{
			return -1;
		}
		else //
		{
			//b = normal(dot)(rayOrigin-(normal*distance))
			double b = normal.dotProduct(ray.getRayOrigin() + ((normal * distance).negative()));
			return -1*b/a; //distance from ray origin to intersection
		}
	}
};

Plane::Plane()
{
	normal = Vector3d(1,0,0);
	distance = 0;
	color = Color(0.5,0.5,0.5,0);
}

Plane::Plane(Vector3d normalValue, double distanceValue, Color colorValue)
{
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif