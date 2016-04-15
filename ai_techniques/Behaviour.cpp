#include "Behaviour.h"
#include "BehaviourDirector.h"

std::atomic<int> Behaviour::nextId = 0;

Behaviour::Behaviour(void) : order(0)
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

void Behaviour::SetOrder(int order) 
{
	this->order = order;
	BehaviourDirector::ReorderBehaviour(this,order);
}

int Behaviour::GetOrder() 
{
	return this->order;
}