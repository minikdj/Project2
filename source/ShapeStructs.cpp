#include "ShapeStructs.h"

Box::Box( color cubeColor, float width, float height, float depth )
{
    color c = cubeColor;

    dvec4 v0(-width/2, height/2, depth/2, 1.0);
    dvec4 v1(-width/2, height/2, -depth/2, 1.0);
    dvec4 v2(width/2, height/2, -depth/2, 1.0);
    dvec4 v3(width/2, height/2, depth/2, 1.0);

    dvec4 v4(-width/2, -height/2, depth/2, 1.0);
    dvec4 v5(-width/2, -height/2, -depth/2, 1.0);
    dvec4 v6(width/2, -height/2, -depth/2, 1.0);
    dvec4 v7(width/2, -height/2, depth/2, 1.0);

    // top
    dvec4 n(findUnitNormal(v0.xyz, v2.xyz, v1.xyz), 0.0);

	triangleVertices.push_back(VertexData(v0, c, n));
	triangleVertices.push_back(VertexData(v2, c, n));
	triangleVertices.push_back(VertexData(v1, c, n));

    n = dvec4(findUnitNormal(v0.xyz, v3.xyz, v2.xyz), 0.0);

    triangleVertices.push_back(VertexData(v0, c, n));
	triangleVertices.push_back(VertexData(v3, c, n));
	triangleVertices.push_back(VertexData(v2, c, n));

    // bottom
    n = dvec4(findUnitNormal(v4.xyz, v5.xyz, v6.xyz), 0.0);

    triangleVertices.push_back(VertexData(v4, c, n));
	triangleVertices.push_back(VertexData(v5, c, n));
	triangleVertices.push_back(VertexData(v6, c, n));

    n = dvec4(findUnitNormal(v7.xyz, v4.xyz, v6.xyz), 0.0);

    triangleVertices.push_back(VertexData(v7, c, n));
	triangleVertices.push_back(VertexData(v4, c, n));
	triangleVertices.push_back(VertexData(v6, c, n));

    // front 
    n = dvec4(findUnitNormal(v4.xyz, v3.xyz, v0.xyz), 0.0);

    triangleVertices.push_back(VertexData(v4, c, n));
	triangleVertices.push_back(VertexData(v3, c, n));
	triangleVertices.push_back(VertexData(v0, c, n));

    n = dvec4(findUnitNormal(v4.xyz, v7.xyz, v3.xyz), 0.0);

    triangleVertices.push_back(VertexData(v4, c, n));
	triangleVertices.push_back(VertexData(v7, c, n));
	triangleVertices.push_back(VertexData(v3, c, n));

    // back 
    n = dvec4(findUnitNormal(v2.xyz, v5.xyz, v1.xyz), 0.0);

    triangleVertices.push_back(VertexData(v2, c, n));
	triangleVertices.push_back(VertexData(v5, c, n));
	triangleVertices.push_back(VertexData(v1, c, n));

    n = dvec4(findUnitNormal(v2.xyz, v6.xyz, v5.xyz), 0.0);

    triangleVertices.push_back(VertexData(v2, c, n));
	triangleVertices.push_back(VertexData(v6, c, n));
	triangleVertices.push_back(VertexData(v5, c, n));

    // left
    n = dvec4(findUnitNormal(v5.xyz, v0.xyz, v1.xyz), 0.0);

    triangleVertices.push_back(VertexData(v5, c, n));
	triangleVertices.push_back(VertexData(v0, c, n));
	triangleVertices.push_back(VertexData(v1, c, n));

    n = dvec4(findUnitNormal(v5.xyz, v4.xyz, v0.xyz), 0.0);

    triangleVertices.push_back(VertexData(v5, c, n));
	triangleVertices.push_back(VertexData(v4, c, n));
	triangleVertices.push_back(VertexData(v0, c, n));

    // right
    n = dvec4(findUnitNormal(v2.xyz, v3.xyz, v7.xyz), 0.0);

    triangleVertices.push_back(VertexData(v2, c, n));
	triangleVertices.push_back(VertexData(v3, c, n));
	triangleVertices.push_back(VertexData(v7, c, n));

    n = dvec4(findUnitNormal(v7.xyz, v6.xyz, v2.xyz), 0.0);

    triangleVertices.push_back(VertexData(v7, c, n));
	triangleVertices.push_back(VertexData(v6, c, n));
	triangleVertices.push_back(VertexData(v2, c, n));

}

