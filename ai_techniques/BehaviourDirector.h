#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Behaviour.h"
#include <iostream>
#include <vector>

class BehaviourDirector
{
private:
	static BehaviourDirector* instance;
	std::vector<Behaviour*> behaviours;
public:
	BehaviourDirector(void);
	~BehaviourDirector(void);
	static BehaviourDirector* Instance()
	{
		if (instance == nullptr)
		{
			instance = new BehaviourDirector();
		}
		return instance;
	}
	static void RegisterBehaviour(Behaviour*);
	static void DeRegisterBehaviour(Behaviour*);
	static void Tick();
};
