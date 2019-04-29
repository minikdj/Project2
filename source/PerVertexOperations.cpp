#include "PerVertexOperations.h"

// static Pipeline transformation matrices
dmat4 PerVertex::modelingTransformation;
dmat4 PerVertex::viewingTransformation;
dmat4 PerVertex::projectionTransformation;
dmat4 PerVertex::viewportTransformation;
dvec3 PerVertex::eyePositionInWorldCoords;

bool PerVertex::perVertexLightingEnabled = false;
Render_Mode PerVertex::polygonRenderMode = FILL;

// static Nnrmalized device coordinate horizontal and vertical limits
const int PerVertex::xNdcMin = -1, PerVertex::yNdcMin = -1, PerVertex::xNdcMax = 1, PerVertex::yNdcMax = 1;

// static view port limits
double PerVertex::xViewportMin, PerVertex::yViewportMin, PerVertex::xViewportMax, PerVertex::yViewportMax;

// static planes describing the normalized device coordinates view volume
std::vector<ClippingPlane> PerVertex::ndcPlanes{ ClippingPlane(dvec3(0, 1, 0), dvec3(0, -1, 0)),
											   ClippingPlane(dvec3(1, 0, 0), dvec3(-1, 0, 0)),
											   ClippingPlane(dvec3(0, 0, 1), dvec3(0, 0, -1)),
											   ClippingPlane(dvec3(-1, 0, 0), dvec3(1, 0, 0)),
											   ClippingPlane(dvec3(0, -1, 0), dvec3(0, 1, 0)),
											   ClippingPlane(dvec3(0, 0, -1), dvec3(0, 0, 1)) };

std::vector<VertexData> PerVertex::removeBackwardFacingTriangles(const std::vector<VertexData> & triangleVerts)
{
    std::vector<VertexData> visibleTriangles;

    for (int i = 0; i <= triangleVerts.size() - 3; i += 3) {
        if (glm::dot(findUnitNormal(triangleVerts[i+2].position, triangleVerts[i+1].position, triangleVerts[i].position), glm::normalize(eyePositionInWorldCoords)) <= 0) {
            visibleTriangles.push_back(triangleVerts[i]);
            visibleTriangles.push_back(triangleVerts[i+1]);
            visibleTriangles.push_back(triangleVerts[i+2]);
        }
    }

    return visibleTriangles;

}

//********************************** Vertex Shading *********************************

void PerVertex::applyLighting( std::vector<VertexData> & worldCoords )
{
	//TODO

} // end applyLighting

//********************************** Vertex Transformation *********************************

// Applies a modeling transformation in the form of a matrix to a list of vertices. Transforms 
// the vertices and normals to World coordinates.
std::vector<VertexData> PerVertex::transformVerticesToWorldCoordinates( const dmat4 & modelMatrix, const std::vector<VertexData> & vertices )
{
	// Create 3 x 3 matrix for transforming normal vectors to world coordinates
	glm::dmat3 modelingTransfomationForNormals( modelMatrix );
	modelingTransfomationForNormals = glm::transpose( glm::inverse( modelingTransfomationForNormals ) );

	std::vector<VertexData> transformedVertices;

	for( VertexData v : vertices ) {

		// Copy the vertex data
		VertexData vt = v;

		// Transform the position and surface normal to world coordinates
		vt.position = modelMatrix * v.position;
		vt.worldPosition = vt.position.xyz;
		vt.worldNormal = modelingTransfomationForNormals * v.localNormal;

		transformedVertices.push_back( vt );
	}

	return transformedVertices;

} // end transformVertices


// Applies a transformation in the form of a matrix to a list of vertices.
std::vector<VertexData> PerVertex::transformVertices( const dmat4 & transMatrix, const std::vector<VertexData> & vertices )
{
	std::vector<VertexData> transformedVertices;

	for( VertexData v : vertices ) {

		// Copy the vertex data
		VertexData vt = v;

		// Transform the position
		vt.position = transMatrix * v.position;

		// Add the vertex for further processing by the pipeline
		transformedVertices.push_back( vt );
	}

	return transformedVertices;

} // end transformVertices

//********************************** Triangle Pipeline *********************************

// Tranforms triangle vertices from world to view port coordinate via eye, clip, and normalized device coordinates. 
// Vertices are clipped and backfaces are culled. Lighting calculations are performed in World coordinates.
void PerVertex::processTriangleVertices(const std::vector<VertexData> & objectCoords)
{
	// Modeling tranformation
	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation, objectCoords);

	if (perVertexLightingEnabled == true) {
	
		// Perform lighting calculations in World coordinates
		applyLighting(worldCoords);
	}
  
	// Viewing Transformation
  	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);
  
	// Projections Transformation
  	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);
  
  	std::vector<VertexData> clipCoords;
  
  	// Perspective division
  	for (VertexData v : projCoords) {
  
  		v.position = v.position / v.position.w;
  		clipCoords.push_back(v);
  	}
  
	// Backface Cullling
    std::vector<VertexData> filtered = removeBackwardFacingTriangles(clipCoords); 
  	// Clipping

	std::vector<VertexData> ndcCoords = filtered;
  
	// Window Transformation
  	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);
  
	if (polygonRenderMode == FILL) {
	
		drawManyFilledTriangles(windowCoords);
	}
	else {

		drawManyWireFrameTriangles(windowCoords);
	}
  
} // end processTriangleVertices
  
//********************************** Line Segment Pipeline *********************************

// Tranforms line segments from world to view port coordinate via eye, clip, and normalized device coordinates. 
// Vertices are clipped . Lighting calculations are performed in World coordinates.
void PerVertex::processLineSegments(const std::vector<VertexData> & objectCoords)
{
	// Modeling Transformation
  	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation,objectCoords);
  
	// Viewing Transformation
  	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);
  
	// Projection Transformation
  	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);
  
  	std::vector<VertexData> clipCoords;
  
  	// Perspective division
  	for (VertexData v : projCoords) {
  
  		v.position = v.position / v.position.w;
  		clipCoords.push_back(v);
  	}
  
  	// Clipping
	//TODO
	std::vector<VertexData> ndcCoords = clipCoords;
  
	// Window Transformation
  	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);
  
	drawManyLines( windowCoords );
  
} // end linePipeline


