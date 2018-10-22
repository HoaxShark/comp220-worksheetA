#version 330 core

out vec4 color;
in vec3 vertColor;
in vec4 vertexColourOut;
in vec2 vertextTextureCoordsOut;
uniform sampler2D textureSampler;

void main()
{
	color = texture2D(textureSampler, vertextTextureCoordsOut);
}