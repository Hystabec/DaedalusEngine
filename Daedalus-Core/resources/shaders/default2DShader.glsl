//This is the default 2D shader that render2D uses

#type vertex
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;
	
uniform mat4 u_projView;
uniform mat4 u_transform;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_texCoord;
	gl_Position = u_projView * u_transform * vec4(a_position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec2 v_TexCoord;

uniform sampler2D u_texture;
uniform vec4 u_colour = vec4(1);

void main()
{
	colour = texture(u_texture, v_TexCoord) * u_colour;
}