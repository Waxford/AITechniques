#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>

namespace Core
{
	class Shader_Loader
	{
	private:
		static std::string ReadShader(char *filename);
		static GLuint CreateShader(GLenum shaderType,
			std::string source,
			char* shaderName);

	public:
		static GLuint CreateProgram(char* VertexShaderFilename,
			char* FragmentShaderFilename);
	};
}
