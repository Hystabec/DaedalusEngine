#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_pos;

uniform vec4 u_colour;

void main()
{
	colour = u_colour;
}