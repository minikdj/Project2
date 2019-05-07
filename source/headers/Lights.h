#pragma once

#include "Defines.h"
#include "Material.h"

/**
* Base struct for all types of lights.Supports only specification of the
* color and intensity of light sources.Parent of sub - structs that simulate
* positional, directional, and spot lights.
*
* Instances of this struct an be used to simulate only ambient light.Ambient
* light is described as bounced light that has been scattered so much that it
* is impossible to tell the direction to its source.If using a LightSource
* instantiation to simulate ambient light, the overall intensity of the light
* should be low in order to avoid washing out shadows as well as diffuse and
* specular effects that can be achieved when using children of this struct.
*/
struct LightSource
{
	LightSource(const color & lightColor )
		: diffuseLightColor( lightColor )
	{
		ambientLightColor = 0.15 * diffuseLightColor;
		specularLightColor = WHITE;
	}

	virtual color illuminate( const dvec3 & eyePosition, const dvec3 & worldPosition, dvec3 worldNormal, Material material )
	{
		if (enabled) {

			return ( ambientLightColor * material.ambientColor);
		}
		else {

			return BLACK;
		}
	}

	/*
	* Ambient color and intensity of the light.
	*/
	color ambientLightColor;

	/*
	* Diffuse color and intensity of the light.
	*/
	color diffuseLightColor;

	/*
	 * Specular color and intensity of the light.
	 */
	color specularLightColor;

	/*
	* Shading caculations are performed if true. BLACK (no light) is returned otherwise.
	*/
	bool enabled = true;
};

/**
* Struct for simulating light sources that have an explicit position and
* shine equally in all directions. Instantiations of the struct will have
* a position property and a color and intensity of the light given off
* by the light.
*/
struct PositionalLight : public LightSource
{
	PositionalLight(dvec3 position, const color & lightColor )
		: LightSource( lightColor ), lightPositionWorldCoordinates(position)
	{
	}

	virtual color illuminate(const dvec3 & eyePosition, const dvec3 & worldPosition, dvec3 worldNormal, Material material)
	{
		if (enabled) {

			color totalForThisLight = BLACK;

            dvec3 lightDir = glm::normalize(lightPositionWorldCoordinates - worldPosition);
            dvec3 r = glm::normalize(2 * glm::dot(lightDir, worldNormal) * worldNormal - lightDir);
            
            totalForThisLight += glm::max(glm::dot(lightDir, worldNormal), 0.0) * diffuseLightColor * material.diffuseColor;
            totalForThisLight += glm::pow(glm::max(glm::dot(lightDir, r), 0.0), material.shininess) * specularLightColor * material.specularColor;
            totalForThisLight += LightSource::illuminate(eyePosition, worldPosition, worldNormal, material);

            return totalForThisLight;
		}
		else {

			return BLACK;
		}
	}

	/**
	* x, y, z position of the light source.
	*/
	dvec3 lightPositionWorldCoordinates;
};

/**
* Struct for simulating light sources that do not have an explicit position.
* Such light sources have only a direction against which they are shinning.
* Instantiations of the struct will have this direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct DirectionalLight : public LightSource
{
	DirectionalLight(dvec3 direction, const color & lightColor )
		: LightSource( lightColor ), lightDirectionWorldCoordinates(glm::normalize(direction))
	{
	}

	virtual color illuminate(const dvec3 & eyePosition, const dvec3 & worldPosition, dvec3 worldNormal, Material material)
	{
		if( enabled ) {

            dvec3 r = glm::normalize(2 * glm::dot(lightDirectionWorldCoordinates, worldNormal) * worldNormal - lightDirectionWorldCoordinates);


			color totalForThisLight = material.emissiveColor;

            totalForThisLight += LightSource::illuminate(eyePosition, worldPosition, worldNormal, material);
            totalForThisLight += glm::max(glm::dot(lightDirectionWorldCoordinates, worldNormal), 0.0) * diffuseLightColor * material.diffuseColor;
            totalForThisLight += glm::pow(glm::max(glm::dot(lightDirectionWorldCoordinates, r), 0.0), material.shininess) * specularLightColor * material.specularColor;

			return totalForThisLight;
		}
		else {

			return BLACK;
		}

	}

	/**
	* Unit vector that points in the direction that is opposite
	* the direction in which the light is shining.
	*/
	dvec3 lightDirectionWorldCoordinates;
};


/**
* Struct for simulating light sources that have an explicit position and
* shine in a specified direction.Width of the associated beam of light is
* controlled using a spot cutoff cosine. Instantiations of the struct will
* have position and direction properties along with
* a color and intensity of the light given off by the light source.
*/
struct SpotLight : public PositionalLight
{
	SpotLight(dvec3 position, dvec3 direction, double cutOffCosineRadians, const color & lightColor )
		: PositionalLight(position, lightColor ), spotDirection(glm::normalize(direction)), cutOffCosineRadians(cutOffCosineRadians)
	{
	}

	virtual color illuminate(const dvec3 & eyePosition, const dvec3 & worldPosition, dvec3 worldNormal, Material material)
	{
		dvec3 negLight = glm::normalize(worldPosition - lightPositionWorldCoordinates);


		if( enabled ) {
            color totalForThisLight = PositionalLight::illuminate(eyePosition, worldPosition, worldNormal, material);
            
            double spotCosine = glm::dot(negLight, -spotDirection);
            if (spotCosine > cutOffCosineRadians) {
                double falloffFactor = (1 - (1 - spotCosine)) / (1 - cutOffCosineRadians);
                return totalForThisLight *= falloffFactor ;
            }

            

       		return totalForThisLight;
		}
		else {

			return BLACK;
		}
	}

	/**
	* Unit vector that points in the direction in which the light is shining.
	*/
	dvec3 spotDirection;

	/**
	* Angle in radians of half the spot light beam
	*/
	double cutOffCosineRadians;

};

