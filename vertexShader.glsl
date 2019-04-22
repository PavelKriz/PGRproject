#version 140
in vec3 position;
in vec2 textureCoord;
out vec2 ShadertextureCoord;

uniform mat4  viewM;

void main() {
	vec3 center = vec3(0.0, 0.0, 0.0);
	mat4 translateDown = mat4(vec4(1.0,0.0,0.0,0.0), vec4(0.0,1.0,0.0,0.0), vec4(0.0,0.0,1.0,0.0),vec4(0.0,-0.4,0.0,1.0));
	gl_Position =  viewM * translateDown * vec4(mix(center, position, 0.2), 1.0); // move and scale
	ShadertextureCoord = textureCoord;
}