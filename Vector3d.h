#ifndef _VECTOR3D_H
#define _VECTOR3D_H

#include "math.h"

class Vector3d
{
	double x, y, z;

public:

	Vector3d();

	Vector3d(double, double, double);

	Vector3d operator+(const Vector3d &v);
	Vector3d operator*(double scalar);

	//method functions
	double getVector3dX() { return x; }
	double getVector3dY() { return y; }
	double getVector3dZ() { return z; }

	double magnitude() {
		return sqrt((x*x) + (y*y) + (z*z));
	}

	Vector3d normalize() {
		double mag = magnitude();
		return Vector3d (x/mag, y/mag, z/mag);
	}

	Vector3d negative() {
		return Vector3d(-x, -y, -z);
	}

	double dotProduct(Vector3d v) {
		return x*v.getVector3dX() + y*v.getVector3dY() + z*v.getVector3dZ();
	}

	Vector3d crossProduct(Vector3d v) {
		return Vector3d (y*v.getVector3dZ() - z*v.getVector3dY(), z*v.getVector3dX() - x*v.getVector3dZ(), x*v.getVector3dY() - y*v.getVector3dX());
	}

	/*Vector3d vector3dAdd(Vector3d v) {
		return Vector3d (x + v.getVector3dX(), y + v.getVector3dY(), z + v.getVector3dZ());
	}

	Vector3d vector3dMult(double scalar) {
		return Vector3d (x*scalar, y*scalar, z*scalar);
	}*/
};

Vector3d::Vector3d()
{
	x = 0;
	y = 0;
	z = 0;
}

Vector3d::Vector3d(double i, double j, double k)
{
	x = i;
	y = j;
	z = k;
}

Vector3d Vector3d::operator+(const Vector3d &v)
{
	Vector3d result;
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	result.z = this->z + v.z;
	return result;
}

Vector3d Vector3d::operator*(double scalar)
{
	Vector3d result;
	result.x = this->x * scalar;
	result.y = this->y * scalar;
	result.z = this->z * scalar;
	return result;
}


#endif

