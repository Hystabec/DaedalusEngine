// This is the default circle 2D shader that render2D uses

#type vertex
#version 450 core

layout(location = 0) in vec3 a_worldPosition;
layout(location = 1) in vec3 a_localPosition;
layout(location = 2) in vec4 a_colour;
layout(location = 3) in float a_thickness;
layout(location = 4) in float a_fade;
layout(location = 5) in uint a_entityID;
	
layout(std140, binding = 0) uniform Camera
{
	mat4 viewProj;
} u_cameraBuffer;

struct vertexOutput
{
    vec3 localPosition;
	vec4 colour;
    float thickness;
    float fade;
};

layout(location = 0) out vertexOutput v_output;
layout(location = 4) out flat uint v_entityID;

void main()
{
	v_output.localPosition = a_localPosition;
	v_output.colour = a_colour;
	v_output.thickness = a_thickness;
    v_output.fade = a_fade;

	v_entityID = a_entityID;

	gl_Position = u_cameraBuffer.viewProj * vec4(a_worldPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 o_colour;
layout(location = 1) out uint o_entityID;

struct vertexOutput
{
    vec3 localPosition;
	vec4 colour;
    float thickness;
    float fade;
};

layout(location = 0) in vertexOutput v_input;
layout(location = 4) in flat uint v_entityID;

void main()
{
    float fade  = clamp(v_input.fade, 0.01, 10.0);
    
    float distance = 1.0 - length(v_input.localPosition);
    float alpha = smoothstep(0.0, fade, distance);
    alpha *= smoothstep(v_input.thickness + fade, v_input.thickness, distance);

    if (alpha == 0.0)
        discard;

    o_colour = v_input.colour;
    o_colour.a *= alpha;

    o_entityID = v_entityID;
}