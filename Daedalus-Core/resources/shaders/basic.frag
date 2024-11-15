#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 lightPos;

in DATA
{
	vec4 pos;
	vec4 colour;

} fs_in;

void main()
{
	//color = colour * (1.0 / (length(fs_in.pos.xy - lightPos)));
	color = fs_in.colour * (1.0 / (length(fs_in.pos.xy - lightPos)));
}