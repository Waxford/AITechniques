#include "Renderable.h"
#include "Shader_Loader.h"

Renderable::Renderable(void) 
{
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;
	scaleX = 1.0f;
	scaleY = 1.0f;
	shader = Core::Shader_Loader::CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");
	std::cout << "Renderable <" << id << "> constructed" << std::endl;
}

Renderable::~Renderable(void) 
{
	std::cout << "Renderable <" << id << "> deconstructed" << std::endl;
	glDeleteProgram(shader);
}

void Renderable::SetColor(float r, float g, float b, float a) {
	float colorArray[4] = {r, g, b, a};
	GLint colorLoc = glGetUniformLocation(shader, "color");
	glProgramUniform4fv(shader, colorLoc, 1, colorArray);
}

void Renderable::Update()
{
	glUseProgram(shader);
	Behaviour::Update();
}