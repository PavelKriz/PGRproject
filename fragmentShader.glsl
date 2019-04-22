#version 140
in vec2 ShadertextureCoord;
out vec4 color;
uniform sampler2D MTexture;

void main() {
	if(ShadertextureCoord.x > 1.0){
		color = vec4(0.0,0.0,1.0,1.0);
	} else {
		color =  texture(MTexture, ShadertextureCoord);
	}
}
