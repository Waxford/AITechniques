#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "BehaviourDirector.h"
#include "Triangle.h"
#include "Time.h"
#include <iostream>
#include <stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;

#include "Shader_Loader.h"

using namespace Core;

Renderable* r;
Triangle* t1;
Triangle* t2;
Triangle* t3;
Triangle* t4;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	float time = Time::GetTime();

	/*t1->x = sinf(time*4.0f)*0.4f;
	t1->y = sinf(time*6.0f)*0.3f;
	t1->rotation = time*650.0f;
	t1->scaleX = sinf(time*2.0f) + 1.5f;

	t2->x = sinf(time*2.0f)*0.2f;
	t2->y = sinf(time*3.0f)*0.25f;
	t2->rotation = time*50.0f;
	t2->scaleX = sinf(time) + 1.2f;
	t2->scaleY = sinf(time*3.0f)*2 + 2.5f;*/

	r->scaleX = sinf(time)*2 + 2.5f;
	r->scaleY = sinf(time * 1.5f) + 1.2f;
	r->SetColor(sin(time) * 0.5f + 0.5f, sin(time*1.1f) * 0.5f + 0.5f, sin(time*1.5f) * 0.5f + 0.5f, 1.0f);

	t1->rotation = 0.0f + time * 100.0f;
	t1->y = 0.75f;

	t2->rotation = 90.0f + time * 100.0f;
	t2->y = 0.25f;

	t3->rotation = 180.0f + time * 100.0f;
	t3->y = -0.25f;

	t4->rotation = 270.0f + time * 100.0f;
	t4->y = -0.75f;

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

	r = new Renderable();
	GLfloat rArray[] = {
		-0.05f, -0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		-0.05f, 0.05f, 0.0f,
		0.05f, -0.05f, 0.0f,
		0.05f, 0.05f, 0.0f,
	};
	std::vector<GLfloat> rVerts(rArray, rArray + sizeof(rArray) / sizeof(rArray[0]));
	r->SetVertices(&rVerts);
	r->SetColor(1.0f, 1.0f, 1.0f, 1.0f);

	t1 = new Triangle();
	t1->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	t2 = new Triangle();
	t2->SetColor(1.0f, 0.5f, 0.5f, 1.0f);
	t3 = new Triangle();
	t3->SetColor(0.5f, 1.0f, 0.5f, 1.0f);
	t4 = new Triangle();
	t4->SetColor(0.5f, 0.5f, 1.0f, 1.0f);

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