#include "Pather.h"
#include "Time.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_set>

//#define DEBUG;
//#define DEEP_DEBUG;

static float sqrt2 = 1.41421356237f;
std::set<Pather*> Pather::g_all_pathers;
static float repath_rate = 0.1f;

Pather::Pather(void) : pathing(false), speed(0.002f), waypoint_radius(0.01f), collider_radius(0.02f), pathing_type(PathingType::MAPPED), time_since_repath(0.0f)
{
	Pather::g_all_pathers.insert(this);
}

Pather::~Pather(void)
{
	Pather::g_all_pathers.erase(this);
}

void Pather::Update()
{
	Renderable::Update();
	if (path.size() > 0)
	{
		LookAt(goal->x, goal->y);
		float dx = goal->x - x;
		float dy = goal->y - y;
		float dist = sqrtf(dx*dx + dy*dy);
		Tile* end_goal = path.back();
		time_since_repath += Time::GetDeltaTime();
		if (time_since_repath >= repath_rate && end_goal)
		{
			Tile* new_start = nullptr;
			path.clear();
			SetDestination(end_goal, true);
			time_since_repath = 0.0f;
			if (path.size() > 1) 
			{
				path.pop_front();
				goal = path.front();
			}
		}
		if (dist <= waypoint_radius)
		{
			x = goal->x;
			y = goal->y;
			pathing = false;
			path.pop_front();
			if (path.size() > 0)
			{
				goal = path.front();
			}
		}
		else
		{
			x += (dx / dist) * speed;
			y += (dy / dist) * speed;
		}
	}
	else {
		Tile* random_tile = grid->GetTile(rand() % 20, rand() % 20);
		if (random_tile->IsPathable()) {
			SetDestination(random_tile, true);
		}
	}
	for (auto it = Pather::g_all_pathers.begin(); it != Pather::g_all_pathers.end(); ++it) {
		float distX = x - (*it)->x;
		float distY = y - (*it)->y;
		if (distX + distY > (GetColliderRadius() + (*it)->GetColliderRadius()) * 2.0f) {
			continue;
		}
		float dist = sqrt(distX * distX + distY * distY);
		if (dist < (GetColliderRadius() + (*it)->GetColliderRadius()) && dist > 0.01) {
			float overlap = (GetColliderRadius() + (*it)->GetColliderRadius()) - dist;
			float xForce = ((distX / dist) / 2.0f) * overlap;
			float yForce = ((distY / dist) / 2.0f) * overlap;
			AddForce(xForce + (float)(rand() / RAND_MAX) * 0.0001f - 0.00005f, yForce + (float)(rand() / RAND_MAX) * 0.0001f - 0.00005f);
		}
	}
}

void Pather::LateUpdate() 
{
	x += forceX;// *speed;
	y += forceY;// *speed;
	forceX = 0;
	forceY = 0;
}

void Pather::AddWaypoint(float x, float y)
{
	Tile* point = grid->GetTileClosestToPosition(x,y);
	AddWaypoint(point);
}

void Pather::AddWaypoint(Tile* point) {
	path.push_back(point);
	if (path.size() == 1)
	{
		goal = point;
	}
}

void Pather::SetGrid(Grid* grid) 
{
	this->grid = grid;
}

void Pather::SetDestination(int x, int y, bool diagonal)
{
	SetDestination(grid->GetTile(x, y), diagonal);
}

