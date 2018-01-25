#ifndef _SPHERE_H
#define _SPHERE_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Math.h"
#include "Object.h"
#include "Vector3d.h"
#include "Color.h"

using namespace std;

class Sphere : public Object
{
	Vector3d center;
	double radius;
	Color color;

public:

	Sphere();

	Sphere(Vector3d, double, Color);

	//method functions
	Vector3d getSphereCenter() { return center; }
	double getSphereRadius() { return radius; }
	Color getColor() { return color; }

	Vector3d getNormalAt(Vector3d point)
	{
		// normal always points away from center point
		Vector3d normalVector = (point + center.negative()).normalize();
		return normalVector;
	}

	double findIntersection(Ray ray) //distance from ray origin to intersection
	{


		// a = 1
		// b = 2OD (O = rayorigin, D = direction)
		// c = O^2 - R^2 (R = radius)
		//cout << "ray direction1: " << ray.getRayDirection().getVector3dX() << ", " << ray.getRayDirection().getVector3dY() << ", " << ray.getRayDirection().getVector3dZ() << endl;

		Vector3d rayOrigin = ray.getRayOrigin();
		double rayOriginX = rayOrigin.getVector3dX();
		double rayOriginY = rayOrigin.getVector3dY();
		double rayOriginZ = rayOrigin.getVector3dZ();

		//cout << "ray origin:" << rayOriginX << ", " << rayOriginY << ", " << rayOriginZ << endl;

		Vector3d rayDirection = ray.getRayDirection();
		double rayDirectionX = rayDirection.getVector3dX();
		double rayDirectionY = rayDirection.getVector3dY();
		double rayDirectionZ = rayDirection.getVector3dZ();

		//cout << "ray direction2:" << rayDirectionX << ", " << rayDirectionY << ", " << rayDirectionZ << endl;

		//cout << "radius: " << radius << endl;

		Vector3d sphereCenter = center;
		double sphereCenterX = sphereCenter.getVector3dX();
		double sphereCenterY = sphereCenter.getVector3dY();
		double sphereCenterZ = sphereCenter.getVector3dZ();

		double a = 1; //normalized
		double bX = 2*(rayOriginX - sphereCenterX)*rayDirectionX;
		double bY = 2*(rayOriginY - sphereCenterY)*rayDirectionY;
		double bZ = 2*(rayOriginZ - sphereCenterZ)*rayDirectionZ;
		double b = bX + bY + bZ;
		double cX = pow(rayOriginX - sphereCenterX, 2);
		double cY = pow(rayOriginY - sphereCenterY, 2);
		double cZ = pow(rayOriginZ - sphereCenterZ, 2);
		double c = cX + cY + cZ - (radius * radius);

		double discriminant = b*b - 4*c; //quadratic function

		if (discriminant > 0)
		{
			// the ray intersects the sphere

			// first root
			double root1 = ((-1 * b - sqrt(discriminant))/2) - 0.000001; //0.00...001 apparently helps with acuracy
			
			if (root1 > 0)
			{
				// the first root is the smallest positive root
				return root1;
			}
			else
			{
				//the second root is the smallest positive root
				double root2 = ((sqrt(discriminant) - b)/2) - 0.000001;
				return root2;
			}
		}
		/*else if (discriminant == 0)
		{
			//one root
			return -b/(2*a);
		}*/
		else
		{
			//ray missed the sphere
			return -1;
		}
	}
};

Sphere::Sphere()
{
	center = Vector3d(0,0,0);
	radius = 1;
	color = Color(0.5,0.5,0.5,0);
}

Sphere::Sphere(Vector3d centerValue, double radiusValue, Color colorValue)
{
	center = centerValue;
	radius = radiusValue;
	color = colorValue;
}

#endif