Pyramid::Pyramid( color pyColor, double width, double height)
{
	color c = pyColor;

	dvec4 v0(0.0, height / 2.0, 0.0, 1.0);
	dvec4 v1(-width / 2.0, -height / 2.0, width / 2.0, 1.0);
	dvec4 v2(width / 2.0, -height / 2.0, width / 2.0, 1.0);
	dvec4 v3(width / 2.0, -height / 2.0, -width / 2.0, 1.0);
	dvec4 v4(-width / 2.0, -height / 2.0, -width / 2.0, 1.0);


	// Set vertex locations for one triangle
	dvec4 n(findUnitNormal(v0.xyz, v1.xyz, v2.xyz), 0.0);

	triangleVertices.push_back(VertexData(v0, c, n));
	triangleVertices.push_back(VertexData(v1, c, n));
	triangleVertices.push_back(VertexData(v2, c, n));

    n = dvec4(findUnitNormal(v0.xyz, v2.xyz, v3.xyz), 0.0);

    triangleVertices.push_back(VertexData(v0, c, n));
    triangleVertices.push_back(VertexData(v2, c, n));
    triangleVertices.push_back(VertexData(v3, c, n));

    n = dvec4(findUnitNormal(v0.xyz, v3.xyz, v4.xyz), 0.0);

    triangleVertices.push_back(VertexData(v0, c, n));
    triangleVertices.push_back(VertexData(v3, c, n));
    triangleVertices.push_back(VertexData(v4, c, n));

    n = dvec4(findUnitNormal(v0.xyz, v4.xyz, v1.xyz), 0.0);

    triangleVertices.push_back(VertexData(v0, c, n));
    triangleVertices.push_back(VertexData(v4, c, n));
    triangleVertices.push_back(VertexData(v1, c, n));

    // should i change these? the bottom is not showing up when it should be
    n = dvec4(findUnitNormal(v3.xyz, v2.xyz, v1.xyz), 0.0);
                                                    
    triangleVertices.push_back(VertexData(v3, c, n));
    triangleVertices.push_back(VertexData(v2, c, n));
    triangleVertices.push_back(VertexData(v1, c, n));
                                                    
    n = dvec4(findUnitNormal(v1.xyz, v4.xyz, v3.xyz), 0.0);
                                                   
    triangleVertices.push_back(VertexData(v1, c, n));
    triangleVertices.push_back(VertexData(v4, c, n));
    triangleVertices.push_back(VertexData(v3, c, n));

}


ReferencePlane::ReferencePlane( double planeWidth, color c1, color c2 )
	: color1(c1), color2(c2)
{
	dvec4 n(0.0, 1.0, 0.0, 0.0);

	triangleVertices.push_back(VertexData(dvec4(0.0, 0.0, 0.0, 1.0), color1, n));
	triangleVertices.push_back(VertexData(dvec4(-planeWidth / 2.0, 0.0, -planeWidth / 2.0, 1.0), color1, n));
	triangleVertices.push_back(VertexData(dvec4(-planeWidth / 2.0, 0.0, planeWidth / 2.0, 1.0), color1, n));
	triangleVertices.push_back(VertexData(dvec4(0.0, 0.0, 0.0, 1.0), color1, n));
	triangleVertices.push_back(VertexData(dvec4(planeWidth / 2.0, 0.0, planeWidth / 2.0, 1.0), color1, n));
	triangleVertices.push_back(VertexData(dvec4(planeWidth / 2.0, 0.0, -planeWidth / 2.0, 1.0), color1, n));

	triangleVertices.push_back(VertexData(dvec4(0.0, 0.0, 0.0, 1.0), color2, n));
	triangleVertices.push_back(VertexData(dvec4(planeWidth / 2.0, 0.0, -planeWidth / 2.0, 1.0), color2, n));
	triangleVertices.push_back(VertexData(dvec4(-planeWidth / 2.0, 0.0, -planeWidth / 2.0, 1.0), color2, n));
	triangleVertices.push_back(VertexData(dvec4(0.0, 0.0, 0.0, 1.0), color2, n));
	triangleVertices.push_back(VertexData(dvec4(-planeWidth / 2.0, 0.0, planeWidth / 2.0, 1.0), color2, n));
	triangleVertices.push_back(VertexData(dvec4(planeWidth / 2.0, 0.0, planeWidth / 2.0, 1.0), color2, n));
}


dvec4 getSphericalCoordinate(double radius, double theta, double phi)
{
	dvec4 coord( radius * glm::cos(theta) * glm::sin(phi),
					 radius * glm::sin(theta) * glm::sin(phi),
					 radius * glm::cos(phi), 
					 1.0 );

	return coord;
}

// Definitions

