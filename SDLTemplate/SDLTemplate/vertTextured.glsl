#version 330 core

// location matches the location in the vertex pointer
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec4 vertexColour;
layout(location = 2) in vec2 vertexTextureCoords;

uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 proj;

out vec2 vertextTextureCoordsOut;
out vec4 vertexColourOut;

void main()
{
	mat4 mvpMatrix = proj * view * modelMatrix;
	vec4 mvpPosition = mvpMatrix*vec4(vertexPos,1.0f);

	gl_Position = mvpPosition;

	vertexColourOut = vertexColour;

	vertextTextureCoordsOut = vertexTextureCoords;
}
