#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <Vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Vector3d.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"

using namespace std;

struct RGBType
{
	double r;
	double g;
	double b;
};

void savebmp (const char *filename, int w, int h, int dpi, RGBType *data)
{
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);
	
	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);
	
	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);
	
	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);
	
	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);
	
	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);

	f = fopen(filename,"wb");

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for (int i = 0; i < k; i++)
	{
		RGBType rgb = data[i];

		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {(int) floor(blue), (int) floor(green), (int) floor(red)};

		fwrite(color,1,3,f);
	}

	fclose(f);
}

int closestObjectIndex(std::vector<double> objectIntersections)
{
	//return the index of the closest intersection
	int indexOfMinValue;

	//preventing unnessesary calculations by checking for intersections
	if (objectIntersections.size() == 0)	
	{
		//if no intersections
		return -1;
	}
	else if (objectIntersections.size() == 1) //if one intersection
	{
		if (objectIntersections.at(0) > 0)
		{
			// if that intersection is greater than zero it is a valid intersection
			return 0;
		}
		else
		{
			// intersection value is negative, ray missed
			return -1;
		}
	}
	else // multiple intersections from that ray
	{
		//finding max value

		double max = 0;
		for (int i = 0; i < objectIntersections.size(); i++)
		{
			if (max < objectIntersections.at(i))
			{
				max = objectIntersections.at(i);
			}
		}

		//then fom the max value, find the min
		if (max > 0) //positive intersection
		{
			for (int j = 0; j < objectIntersections.size(); j++)
			{
				if (objectIntersections.at(j) > 0 && objectIntersections.at(j) <= max)
				{
					max = objectIntersections.at(j);
					indexOfMinValue = j;
				}
			}

			return indexOfMinValue;
		}
		else
		{
			// all intersections were negative
			return -1;
		}
	}
}

