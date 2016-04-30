#include "Pather.h"
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include <list>
#include <set>
#include <unordered_set>

static int sqrt2 = 1.41421356237f;

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
	Step* last_step = CalculateAStarPath(start, destination, diagonal);
	std::cout << "Evaluated Tiles: " << evaluated_tiles << std::endl;
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

	while (search_space.size() > 0) {
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
	}
	return last_step;
}

Step* Pather::CalculateAStarPath(Tile* start, Tile* destination, bool diagonal) 
{
	std::unordered_set<Tile*> visited;
	std::set<Tile*> frontier;
	frontier.insert(start);
	std::map<Tile*, Tile*> cameFrom;
	std::vector<float> gScore;
	std::vector<float> fScore;
	for (int i = 0; i < grid->Width()*grid->Height(); ++i) {
		gScore.push_back(std::numeric_limits<float>::max());
		fScore.push_back(std::numeric_limits<float>::max());
	}
	gScore[grid->GetTileIndex(start)] = 0;
	fScore[grid->GetTileIndex(start)] = EstimateHeuristicCost(start, destination);
	Tile* current;
	while (frontier.size() > 0) {
		evaluated_tiles++;
		current = *frontier.begin();
		float cost = fScore[grid->GetTileIndex(current)];
		for (auto it = frontier.begin(); it != frontier.end(); ++it) {
			float compareCost = fScore[grid->GetTileIndex(*it)];
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
		std::vector<Tile*> neighbours;
		for (auto it = current->neighbours.begin(); it != current->neighbours.end(); ++it) {
			neighbours.push_back(*it);
		}
		if (diagonal) {
			for (auto it = current->diagonal_neighbours.begin(); it != current->diagonal_neighbours.end(); ++it) {
				Tile* crossTile1 = grid->GetTileClosestToPosition((*it)->x, current->y);
				Tile* crossTile2 = grid->GetTileClosestToPosition(current->x, (*it)->y);
				if (crossTile1->IsPathable() && crossTile2->IsPathable()) {
					neighbours.push_back(*it);
				}
			}
		}
		for (auto it = neighbours.begin(); it != neighbours.end(); it++) 
		{
			if (visited.count(*it) > 0 || !(*it)->IsPathable()) {
				continue;
			}
			float tentative_gScore = gScore[grid->GetTileIndex(current)] + EstimateHeuristicCost(current, *it);
			float tentative_fScore = tentative_gScore + EstimateHeuristicCost(*it, destination);
			if (frontier.count(*it) > 0 && fScore[grid->GetTileIndex(*it)] < tentative_fScore) {
				continue;
			}
			if (visited.count(*it) > 0 && fScore[grid->GetTileIndex(*it)] < tentative_fScore) {
				continue;
			}
			cameFrom[*it] = current;
			gScore[grid->GetTileIndex(*it)] = tentative_gScore;
			fScore[grid->GetTileIndex(*it)] = tentative_fScore;
			frontier.insert(*it);
		}
	}
}

float Pather::EstimateHeuristicCost(Tile* current, Tile* destination) 
{
	float dx = abs(current->x - destination->x);
	float dy = abs(current->y - destination->y);
	return sqrt(dx*dx + dy*dy);
}

Step* Pather::ReconstructPath(std::map<Tile*,Tile*> cameFrom, Tile* current) 
{
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
	return lastStep;
}