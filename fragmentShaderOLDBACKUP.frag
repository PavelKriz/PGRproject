#version 330
in vec2 ShadertextureCoord;
in vec3 normal;
in vec3 FragPos;
in vec3 cameraFragPos;

uniform vec3 sunDirection;
uniform int flashlight;
uniform int countOflights;
uniform vec4 lights[6];
uniform int objectType;
out vec4 color;
uniform vec3 eyePos;
uniform vec3 eyeDirection;

uniform sampler2D MTexture;
uniform sampler2D skyboxSun;
uniform float sunAlpha;
uniform int explosionAlpha;
uniform vec3 lightColor;
uniform float ambientRegulator;

//================================================================================================

float ambient = 0.5;
float specularStrength = 0.8;
float diffuseStrength = 0.8;
float amplifier = 1.0;

//================================================================================================


float getFlashlight(){
	vec3 lightDir = normalize(eyePos - FragPos);
	float spotAngle = max(0.0f, dot(-lightDir, normalize(eyeDirection)));

	if(spotAngle < 0.96f)
		return 0.0;
	else
		return pow(spotAngle , 102);
}

//================================================================================================

float flashlighPhong(){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(eyePos - FragPos);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	return ((diffuse + ambient  + specular) * getFlashlight());

}

//================================================================================================

float directionPhong(){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(sunDirection);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  

	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	return (diffuse + ambient  + specular);
}

//================================================================================================

float pointPhong( vec3 lightPointPos){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(FragPos - lightPointPos );
	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;
	float distance    = length(lightPointPos - FragPos);
	float attenuation = 1.0 / (0.5 * distance + 0.5 *distance * distance); 
	return (diffuse + specular) * attenuation;
}

//================================================================================================

float  ananasPhong( vec3 lightPointPos){
	float distance =length(vec3(0.0,0.0,0.0) - FragPos);
	distance -= 0.7;
	float attenuation = 1.0 / (0.6 * distance +  distance * distance * distance); 
	return attenuation ;
}

//================================================================================================

float lightFog(vec3 objPos){
	float objDist = length(FragPos - objPos);
	if(objectType != 3){
		if(objDist > 10){
			vec3 ananasDirection = normalize(eyePos - objPos);
			vec3 fragDirection = normalize(eyePos - FragPos);
			vec3 ananasLightDir = normalize(objPos - FragPos);
			float spotAngle = max(0.0f, dot(ananasDirection,fragDirection));

			float dist = length(eyePos - objPos);
			if(dist < 10){
				if(spotAngle > 0.95f * 0.95 * (dist/30 + 0.7)  ){
					float viewable = max(dot(normalize(normal), ananasLightDir), 0.0) ;					
					return pow(spotAngle, floor(64/(10/dist))) ;
				}
			} else {
				if(spotAngle > 0.95f * 0.95 *(1/(dist/(dist-0.29)) + 0.062)  ){
					float viewable = max(dot(normalize(normal), ananasLightDir), 0.0) ;
					return pow(spotAngle, 64) ;
				}
			}
		} else {
			return ananasPhong( vec3(0.0,FragPos.y * 1.2,0.0));
		}
	}
}

//================================================================================================

vec4 explosion(){
		float iSize = 0.25;
		float xC  = ShadertextureCoord.x;
		float yC  = ShadertextureCoord.y;
		xC = xC / 4;
		yC = yC / 4;
		xC += (explosionAlpha % 4) * iSize;
		float tmp = explosionAlpha / 4;
		yC += floor(tmp) * iSize;
		return texture(MTexture, vec2(xC,yC));

}

//================================================================================================

vec4 skybox(){
	vec4 ret =  texture(MTexture, ShadertextureCoord);

	vec2 tmpCoords = vec2(ShadertextureCoord.x + sunAlpha, ShadertextureCoord.y);
	if(tmpCoords.x > 1.0f){
		tmpCoords.x -= 1.0f;
	}

	//adding of the light from the sun
	vec4 tmpColor = texture(skyboxSun, tmpCoords);

	ret += tmpColor;

	return ret;
}

//================================================================================================

void setupSKYBOX(){
	ambient = 0.5;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

void setupPIZZA(){
	ambient = 0.5;
	specularStrength = 0.3;
	diffuseStrength = 0.4;
	amplifier = 1.0;
}

//================================================================================================

void setupANANAS(){
	ambient = 0.5;
	specularStrength = 0.8;
	diffuseStrength = 1.0;
	amplifier = 1.0;
}

//================================================================================================

void setupANANAS_PIECE(){
	ambient = 0.5;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

void setupEXPLOSION(){
	ambient = 0.5;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

void main() {

	switch (objectType){
		case 1:
			setupPIZZA();
			break;
		case 2:
			setupSKYBOX();
			break;
		case 3:
			setupANANAS();
			break;
		case 4:
			setupANANAS_PIECE();
			break;
		case 5:
			setupEXPLOSION();
			break;
		default:
			break;
	}

	ambient *= ambientRegulator;

	if( objectType == 2){ 
		color = skybox();
	} else if (objectType == 5){
		color = explosion();
	} else {
		vec3 lightning = vec3(0.0,0.0,0.0);

		//flashlight from the view of player/watcher
		if(flashlight > 0) {
			lightning += flashlighPhong() * lightColor;	
		} 

		//directionPhong means the light from the sun, which is going around the skybox

		lightning += directionPhong()  * lightColor;


		for(int i = 0; i < countOflights; ++i){
			lightning += pointPhong( lights[i].xyz) * lights[i].w;
		}

		color =  vec4(lightning,1.0) * texture(MTexture, ShadertextureCoord);
	}

	//ananas light fog
	if(objectType != 5){
		color += vec4(lightColor, 1.0) * lightFog(vec3(0.0,0.0,0.0));
	}

}
