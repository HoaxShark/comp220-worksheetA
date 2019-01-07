#version 330 core

// location matches the location in the vertex pointer from main.cpp
layout(location = 0) in vec3 vertexPos;

uniform mat4 view;
uniform mat4 proj;

out vec3 texCoords;

void main()
{
	texCoords = vertexPos;
    gl_Position = proj * view * vec4(vertexPos, 1.0);
}
