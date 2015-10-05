#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "BehaviourDirector.h"
#include "Triangle.h"
#include <iostream>
#include <stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;

#include "Shader_Loader.h"

using namespace Core;

GLuint program;

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);//clear red

	glUseProgram(program);

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

	Triangle* t = new Triangle();

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