#version 330
in vec2 ShadertextureCoord;
in vec3 normal;
in vec3 FragPos;
in vec3 cameraFragPos;
in vec3 outCenter;
in vec3 centerLookFragPos;

uniform vec3 sunDirection;
uniform int flashlight;
uniform int countOflights;
uniform vec3 lights[6];
uniform int objectType;
out vec4 color;
uniform vec3 eyePos;
uniform vec3 eyeDirection;

uniform sampler2D MTexture;
uniform sampler2D skyboxSun;
uniform float sunAlpha;
uniform int explosionAlpha;

//ENDTASK potreba udelat zde nejakou intovou alfu ktera bude blikat mezi framy
//ENDTASK pridat velikost jednoho bloku 

float ambient = 0.5;
float specularStrength = 0.8;
float diffuseStrength = 0.8;
vec3 lightColor = vec3(1.0,0.9,0.9);

float getFlashlight(){
	vec3 lightDir = normalize(eyePos - FragPos);
	float spotAngle = max(0.0f, dot(-lightDir, normalize(eyeDirection)));

	if(spotAngle < 0.96f)
		return 0.0;
	else
		return pow(spotAngle , 102);
}

float flashlighPhong(){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(eyePos - FragPos);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	return ((diffuse + ambient  + specular) * getFlashlight());

}

float directionPhong(){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(sunDirection);
	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;

	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;
	return (diffuse + ambient  + specular);
}

float pointPhong( vec3 lightPointPos){
	vec3 normalizedNormal = normalize(normal);
	vec3 lightDir = normalize(lightPointPos - FragPos);
	float diffuse = max(dot(normalizedNormal, lightDir), 0.0)  * diffuseStrength ;

	vec3 viewDir = normalize(eyePos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);  
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), 128) * specularStrength;
	float distance    = length(lightPointPos - FragPos);
	float attenuation = 1.0 / (0.5 * distance + 0.5 *distance * distance); 

	return (diffuse + ambient  + specular) * attenuation;
}

float  ananasPhong( vec3 lightPointPos){
	float distance =length(vec3(0.0,0.0,0.0) - FragPos);
	distance -= 0.7;
	float attenuation = 1.0 / (0.6 * distance +  distance * distance * distance); 
	return attenuation ;
}

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


void main() {

	if( objectType == 2){ 
		color =  texture(MTexture, ShadertextureCoord);

		vec2 tmpCoords = vec2(ShadertextureCoord.x + sunAlpha, ShadertextureCoord.y);
		if(tmpCoords.x > 1.0f){
			tmpCoords.x -= 1.0f;
		}

		vec4 tmpColor = texture(skyboxSun, tmpCoords);
		if(tmpColor.x > 0.01){
			color += tmpColor;
		}
	} else if (objectType == 5){
		float imageSize = 0.25;
		e
	} else {
		vec3 lightning = vec3(0.0,0.0,0.0);
		if(flashlight > 0) {
			lightning += flashlighPhong() * lightColor;
			lightning += directionPhong() * 0.3 * lightColor;
			for(int i = 0; i < countOflights; ++i){
				lightning += pointPhong( lights[i]);
			}
		} else {
			lightning += directionPhong() * 1.2  * lightColor;
		}

		color =  vec4(lightning,1.0) * texture(MTexture, ShadertextureCoord);
	}

	if(objectType != 5){
		color += vec4(lightColor, 1.0) * lightFog(vec3(0.0,0.0,0.0));
	}
}
