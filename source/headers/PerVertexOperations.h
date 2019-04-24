#pragma once

#include "Defines.h"
#include "Lights.h"
#include "Rasterization.h"
#include "VertexData.h"
#include "ClippingPlane.h"

extern std::vector<LightSource*> lights;

enum Render_Mode { FILL, LINE };


class PerVertex {

public:

	// Pipeline transformation matrices
	static dmat4 modelingTransformation;
	static  dmat4 viewingTransformation;
	static dmat4 projectionTransformation;
	static dmat4 viewportTransformation;
	static  dvec3 eyePositionInWorldCoords;

	// View port limits
	static double xViewportMin, yViewportMin, xViewportMax, yViewportMax;

	// Normalized device coordinate horizontal and vertical limits
	static const int xNdcMin, yNdcMin, xNdcMax, yNdcMax;

	// Planes describing the normalized device coordinates view volume
	static std::vector<ClippingPlane> ndcPlanes;

	static bool perVertexLightingEnabled;

	static Render_Mode polygonRenderMode;

	static void processTriangleVertices(const std::vector<VertexData> & objectCoords);

	static void processLineSegments(const std::vector<VertexData> & objectCoords);

protected:

	static std::vector<VertexData> clipAgainstPlane(std::vector<VertexData> & verts, ClippingPlane & plane);

	static std::vector<VertexData> triangulate(const std::vector<VertexData> & poly);

	static std::vector<VertexData> clipPolygon(const std::vector<VertexData> & clipCoords);

	static std::vector<VertexData> clipLineSegments(const std::vector<VertexData> & clipCoords);

	static std::vector<VertexData> removeBackwardFacingTriangles(const std::vector<VertexData> & triangleVerts);

	static std::vector<VertexData> transformVerticesToWorldCoordinates(const dmat4 & modelMatrix, const std::vector<VertexData> & vertices);

	static void applyLighting(std::vector<VertexData> & worldCoords);

	static std::vector<VertexData> transformVertices(const dmat4 & transMatrix, const std::vector<VertexData> & vertices);

}; // end PerVertex class



