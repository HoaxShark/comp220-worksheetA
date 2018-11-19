#pragma once

// Create a structure for the vertex, can be expanded upon
struct Vertex
{
	float x, y, z;
	float r, g, b, a;
	float tu, tv;
	float normalX, normalY, normalZ;
	float tangentX, tangentY, tangentZ;
	float biTangentX, biTangentY, biTangentZ;
};