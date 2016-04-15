#pragma once
#include "Renderable.h"
#include "Grid.h"
#include <list>
#include <vector>

struct Destination
{
	float x;
	float y;
	float r;
};

class Step {
public:
	Step* parent;
	Tile* tile;
	std::vector<Step*> children;
	float cost;
};

class Pather : public Renderable 
{
private:
	float speed;
	Destination* destination;
	std::list<Destination*> path;
	bool pathing;
	Grid* grid;
	Step* CalculateBFSPath(Tile*,Tile*,bool);

public:
	Pather(void);
	virtual ~Pather(void);
	virtual void Update();
	void AddDestination(float x, float y, float r);
	void SetGrid(Grid*);
	void SetDestination(int, int, bool);
};