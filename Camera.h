#ifndef _CAMERA_H
#define _CAMERA_H

#include "Vector3d.h"

class Camera
{
	Vector3d campos, camdir, camright, camdown;

public:

	Camera();

	Camera(Vector3d, Vector3d, Vector3d, Vector3d);

	//method functions
	Vector3d getCameraPosition() { return campos; }
	Vector3d getCameraDirection() { return camdir; } //front of camera, direction it is facing
	Vector3d getCameraRight() { return camright; } //the right side of the camera, helps define orientation
	Vector3d getCameraDown() { return camdown; } //the bottom of the camera, helps define orientation
};

Camera::Camera()
{
	campos = Vector3d(0,0,0);
	camdir = Vector3d(0,0,1);
	camright = Vector3d(0,0,0);
	camdown = Vector3d(0,0,0);
}

Camera::Camera(Vector3d pos, Vector3d dir, Vector3d right, Vector3d down)
{
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;
}

#endif