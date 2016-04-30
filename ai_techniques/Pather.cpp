#include "Pather.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <cmath>
#include <unordered_set>

static int sqrt2 = 1.41421356237f;

Pather::Pather(void) : pathing(false), speed(0.008f), waypoint_radius(0.01f)
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
		if (random_tile->IsPathable())
			SetDestination(random_tile, true);
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
	Step* last_step = CalculateBFSPath(start, destination, diagonal);
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