#include "Behaviour.h"
#include "BehaviourDirector.h"

std::atomic<int> Behaviour::nextId = 0;

Behaviour::Behaviour(void)
{
	id = nextId++;
	BehaviourDirector::RegisterBehaviour(this);
}

Behaviour::~Behaviour(void)
{
	BehaviourDirector::DeRegisterBehaviour(this);
}

void Behaviour::Update()
{

}

int Behaviour::GetID()
{
	return id;
}