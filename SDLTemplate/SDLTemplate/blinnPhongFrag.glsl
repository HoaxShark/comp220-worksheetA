#version 330 core

out vec4 color;
in vec3 vertColor;
in vec4 vertexColourOut;
in vec2 vertextTextureCoordsOut;
in vec3 vertexNormalOut;
in vec3 viewDirection;
in vec3 worldVertexPosition;

uniform sampler2D diffuseTexture;

uniform vec4 ambientLightColour;
uniform vec4 diffuseLightColour;
uniform vec4 specularLightColour;

uniform vec4 ambientMaterialColour;
uniform vec4 diffuseMaterialColour;
uniform vec4 specularMaterialColour;

uniform float specularMaterialPower;

uniform vec3 lightDirection;
uniform vec3 pointLightPos;

vec4 CalculateLightColour(vec4 diffuseLightColour,vec4 specularLightColour,vec3 lightDirection,vec4 diffuseTextureColour)
{
	float nDotl=clamp(dot(vertexNormalOut,normalize(lightDirection)),0.0,1.0);

	vec3 halfWay=normalize(lightDirection+viewDirection);
	float nDoth=pow(clamp(dot(vertexNormalOut,halfWay),0.0,1.0),specularMaterialPower);

	return 	(diffuseLightColour*nDotl*diffuseMaterialColour*diffuseTextureColour)+
			(specularLightColour*nDoth*specularMaterialColour);
}

void main()
{
	vec3 pointLightDirection = worldVertexPosition - pointLightPos;
	float pointLightDistance = length(pointLightDirection);
	pointLightDirection = normalize(pointLightDirection);

	// look up the texture colour
	vec4 diffuseTextureColour = texture(diffuseTexture,vertextTextureCoordsOut);

	vec4 colour=CalculateLightColour(diffuseLightColour,
									specularLightColour,
									-pointLightDirection,
									diffuseTextureColour);

	//float attenuation=1.0/(1.0+0.1*pointLightDistance+0.01*pointLightDistance*pointLightDistance);

	color = colour; //*attenuation;

	/*
	// dot product gives us how much light is hitting the surface
	float nDotl = dot(vertexNormalOut, -lightDirection);

	vec3 maxReflectionVector = normalize(-lightDirection + viewDirection);
	// defines where the highlight will be on the surface
	float nDoth = pow(dot(vertexNormalOut, maxReflectionVector),specularMaterialPower);

	color = (ambientLightColour*ambientMaterialColour) + 
			(diffuseLightColour*nDotl*diffuseMaterialColour*diffuseTextureColour) + 
			(specularLightColour*nDoth*specularMaterialColour);*/
}