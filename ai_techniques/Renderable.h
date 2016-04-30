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
	float r;
	float g;
	float b;
	float a;
	GLuint shader; 
	std::vector<GLfloat> vertexBufferData;

	Renderable(void);
	virtual ~Renderable(void);

	void SetVertices(std::vector<GLfloat>* newVertexBufferData);
	void SetColor(float r, float g, float b, float a);
	void LookAt(float x, float y);

	virtual void Update();
};