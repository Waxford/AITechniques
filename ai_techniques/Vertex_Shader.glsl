#version 140 core

in vec4 position;

void main(void)
{
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * position;
}