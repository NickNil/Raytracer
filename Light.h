#ifndef _LIGHT_H
#define _LIGHT_H

#include "Vector3d.h"
#include "Color.h"

class Light
{
	Vector3d position;
	Color color;

public:

	Light();

	Light(Vector3d, Color);

	//method functions
	Vector3d getLightPosition() { return position; }
	Color getColor() { return color; }
};

Light::Light()
{
	position = Vector3d(0,0,0);
	color = Color(1,1,1,0);
}

Light::Light(Vector3d p, Color c)
{
	position = p;
	color = c;
}

#endif