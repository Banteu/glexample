#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec3 uvIn;
layout(location = 3) in vec3 tgIn;


layout(location = 12) uniform mat4 projectionMatrix;
layout(location = 16) uniform mat4 modelMatrix;



out vec3 normal;
out vec3 varPosition;
out vec3 uv;
out vec3 tg1;
out vec3 tg2;



void main()
{
	uv = uvIn;
	normal = (modelMatrix * vec4(normalIn, 0)).xyz;
	tg1 = (modelMatrix * vec4(tgIn, 0)).xyz;
	tg2 = (modelMatrix * vec4(cross(normal, tgIn), 0)).xyz;
	varPosition = position;
	
	gl_Position =  projectionMatrix * vec4(position.x, position.y, position.z, 1.0);
}