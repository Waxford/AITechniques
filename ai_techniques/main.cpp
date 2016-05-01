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
static int number_of_pathers = 1;

Grid* grid;

int tileDefinition[] = {
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
	1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,
	0,1,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,
	0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,
	0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
	0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
	0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
	0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0 };

float frameTimes[10];
int frameIndex = 0;
bool debugFrameTime = false;
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.3, 0.3, 0.1, 1.0);

	BehaviourDirector::Tick();
	if (frameIndex++ == 9) {
		float avg = 0;
		for (int i = 0; i < 10; ++i) {
			avg += frameTimes[i];
			avg /= 10;
		}
		if(debugFrameTime)
			std::cout << "Frame time: " << avg << std::endl;
		frameIndex = 0;
	}
	frameTimes[frameIndex] = Time::GetDeltaTime();
	glutSwapBuffers();
	glutPostRedisplay();
}

void Init()
{
	for (int i = 0; i < 10; ++i) {
		frameTimes[i] = 0.0f;
	}
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

	Tile* startTile;

	for (int i = 0; i < number_of_pathers; ++i) {
		p = new Pather();
		p->SetVertices(&walkerVerts);
		p->SetColor((double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX), 1.0f);
		p->scaleX = 0.5f;
		p->scaleY = 0.5f;
		startTile = grid->GetTile(12, 15);
		p->x = startTile->x;
		p->y = startTile->y;
		p->SetOrder(-1);
		p->SetGrid(grid);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("AI Techniques");

	glewInit();

	Init();

	glutDisplayFunc(renderScene);
	glutMainLoop();
	return 0;
}