/**
* Generates Cartesion xyz coordinates on the surface of a sphere of a specified
* radius from polar coordinates. Assumes the "poles" of the sphere on on the Y
* axis. Stack angles are relative the Y axis. Slice angles
* are around the Y axis in plane that is perpendicular to it.
* @param radius of the sphere on which the Cartesion coordinates are being generated
* @param sliceAngle - angle in the XZ plane
* @param stackAngle - angle relative to the Y axis.
* @returns homogenous 4 dimensional vector containing the Cartesion coordinates
* for the specified slice and stack angles.
*/
dvec4 sphericalToCartesion( double sliceAngle, double stackAngle, double radius )
{
	dvec4 v( glm::cos( stackAngle ) * glm::sin( sliceAngle ) * radius,
		glm::sin( stackAngle ) * radius,
		glm::cos( stackAngle ) * cos( sliceAngle ) * radius,
		1.0 );

	return v;

} // end sphericalToCartesion


Sphere::Sphere( color col, double radius, int slices, int stacks )
	: radius( radius ), stacks( stacks ), slices( slices ), material( col )
{
	double stackInc = ( M_PI ) / ( stacks );
	double sliceInc = ( 2 * M_PI ) / ( slices );

	double angle = -M_PI / 2 + stackInc;
	stackAngles.push_back( angle );

	for( int i = 1; i < stacks - 1; i++ ) {

		angle += stackInc;
		stackAngles.push_back( angle );
	}

	angle = 0;
	sliceAngles.push_back( angle );

	for( int i = 0; i < slices; i++ ) {

		angle += sliceInc;
		sliceAngles.push_back( angle );
	}

	initializeTop( );
	initializeBody( );
	initializeBottom( );

}


void Sphere::initializeTop( )
{
	dvec4 vertTop = dvec4( 0, radius, 0, 1 );
	dvec4 normTop = dvec4( glm::normalize( vertTop ) );

	dvec4 vert = sphericalToCartesion( sliceAngles[0], stackAngles[stackAngles.size( ) - 1], radius );
	dvec4 norm = dvec4( glm::normalize( vert ) );

	for( int sliceIndex = 0; sliceIndex < slices; sliceIndex++ ) {

		triangleVertices.push_back( VertexData( vertTop, material, normTop ) );

		triangleVertices.push_back( VertexData( vert, material, norm ) );

		vert = sphericalToCartesion( sliceAngles[sliceIndex + 1], stackAngles[stackAngles.size( ) - 1], radius );
		norm = dvec4( glm::normalize( vert ) );
		triangleVertices.push_back( VertexData( vert, material, norm ) );

	}

} // end initializeTop

void Sphere::initializeBody( )
{
	for( unsigned int stackIndex = 0; stackIndex < stackAngles.size( ) - 1; stackIndex++ ) {

		for( unsigned int sliceIndex = 0; sliceIndex < sliceAngles.size( ) - 1; sliceIndex++ ) {

			dvec4 vert0 = sphericalToCartesion( sliceAngles[sliceIndex], stackAngles[stackIndex + 1], radius );
			dvec4 vert1 = sphericalToCartesion( sliceAngles[sliceIndex], stackAngles[stackIndex], radius );
			dvec4 vert2 = sphericalToCartesion( sliceAngles[sliceIndex + 1], stackAngles[stackIndex], radius );
			dvec4 vert3 = sphericalToCartesion( sliceAngles[sliceIndex + 1], stackAngles[stackIndex + 1], radius );

			triangleVertices.push_back( VertexData( vert0, material, dvec4( glm::normalize( vert0 ) ) ) );
			triangleVertices.push_back( VertexData( vert1, material, dvec4( glm::normalize( vert1 ) ) ) );
			triangleVertices.push_back( VertexData( vert2, material, dvec4( glm::normalize( vert2 ) ) ) );

			triangleVertices.push_back( VertexData( vert0, material, dvec4( glm::normalize( vert0 ) ) ) );
			triangleVertices.push_back( VertexData( vert2, material, dvec4( glm::normalize( vert2 ) ) ) );
			triangleVertices.push_back( VertexData( vert3, material, dvec4( glm::normalize( vert3 ) ) ) );

		}
	}

} // end initializeBody


void Sphere::initializeBottom( )
{
	dvec4 vertBottom = dvec4( 0, -radius, 0, 1 );
	dvec4 normBottom = dvec4( glm::normalize( vertBottom ) );

	dvec4 vert = sphericalToCartesion( sliceAngles[slices], stackAngles[0], radius );
	dvec4 norm = dvec4( glm::normalize( vert ) );

	for( int sliceIndex = slices; sliceIndex > 0; sliceIndex-- ) {

		triangleVertices.push_back( VertexData( vertBottom, material, normBottom ) );

		triangleVertices.push_back( VertexData( vert, material, norm ) );

		vert = sphericalToCartesion( sliceAngles[sliceIndex - 1], stackAngles[0], radius );
		norm = dvec4( glm::normalize( vert ) );
		triangleVertices.push_back( VertexData( vert, material, norm ) );
	}

} // end initializeBottom
