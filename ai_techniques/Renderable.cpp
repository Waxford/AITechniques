#define _USE_MATH_DEFINES

#include "Renderable.h"
#include "Shader_Loader.h"
#include <cmath>
#include <iostream>

Renderable::Renderable(void) : depth(0), x(0.0f), y(0.0f), rotation(0.0f), scaleX(1.0f), scaleY(1.0f)
{
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	shader = Core::Shader_Loader::CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");
}

Renderable::~Renderable(void) 
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(shader);
}

void Renderable::SetVertices(std::vector<GLfloat>* newVertexBufferData) {
	vertexBufferData.clear();
	float cachedX = x;
	float cachedY = y;
	float cachedRotation = rotation;
	float cachedScaleX = scaleX;
	float cachedScaleY = scaleY;
	//x = 0.0f;
	//y = 0.0f;
	//rotation = 0.0f;
	//scaleX = 1.0f;
	//scaleY = 1.0f;
	for (int i = 0; i < newVertexBufferData->size(); ++i) {
		vertexBufferData.push_back(newVertexBufferData->at(i));
	}
	x = cachedX;
	y = cachedY;
	rotation = cachedRotation;
	scaleX = cachedScaleX;
	scaleY = cachedScaleY;
}

void Renderable::SetColor(float r, float g, float b, float a) {
	float colorArray[4] = {r, g, b, a};
	GLint colorLoc = glGetUniformLocation(shader, "color");
	glProgramUniform4fv(shader, colorLoc, 1, colorArray);
}

void Renderable::LookAt(float x, float y)
{
	float dx = x - this->x;
	float dy = y - this->y;
	rotation = 270.0f + atan2f(dy, dx) * 180.0f * M_1_PI;
}

void Renderable::Update()
{
	Behaviour::Update();
	glUseProgram(shader);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);
	glScalef(scaleX, scaleY, 0.0f);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	GLfloat* vArray = &vertexBufferData[0];

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof &vertexBufferData[0] * vertexBufferData.size(), vArray, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, vertexBufferData.size()/3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glPopMatrix();

	glDisableVertexAttribArray(0);
}