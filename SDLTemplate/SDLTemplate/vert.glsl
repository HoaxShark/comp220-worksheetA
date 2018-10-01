#version 330 core

// location matches the location in the vertex pointer from main.cpp
layout(location = 0) in vec3 vertexPos;

out vec3 vertColor;

void main()
{
	gl_Position = vec4(vertexPos,1.0);
	vertColor = vertexPos;
}