int evaluated_tiles = 0;
void Pather::SetDestination(Tile* destination, bool diagonal) {
	if (!grid)
		return;

	if (goal)
		goal->Recolour(0.0f, 0.0f, 0.0f, 1.0f);

	goal = destination;
	goal->Recolour(this->r / 4.0f, this->g / 4.0f, this->b / 4.0f, this->a);
	Tile* start;
	if (path.size() == 0) {
		start = grid->GetTileClosestToPosition(this->x, this->y);
	}
	else {
		start = path.back();
	}
	evaluated_tiles = 0;
	Step* last_step;
	switch(pathing_type)
	{
		case PathingType::BFS:
		{
			last_step = CalculateBFSPath(start, goal, diagonal);
			int bfs_eval_tiles = evaluated_tiles;
			evaluated_tiles = 0;
			break;
		}
		case PathingType::ASTAR:
		{
			last_step = CalculateAStarPath(start, goal, diagonal);
			int a_star_eval_tiles = evaluated_tiles;
			evaluated_tiles = 0;
			break;
		}
		case PathingType::MAPPED:
		{
			last_step = CalculateMappedPath(start, goal, diagonal);
			int mapped_eval_tiles = evaluated_tiles;
			evaluated_tiles = 0;
			break;
		}
		case PathingType::CONGESTION:
		{
			last_step = CalculateCongestionPath(start, goal, diagonal);
			int congestion_eval_tiles = evaluated_tiles;
			evaluated_tiles = 0;
			break;
		}
		default:
		{
			last_step = CalculateBFSPath(start, goal, diagonal);
			int bfs_eval_tiles = evaluated_tiles;
			evaluated_tiles = 0;
			break;
		}
	}
	//std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point t4 = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point t5 = std::chrono::high_resolution_clock::now();
	//std::chrono::high_resolution_clock::time_point t6 = std::chrono::high_resolution_clock::now();
#ifdef DEBUG
	//std::cout << "------------STATS------------" << std::endl;
	//std::cout << "~Runtime~" << std::endl;
	//std::cout << "BFS:     " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	//std::cout << "A*:      " << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << std::endl;
	//std::cout << "Mapped:  " << std::chrono::duration_cast<std::chrono::microseconds>(t6 - t5).count() << std::endl;
	//std::cout << "~Evaluated Tiles~" << std::endl;
	//std::cout << "BFS:     " << bfs_eval_tiles << std::endl;
	//std::cout << "A*:      " << a_star_eval_tiles << std::endl;
	//std::cout << "Mapped:  " << mapped_eval_tiles << std::endl;
#endif
	std::list<Tile*> solution;
	int stepcount = 0;
	while (last_step->parent != last_step) {
		stepcount++;
		solution.push_front(last_step->tile);
		Step* to_delete = last_step;
		last_step = last_step->parent;
		delete(to_delete);
	}
	solution.push_front(last_step->tile);
	for (auto it = solution.begin(); it != solution.end(); ++it) {
		AddWaypoint((*it));
	}
}

Step* Pather::CalculateBFSPath(Tile* start, Tile* destination, bool diagonal)
{
	Step* first_step = new Step();
	first_step->tile = start;
	first_step->parent = first_step;
	first_step->cost = 0;
	Step* last_step = first_step;
	std::list<Step*> search_space;
	search_space.push_back(first_step);
	
	std::unordered_set<Tile*> explored;
	explored.insert(first_step->tile);

	std::vector<Tile*> neighbours;

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

	while (search_space.size() > 0) {
		t1 = std::chrono::high_resolution_clock::now();
		evaluated_tiles++;
		Step* current = search_space.front();
		search_space.pop_front();
		neighbours = current->tile->neighbours;
		for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
			if ((*it)->IsPathable() && explored.count(*it) == 0) {
				Step* child = new Step();
				child->tile = *it;
				child->parent = current;
				child->cost = current->cost + 1;
				current->children.push_back(child);
				search_space.push_back(child);
				explored.insert(*it);
				if (*it == destination) {
					last_step = child;
					search_space.clear();
				}
			}
		}
		if (diagonal) {
			neighbours = current->tile->diagonal_neighbours;
			for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
				if ((*it)->IsPathable() && explored.count(*it) == 0) {
					Tile* crossTile1 = grid->GetTileClosestToPosition((*it)->x, current->tile->y);
					Tile* crossTile2 = grid->GetTileClosestToPosition(current->tile->x, (*it)->y);
					if (crossTile1->IsPathable() && crossTile2->IsPathable()) {
						Step* child = new Step();
						child->tile = *it;
						child->parent = current;
						child->cost = current->cost + sqrt2;
						current->children.push_back(child);
						search_space.push_back(child);
						explored.insert(*it);
						if (*it == destination) {
							last_step = child;
							search_space.clear();
						}
					}
				}
			}
		}
		t2 = std::chrono::high_resolution_clock::now();
#ifdef DEEP_DEBUG
		std::cout << "tileSearch[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	}
	return last_step;
}

Step* Pather::CalculateAStarPath(Tile* start, Tile* destination, bool diagonal) 
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
#ifdef DEEP_DEBUG
	t1 = std::chrono::high_resolution_clock::now();
