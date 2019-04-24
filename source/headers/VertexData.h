#pragma once

#include "Defines.h"
#include "Material.h"

struct VertexData
{
	dvec4 position = dvec4( 0.0, 0.0, 0.0, 1.0 );		  // Transformed position of vertex
	dvec3 localNormal  = dvec3( 0.0, 0.0, 1.0 );		  // Normal vector relative to Object coordinates
	dvec3 worldPosition = dvec3( 0.0, 0.0, 0.0 );		  // Position relative to World coordinates
	dvec3 worldNormal = dvec3( 0.0, 0.0, 1.0 );			  // Normal vector relative to World coordinates
	Material material = Material( color( 0.75, 0.75, 0.75, 1.0 ) ); // Matrial properties for the vertex
	color shadedColor;	// Result of per vertex lighting calculations;

	VertexData(  )
	{
		shadedColor = material.diffuseColor;
	}

	VertexData( const dvec4 & pos, 
				const color & col = LIGHT_GRAY, 
				const dvec3 & norm = dvec3( 0.0, 0.0, 1.0 ) )
		: position( pos ), material( Material( col ) ), localNormal( glm::normalize( norm ) )
	{
		shadedColor = material.diffuseColor;
	}

	VertexData( const dvec4 & pos, 
				const Material & material = Material( LIGHT_GRAY ), 
				const dvec3 & norm = dvec3( 0.0, 0.0, 1.0 ) )
		: position( pos ), material( Material( material ) ), localNormal( glm::normalize( norm ) )
	{
		shadedColor = material.diffuseColor;
	}

};