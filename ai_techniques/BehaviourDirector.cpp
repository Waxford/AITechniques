#include "BehaviourDirector.h"
#include "Behaviour.h"

BehaviourDirector* BehaviourDirector::instance;

BehaviourDirector::BehaviourDirector(void) {}
BehaviourDirector::~BehaviourDirector(void) {}

void BehaviourDirector::RegisterBehaviour(Behaviour* behaviour, int order)
{
	Instance()->layers.insert(order);
	Instance()->behaviours[order].push_back(behaviour);
}

void BehaviourDirector::DeRegisterBehaviour(Behaviour* behaviour)
{
	for (int i = 0; i < Instance()->behaviours[behaviour->GetOrder()].size(); ++i)
	{
		if (Instance()->behaviours[behaviour->GetOrder()][i]->GetID() == behaviour->GetID())
		{
			Instance()->behaviours[behaviour->GetOrder()].erase(Instance()->behaviours[behaviour->GetOrder()].begin() + i);
		}
	}
}

void BehaviourDirector::ReorderBehaviour(Behaviour* behaviour, int order)
{
	DeRegisterBehaviour(behaviour);
	RegisterBehaviour(behaviour, order);
}

void BehaviourDirector::Tick()
{
	for (auto it = Instance()->layers.begin(); it != Instance()->layers.end(); it++)
	{
		for (int i = 0; i < Instance()->behaviours[*it].size(); ++i)
		{
			Instance()->behaviours[*it][i]->Update();
		}
	}
}