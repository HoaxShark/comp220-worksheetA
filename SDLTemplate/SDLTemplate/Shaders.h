#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>

#include <GL\glew.h>
#include <SDL_opengl.h>


class Shader
{
public:
	Shader();
	~Shader();

	GLint GetUniform(std::string name);
	GLuint& GetshaderProgramID()
	{
		return shaderProgramID;
	}
	
	GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);
	void Use();
	bool Load(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

private:
	GLuint shaderProgramID;
	std::map<std::string, GLint> uniformMap;

	void InitialiseUniforms();
};