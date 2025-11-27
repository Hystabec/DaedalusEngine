// This is the default text shader that render2D uses
// MSDF text shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_colour;
layout(location = 2) in vec2 a_texCoord;
layout(location = 3) in uint a_entityID;
	
layout(std140, binding = 0) uniform Camera
{
	mat4 viewProj;
} u_cameraBuffer;

struct vertexOutput
{
	vec4 colour;
	vec2 texCoord;
};

layout(location = 0) out vertexOutput v_output;
layout(location = 2) out flat uint v_entityID;

void main()
{
	v_output.colour = a_colour;
	v_output.texCoord = a_texCoord;

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
};

layout(location = 0) in vertexOutput v_input;
layout(location = 2) in flat uint v_entityID;

layout(binding = 0) uniform sampler2D u_fontAtlas;


float screenPxRange() {
	const float pxRange = 2.0; // this is also hard coded in font.cpp
	vec2 unitRange = vec2(pxRange)/vec2(textureSize(u_fontAtlas, 0));
	vec2 screenTexSize = vec2(1.0)/fwidth(v_input.texCoord);
	return max(0.5*dot(unitRange, screenTexSize), 1.0);
}

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	vec4 texColour = v_input.colour * texture(u_fontAtlas, v_input.texCoord);

	vec3 msd = texture(u_fontAtlas, v_input.texCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange()*(sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	if(opacity == 0.0)
		discard;

	vec4 bgColour = vec4(vec3(0.0), 0.0);
	o_colour = mix(bgColour, v_input.colour, opacity);

	if(o_colour.a == 0.0)
		discard;

	o_entityID = v_entityID;
}