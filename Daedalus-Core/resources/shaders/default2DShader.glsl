//This is the default 2D shader that render2D uses

#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in float a_texIndex;
layout(location = 3) in float a_tilingFactor;
layout(location = 4) in vec4 a_colour;
	
uniform mat4 u_projView;

out vec2 v_texCoord;
out float v_texIndex;
out float v_tilingFactor;
out vec4 v_colour;

void main()
{
	v_texCoord = a_texCoord;
	v_texIndex = a_texIndex;
	v_tilingFactor = a_tilingFactor;
	v_colour = a_colour;
	gl_Position = u_projView * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_texCoord;
in float v_texIndex;
in float v_tilingFactor;
in vec4 v_colour;

uniform sampler2D u_textures[32];

void main()
{
	colour = texture(u_textures[int(v_texIndex)], v_texCoord * v_tilingFactor) * v_colour;
}