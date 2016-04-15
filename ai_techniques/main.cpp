#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "BehaviourDirector.h"
#include "Pather.h"
#include "Grid.h"
#include "Time.h"
#include <iostream>
#include <stdio.h>;
#include <stdlib.h>;
#include <string>
#include <fstream>;
#include <vector>;

#include "Shader_Loader.h"

using namespace Core;

Pather* p;

Grid* grid;

int tileDefinition[] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
	1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.8, 0.8, 0.3, 1.0);

	BehaviourDirector::Tick();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	//Core::Shader_Loader shaderLoader;
	//program = shaderLoader.CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");

	glutSetColor(0, 1, 0, 0);

	grid = new Grid(tileDefinition, 20, 20);

	GLfloat walkerArray[] = {
		-0.05f, -0.05f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.05f, 0.0f,
		0.0f, 0.05f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.05f, -0.05f, 0.0f,
	};
	std::vector<GLfloat> walkerVerts(walkerArray, walkerArray + sizeof(walkerArray) / sizeof(walkerArray[0]));

	p = new Pather();
	p->SetVertices(&walkerVerts);
	p->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	p->scaleX = 0.5f;
	p->scaleY = 0.5f;
	Tile* startTile = grid->GetTile(10, 15);
	p->x = startTile->x;
	p->y = startTile->y;
	p->SetOrder(-1);
	p->SetGrid(grid);
	p->SetDestination(8, 2, false);
	p->SetDestination(10,15, false);
	p->SetDestination(8, 2, true);
	//for (int i = 0; i < 10; ++i)
	//{
	//	p->AddDestination(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 - 1, static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1, 0.01);
	//}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 800);
	glutCreateWindow("AI Techniques");

	glewInit();

	Init();

	glutDisplayFunc(renderScene);
	glutMainLoop();
	return 0;
}