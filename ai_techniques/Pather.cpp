#include "Pather.h"
#include <stdio.h>
#include <stdlib.h>
#include <list>


Pather::Pather(void)
{
	pathing = false;
	speed = 0.008f;
}


Pather::~Pather(void)
{
	free(destination);
}

void Pather::Update()
{
	Renderable::Update();
	if(path.size() > 0)
	{
		LookAt(destination->x, destination->y);
		float dx = destination->x - x;
		float dy = destination->y - y;
		float dist = sqrtf(dx*dx + dy*dy);
		if(dist <= destination->r)
		{
			x = destination->x;
			y = destination->y;
			pathing = false;
			path.pop_front();
			free(destination);
			if(path.size() > 0)
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

void  Pather::AddDestination(float x, float y, float r)
{
	Destination* newDestination = new Destination();
	newDestination->x = x;
	newDestination->y = y;
	newDestination->r = r;
	path.push_back(newDestination);
	if(path.size() == 1)
	{
		destination = newDestination;
	}
}