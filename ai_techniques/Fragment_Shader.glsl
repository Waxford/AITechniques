#version 430 core

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

layout(location = 0) out vec4 outputF;

void main(void)
{
	outputF = color;
}