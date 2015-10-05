#include "Renderable.h"

Renderable::Renderable(void) 
{
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	x = 0.0f;
	y = 0.0f;
	rotation = 0.0f;
	scaleX = 1.0f;
	scaleY = 1.0f;
	std::cout << "Renderable <" << id << "> constructed" << std::endl;
}
Renderable::~Renderable(void) 
{
	std::cout << "Renderable <" << id << "> deconstructed" << std::endl;
}

void Renderable::Update()
{
	Behaviour::Update();
}