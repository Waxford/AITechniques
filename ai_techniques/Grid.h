#pragma once
#include "Tile.h"
#include <vector>
class Grid
{
private:
	int width;
	int height;
	int* definition;
public:
	Grid(int[],int,int);
	~Grid();
	std::vector<Tile*> tiles;
	Tile* GetTile(int, int);
	Tile* GetTileClosestToPosition(float, float);
	int Width();
	int Height();
};