Color getColorAt(Vector3d intersectionPosition, Vector3d intersectionRayDirection, vector<Object*> sceneObjects, int indexOfClosestObject, 
						vector<Light*> lightSources, double accuracy, double ambientLight, int recursiveCounter)
{
	Color closestObjectColor = sceneObjects.at(indexOfClosestObject)->getColor();
	Vector3d closestObjectNormal = sceneObjects.at(indexOfClosestObject)->getNormalAt(intersectionPosition);

	if (closestObjectColor.getColorSpecial() == 2)
	{
		int square = (int)floor(intersectionPosition.getVector3dX()) + (int)floor(intersectionPosition.getVector3dZ());
	
		if ((square % 2) == 0)
		{
			closestObjectColor.setColorRed(0);
			closestObjectColor.setColorGreen(0);
			closestObjectColor.setColorBlue(0);
		}
		else
		{
			closestObjectColor.setColorRed(1);
			closestObjectColor.setColorGreen(1);
			closestObjectColor.setColorBlue(1);
		}
	}

	Color finalColor = closestObjectColor.scaleColor(ambientLight); //scale final color by ambient light factor

	if (closestObjectColor.getColorSpecial() > 0 && closestObjectColor.getColorSpecial() <= 1)
	{
		// reflections of objects with specular value
		double dot = closestObjectNormal.dotProduct(intersectionRayDirection.negative());
		Vector3d mult1 = closestObjectNormal * dot;
		Vector3d add1 = mult1  + intersectionRayDirection;
		Vector3d mult2 = add1 * 2;
		Vector3d add2 = intersectionRayDirection.negative() + mult2;
		Vector3d reflectionDirection = add2.normalize();

		Ray reflectionRay(intersectionPosition, reflectionDirection);

		//find first ray intersection
		vector<double> reflectionIntersections;

		for (int reflectionIndex = 0; reflectionIndex < sceneObjects.size(); reflectionIndex++)
		{
			reflectionIntersections.push_back(sceneObjects.at(reflectionIndex)->findIntersection(reflectionRay));
		}

		int indexOfClosestObjectWithReflection = closestObjectIndex(reflectionIntersections);

		if (indexOfClosestObjectWithReflection != -1)
		{
			if (reflectionIntersections.at(indexOfClosestObjectWithReflection) > accuracy)
			{
				//find position and direction at reflection ray intersection

				Vector3d reflectionIntersectionPosition = intersectionPosition + (reflectionDirection * reflectionIntersections.at(indexOfClosestObjectWithReflection));
				Vector3d reflectionIntersectionRayDirection = reflectionDirection;
				Color reflectionIntersectionColor; 
				if (recursiveCounter <= 10)
				{
					recursiveCounter++;
					reflectionIntersectionColor = getColorAt(reflectionIntersectionPosition, reflectionIntersectionRayDirection, sceneObjects, 
						indexOfClosestObjectWithReflection, lightSources, accuracy, ambientLight, recursiveCounter);
				}
				else	
				{
					recursiveCounter = 0;
					reflectionIntersectionColor = Color(0,0,0,0);
				}
				finalColor = finalColor + reflectionIntersectionColor.scaleColor(closestObjectColor.getColorSpecial());
			}
		}
	}

	for (int lightIndex = 0; lightIndex < lightSources.size(); lightIndex++)
	{
		Vector3d lightDirection = lightSources.at(lightIndex)->getLightPosition();
		lightDirection = (lightDirection + intersectionPosition.negative()).normalize();

		float cosineAngle = closestObjectNormal.dotProduct(lightDirection); //Angle between vectors

		if (cosineAngle > 0) // test for shadows
		{
			
			bool shadowed = false; //not shadowed by default

			Vector3d distanceToLight = lightSources.at(lightIndex)->getLightPosition();
			distanceToLight = distanceToLight + intersectionPosition.negative();
			float distanceToLightMagnitude = distanceToLight.magnitude(); //distance to lightsource from intersection point
			
			Vector3d DirectionToLight = lightSources.at(lightIndex)->getLightPosition();
			DirectionToLight = (DirectionToLight + intersectionPosition.negative()).normalize();

			//creating secondary shadow ray towards the lightsources
			Ray shadowRay(intersectionPosition, DirectionToLight);

			vector<double> secondaryIntersections;

			//find shadow ray intersections
			for (int objectIndex = 0; objectIndex < sceneObjects.size() && shadowed == false; objectIndex++)
			{
				secondaryIntersections.push_back(sceneObjects.at(objectIndex)->findIntersection(shadowRay));
			}

			//if the shadow ray intersects with anything before hitting the lightsource the original intersection is shadowed
			for (int i = 0; i < secondaryIntersections.size(); i++)
			{
				if (secondaryIntersections.at(i) > accuracy)
				{
					if (secondaryIntersections.at(i) <= distanceToLightMagnitude)
					{
						shadowed = true;
					}
					break;
				}
			}

			if (shadowed == false)
			{
				//final color is affected by the objects color as well as light color scaled with cosine angle
				finalColor = finalColor + ((closestObjectColor * lightSources.at(lightIndex)->getColor()).scaleColor(cosineAngle));
			
				//if special is between 0 and 1, special refers to shinyness of the object
				if (closestObjectColor.getColorSpecial() > 0 && closestObjectColor.getColorSpecial() <= 1)
				{
					double dot = closestObjectNormal.dotProduct(intersectionRayDirection.negative());
					Vector3d  mult1 = closestObjectNormal * dot;
					Vector3d add1 = mult1 + intersectionRayDirection;
					Vector3d mult2 = add1 * 2;
					Vector3d add2 = intersectionRayDirection.negative() + mult2;
					Vector3d reflectionDirection = add2.normalize();

					double specular = reflectionDirection.dotProduct(lightDirection);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						finalColor = finalColor + lightSources.at(lightIndex)->getColor().scaleColor(specular * closestObjectColor.getColorSpecial());
					}
				}
			}

		}
	}
	return finalColor.clip();
}

