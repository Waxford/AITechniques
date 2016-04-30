#include "Grid.h"
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
}


Grid::~Grid()
{
}

Tile* Grid::GetTile(int x, int y) 
{
	return tiles[x + y * height];
}


Tile* Grid::GetTileClosestToPosition(float x, float y)
{
	x += 0.95;
	x *= 0.5f;
	x *= width;

	y += 0.95;
	y *= 0.5f;
	y *= height;
	return GetTile(std::round(x), std::round(y));
}

int Grid::Width() {
	return width;
}

int Grid::Height() {
	return height;
}