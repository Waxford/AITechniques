#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <string>
#include <map>

namespace Core
{
	class Shader_Loader
	{
	private:
		static std::map<std::string, GLuint>* shaderMap;
		static std::string ReadShader(std::string filename);
		static GLuint CreateShader(GLenum shaderType,
			std::string source,
			std::string shaderName);

	public:
		static GLuint CreateProgram(char* VertexShaderFilename,
			char* FragmentShaderFilename);
	};
}
