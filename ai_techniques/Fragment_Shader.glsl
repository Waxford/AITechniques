#version 140 core

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

void main(void)
{
	gl_FragColor = color;
}