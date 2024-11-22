#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 lightPos;

in DATA
{
	vec4 pos;
	vec2 uv;
	vec4 colour;

} fs_in;

uniform sampler2D tex;

void main()
{
	float intensity = (1.0 / (length(fs_in.pos.xy - lightPos)));
	color = fs_in.colour * intensity;
	color = texture(tex, fs_in.uv);
}