int main (int argc, char *argv[]) 
{
	cout << "casting rays..." << endl;

	clock_t t1, t2;
	t1 = clock();

	int dpi = 72;
	int width = 640;
	int height = 480;
	
	int aaDepth = 4; //1 = 4 rays to average color (2 = 8 and so on)
	double aaThreshold = 0.1;

	double aspectratio = (double)width/(double)height;
	double ambientLight = 0.2;
	//makes sure the intersection point is outside of a given surface, not inside
	double accuracy = 0.000001; 

	int n = width*height;
	RGBType *pixels = new RGBType[n];

	Vector3d O(0.0, 0.0, 0.0);
	Vector3d X(1, 0, 0);
	Vector3d Y(0, 1, 0);
	Vector3d Z(0, 0, 1);

	Vector3d campos(5, 1, -4);

	Vector3d look_at (0.0, 0.0, 0.0); //point camera looks at
	Vector3d difference (campos.getVector3dX() - look_at.getVector3dX(), campos.getVector3dY() - look_at.getVector3dY(), campos.getVector3dZ() - look_at.getVector3dZ());

	Vector3d camdir = difference.negative().normalize();

	Vector3d camright = Y.crossProduct(camdir).normalize();
	Vector3d camdown = camright.crossProduct(camdir);
	Camera sceneCam(campos, camdir, camright, camdown);

	Color white (1.0, 1.0, 1.0, 0.5);
	Color lightGreen (0.5, 1.0, 0.5, 0.1);
	Color maroon (0.5, 0.25, 0.25, 0);
	Color gray (0.5, 0.5, 0.5, 0);
	Color black (0.0, 0.0, 0.0, 0);
	Color lightBlue(0.5, 0.5, 1.0, 0.5);
	Color checkerBoard(0, 0, 0, 2);

	// light sources
	Vector3d lightPosition(-7, 10, -10);
	Vector3d lightPosition2(7, 10, 10);
	Light sceneLight(lightPosition, gray);
	Light sceneLight2(lightPosition2, lightBlue);

	vector<Light*> lightSources;
	lightSources.push_back(dynamic_cast<Light*>(&sceneLight));
	lightSources.push_back(dynamic_cast<Light*>(&sceneLight2));

	// scene objects
	Vector3d spherePos2(0, 0.5, 3);
	Vector3d spherePos3(8, 1, -6);
	Sphere sceneSphere2(spherePos2, 1.5, white);
	Sphere sceneSphere3(spherePos3, 2, maroon);
	Sphere sceneSphere(O, 1, lightGreen);
	Plane scenePlane(Y, -1, checkerBoard);

	vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere2));
	sceneObjects.push_back(dynamic_cast<Object*>(&sceneSphere3));
	sceneObjects.push_back(dynamic_cast<Object*>(&scenePlane));

	int pixelIndex, aaIndex;
	double xOffset, yOffset; //offset of the center pixel
	double tempRed, tempGreen, tempBlue;

	 //loop through every pixel in the image
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) //at specific pixel
		{
			pixelIndex = y*width + x;

			double tempRed[aaDepth*aaDepth];
			double tempGreen[aaDepth*aaDepth];
			double tempBlue[aaDepth*aaDepth];

			for (int aax = 0; aax < aaDepth; aax++)
			{
				for (int aay = 0; aay < aaDepth; aay++)
				{
					aaIndex = aay * aaDepth + aax;

					//create ray from camera to this pixel
					if (aaDepth == 1)
					{
							// no anti-aliasing
						if (width > height)
						{
							// the image is wider than it is tall
							xOffset = ((x + 0.5)/width)*aspectratio - (((width-height)/(double)height)/2);
							yOffset = ((height - y) + 0.5)/height;
						}
						else if(height > width)
						{
							// the image is taller than it is wide
							xOffset = (x + 0.5)/width;
							yOffset = (((height - y) + 0.5)/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else
						{
							//the image is a square
							xOffset = (x + 0.5)/width;
							yOffset = ((height - y) + 0.5)/height;
						}
					}
					else //with anti aliasing
					{
						if (width > height)
						{
							// the image is wider than it is tall
							xOffset = ((x + (double)aax/((double)aaDepth - 1))/width)*aspectratio - (((width-height)/(double)height)/2);
							yOffset = ((height - y) + (double)aax/((double)aaDepth - 1))/height;
						}
						else if(height > width)
						{
							// the image is taller than it is wide
							xOffset = (x + (double)aax/((double)aaDepth - 1))/width;
							yOffset = (((height - y) + (double)aax/((double)aaDepth - 1))/height)/aspectratio - (((height - width)/(double)width)/2);
						}
						else
						{
							//the image is a square
							xOffset = (x + (double)aax/((double)aaDepth - 1))/width;
							yOffset = ((height - y) + (double)aax/((double)aaDepth - 1))/height;
						}
					}

					//creating rays
					Vector3d camRayOrigin = sceneCam.getCameraPosition();
					//using offset value to get correct ray direction

					Vector3d step1 = camdown * (yOffset - 0.5);
					Vector3d step2 = camright * (xOffset - 0.5);
					Vector3d step3 = step2 + step1;
					Vector3d step4 = camdir + step3;
					Vector3d camRayDirection = step4.normalize();

					//Vector3d camRayDirection = camdir.vector3dAdd(camright.vector3dMult(xOffset - 0.5).vector3dAdd(camdown.vector3dMult(yOffset - 0.5))).normalize();

					Ray camRay(camRayOrigin, camRayDirection);


					vector<double> intersections;

					//does ray intersect?
					for (int index = 0; index < sceneObjects.size(); index++)
					{
						//finds intersections in object pointed to by scene objects vector and pushes it into intersections vector
						intersections.push_back(sceneObjects.at(index)->findIntersection(camRay));
					}

					int indexOfClosestObject = closestObjectIndex(intersections);

					//cout << "index: " << indexOfClosestObject << endl << endl;

					if (indexOfClosestObject == -1) // set background color
					{
						tempRed[aaIndex] = 0;
						tempGreen[aaIndex] = 0;
						tempBlue[aaIndex] = 0;
					}
					else //index corresponds to object 
					{
						if(intersections.at(indexOfClosestObject) > accuracy)
						{
							//find position and direction of vectors at intersection point
							Vector3d intersectionPosition = camRayOrigin + (camRayDirection * intersections.at(indexOfClosestObject));
							Vector3d intersectionRayDirection = camRayDirection;

							Color intersectionColor = getColorAt(intersectionPosition, intersectionRayDirection, sceneObjects, indexOfClosestObject, 
								lightSources, accuracy, ambientLight, 0);

							tempRed[aaIndex] = intersectionColor.getColorRed();
							tempGreen[aaIndex] = intersectionColor.getColorGreen();
							tempBlue[aaIndex] = intersectionColor.getColorBlue();
						}

					}
					
				}
			}

			// average the pixel color
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for (int iRed = 0; iRed < aaDepth*aaDepth; iRed++)
			{
				totalRed = totalRed + tempRed[iRed];
			}
			for (int iGreen = 0; iGreen < aaDepth*aaDepth; iGreen++)
			{
				totalGreen = totalGreen + tempGreen[iGreen];
			}
			for (int iBlue = 0; iBlue < aaDepth*aaDepth; iBlue++)
			{
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed/(aaDepth*aaDepth);
			double avgGreen = totalGreen/(aaDepth*aaDepth);
			double avgBlue = totalBlue/(aaDepth*aaDepth);

			pixels[pixelIndex].r = avgRed;
			pixels[pixelIndex].g = avgGreen;
			pixels[pixelIndex].b = avgBlue;
		}
	}

	savebmp("scene_with_aa.bmp",width,height,dpi,pixels);

	delete pixels, tempRed, tempGreen, tempBlue;

	t2 = clock();
	float tDiff = ((float)t2 - (float)t1) / 1000;

	cout << tDiff << " seconds" << endl;

	return 0;
}
	