#pragma once
#include "Renderable.h"
#include "Grid.h"
#include <list>
#include <set>
#include <map>
#include <vector>

class Step {
public:
	Step* parent;
	std::vector<Step*> children;
	Tile* tile;
	float cost;
};

enum PathingType
{
	BFS = 0,
	ASTAR = 1,
	MAPPED = 2,
	CONGESTION = 3
};

class Pather : public Renderable 
{
private:
	bool pathing;
	float speed;
	float waypoint_radius;
	float collider_radius;
	float forceX;
	float forceY;
	float time_since_repath;
	Tile* goal;
	std::list<Tile*> path;
	Grid* grid;
	Step* CalculateBFSPath(Tile*,Tile*,bool);
	Step* CalculateAStarPath(Tile*, Tile*, bool);
	Step* CalculateCongestionPath(Tile*, Tile*, bool);
	Step* CalculateMappedPath(Tile*, Tile*, bool);
	float CalculateDistance(Tile*, Tile*);
	static float EstimateHeuristicCost(Tile*, Tile*, bool);
	static Step* ReconstructPath(std::map<Tile*,Tile*>, Tile*);


public:
	static std::set<Pather*> g_all_pathers;
	PathingType pathing_type;

	Pather(void);
	virtual ~Pather(void);
	virtual void Update() override;
	virtual void LateUpdate() override;
	void AddWaypoint(float x, float y);
	void AddWaypoint(Tile*);
	void SetGrid(Grid*);
	void SetDestination(int, int, bool);
	void SetDestination(Tile*, bool);
	void AddForce(float x, float y);
	float GetColliderRadius();

	static Step* CalculateAStarPathGlobal(Grid*, Tile*, Tile*, bool);
};