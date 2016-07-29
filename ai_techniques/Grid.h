#pragma once
#include "Tile.h"
#include <vector>
#include <unordered_map>
#include <tuple>
class Grid
{
private:
	int width;
	int height;
	int* definition;
	std::unordered_map<Tile*, std::unordered_map<Tile*, Tile*>> best_path_map_diagonal;
	std::unordered_map<Tile*, std::unordered_map<Tile*, Tile*>> best_path_map_manhattan;

	void SetupBestPathMap();
public:
	Grid(int[],int,int);
	~Grid();
	std::vector<Tile*> tiles;
	Tile* GetTile(int, int);
	Tile* GetTileClosestToPosition(float, float);
	Tile* GetNextBestTile(Tile*, Tile*, bool);
	int Width();
	int Height();
};

