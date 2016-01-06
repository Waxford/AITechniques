#pragma once
#include "Renderable.h"
#include <list>

struct Destination
{
	float x;
	float y;
	float r;
};

class Pather :
	public Renderable
{
private:
	float speed;
	Destination* destination;
	std::list<Destination*> path;
	bool pathing;

public:
	Pather(void);
	virtual ~Pather(void);
	virtual void Update();
	void AddDestination(float x, float y, float r);
};

