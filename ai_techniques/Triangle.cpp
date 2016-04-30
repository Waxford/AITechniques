#include "Triangle.h"
#include "Time.h"

static const GLfloat g_vertex_buffer_data[] = {
	-0.05f, -0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,
	0.0f,  0.05f, 0.0f,
};

Triangle::Triangle(void)
{
	//Renderable::Renderable();
	std::vector<GLfloat>* v = new std::vector<GLfloat>(g_vertex_buffer_data, g_vertex_buffer_data + sizeof(g_vertex_buffer_data) / sizeof(g_vertex_buffer_data[0]));
	SetVertices(v);
	free(v);
}

Triangle::~Triangle(void)
{
}

void Triangle::Update()
{
	Renderable::Update();
}