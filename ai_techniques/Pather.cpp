#include "Pather.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <cmath>
#include <unordered_set>

static int sqrt2 = 1.41421356237f;

Pather::Pather(void) : pathing(false), speed(0.0028f)
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
		if (dist <= destination->r)
		{
			x = destination->x;
			y = destination->y;
			pathing = false;
			path.pop_front();
			free(destination);
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
}

void Pather::AddDestination(float x, float y, float r)
{
	Destination* newDestination = new Destination();
	newDestination->x = x;
	newDestination->y = y;
	newDestination->r = r;
	path.push_back(newDestination);
	if (path.size() == 1)
	{
		destination = newDestination;
	}
}

void Pather::SetGrid(Grid* grid) 
{
	this->grid = grid;
}

void Pather::SetDestination(int x, int y, bool diagonal)
{
	if (!grid)
		return;
	Tile* destination = grid->GetTile(x, y);
	grid->GetTile(x,y)->Recolour(0.3f, 0.8f, 0.3f, 1.0f);
	Tile* start;
	if (path.size() == 0) {
		start = grid->GetTileClosestToPosition(this->x, this->y);
	}
	else {
		Destination* d = path.back();
		start = grid->GetTileClosestToPosition(d->x, d->y);
	}
	Step* last_step = CalculateBFSPath(start, destination, diagonal);
	std::list<Tile*> solution;
	while (last_step->parent != last_step) {
		solution.push_front(last_step->tile);
		last_step = last_step->parent;
	}
	solution.push_front(last_step->tile);
	for (auto it = solution.begin(); it != solution.end(); ++it) {
		std::cout << "Step" << (*it)->x << "," << (*it)->y << std::endl;
		AddDestination((*it)->x, (*it)->y, 0.01f);
	}
}

Step* Pather::CalculateBFSPath(Tile* start, Tile* destination, bool diagonal)
{
	std::cout << "Start: " << start->x << "," << start->y << " === End: " << destination->x << "," << destination->y << std::endl;
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
	return last_step;
}