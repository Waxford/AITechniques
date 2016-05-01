#pragma once
#include "Renderable.h"
#include "Grid.h"
#include <list>
#include <map>
#include <vector>

class Step {
public:
	Step* parent;
	std::vector<Step*> children;
	Tile* tile;
	float cost;
};

class Pather : public Renderable 
{
private:
	bool pathing;
	float speed;
	float waypoint_radius;
	Tile* destination;
	std::list<Tile*> path;
	Grid* grid;
	Step* CalculateBFSPath(Tile*,Tile*,bool);
	Step* CalculateAStarPath(Tile*, Tile*, bool);
	float CalculateDistance(Tile*, Tile*);
	float EstimateHeuristicCost(Tile*, Tile*, bool);
	Step* ReconstructPath(std::map<Tile*,Tile*>, Tile*);


public:
	Pather(void);
	virtual ~Pather(void);
	virtual void Update();
	void AddWaypoint(float x, float y);
	void AddWaypoint(Tile*);
	void SetGrid(Grid*);
	void SetDestination(int, int, bool);
	void SetDestination(Tile*, bool);
};