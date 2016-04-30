#pragma once
#include "Renderable.h"
#include <vector>
class Tile :
	public Renderable
{
private:
	bool pathable;
public:
	Tile(bool);
	~Tile();
	bool IsPathable();
	void Recolour(float,float,float,float);
	virtual void Update();
	std::vector<Tile*> neighbours;
	std::vector<Tile*> diagonal_neighbours;
};
