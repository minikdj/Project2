#include "Rasterization.h"

// ColorBuffer object that is global to all files
extern FrameBuffer frameBuffer;

// Swaps two vertices
void swap(VertexData &v0, VertexData &v1);

// Rasterizes a vertical line
void drawVerticalLine(VertexData v0, VertexData v1);

// Rasterizes a horizontal line
void drawHorizontalLine(VertexData v0, VertexData v1);

// Uses the midpoint algorithm to render a sloped line
void midPointLine(VertexData v0, VertexData v1);

double cheapNonPerspectiveCorrectInterpolationForLines(const dvec2 start, const dvec2 end, const dvec2 & online)
{
	return glm::length(online - start) / glm::length(end-start);
}



inline void swap(VertexData &v0, VertexData &v1)
{
	VertexData temp = v0;
	v0 = v1;
	v1 = temp;

} // end swap


void drawVerticalLine(VertexData v0, VertexData v1)
{
	if (v1.position.y < v0.position.y) {

		swap(v0, v1);
	}

	double zDifference = v1.position.z - v0.position.z;

	for (double y = v0.position.y; y < v1.position.y; y++) {

		// Interpolate vertex attributes
		double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(v0.position.x, y));

		Fragment fragment;

		// Interpolate vertex attributes using weights
		fragment.shadedColor = (1.0 - weight) * v0.shadedColor + weight * v1.shadedColor;
		fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
		fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
		fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
		fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
		fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

		double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
		fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
		fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
		fragment.windowPosition = dvec3(v0.position.x, y, z);

		FragmentOperations::processFragment(fragment);
	}

} // end drawVerticalLine


void drawHorizontalLine(VertexData v0, VertexData v1)
{
	if (v1.position.x < v0.position.x) {

		swap(v0, v1);
	}

	for (double x = v0.position.x; x < v1.position.x; x++) {

		// Interpolate vertex attributes
		double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(x, v0.position.y));

		Fragment fragment;

		// Interpolate vertex attributes using weights
		fragment.shadedColor = ( 1.0 - weight ) * v0.shadedColor + weight * v1.shadedColor;
		fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
		fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
		fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
		fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
		fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

		double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
		fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
		fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
		fragment.windowPosition = dvec3(x, v1.position.y, z);

		FragmentOperations::processFragment(fragment);
	}

} // end drawVerticalLine


