#version 330 core

out vec4 color;
in vec3 vertColor;
in vec4 vertexColourOut;
in vec2 vertextTextureCoordsOut;
in vec3 vertexNormalOut;
in vec3 viewDirection;

uniform sampler2D diffuseTexture;

uniform vec4 ambientLightColour;
uniform vec4 diffuseLightColour;
uniform vec4 specularLightColour;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;

uniform float specularMaterialPower;

uniform vec3 lightDirection;

void main()
{
	// dot product gives us how much light is hitting the surface
	float nDotl = dot(vertexNormalOut, -lightDirection);

	vec3 maxReflectionVector = normalize(-lightDirection + viewDirection);
	// defines where the highlight will be on the surface
	float nDoth = pow(dot(vertexNormalOut, maxReflectionVector),specularMaterialPower);

	// look up the texture colour
	vec4 diffuseTextureColour = texture(diffuseTexture,vertextTextureCoordsOut);

	color = (ambientLightColour*ambientMaterialColour) + 
			(diffuseLightColour*nDotl*diffuseMaterialColour*diffuseTextureColour) + 
			(specularLightColour*nDoth*specularMaterialColour);
}