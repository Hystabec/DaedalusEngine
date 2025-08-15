// This is the default quad 2D shader that render2D uses

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
	float tilingFactor;
};

layout(location = 0) out vertexOutput v_output;
layout(location = 4) out flat float v_texIndex;
layout(location = 5) out flat uint v_entityID;

void main()
{
	v_output.colour = a_colour;
	v_output.texCoord = a_texCoord;
	v_output.tilingFactor = a_tilingFactor;

	v_texIndex = a_texIndex;
	v_entityID = a_entityID;

	gl_Position = u_cameraBuffer.viewProj * vec4(a_position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_colour;
layout(location = 1) out uint o_entityID;

struct vertexOutput
{
	vec4 colour;
	vec2 texCoord;
	float tilingFactor;
};

layout(location = 0) in vertexOutput v_input;
layout(location = 4) in flat float v_texIndex;
layout(location = 5) in flat uint v_entityID;

layout(binding = 0) uniform sampler2D u_textures[32];

void main()
{
	vec4 texColour = v_input.colour;

	switch(int(v_texIndex))
	{
		case  0: texColour *= texture(u_textures[0],  v_input.texCoord * v_input.tilingFactor); break;
		case  1: texColour *= texture(u_textures[1],  v_input.texCoord * v_input.tilingFactor); break;
		case  2: texColour *= texture(u_textures[2],  v_input.texCoord * v_input.tilingFactor); break;
		case  3: texColour *= texture(u_textures[3],  v_input.texCoord * v_input.tilingFactor); break;
		case  4: texColour *= texture(u_textures[4],  v_input.texCoord * v_input.tilingFactor); break;
		case  5: texColour *= texture(u_textures[5],  v_input.texCoord * v_input.tilingFactor); break;
		case  6: texColour *= texture(u_textures[6],  v_input.texCoord * v_input.tilingFactor); break;
		case  7: texColour *= texture(u_textures[7],  v_input.texCoord * v_input.tilingFactor); break;
		case  8: texColour *= texture(u_textures[8],  v_input.texCoord * v_input.tilingFactor); break;
		case  9: texColour *= texture(u_textures[9],  v_input.texCoord * v_input.tilingFactor); break;
		case 10: texColour *= texture(u_textures[10], v_input.texCoord * v_input.tilingFactor); break;
		case 11: texColour *= texture(u_textures[11], v_input.texCoord * v_input.tilingFactor); break;
		case 12: texColour *= texture(u_textures[12], v_input.texCoord * v_input.tilingFactor); break;
		case 13: texColour *= texture(u_textures[13], v_input.texCoord * v_input.tilingFactor); break;
		case 14: texColour *= texture(u_textures[14], v_input.texCoord * v_input.tilingFactor); break;
		case 15: texColour *= texture(u_textures[15], v_input.texCoord * v_input.tilingFactor); break;
		case 16: texColour *= texture(u_textures[16], v_input.texCoord * v_input.tilingFactor); break;
		case 17: texColour *= texture(u_textures[17], v_input.texCoord * v_input.tilingFactor); break;
		case 18: texColour *= texture(u_textures[18], v_input.texCoord * v_input.tilingFactor); break;
		case 19: texColour *= texture(u_textures[19], v_input.texCoord * v_input.tilingFactor); break;
		case 20: texColour *= texture(u_textures[20], v_input.texCoord * v_input.tilingFactor); break;
		case 21: texColour *= texture(u_textures[21], v_input.texCoord * v_input.tilingFactor); break;
		case 22: texColour *= texture(u_textures[22], v_input.texCoord * v_input.tilingFactor); break;
		case 23: texColour *= texture(u_textures[23], v_input.texCoord * v_input.tilingFactor); break;
		case 24: texColour *= texture(u_textures[24], v_input.texCoord * v_input.tilingFactor); break;
		case 25: texColour *= texture(u_textures[25], v_input.texCoord * v_input.tilingFactor); break;
		case 26: texColour *= texture(u_textures[26], v_input.texCoord * v_input.tilingFactor); break;
		case 27: texColour *= texture(u_textures[27], v_input.texCoord * v_input.tilingFactor); break;
		case 28: texColour *= texture(u_textures[28], v_input.texCoord * v_input.tilingFactor); break;
		case 29: texColour *= texture(u_textures[29], v_input.texCoord * v_input.tilingFactor); break;
		case 30: texColour *= texture(u_textures[30], v_input.texCoord * v_input.tilingFactor); break;
		case 31: texColour *= texture(u_textures[31], v_input.texCoord * v_input.tilingFactor); break;
	}

	if(texColour.a == 0.0)
		discard;

	o_colour = texColour;
	o_entityID = v_entityID;
}