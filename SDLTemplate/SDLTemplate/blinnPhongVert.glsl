#version 330 core

// location matches the location in the vertex pointer
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec2 vertexTextureCoords;
layout(location = 3) in vec3 vertexNormal;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 cameraPosition;

out vec2 vertextTextureCoordsOut;
out vec4 vertexColourOut;
out vec3 vertexNormalOut;
out vec3 viewDirection;
out vec3 worldVertexPosition;

void main()
{
	mat4 mvpMatrix = proj * view * modelMatrix;

	// translate vertex positions to world space
	vec4 vertexWorldPosition = modelMatrix * vec4(vertexPos,1.0f);
	vec4 mvpPosition = mvpMatrix*vec4(vertexPos,1.0f);
	
	vertexNormalOut = normalize(modelMatrix*vec4(vertexNormal,0.0f)).xyz;;
	vertexColourOut = vertexColour;
	worldVertexPosition = vertexWorldPosition.xyz;
	viewDirection = normalize(cameraPosition - vertexWorldPosition.xyz);
	vertextTextureCoordsOut = vertexTextureCoords;

	gl_Position = mvpPosition;
}
