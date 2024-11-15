#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 lightPos;

in vec4 pos;

void main()
{
	color = colour * (1.0 / (length(pos.xy - lightPos)));
}