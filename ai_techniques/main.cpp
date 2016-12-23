#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "BehaviourDirector.h"
#include "Pather.h"
#include "Grid.h"
#include "Time.h"
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <unordered_set>

#include "Shader_Loader.h"

using namespace Core;

Pather* p;
static int number_of_pathers = 25;

Grid* grid;

//int tileDefinition[] = {
//	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,
//	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	1,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
//	1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,
//	0,1,0,0,0,0,1,1,1,1,1,1,1,0,1,0,1,0,0,0,
//	0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,
//	0,0,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
//	0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
//	0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
//	0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,
//	0,0,0,1,1,1,1,1,1,1,1,1,1,0,1,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0 };
//int tileDefinition[] = {
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int tileDefinition[] = {
	0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,0,1,1,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,1,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

float frameTimes[10];
int frameIndex = 0;
bool debugFrameTime = false;
void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.1f, 1.0f);

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
		p->SetColor(0.1f, 0.2f, 0.9f, 1.0f);//((float)rand() / (RAND_MAX) / 2.0f + 0.5f, (float)rand() / (RAND_MAX) / 2.0f + 0.5f, (float)rand() / (RAND_MAX) / 2.0f + 0.5f, 1.0f);
		p->scaleX = 0.5f;
		p->scaleY = 0.5f;
		startTile = grid->GetTile(11, 15);
		p->x = startTile->x;
		p->y = startTile->y;
		p->SetOrder(-1);
		p->SetGrid(grid);
		//p->pathing_type = PathingType::CONGESTION;
	}


	p = new Pather();
	p->SetVertices(&walkerVerts);
	p->SetColor(0.9f, 0.9f, 0.2f, 1.0f);//((float)rand() / (RAND_MAX) / 2.0f + 0.5f, (float)rand() / (RAND_MAX) / 2.0f + 0.5f, (float)rand() / (RAND_MAX) / 2.0f + 0.5f, 1.0f);
	p->scaleX = 0.5f;
	p->scaleY = 0.5f;
	startTile = grid->GetTile(11, 15);
	p->x = startTile->x;
	p->y = startTile->y;
	p->SetOrder(-1);
	p->SetGrid(grid);
	p->pathing_type = PathingType::CONGESTION;

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

void ProfileDataStructures()
{
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	std::unordered_set<Tile*> u_set_tiles;
	std::set<Tile*> set_tiles;
	std::vector<Tile*> v_tiles;
	Tile* test_tile = new Tile(true);
	int64_t u_insert_micros = 0;
	int64_t s_insert_micros = 0;
	int64_t v_push_micros = 0;
	int64_t v_assign_micros = 0;

	int64_t bu = 10000000;
	int64_t bs = 10000000;
	int64_t bv = 10000000;
	int64_t ba = 10000000;
	int64_t wu = 0;
	int64_t ws = 0;
	int64_t wv = 0;
	int64_t wa = 0;

	int64_t time = 0;
	std::cout << "Averaging 1,000 iterations..." << std::endl;
	for (int i = 0; i < 1000; ++i) {
		t1 = std::chrono::high_resolution_clock::now();
		u_set_tiles.insert(test_tile);
		t2 = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		u_insert_micros += time;
		bu = (bu < time) ? bu : time;
		wu = (wu > time) ? wu : time;
		t1 = std::chrono::high_resolution_clock::now();
		set_tiles.insert(test_tile);
		t2 = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		s_insert_micros += time;
		bs = (bs < time) ? bs : time;
		ws = (ws > time) ? ws : time;
		t1 = std::chrono::high_resolution_clock::now();
		v_tiles.push_back(test_tile);
		t2 = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		v_push_micros += time;
		bv = (bv < time) ? bv : time;
		wv = (wv > time) ? wv : time;
		t1 = std::chrono::high_resolution_clock::now();
		v_tiles[i] = test_tile;
		t2 = std::chrono::high_resolution_clock::now();
		time = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		v_assign_micros += time;
		ba = (ba < time) ? ba : time;
		wa = (wa > time) ? wa : time;
	}
	std::cout << " unordered_set insert " << (u_insert_micros / 1000.0f) << std::endl;
	std::cout << " set insert " << (s_insert_micros / 1000.0f) << std::endl;
	std::cout << " vector push_back " << (v_push_micros / 1000.0f) << std::endl;
	std::cout << " vector assign " << (v_assign_micros / 1000.0f) << std::endl;

	std::cout << "Best cases... " << std::endl;
	std::cout << " unordered_set insert " << bu << std::endl;
	std::cout << " set insert " << bs << std::endl;
	std::cout << " vector push_back " << bv << std::endl;
	std::cout << " vector assign " << ba << std::endl;

	std::cout << "Worst cases... " << std::endl;
	std::cout << " unordered_set insert " << wu << std::endl;
	std::cout << " set insert " << ws << std::endl;
	std::cout << " vector push_back " << wv << std::endl;
	std::cout << " vector assign " << wa << std::endl;
}