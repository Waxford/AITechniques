#pragma once
#include "Behaviour.h"
#include "Renderable.h"

class Triangle : public Renderable
{
public:
	Triangle(void);
	~Triangle(void);
	void Update();
};