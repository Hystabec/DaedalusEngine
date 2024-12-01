#version 330 core

layout (location = 0) out vec4 color;

uniform vec4 colour;
uniform vec2 lightPos;

in DATA
{
	vec4 pos;
	vec2 uv;
	float tid;
	vec4 colour;

} fs_in;

uniform sampler2D textures[32];

void main()
{
	float intensity = (1.0 / (length(fs_in.pos.xy - lightPos)));
	vec4 texColour = fs_in.colour;
	
	if(fs_in.tid > 0.0)
	{
		int tid = int(fs_in.tid - 0.5);
		texColour = texture(textures[tid], fs_in.uv) * texColour;
	}

	color = texColour * intensity;
}