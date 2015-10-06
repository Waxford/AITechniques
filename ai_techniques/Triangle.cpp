#include "Triangle.h"
#include "Time.h"

Triangle::Triangle(void)
{
	//Renderable::Renderable();
	std::cout << "Triangle <" << id << "> constructed" << std::endl;
}

Triangle::~Triangle(void)
{
	std::cout << "Triangle <" << id << "> deconstructed" << std::endl;
	//Renderable::~Renderable();
}

static const GLfloat g_vertex_buffer_data[] = {
	-0.05f, -0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,
	0.0f,  0.05f, 0.0f,
};

void Triangle::Update()
{
	Renderable::Update(); 

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glRotatef(rotation, 0.0f, 0.0f, 1.0f);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

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
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glPopMatrix();

	glDisableVertexAttribArray(0);
}