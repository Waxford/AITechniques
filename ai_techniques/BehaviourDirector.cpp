#include "BehaviourDirector.h"
#include "Behaviour.h"

BehaviourDirector* BehaviourDirector::instance;

BehaviourDirector::BehaviourDirector(void) {}
BehaviourDirector::~BehaviourDirector(void) {}

void BehaviourDirector::RegisterBehaviour(Behaviour* behaviour)
{
	Instance()->behaviours.push_back(behaviour);
}

void BehaviourDirector::DeRegisterBehaviour(Behaviour* behaviour)
{
	for (int i = 0; i < Instance()->behaviours.size(); ++i)
	{
		if (Instance()->behaviours[i]->GetID() == behaviour->GetID())
		{
			Instance()->behaviours.erase(Instance()->behaviours.begin() + i);
		}
	}
}

void BehaviourDirector::Tick()
{
	for (int i = 0; i < Instance()->behaviours.size(); ++i)
	{
		Instance()->behaviours[i]->Update();
	}
}