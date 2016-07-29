#include "Grid.h"
#include "Pather.h"
#include "Dependencies\freeglut\freeglut.h"
#include <cmath>

GLfloat squareArray[] = {
	-0.05f, -0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,
	-0.05f, 0.05f, 0.0f,
	-0.05f, 0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,
	0.05f, 0.05f, 0.0f,
};
std::vector<GLfloat> squareVerts(squareArray, squareArray + sizeof(squareArray) / sizeof(squareArray[0]));

Grid::Grid(int definition[], int width, int height)
{
	this->definition = definition;
	this->width = width;
	this->height = height;
	for (int i = height - 1; i >= 0; --i) {
		for (int j = 0; j < width; ++j) {
			Tile* gridTile = new Tile(definition[j + (height * i)] == 0);
			gridTile->id = j + height * i;
			gridTile->indexX = height - i;
			gridTile->indexY = j;
			gridTile->grid = this;
			gridTile->SetVertices(&squareVerts);
			gridTile->x = (j - 9.5f) / 10.0f;
			gridTile->y = (9.5f - i) / 10.0f;
			gridTile->scaleX = .95f;
			gridTile->scaleY = .95f;
			tiles.push_back(gridTile);
		}
	}
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Tile* tile = GetTile(j, i);
			if (j > 0)
				tile->neighbours.push_back(GetTile(j - 1, i));
			if (j < width - 1)
				tile->neighbours.push_back(GetTile(j + 1, i));
			if (i > 0)
				tile->neighbours.push_back(GetTile(j, i - 1));
			if (i < height - 1)
				tile->neighbours.push_back(GetTile(j, i + 1));

			if (j > 0 && i > 0)
				tile->diagonal_neighbours.push_back(GetTile(j - 1, i - 1));
			if (j < width - 1 && i > 0)
				tile->diagonal_neighbours.push_back(GetTile(j + 1, i - 1));
			if (j > 0 && i < height - 1)
				tile->diagonal_neighbours.push_back(GetTile(j - 1, i + 1));
			if (j < width - 1 && i < height - 1)
				tile->diagonal_neighbours.push_back(GetTile(j + 1, i + 1));
		}
	}
	SetupBestPathMap();
}


Grid::~Grid()
{

}

void Grid::SetupBestPathMap()
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			Tile* tile = GetTile(j, i);
			if (!tile->IsPathable()) {
				continue;
			}
			for (int k = height - 1; k >= 0; --k) {
				for (int l = width - 1; l >= 0; --l) {
					if (i == k && j == l) 
					{
						best_path_map_diagonal[tile][tile] = tile;
						best_path_map_manhattan[tile][tile] = tile;
						continue;
					}
					Tile* dest = GetTile(l, k);
					if (!dest->IsPathable()) {
						continue;
					}
					Step* step;
					if (best_path_map_diagonal[tile][dest] == NULL) {
						step = Pather::CalculateAStarPathGlobal(this, tile, dest, true);
						Step* root = step;
						while (step->parent != step) {
							Step* back_step = root;
							while (back_step != step->parent) {
								best_path_map_diagonal[step->parent->tile][back_step->tile] = step->tile;
								best_path_map_diagonal[back_step->tile][step->parent->tile] = back_step->parent->tile;
								back_step = back_step->parent;
							}
							step = step->parent;
						}
					}
					if (best_path_map_manhattan[tile][dest] == NULL) {
						step = Pather::CalculateAStarPathGlobal(this, tile, dest, true);
						Step* root = step;
						while (step->parent != step) {
							Step* back_step = root;
							while (back_step != step->parent) {
								best_path_map_manhattan[step->parent->tile][back_step->tile] = step->tile;
								best_path_map_manhattan[back_step->tile][step->parent->tile] = back_step->parent->tile;
								back_step = back_step->parent;
							}
							step = step->parent;
						}
					}
				}
			}
		}
	}
}

Tile* Grid::GetTile(int x, int y) 
{
	return tiles[x + y * height];
}


Tile* Grid::GetTileClosestToPosition(float x, float y)
{
	x += 0.95f;
	x *= 0.5f;
	x *= width;

	y += 0.95f;
	y *= 0.5f;
	y *= height;
	return GetTile((int)std::roundf(x), (int)std::roundf(y));
}

Tile* Grid::GetNextBestTile(Tile* start, Tile* end, bool diagonal)
{
	if (diagonal) {
		return best_path_map_diagonal[start][end];
	}
	return best_path_map_manhattan[start][end];
}

int Grid::Width() {
	return width;
}

int Grid::Height() {
	return height;
}