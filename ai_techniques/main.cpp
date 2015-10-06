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

GLuint program;

Triangle* t1;
Triangle* t2;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glUseProgram(program);

	float time = Time::GetTime();

	t1->x = sinf(time*4.0f)*0.4f;
	t1->y = sinf(time*6.0f)*0.3f;
	t1->rotation = time*650.0f;

	t2->x = sinf(time*5.0f)*0.2f;
	t2->y = sinf(time*7.0f)*0.25f;
	t2->rotation = time*350.0f;

	BehaviourDirector::Tick();

	glutSwapBuffers();
	glutPostRedisplay();
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	//load and compile shaders
	Core::Shader_Loader shaderLoader;
	program = shaderLoader.CreateProgram("Vertex_Shader.glsl", "Fragment_Shader.glsl");

	t1 = new Triangle();
	t2 = new Triangle();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 600);
	glutCreateWindow("AI Techniques");

	glewInit();

	Init();

	glutDisplayFunc(renderScene);
	glutMainLoop();
	glDeleteProgram(program);
	return 0;
}