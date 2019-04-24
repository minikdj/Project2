#pragma once

#include "Defines.h"
#include "VertexData.h"

struct ClippingPlane
{
	ClippingPlane( ) { };

	ClippingPlane( glm::vec3 point, glm::vec3 normal );

	// Returns true is a point is to the "left" of a plane.
	bool insidePlane( const VertexData & point );

	// Finds the point of intersection for a line segment and a plane
	VertexData findIntersection( const VertexData &  p1, const VertexData & p2 );

	dvec3 a; // point on the plane
	dvec3 n; // vector that is normal to the front face of a plane
};



