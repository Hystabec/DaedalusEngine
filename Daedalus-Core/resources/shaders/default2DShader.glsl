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

layout(location = 0) out vertexOutput v_output;
layout(location = 4) out flat uint v_entityID;
layout(location = 5) out flat float v_testFFloat;

void main()
{
	v_output.colour = a_colour;
	v_output.texCoord = a_texCoord;
	v_output.texIndex = a_texIndex;
	v_output.tilingFactor = a_tilingFactor;
	v_entityID = a_entityID;
	v_testFFloat = 0.75;

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

layout(location = 0) in vertexOutput v_input;
layout(location = 4) in flat uint v_entityID;
layout(location = 5) in flat float v_testFFloat;

layout(binding = 0) uniform sampler2D u_textures[32];

void main()
{
	vec4 texColor = v_input.colour;

	switch(int(v_input.texIndex))
	{
		case  0: texColor *= texture(u_textures[0],  v_input.texCoord * v_input.tilingFactor); break;
		case  1: texColor *= texture(u_textures[1],  v_input.texCoord * v_input.tilingFactor); break;
		case  2: texColor *= texture(u_textures[2],  v_input.texCoord * v_input.tilingFactor); break;
		case  3: texColor *= texture(u_textures[3],  v_input.texCoord * v_input.tilingFactor); break;
		case  4: texColor *= texture(u_textures[4],  v_input.texCoord * v_input.tilingFactor); break;
		case  5: texColor *= texture(u_textures[5],  v_input.texCoord * v_input.tilingFactor); break;
		case  6: texColor *= texture(u_textures[6],  v_input.texCoord * v_input.tilingFactor); break;
		case  7: texColor *= texture(u_textures[7],  v_input.texCoord * v_input.tilingFactor); break;
		case  8: texColor *= texture(u_textures[8],  v_input.texCoord * v_input.tilingFactor); break;
		case  9: texColor *= texture(u_textures[9],  v_input.texCoord * v_input.tilingFactor); break;
		case 10: texColor *= texture(u_textures[10], v_input.texCoord * v_input.tilingFactor); break;
		case 11: texColor *= texture(u_textures[11], v_input.texCoord * v_input.tilingFactor); break;
		case 12: texColor *= texture(u_textures[12], v_input.texCoord * v_input.tilingFactor); break;
		case 13: texColor *= texture(u_textures[13], v_input.texCoord * v_input.tilingFactor); break;
		case 14: texColor *= texture(u_textures[14], v_input.texCoord * v_input.tilingFactor); break;
		case 15: texColor *= texture(u_textures[15], v_input.texCoord * v_input.tilingFactor); break;
		case 16: texColor *= texture(u_textures[16], v_input.texCoord * v_input.tilingFactor); break;
		case 17: texColor *= texture(u_textures[17], v_input.texCoord * v_input.tilingFactor); break;
		case 18: texColor *= texture(u_textures[18], v_input.texCoord * v_input.tilingFactor); break;
		case 19: texColor *= texture(u_textures[19], v_input.texCoord * v_input.tilingFactor); break;
		case 20: texColor *= texture(u_textures[20], v_input.texCoord * v_input.tilingFactor); break;
		case 21: texColor *= texture(u_textures[21], v_input.texCoord * v_input.tilingFactor); break;
		case 22: texColor *= texture(u_textures[22], v_input.texCoord * v_input.tilingFactor); break;
		case 23: texColor *= texture(u_textures[23], v_input.texCoord * v_input.tilingFactor); break;
		case 24: texColor *= texture(u_textures[24], v_input.texCoord * v_input.tilingFactor); break;
		case 25: texColor *= texture(u_textures[25], v_input.texCoord * v_input.tilingFactor); break;
		case 26: texColor *= texture(u_textures[26], v_input.texCoord * v_input.tilingFactor); break;
		case 27: texColor *= texture(u_textures[27], v_input.texCoord * v_input.tilingFactor); break;
		case 28: texColor *= texture(u_textures[28], v_input.texCoord * v_input.tilingFactor); break;
		case 29: texColor *= texture(u_textures[29], v_input.texCoord * v_input.tilingFactor); break;
		case 30: texColor *= texture(u_textures[30], v_input.texCoord * v_input.tilingFactor); break;
		case 31: texColor *= texture(u_textures[31], v_input.texCoord * v_input.tilingFactor); break;
	}

	colour = texColor;
	colour = vec4(v_testFFloat, 0, 0, 1.0);
	entityID = v_entityID;
}