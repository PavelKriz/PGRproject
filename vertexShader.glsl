#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexShaderNormal;
layout(location = 2) in vec2 textureCoord;
out vec2 ShadertextureCoord;
out vec3 normal;
out vec3 FragPos;
out vec3 cameraFragPos;
out vec3 outCenter;
out vec3 centerLookFragPos;

uniform mat4 transform;
uniform mat4 centerLookM;
uniform mat4  viewM;

void main() {
	mat4 translateDown = mat4(vec4(1.0,0.0,0.0,0.0), vec4(0.0,1.0,0.0,0.0), vec4(0.0,0.0,1.0,0.0),vec4(0.0,-0.4,0.0,1.0));
	normal = mat3(transpose(inverse(translateDown))) * vertexShaderNormal; 
	vec3 center = vec3(0.0, 0.0, 0.0);
	vec4 locPosition = transform * translateDown * vec4(mix(center, position, 1.0), 1.0);
	gl_Position = viewM * locPosition;
	cameraFragPos = gl_Position.xyz;
	FragPos = (transform * vec4(position,1.0)).xyz;
	ShadertextureCoord = textureCoord;

	vec4 tmp1 = centerLookM * vec4(position,1.0);
	centerLookFragPos = tmp1.xyz;
	vec4 tmp2 = viewM * vec4(center,1.0);
	outCenter = tmp2.xyz;
}