#version 430 core

layout(std140 uniform Matrices{
	mat4 pvm;
};

in vec4 position;

void main(void)
{
    gl_Position = pvm * position;
}