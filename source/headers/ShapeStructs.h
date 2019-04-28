#pragma once

#include "Defines.h"
#include "Lab.h"
#include "Shape.h"

struct ReferencePlane:Shape
{
	ReferencePlane( double planeWidth = 8.0, 
					color c1 = color(0.0, 0.0, 0.0, 1.0), 
					color c2 = color(1.0, 1.0, 1.0, 1.0));

	color color1;
	color color2;

};


struct Pyramid:Shape
{
	Pyramid(color pyColor, double width = 1.0, double height = 1.0);

};


struct Sphere:Shape
{
	Sphere( color col, double radius = 0.5f, int slices = 16, int stacks = 16 );

	Material material;

	protected:

	void initializeTop( );
	void initializeBody( );
	void initializeBottom( );

	int stacks;
	int slices;
	double radius;

	std::vector<double> stackAngles;
	std::vector<double> sliceAngles;

};




