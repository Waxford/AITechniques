#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;
void main(void)
{
    gl_Position.xyz = vec4(vertexPosition_modelspace, 1.0);
}