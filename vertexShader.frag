#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexShaderNormal;
layout(location = 2) in vec2 textureCoord;
out vec2 ShadertextureCoord;
out vec3 normal;
out vec3 FragPos;
out vec3 cameraFragPos;

uniform mat4 transform;
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


}