#include "Behaviour.h"
#include "BehaviourDirector.h"

int Behaviour::nextId = 0;

Behaviour::Behaviour(void)
{
	id = nextId++;
	BehaviourDirector::RegisterBehaviour(this);
	std::cout << "Behaviour <" << id << "> constructed" << std::endl;
}

Behaviour::~Behaviour(void)
{
	std::cout << "Behaviour <" << id << "> deconstructed" << std::endl;
	BehaviourDirector::DeRegisterBehaviour(this);
}

void Behaviour::Update()
{

}

int Behaviour::GetID()
{
	return id;
}