#endif
	std::unordered_set<Tile*> visited;
	std::unordered_set<Tile*> frontier;
	frontier.insert(start);
	std::map<Tile*, Tile*> cameFrom;
	std::vector<float> gScore;
	std::vector<float> fScore;
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Setup: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	t1 = std::chrono::high_resolution_clock::now();
#endif
	for (int i = 0; i < grid->Width()*grid->Height(); ++i) {
		gScore.push_back(std::numeric_limits<float>::max());
		fScore.push_back(std::numeric_limits<float>::max());
	}
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Iniialization: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	t1 = std::chrono::high_resolution_clock::now();
#endif
	gScore[start->id] = 0;
	fScore[start->id] = EstimateHeuristicCost(start, destination, diagonal);
	Tile* current;
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Init Heuristic: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	std::vector<Tile*> neighbours = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	std::vector<float> neighbourCosts = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	while (frontier.size() > 0) {
#ifdef DEEP_DEBUG
		t1 = std::chrono::high_resolution_clock::now();
#endif
		evaluated_tiles++;
		current = *frontier.begin();
		float cost = fScore[current->id];
		for (auto it = frontier.begin(); it != frontier.end(); ++it) {
			float compareCost = fScore[(*it)->id];
			if (compareCost < cost) {
				current = *it;
				cost = compareCost;
			}
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "tileSearch["<<evaluated_tiles<<"]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
		if (current == destination) {
			return ReconstructPath(cameFrom, current);
		}
#ifdef DEEP_DEBUG
		t1 = std::chrono::high_resolution_clock::now();
#endif
		frontier.erase(current);
		visited.insert(current);
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "insert/delete[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		t1 = std::chrono::high_resolution_clock::now();
#endif
		int neighbourCount = 0;
		for (int i = 0; i < current->neighbours.size(); ++i) {
			if (current->neighbours[i]->IsPathable()) {
				neighbours[neighbourCount] = current->neighbours[i];
				neighbourCosts[neighbourCount] = 1.0f;
				++neighbourCount;
			}
		}
		if (diagonal) {
			for (int i = 0; i < current->diagonal_neighbours.size(); ++i) {
				if (current->diagonal_neighbours[i]->IsPathable())
				{
					Tile* crossTile1 = grid->GetTileClosestToPosition(current->diagonal_neighbours[i]->x, current->y);
					Tile* crossTile2 = grid->GetTileClosestToPosition(current->x, current->diagonal_neighbours[i]->y);
					if (crossTile1->IsPathable() && crossTile2->IsPathable()) {
						neighbours[neighbourCount] = current->diagonal_neighbours[i];
						neighbourCosts[neighbourCount] = sqrt2;
						++neighbourCount;
					}
				}
			}
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "neighbourGet[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		t1 = std::chrono::high_resolution_clock::now();
#endif
		for (int i = 0; i < neighbourCount; ++i) 
		{
			Tile* neighbour = neighbours[i];
			if (visited.count(neighbour) > 0) {
				continue;
			}
#ifdef DEEP_DEBUG
			t1 = std::chrono::high_resolution_clock::now();
#endif
			float tentative_gScore = gScore[current->id] + neighbourCosts[i];
			float tentative_fScore = tentative_gScore + EstimateHeuristicCost(neighbour, destination, diagonal);
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation1[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
			if (frontier.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
			if (visited.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
#ifdef DEEP_DEBUG
			t1 = std::chrono::high_resolution_clock::now();
#endif

			cameFrom[neighbour] = current;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation2[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			gScore[neighbour->id] = tentative_gScore;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation3[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			fScore[neighbour->id] = tentative_fScore;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation4[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			frontier.insert(neighbour);
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation5[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "aStarStuff[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	}
	return nullptr;
}

Step* Pather::CalculateCongestionPath(Tile* start, Tile* destination, bool diagonal)
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
#ifdef DEEP_DEBUG
	t1 = std::chrono::high_resolution_clock::now();
#endif
	std::unordered_set<Tile*> visited;
	std::unordered_set<Tile*> frontier;
	frontier.insert(start);
	std::map<Tile*, Tile*> cameFrom;
	std::vector<float> gScore;
	std::vector<float> fScore;
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Setup: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	t1 = std::chrono::high_resolution_clock::now();
#endif
	for (int i = 0; i < grid->Width()*grid->Height(); ++i) {
		gScore.push_back(std::numeric_limits<float>::max());
		fScore.push_back(std::numeric_limits<float>::max());
	}
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Iniialization: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	t1 = std::chrono::high_resolution_clock::now();
#endif
	gScore[start->id] = 0;
	fScore[start->id] = EstimateHeuristicCost(start, destination, diagonal) + start->GetCongestion();
	Tile* current;
#ifdef DEEP_DEBUG
	t2 = std::chrono::high_resolution_clock::now();
	std::cout << "Init Heuristic: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	std::vector<Tile*> neighbours = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	std::vector<float> neighbourCosts = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	while (frontier.size() > 0) {
#ifdef DEEP_DEBUG
		t1 = std::chrono::high_resolution_clock::now();
#endif
		evaluated_tiles++;
		current = *frontier.begin();
		float cost = fScore[current->id];
		for (auto it = frontier.begin(); it != frontier.end(); ++it) {
			float compareCost = fScore[(*it)->id];
			if (compareCost < cost) {
				current = *it;
				cost = compareCost;
			}
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "tileSearch[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
		if (current == destination) {
			return ReconstructPath(cameFrom, current);
		}
#ifdef DEEP_DEBUG
		t1 = std::chrono::high_resolution_clock::now();
#endif
		frontier.erase(current);
		visited.insert(current);
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "insert/delete[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		t1 = std::chrono::high_resolution_clock::now();
#endif
		int neighbourCount = 0;
		for (int i = 0; i < current->neighbours.size(); ++i) {
			if (current->neighbours[i]->IsPathable()) {
				neighbours[neighbourCount] = current->neighbours[i];
				neighbourCosts[neighbourCount] = 1.0f;
				++neighbourCount;
			}
		}
		if (diagonal) {
			for (int i = 0; i < current->diagonal_neighbours.size(); ++i) {
				if (current->diagonal_neighbours[i]->IsPathable())
				{
					Tile* crossTile1 = grid->GetTileClosestToPosition(current->diagonal_neighbours[i]->x, current->y);
					Tile* crossTile2 = grid->GetTileClosestToPosition(current->x, current->diagonal_neighbours[i]->y);
					if (crossTile1->IsPathable() && crossTile2->IsPathable()) {
						neighbours[neighbourCount] = current->diagonal_neighbours[i];
						neighbourCosts[neighbourCount] = sqrt2;
						++neighbourCount;
					}
				}
			}
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "neighbourGet[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		t1 = std::chrono::high_resolution_clock::now();
#endif
		for (int i = 0; i < neighbourCount; ++i)
		{
			Tile* neighbour = neighbours[i];
			if (visited.count(neighbour) > 0) {
				continue;
			}
#ifdef DEEP_DEBUG
			t1 = std::chrono::high_resolution_clock::now();
#endif
			float tentative_gScore = gScore[current->id] + neighbourCosts[i];
			float tentative_fScore = tentative_gScore + EstimateHeuristicCost(neighbour, destination, diagonal) + neighbour->GetCongestion();
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation1[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
			if (frontier.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
			if (visited.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
#ifdef DEEP_DEBUG
			t1 = std::chrono::high_resolution_clock::now();
#endif

			cameFrom[neighbour] = current;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation2[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			gScore[neighbour->id] = tentative_gScore;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation3[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			fScore[neighbour->id] = tentative_fScore;
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation4[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			t1 = std::chrono::high_resolution_clock::now();
#endif
			frontier.insert(neighbour);
#ifdef DEEP_DEBUG
			t2 = std::chrono::high_resolution_clock::now();
			std::cout << "estimation5[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
		}
#ifdef DEEP_DEBUG
		t2 = std::chrono::high_resolution_clock::now();
		std::cout << "aStarStuff[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	}
	return nullptr;
}

Step* Pather::CalculateMappedPath(Tile* start, Tile* destination, bool diagonal) 
{
	Tile* current = destination;
	Step* step = new Step();
	step->tile = current;
	Step* lastStep = step;
	evaluated_tiles++;
	while (current != start) 
	{
		evaluated_tiles++;
		current = grid->GetNextBestTile(current, start, diagonal);
		Step* prevStep = new Step();
		prevStep->tile = current;
		step->parent = prevStep;
		prevStep->children.push_back(step);
		step = prevStep;
	}
	step->parent = step;
	return lastStep;
}

Step* Pather::CalculateAStarPathGlobal(Grid* grid, Tile* start, Tile* destination, bool diagonal)
{
	std::unordered_set<Tile*> visited;
	std::unordered_set<Tile*> frontier;
	frontier.insert(start);
	std::map<Tile*, Tile*> cameFrom;
	std::vector<float> gScore;
	std::vector<float> fScore;
	for (int i = 0; i < grid->Width()*grid->Height(); ++i) {
		gScore.push_back(std::numeric_limits<float>::max());
		fScore.push_back(std::numeric_limits<float>::max());
	}
	gScore[start->id] = 0;
	fScore[start->id] = EstimateHeuristicCost(start, destination, diagonal);
	Tile* current;
	std::vector<Tile*> neighbours = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	std::vector<float> neighbourCosts = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	while (frontier.size() > 0) {
		evaluated_tiles++;
		current = *frontier.begin();
		float cost = fScore[current->id];
		for (auto it = frontier.begin(); it != frontier.end(); ++it) {
			float compareCost = fScore[(*it)->id];
			if (compareCost < cost) {
				current = *it;
				cost = compareCost;
			}
		}
		if (current == destination) {
			return ReconstructPath(cameFrom, current);
		}
		frontier.erase(current);
		visited.insert(current);
		int neighbourCount = 0;
		for (int i = 0; i < current->neighbours.size(); ++i) {
			if (current->neighbours[i]->IsPathable()) {
				neighbours[neighbourCount] = current->neighbours[i];
				neighbourCosts[neighbourCount] = 1.0f;
				++neighbourCount;
			}
		}
		if (diagonal) {
			for (int i = 0; i < current->diagonal_neighbours.size(); ++i) {
				if (current->diagonal_neighbours[i]->IsPathable())
				{
					Tile* crossTile1 = grid->GetTileClosestToPosition(current->diagonal_neighbours[i]->x, current->y);
					Tile* crossTile2 = grid->GetTileClosestToPosition(current->x, current->diagonal_neighbours[i]->y);
					if (crossTile1->IsPathable() && crossTile2->IsPathable()) {
						neighbours[neighbourCount] = current->diagonal_neighbours[i];
						neighbourCosts[neighbourCount] = sqrt2;
						++neighbourCount;
					}
				}
			}
		}
		for (int i = 0; i < neighbourCount; ++i)
		{
			Tile* neighbour = neighbours[i];
			if (visited.count(neighbour) > 0) {
				continue;
			}
			float tentative_gScore = gScore[current->id] + neighbourCosts[i];
			float tentative_fScore = tentative_gScore + EstimateHeuristicCost(neighbour, destination, diagonal);
			if (frontier.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
			if (visited.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}

			cameFrom[neighbour] = current;
			gScore[neighbour->id] = tentative_gScore;
			fScore[neighbour->id] = tentative_fScore;
			frontier.insert(neighbour);
		}
	}
}

float Pather::CalculateDistance(Tile* current, Tile* destination)
{
	float dx = abs((float)(current->indexX - destination->indexX));
	float dy = abs((float)(current->indexY - destination->indexY));
	return sqrt(dx*dx + dy*dy);
}

float Pather::EstimateHeuristicCost(Tile* current, Tile* destination, bool diagonal) 
{
	//return CalculateDistance(current, destination);
	float dx = abs((float)(current->indexX - destination->indexX));
	float dy = abs((float)(current->indexY - destination->indexY));
	return diagonal ? dx + dy + (sqrt2 - 2.0f) * std::fminf(dx, dy) : dx + dy;
}

Step* Pather::ReconstructPath(std::map<Tile*,Tile*> cameFrom, Tile* current) 
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	Step* step = new Step();
	step->tile = current;
	Step* lastStep = step;
	while (cameFrom.count(current) > 0) {
		current = cameFrom[current];
		Step* prevStep = new Step();
		prevStep->tile = current;
		step->parent = prevStep;
		prevStep->children.push_back(step);
		step = prevStep;
	}
	step->parent = step;
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
#ifdef DEEP_DEBUG
	std::cout << "Recon: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
#endif
	return lastStep;
}

void Pather::AddForce(float x, float y) 
{
	forceX += x;
	forceY += y;
}

float Pather::GetColliderRadius() {
	return collider_radius;
}