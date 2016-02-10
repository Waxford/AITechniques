#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "BehaviourDirector.h"
#include "Pather.h"
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

std::vector<Renderable*> grid;

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
	for (int i = 0; i < 10; ++i)
	{
		p->AddDestination(static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 - 1, static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2 - 1, 0.01);
	}
	
	GLfloat squareArray[] = {
		-0.05f, -0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		0.05f, 0.05f, 0.0f,
	};
	std::vector<GLfloat> squareVerts(squareArray, squareArray + sizeof(squareArray) / sizeof(squareArray[0]));

	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			Renderable* gridTile = new Renderable();
			gridTile->SetVertices(&squareVerts);
			gridTile->x = (i - 9.5f) / 10.0f;
			gridTile->y = (j - 9.5f) / 10.0f;
			gridTile->scaleX = .95f;
			gridTile->scaleY = .95f;
			gridTile->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
			grid.push_back(gridTile);
		}
	}

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