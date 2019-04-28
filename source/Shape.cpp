#include "Shape.h"
#include "PerVertexOperations.h"

void Shape::draw()
{
    PerVertex::modelingTransformation = position * orientation * scale;
    PerVertex::processTriangleVertices(triangleVertices);
}

void Shape::setPosition(const dvec3 & positionVector)
{
    position = glm::translate(positionVector);
}

void Shape::setOrientation(const double & angle, const dvec3 & axis)
{
    orientation = glm::rotate(angle, axis);
}

void Shape::setScale(const double & scaleValue)
{
    glm::dmat4 scaleModel;
    scaleModel[0][0] = scaleValue;
    scaleModel[1][1] = scaleValue;
    scaleModel[2][2] = scaleValue;

    scale = glm::scale(scaleModel, dvec3(1.0, 1.0, 1.0));
}
