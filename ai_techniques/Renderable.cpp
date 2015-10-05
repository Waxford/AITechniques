#include "Renderable.h"

Renderable::Renderable(void) 
{
	//Behaviour::Behaviour();
	std::cout << "Renderable <" << id << "> constructed" << std::endl;
}
Renderable::~Renderable(void) 
{
	std::cout << "Renderable <" << id << "> deconstructed" << std::endl;
	//Behaviour::~Behaviour();
}

void Renderable::Update()
{
	Behaviour::Update();
}