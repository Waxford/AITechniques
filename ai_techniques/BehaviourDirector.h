#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>

class Behaviour;

class BehaviourDirector
{
private:
	static BehaviourDirector* instance;
	std::set<int> layers;
	std::unordered_map<int,std::vector<Behaviour*>> behaviours;
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
	static void RegisterBehaviour(Behaviour*,int = 0);
	static void DeRegisterBehaviour(Behaviour*);
	static void ReorderBehaviour(Behaviour*, int);
	static void Tick();
};
