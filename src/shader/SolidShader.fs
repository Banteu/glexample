#version 430 core

const vec3 sunPosition = vec3(100, 100, 100);
const float ambientLight = 0.2;

out vec4 diffuseColor;

layout(location = 25) uniform vec3 viewerPosition;

layout(binding=0) uniform sampler2D diffuseSampler;
layout(binding=1) uniform sampler2D normalMapSampler;
layout(binding=2) uniform sampler2D roghnessSampler;
layout(binding=8) uniform sampler2D envSampler;

layout(location=30) uniform float rotation;

in vec3 varPosition;
in vec3 normal;
in vec3 uv;
in vec3 tg1;
in vec3 tg2;


void main()
{
	vec3 viewerDir = viewerPosition - varPosition;
	vec3 dirToLight = sunPosition - varPosition;
	float distToLight = length(dirToLight);
	dirToLight *= (1.0f / distToLight);
	
	diffuseColor = vec4(0.8, 0.8, 0.8, 1) * (max(0.0f, dot(normal, dirToLight) * dot(normal, normalize(viewerDir))) + ambientLight);
	diffuseColor.w = 1;
}