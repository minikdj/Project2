#pragma once

#include "Defines.h"
#include "FrameBuffer.h" // Eventually remove
#include "PerFragmentOperations.h"
#include "VertexData.h"

// Draws a line segment if there are at least two vertices
void drawLine(VertexData v0, VertexData v1);

// Takes vertices in sequential pairs and draws a line segment between each pair
void drawManyLines(const std::vector<VertexData> & vertices);

// Draws a wire frame if there are at least three vertices
void drawWireFrameTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2);

// Draws a filled triangle if there are at least three vertices
void drawFilledTriangle(const VertexData & v0, const VertexData & v1, const VertexData & v2);

// Draws one or more wire frame triangles if there are at least three vertices
void drawManyWireFrameTriangles(const std::vector<VertexData> & vertices);

// Draws one or more solid triangles if there are at least three vertices
void drawManyFilledTriangles(const std::vector<VertexData> & vertices);
