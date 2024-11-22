#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 colour;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out DATA
{
	vec4 pos;
	vec2 uv;
	vec4 colour;

} vs_out;

void main()
{
	gl_Position = pr_matrix * vw_matrix * ml_matrix * position;
	vs_out.uv = uv;
	vs_out.pos = ml_matrix * position;
	vs_out.colour = colour;
}