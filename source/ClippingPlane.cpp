#include "ClippingPlane.h"


ClippingPlane::ClippingPlane( glm::vec3 point, glm::vec3 normal )
{
	a = point;
	n = glm::normalize( normal );
}

// Returns true is a point is to the "left" of a plane.
bool ClippingPlane::insidePlane( const VertexData & point )
{
	// If dot product is positive the point is on the "positive" side of the plane
	if( glm::dot( ( dvec3( point.position.xyz ) - a ), n ) > 0.0 ) {
		return true;
	}
	else {
		return false;
	}

} // end insidePlane

// Finds the point of intersection for a line segment and a plane
VertexData ClippingPlane::findIntersection( const VertexData &  p1, const VertexData & p2 )
{
	// Find the distance of each point from the plane
	double d1 = glm::dot( dvec3( p1.position.xyz ) - a, n );
	double d2 = glm::dot( dvec3( p2.position.xyz ) - a, n );

	// Find the paramter of the intercept with the plane
	double t = d1 / ( d1 - d2 );

	// Generate the vertex data for the point of intersection
	VertexData I;

	// Find point the coordinates of the intersection with the plane
	I.position = p1.position + t * ( p2.position - p1.position );

	// Find interpolated values for the vertex data
	I.shadedColor = p1.shadedColor + t * ( p2.shadedColor - p1.shadedColor );

	I.material.emissiveColor = p1.material.emissiveColor + t * ( p2.material.emissiveColor - p1.material.emissiveColor );
	I.material.ambientColor = p1.material.ambientColor + t * ( p2.material.ambientColor - p1.material.ambientColor );
	I.material.diffuseColor = p1.material.diffuseColor + t * ( p2.material.diffuseColor - p1.material.diffuseColor );
	I.material.specularColor = p1.material.specularColor + t * ( p2.material.specularColor - p1.material.specularColor );
	I.material.shininess = p1.material.shininess + t * ( p2.material.shininess - p1.material.shininess );

	I.localNormal = p1.localNormal + t * ( p2.localNormal - p1.localNormal );

	return I;

} // end findIntersection
