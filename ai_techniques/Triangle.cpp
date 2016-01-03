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
	std::cout << "Triangle <" << id << "> constructed" << std::endl;
}

Triangle::~Triangle(void)
{
	std::cout << "Triangle <" << id << "> deconstructed" << std::endl;
	//Renderable::~Renderable();
}

void Triangle::Update()
{
	Renderable::Update();
}