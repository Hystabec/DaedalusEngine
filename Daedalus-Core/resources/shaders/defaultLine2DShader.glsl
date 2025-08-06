// This is the default line 2D shader that render2D uses

#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_colour;
	
layout(std140, binding = 0) uniform Camera
{
	mat4 viewProj;
} u_cameraBuffer;

struct vertexOutput
{
	vec4 colour;
};

layout(location = 0) out vertexOutput v_output;

void main()
{
	v_output.colour = a_colour;

	gl_Position = u_cameraBuffer.viewProj * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_colour;

struct vertexOutput
{
	vec4 colour;
};

layout(location = 0) in vertexOutput v_input;

void main()
{
	o_colour = v_input.colour;
}