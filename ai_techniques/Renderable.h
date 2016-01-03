#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <vector>
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
	GLuint shader; 
	std::vector<GLfloat> vertexBufferData;

	Renderable(void);
	~Renderable(void);

	void SetVertices(std::vector<GLfloat>* newVertexBufferData);
	void SetColor(float r, float g, float b, float a);

	void Update();
};