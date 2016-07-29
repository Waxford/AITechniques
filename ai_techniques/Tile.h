#pragma once
#include "Renderable.h"
#include <vector>

class Grid;

class Tile :
	public Renderable
{
private:
	bool pathable;
public:
	int id;
	int indexX;
	int indexY;
	Grid* grid;
	Tile(bool);
	~Tile();
	bool IsPathable();
	void Recolour(float,float,float,float);
	virtual void Update();
	std::vector<Tile*> neighbours;
	std::vector<Tile*> diagonal_neighbours;
	std::string ToString();
};
