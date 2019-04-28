#include "Defines.h"
#include "VertexData.h"

struct Shape
{
        virtual void draw(  );

        virtual void setPosition( const dvec3 & positionVector  );

        virtual void setOrientation( const double & angle, const dvec3 & axis  ); 

        virtual void setScale( const double & scaleValue );

        protected:

        dmat4 orientation;
        dmat4 position;
        dmat4 scale;

        std::vector<VertexData> triangleVertices;

}; // end Shape
