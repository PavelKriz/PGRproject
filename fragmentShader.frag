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
uniform vec3 eyePos;
uniform vec3 eyeDirection;

uniform sampler2D MTexture;
uniform sampler2D skyboxSun;
uniform float sunAlpha;
uniform int explosionAlpha;
uniform vec3 lightColor;
uniform float ambientRegulator;

out vec4 color;


//================================================================================================

float ambient = 0.5;
float specularStrength = 0.8;
float diffuseStrength = 0.8;
float amplifier = 1.0;

//================================================================================================

void setupSKYBOX(){
	ambient = 0.1;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

void setupPIZZA(){
	ambient = 0.1;
	specularStrength = 0.3;
	diffuseStrength = 0.4;
	amplifier = 1.0;
}

//================================================================================================

void setupANANAS(){
	ambient = 0.1;
	specularStrength = 0.8;
	diffuseStrength = 1.0;
	amplifier = 1.0;
}

//================================================================================================

void setupANANAS_PIECE(){
	ambient = 0.1;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

void setupEXPLOSION(){
	ambient = 0.1;
	specularStrength = 0.8;
	diffuseStrength = 0.8;
	amplifier = 1.0;
}

//================================================================================================

float  ananasPhong( vec3 lightPointPos){
	float distance =length(vec3(0.0,0.0,0.0) - FragPos);
	distance -= 0.7;
	float attenuation = 1.0 / (0.6 * distance +  distance * distance * distance); 
	return attenuation ;
}

//================================================================================================

float lightFog(int objectType){
	float objDist = length(FragPos - vec3(0.0,0.0,0.0));

	//If is objDist greater then 10 then it is only skybox and skybox has the fog ilusion on it
	if(objectType == 2){
		vec3 ananasDirection = normalize(eyePos - vec3(0.0,0.0,0.0));
		vec3 fragDirection = normalize(eyePos - FragPos);
		vec3 ananasLightDir = normalize(vec3(0.0,0.0,0.0) - FragPos);
		float spotAngle = max(0.0f, dot(ananasDirection,fragDirection));

		float distFromCamera = length(eyePos - vec3(0.0,0.0,0.0));
		if(distFromCamera < 10){
			if(spotAngle > 0.95f * 0.95 * (distFromCamera/30 + 0.7)  ){
				float viewable = max(dot(normalize(normal), ananasLightDir), 0.0) ;					
				return pow(spotAngle, floor(64/(10/distFromCamera))) ;
			}
		} else {
			if(spotAngle > 0.95f * 0.95 *(1/(distFromCamera/(distFromCamera-0.29)) + 0.062)  ){
				float viewable = max(dot(normalize(normal), ananasLightDir), 0.0) ;
				return pow(spotAngle, 64) ;
			}
		}
	} else {
		return ananasPhong( vec3(0.0,FragPos.y * 1.2,0.0));
	}
	return 0.0;
}

//================================================================================================

vec4 skybox(){
	//calculations of sun coords - sun is traveling around the space
	vec2 sunCoords = vec2(ShadertextureCoord.x + sunAlpha, ShadertextureCoord.y);
	if(sunCoords.x > 1.0f){
		sunCoords.x -= 1.0f;
	}

	vec4 spaceTexture =  texture(MTexture, ShadertextureCoord);
	vec4 sunColor = texture(skyboxSun, sunCoords);  //adding of the light from the sun

	return (spaceTexture + sunColor);
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
	if(flashlight == 0) {
		return 0.0;
	}
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(eyePos - FragPos);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	return ((diffuse + specular) * getFlashlight());

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
	float diffuse = max(dot(normalizedNormal, -lightDir), 0.0)  * diffuseStrength ;

	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, -reflectDir), 0.0), 128) * specularStrength;
	float distance    = length(lightPointPos - FragPos);
	float attenuation = 1.0 / (0.5 * distance + 0.5 *distance * distance); 
	return (diffuse + specular) * attenuation;
}

//================================================================================================

float pointLights(){
	float ret = 0.0;
	for(int i = 0; i < countOflights; ++i){
		ret += pointPhong( lights[i].xyz) * lights[i].w;
	}
	return ret;
}



void main() {
	float lightning = 0.0;
	ambient *= ambientRegulator;

	switch (objectType){
		case 1:
			setupPIZZA();
			color =  texture(MTexture, ShadertextureCoord);
			lightning += lightFog(objectType);
			lightning += flashlighPhong(); //flashlight from the view of player/watcher
			lightning += directionPhong(); //light from the sun
			lightning += pointLights(); // Lights from AnanasPieces
			color *= vec4(lightColor, 1.0) * lightning;
			break;
		case 2:
			setupSKYBOX();
			color = skybox();
			color += vec4(lightColor, 1.0) * lightFog(objectType); //its only lighting, to not to be so grey it need to be ADD and not MULTIPLY
			break;
		case 3:
			setupANANAS();
			color =  texture(MTexture, ShadertextureCoord);
			lightning += flashlighPhong(); //flashlight from the view of player/watcher
			lightning += directionPhong(); //light from the sun
			lightning += pointLights(); // Lights from AnanasPieces
			color *= vec4(lightColor, 1.0) * lightning;
			break;
		case 4:
			setupANANAS_PIECE();
			color =  texture(MTexture, ShadertextureCoord);
			lightning += lightFog(objectType);
			lightning += flashlighPhong(); //flashlight from the view of player/watcher
			lightning += directionPhong(); //light from the sun
			lightning += pointLights(); // Lights from AnanasPieces
			color *= vec4(lightColor, 1.0) * lightning;
			break;
		case 5:
			setupEXPLOSION();
			color = explosion();
			break;
		default:
			color =  texture(MTexture, ShadertextureCoord);
			break;
	}
}
