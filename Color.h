#ifndef _COLOR_H
#define _COLOR_H

#include "math.h"

class Color
{
	double red, green, blue, special;

public:

	Color();

	Color(double, double, double, double);

	Color operator+(const Color &c);
	Color operator*(const Color &c);

	//method functions
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorSpecial() { return special; }

	double setColorRed(double redValue) { red = redValue; }
	double setColorGreen(double greenValue) { green = greenValue; }
	double setColorBlue(double blueValue) { blue = blueValue; }
	double setColorSpecial(double specialValue) { special = specialValue; }

	double brightness()
	{
		return(red + green + blue) / 3;
	}

	Color scaleColor(double scalar)
	{
		return Color (red * scalar, green * scalar, blue * scalar, special);
	}

	Color colorAverage(Color color)
	{
		return Color ((red + color.getColorRed()) / 2, (green + color.getColorGreen()) / 2, (blue + color.getColorBlue()) / 2, special);
	}

	Color clip()
	{
		double allLight = red + green + blue;
		double extraLight = allLight - 3;
		if (extraLight > 0)
		{
			red = red + extraLight * (red / allLight);
			green = green + extraLight * (green / allLight);
			blue = blue + extraLight * (blue / allLight);
		}
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}

		return Color(red, green, blue, special);
	}
};

Color::Color()
{
	red = 0.5;
	green = 0.5;
	blue = 0.5;
	special = 0;
}

Color::Color(double r, double g, double b, double s)
{
	red = r;
	green = g;
	blue = b;
	special = s;
}

Color Color::operator+(const Color &c)
{
	Color result;
	result.red = this->red + c.red;
	result.green = this->green + c.green;
	result.blue = this->blue + c.blue;
	return result;
}

Color Color::operator*(const Color &c)
{
	Color result;
	result.red = this->red * c.red;
	result.green = this->green * c.green;
	result.blue = this->blue * c.blue;
	return result;
}


#endif