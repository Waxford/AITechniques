#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include "Behaviour.h"

class Renderable : public Behaviour
{
protected:
	GLuint vertexbuffer;
public:
	int depth;
	float x;
	float y;
	float rotation;
	float scaleX;
	float scaleY;

	Renderable(void);
	~Renderable(void);

	void Update();
};