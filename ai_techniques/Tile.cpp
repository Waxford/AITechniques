#include "Tile.h"
#include <stdio.h>


Tile::Tile(bool pathable)
{
	this->pathable = pathable;
	if (pathable) {
		this->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		this->SetColor(1.0f, 0.3f, 0.3f, 1.0f);
	}
}


Tile::~Tile()
{
}

bool Tile::IsPathable() {
	return pathable;
}

void Tile::Recolour(float r, float g, float b, float a)
{
	this->SetColor(r, g, b, a);
}