void midPointLine(VertexData v0, VertexData v1)
{
	// Check if v0 is left of v1
	if (v1.position.x < v0.position.x) {

		// Make v0 left of v1
		swap(v0, v1);
	}

	// Calculate slope of the line
	double m = (v1.position.y - v0.position.y) / (v1.position.x - v0.position.x);

	if (m > 0 && m < 1.0) { // For slope in (0,1] More "run" than "rise"

		double y = v0.position.y;

		for (double x = v0.position.x; x < v1.position.x; x += 1.0) {

			// Interpolate vertex attributes
			double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using weights
			fragment.shadedColor = ( 1.0 - weight ) * v0.shadedColor + weight * v1.shadedColor;
			fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
			fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
			fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
			fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
			fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

			double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
			fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = dvec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be above the midpoint between the pixel centers.
			double fXY = (v0.position.y - v1.position.y) * (x + 1.0) +
				(v1.position.x - v0.position.x) * (y + 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				y += 1.0;
			}
		}
	}
	else if (m > 1) { // For slope in (1,infinity] More "run" than "rise"

		double x = v0.position.x;

		for (double y = v0.position.y; y < v1.position.y; y += 1.0) {

			// Interpolate vertex attributes
			double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using weights
			fragment.shadedColor = ( 1.0 - weight ) * v0.shadedColor + weight * v1.shadedColor;
			fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
			fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
			fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
			fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
			fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

			double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
			fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = dvec3(x, y, z);

			FragmentOperations::processFragment(fragment);
			
			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			double fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y + 1.0) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {
				x += 1.0;
			}
		}
	}
	else if (m >= -1.0 && m < 0) { // For slope in [-1,0) More "run" than "rise"

		double y = v0.position.y;
		double x = v0.position.x;

		for (double x = v0.position.x; x < v1.position.x; x += 1.0) {

			// Interpolate vertex attributes
			double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using alpha, beta, and gama weights
			fragment.shadedColor = ( 1.0 - weight ) * v0.shadedColor + weight * v1.shadedColor;
			fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
			fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
			fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
			fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
			fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

			double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
			fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = dvec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be below the midpoint between the pixel centers.
			double fXY = (v0.position.y - v1.position.y) * (x + 1.0) +
				(v1.position.x - v0.position.x) * (y - 0.5f) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY > 0) {

				y -= 1.0;
			}
		}
	}
	else if (m < -1) { // For slope in [-infinity,-1) More "run" than "rise"

		double x = v0.position.x;

		for (double y = v0.position.y; y > v1.position.y; y -= 1.0) {

			// Interpolate vertex attributes
			double weight = cheapNonPerspectiveCorrectInterpolationForLines(v0.position.xy, v1.position.xy, dvec2(x, y));

			Fragment fragment;

			// Interpolate vertex attributes using weights
			fragment.shadedColor = ( 1.0 - weight ) * v0.shadedColor + weight * v1.shadedColor;
			fragment.material.emissiveColor = ( 1.0 - weight ) * v0.material.emissiveColor + weight * v1.material.emissiveColor;
			fragment.material.ambientColor = ( 1.0 - weight ) * v0.material.ambientColor + weight * v1.material.ambientColor;
			fragment.material.diffuseColor = ( 1.0 - weight ) * v0.material.diffuseColor + weight * v1.material.diffuseColor;
			fragment.material.specularColor = ( 1.0 - weight ) * v0.material.specularColor + weight * v1.material.specularColor;
			fragment.material.shininess = ( 1.0 - weight ) * v0.material.shininess + weight * v1.material.shininess;

			double z = (1 - weight)  * v0.position.z + weight * v1.position.z;
			fragment.worldNormal = (1.0 - weight) * v0.worldNormal + weight * v1.worldNormal;
			fragment.worldPosition = (1.0 - weight) * v0.worldPosition + weight * v1.worldPosition;
			fragment.windowPosition = dvec3(x, y, z);

			FragmentOperations::processFragment(fragment);

			// Evaluate the implicit equation for the line to determine if
			// the line will be left or right the midpoint between the pixel centers.
			double fXY = (v0.position.y - v1.position.y) * (x + 0.5f) +
				(v1.position.x - v0.position.x) * (y - 1.0) +
				v0.position.x * v1.position.y - v1.position.x * v0.position.y;

			if (fXY < 0) {
				x += 1.0;
			}
		}
	}

} // end midPointLine


void drawLine(VertexData v0, VertexData v1)
{
	if (v0.position.x == v1.position.x) {
		drawVerticalLine(v0, v1);
	}
	else if (v0.position.y == v1.position.y) {
		drawHorizontalLine(v0, v1);
	}
	else {
		midPointLine(v0, v1);
	}

} // end drawLine


void drawManyLines(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 1) < vertices.size(); i += 2) {

		drawLine(vertices[i], vertices[i + 1]);
	}

} // end drawManyLines


void drawWireFrameTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2)
{
	drawLine(v0, v1);
	drawLine(v1, v2);
	drawLine(v2, v0);

} // end drawWireFrameTriangle


void drawManyWireFrameTriangles(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		drawWireFrameTriangle(vertices[i], vertices[i + 1], vertices[i + 2]);
	}

} // end drawManyWireFrameTriangles


  // Implicit equation for the line between v0 and v1
inline double f01(const VertexData & v0, const VertexData & v1, const VertexData & v2, double x, double y)
{
	return (v0.position.y - v1.position.y) * x + (v1.position.x - v0.position.x) * y + 
		   (v0.position.x * v1.position.y) - (v1.position.x * v0.position.y);

} // end f01


  // Implicit equation for the line between v1 and v2
