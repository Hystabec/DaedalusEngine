//basic flat shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
	
uniform mat4 u_projView;
uniform mat4 u_transform;

out vec3 v_pos;

void main()
{
	v_pos = a_position;
	gl_Position = u_projView * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec3 v_pos;

uniform vec4 u_colour;

void main()
{
	colour = u_colour;
}