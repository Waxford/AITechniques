#include "Pather.h"
#include "Tile.h"
#include "Time.h"
#include <stdio.h>

Tile::Tile(bool pathable)
{
	this->pathable = pathable;
	if (pathable) {
		this->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		this->SetColor(0.9f, 0.2f, 0.1f, 1.0f);
	}
}


Tile::~Tile()
{
}

void Tile::Update() {
	Renderable::Update();
	if (!pathable) {
		for (auto it = Pather::g_all_pathers.begin(); it != Pather::g_all_pathers.end(); ++it) {
			float distX = (*it)->x - x;
			float distY = (*it)->y - y;
			if (abs(distX) > abs(distY)) {
				if (abs(distX) - (*it)->GetColliderRadius() < 0.05f) {
					if (distX > 0) {
						(*it)->AddForce((0.05f - distX) + ((*it)->GetColliderRadius()), 0.0f);
					}
					else {
						(*it)->AddForce((-0.05f - distX) - ((*it)->GetColliderRadius()), 0.0f);
					}
				}
			}
			else {
				if (abs(distY) - (*it)->GetColliderRadius() < 0.05f) {
					if (distY > 0) {
						(*it)->AddForce(0.0f, (0.05f - distY) + ((*it)->GetColliderRadius()));
					}
					else {
						(*it)->AddForce(0.0f, (-0.05f - distY) - ((*it)->GetColliderRadius()));
					}
				}
			}
		}
	}
}

bool Tile::IsPathable() {
	return pathable;
}

void Tile::Recolour(float r, float g, float b, float a)
{
	this->SetColor(r, g, b, a);
}

std::string Tile::ToString() {
	return indexX + "," + indexY;
}