inline double f12(const VertexData & v0, const VertexData & v1, const VertexData & v2, double x, double y)
{
	return (v1.position.y - v2.position.y) * x + (v2.position.x - v1.position.x) * y + 
		   (v1.position.x * v2.position.y) - (v2.position.x * v1.position.y);

} // end f12


  // Implicit equation for the line between v2 and v0
inline double f20(const VertexData & v0, const VertexData & v1, const VertexData & v2, double x, double y)
{
	return (v2.position.y - v0.position.y) * x + (v0.position.x - v2.position.x) * y + 
		   (v2.position.x * v0.position.y) - (v0.position.x * v2.position.y);

} // end f20


void drawFilledTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2)
{
	// Find minimimum and maximum x and y limits for the triangle
	double xMin = glm::floor(glm::min(glm::min(v0.position.x, v1.position.x), v2.position.x));
	double xMax = glm::ceil(glm::max(glm::max(v0.position.x, v1.position.x), v2.position.x));
	double yMin = glm::floor(glm::min(glm::min(v0.position.y, v1.position.y), v2.position.y));
	double yMax = glm::ceil(glm::max(glm::max(v0.position.y, v1.position.y), v2.position.y));

	double fAlpha = f12(v0, v1, v2, v0.position.x, v0.position.y);
	double fBeta = f20(v0, v1, v2, v1.position.x, v1.position.y);
	double fGama = f01(v0, v1, v2, v2.position.x, v2.position.y);

	// Check all the pixels in the rows between the minimum and maximum y
	for (double y = yMin; y <= yMax; y++) {

		// Check all the pixels in the columns between the minimum and maximum x
		for (double x = xMin; x <= xMax; x++) {

			// Calculate the weights for Gouraud inperpolation
			// If any weight is negative, the fragment is not in the triangle
			double alpha = f12(v0, v1, v2, x, y) / fAlpha;
			double beta = f20(v0, v1, v2, x, y) / fBeta;
			double gama = f01(v0, v1, v2, x, y) / fGama;

			// Determine if the pixel position is inside the triangle
			if (alpha >= 0 && beta >= 0 && gama >= 0) {

				if ((alpha > 0 || fAlpha * f12(v0, v1, v2, -1, -1) > 0) &&
					(beta > 0 || fBeta * f20(v0, v1, v2, -1, -1) > 0) &&
					(gama > 0 || fGama * f01(v0, v1, v2, -1, -1) > 0)) {

					Fragment fragment;

					// Interpolate vertex attributes using alpha, beta, and gama weights
					fragment.shadedColor = alpha * v0.shadedColor + beta * v1.shadedColor + gama * v2.shadedColor;
					fragment.material.emissiveColor = alpha * v0.material.emissiveColor + beta * v1.material.emissiveColor + gama * v2.material.emissiveColor;
					fragment.material.ambientColor = alpha * v0.material.ambientColor + beta * v1.material.ambientColor + gama * v2.material.ambientColor;
					fragment.material.diffuseColor = alpha * v0.material.diffuseColor + beta * v1.material.diffuseColor + gama * v2.material.diffuseColor;
					fragment.material.specularColor = alpha * v0.material.specularColor + beta * v1.material.specularColor + gama * v2.material.specularColor;
					fragment.material.shininess = alpha * v0.material.shininess + beta * v1.material.shininess + gama * v2.material.shininess;
					
					double z = alpha * v0.position.z + beta * v1.position.z + gama * v2.position.z;
					fragment.windowPosition = dvec3(x, y, z);
					fragment.worldNormal = alpha * v0.worldNormal + beta * v1.worldNormal + gama * v2.worldNormal;
					fragment.worldPosition = alpha * v0.worldPosition + beta * v1.worldPosition + gama * v2.worldPosition;

					FragmentOperations::processFragment(fragment);
				}
			}
		}
	}
} // end drawFilledTriangle


void drawManyFilledTriangles(const std::vector<VertexData> & vertices)
{
	for (unsigned int i = 0; (i + 2) < vertices.size(); i += 3) {

		drawFilledTriangle(vertices[i], vertices[i + 1], vertices[i + 2]);
	}

} // end drawManyFilledTriangles