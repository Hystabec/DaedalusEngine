//This is the default 2D shader that render2D uses

#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_colour;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in float a_texIndex;
layout(location = 4) in float a_tilingFactor;
layout(location = 5) in uint a_entityID;
	
layout(std140, binding = 0) uniform Camera
{
	mat4 viewProj;
} u_cameraBuffer;

struct vertexOutput
{
	vec4 colour;
	vec2 texCoord;
	float texIndex;
	float tilingFactor;
};

layout(location = 0) out vertexOutput output;
layout(location = 4) out flat uint v_entityID;

void main()
{
	output.colour = a_colour;
	output.texCoord = a_texCoord;
	output.texIndex = a_texIndex;
	output.tilingFactor = a_tilingFactor;
	v_entityID = a_entityID;

	gl_Position = u_cameraBuffer.viewProj * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 colour;
layout(location = 1) out uint entityID;

struct vertexOutput
{
	vec4 colour;
	vec2 texCoord;
	float texIndex;
	float tilingFactor;
};

layout(location = 0) in vertexOutput input;
layout(location = 4) in flat uint v_entityID;

layout(binding = 0) uniform sampler2D u_textures[32];

void main()
{
	vec4 texColor = input.colour;

	switch(int(v_texIndex))
	{
		case  0: texColor *= texture(u_textures[0],  input.texCoord * input.tilingFactor); break;
		case  1: texColor *= texture(u_textures[1],  input.texCoord * input.tilingFactor); break;
		case  2: texColor *= texture(u_textures[2],  input.texCoord * input.tilingFactor); break;
		case  3: texColor *= texture(u_textures[3],  input.texCoord * input.tilingFactor); break;
		case  4: texColor *= texture(u_textures[4],  input.texCoord * input.tilingFactor); break;
		case  5: texColor *= texture(u_textures[5],  input.texCoord * input.tilingFactor); break;
		case  6: texColor *= texture(u_textures[6],  input.texCoord * input.tilingFactor); break;
		case  7: texColor *= texture(u_textures[7],  input.texCoord * input.tilingFactor); break;
		case  8: texColor *= texture(u_textures[8],  input.texCoord * input.tilingFactor); break;
		case  9: texColor *= texture(u_textures[9],  input.texCoord * input.tilingFactor); break;
		case 10: texColor *= texture(u_textures[10], input.texCoord * input.tilingFactor); break;
		case 11: texColor *= texture(u_textures[11], input.texCoord * input.tilingFactor); break;
		case 12: texColor *= texture(u_textures[12], input.texCoord * input.tilingFactor); break;
		case 13: texColor *= texture(u_textures[13], input.texCoord * input.tilingFactor); break;
		case 14: texColor *= texture(u_textures[14], input.texCoord * input.tilingFactor); break;
		case 15: texColor *= texture(u_textures[15], input.texCoord * input.tilingFactor); break;
		case 16: texColor *= texture(u_textures[16], input.texCoord * input.tilingFactor); break;
		case 17: texColor *= texture(u_textures[17], input.texCoord * input.tilingFactor); break;
		case 18: texColor *= texture(u_textures[18], input.texCoord * input.tilingFactor); break;
		case 19: texColor *= texture(u_textures[19], input.texCoord * input.tilingFactor); break;
		case 20: texColor *= texture(u_textures[20], input.texCoord * input.tilingFactor); break;
		case 21: texColor *= texture(u_textures[21], input.texCoord * input.tilingFactor); break;
		case 22: texColor *= texture(u_textures[22], input.texCoord * input.tilingFactor); break;
		case 23: texColor *= texture(u_textures[23], input.texCoord * input.tilingFactor); break;
		case 24: texColor *= texture(u_textures[24], input.texCoord * input.tilingFactor); break;
		case 25: texColor *= texture(u_textures[25], input.texCoord * input.tilingFactor); break;
		case 26: texColor *= texture(u_textures[26], input.texCoord * input.tilingFactor); break;
		case 27: texColor *= texture(u_textures[27], input.texCoord * input.tilingFactor); break;
		case 28: texColor *= texture(u_textures[28], input.texCoord * input.tilingFactor); break;
		case 29: texColor *= texture(u_textures[29], input.texCoord * input.tilingFactor); break;
		case 30: texColor *= texture(u_textures[30], input.texCoord * input.tilingFactor); break;
		case 31: texColor *= texture(u_textures[31], input.texCoord * input.tilingFactor); break;
	}

	colour = texColor;
	entityID = v_entityID;
}