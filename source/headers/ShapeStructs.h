#pragma once

#include "Defines.h"
#include "Lab.h"
#include "VertexData.h"

struct ReferencePlane
{
	ReferencePlane( double planeWidth = 8.0, 
					color c1 = color(0.0, 0.0, 0.0, 1.0), 
					color c2 = color(1.0, 1.0, 1.0, 1.0));

	color color1;
	color color2;

	std::vector<VertexData> c1PlaneVertices;
	std::vector<VertexData> c2PlaneVertices;

};


struct Pyramid
{
	Pyramid(color pyColor, double width = 1.0, double height = 1.0);

	std::vector<VertexData> triangleVertices;
};


struct Sphere
{
	Sphere( color col, double radius = 0.5f, int slices = 16, int stacks = 16 );

	Material material;

	std::vector<VertexData> triangleVertices;

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




