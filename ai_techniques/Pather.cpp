#include "Pather.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_set>

static float sqrt2 = 1.41421356237f;
static bool debug = true;
static bool deep_debug = true;

Pather::Pather(void) : pathing(false), speed(0.003f), waypoint_radius(0.01f)
{
}

Pather::~Pather(void)
{
	free(destination);
}

void Pather::Update()
{
	Renderable::Update();
	if (path.size() > 0)
	{
		LookAt(destination->x, destination->y);
		float dx = destination->x - x;
		float dy = destination->y - y;
		float dist = sqrtf(dx*dx + dy*dy);
		if (dist <= waypoint_radius)
		{
			x = destination->x;
			y = destination->y;
			pathing = false;
			path.pop_front();
			if (path.size() > 0)
			{
				destination = path.front();
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
		destination = point;
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
void Pather::SetDestination(Tile* dest, bool diagonal) {
	if (!grid)
		return;

	if (destination)
		destination->Recolour(0.0f, 0.0f, 0.0f, 1.0f);

	destination = dest;
	destination->Recolour(this->r, this->g, this->b, this->a);
	Tile* start;
	if (path.size() == 0) {
		start = grid->GetTileClosestToPosition(this->x, this->y);
	}
	else {
		start = path.back();
	}
	evaluated_tiles = 0;
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	Step* last_step = CalculateAStarPath(start, destination, diagonal);
	//Step* last_step = CalculateBFSPath(start, destination, diagonal);
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	if (debug) std::cout << "Runtime " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	if(debug) std::cout << "Evaluated Tiles: " << evaluated_tiles << std::endl;
	std::list<Tile*> solution;
	while (last_step->parent != last_step) {
		solution.push_front(last_step->tile);
		last_step = last_step->parent;
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
		if (deep_debug) std::cout << "tileSearch[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	}
	return last_step;
}

Step* Pather::CalculateAStarPath(Tile* start, Tile* destination, bool diagonal) 
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
	if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
	std::unordered_set<Tile*> visited;
	std::unordered_set<Tile*> frontier;
	frontier.insert(start);
	std::map<Tile*, Tile*> cameFrom;
	std::vector<float> gScore;
	std::vector<float> fScore;
	if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
	if (deep_debug) std::cout << "Setup: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < grid->Width()*grid->Height(); ++i) {
		gScore.push_back(std::numeric_limits<float>::max());
		fScore.push_back(std::numeric_limits<float>::max());
	}
	if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
	if (deep_debug) std::cout << "Iniialization: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
	gScore[start->id] = 0;
	fScore[start->id] = EstimateHeuristicCost(start, destination, diagonal);
	Tile* current;
	if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
	if (deep_debug) std::cout << "Init Heuristic: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	std::vector<Tile*> neighbours = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	std::vector<float> neighbourCosts = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }
	;
	while (frontier.size() > 0) {
		if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
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
		if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
		if (deep_debug) std::cout << "tileSearch["<<evaluated_tiles<<"]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		if (current == destination) {
			return ReconstructPath(cameFrom, current);
		}
		if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
		frontier.erase(current);
		visited.insert(current);
		if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
		if (deep_debug) std::cout << "insert/delete[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
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
		if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
		if (deep_debug) std::cout << "neighbourGet[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < neighbourCount; ++i) 
		{
			Tile* neighbour = neighbours[i];
			if (visited.count(neighbour) > 0) {
				continue;
			}
			if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
			float tentative_gScore = gScore[current->id] + neighbourCosts[i];
			float tentative_fScore = tentative_gScore + EstimateHeuristicCost(neighbour, destination, diagonal);
			if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
			if (deep_debug) std::cout << "estimation1[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			if (frontier.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
			if (visited.count(neighbour) > 0 && fScore[neighbour->id] < tentative_fScore) {
				continue;
			}
			if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
			cameFrom[neighbour] = current;
			if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
			if (deep_debug) std::cout << "estimation2[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
			gScore[neighbour->id] = tentative_gScore;
			if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
			if (deep_debug) std::cout << "estimation3[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
			fScore[neighbour->id] = tentative_fScore;
			if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
			if (deep_debug) std::cout << "estimation4[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
			if (deep_debug) t1 = std::chrono::high_resolution_clock::now();
			frontier.insert(neighbour);
			if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
			if (deep_debug) std::cout << "estimation5[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
		}
		if (deep_debug) t2 = std::chrono::high_resolution_clock::now();
		if (deep_debug) std::cout << "aStarStuff[" << evaluated_tiles << "]: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	}
}

float Pather::CalculateDistance(Tile* current, Tile* destination)
{
	float dx = abs(current->indexX - destination->indexX);
	float dy = abs(current->indexY - destination->indexY);
	return sqrt(dx*dx + dy*dy);
}

float Pather::EstimateHeuristicCost(Tile* current, Tile* destination, bool diagonal) 
{
	//return CalculateDistance(current, destination);
	float dx = abs(current->indexX - destination->indexX);
	float dy = abs(current->indexY - destination->indexY);
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
	if (deep_debug) std::cout << "Recon: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << std::endl;
	return lastStep;
}