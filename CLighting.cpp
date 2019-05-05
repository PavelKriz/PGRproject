#include "CLighting.h"

void CLighting::setSunDirectionVector() {
	float tmpAlpha = sunAlpha - 0.25f;
	if (tmpAlpha < 0.0f) {
		tmpAlpha += 1.0f;
	}
	sunDirection.x = cos(tmpAlpha * 2 * M_PI);
	sunDirection.y = sin(tmpAlpha * 2 * M_PI);
	sunDirection.z = 0.0f;
}

CLighting::CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights)
{
	sunDirection = defaultSunDirection;
	maxOfPointLights = setMaxOfPointLights;
	flashlight = 0;
}


CLighting::~CLighting()
{
}

float points[] = {	-3.0f, 1.0f, 0.0f, 
					3.0f, 1.0f, 0.0f,
					0.0f, 1.0f, 3.0f };

void CLighting::init(GLuint shaders) {
	for (int i = 0; i < 9; ++i) {
		pointLights.push_back(points[i]);
	}
	shaderProgram = shaders;
	sunDirectionPos = glGetUniformLocation(shaderProgram, "sunDirection");
	flashlightPos = glGetUniformLocation(shaderProgram, "flashlight");
	pointLightsCountPos = glGetUniformLocation(shaderProgram, "countOflights");
	pointLightsPos = glGetUniformLocation(shaderProgram, "lights");
	sunAlphaPos = glGetUniformLocation(shaderProgram, "sunAlpha");
	CHECK_GL_ERROR();
}


bool sunStart;
void CLighting::draw(float time) {
	static float startTime;
	if (sunStart) {
		sunStart = false;
		startTime = time;
	}

	sunAlpha = (time - startTime)/60;
	std::cout << sunAlpha << std::endl;
	if (sunAlpha > 1.0f) {
		sunAlpha = 0.0f;
		startTime = time;
		sunStart = true;
	}
	setSunDirectionVector();


	std::cout << sunDirection.x << "\t" << sunDirection.y << "\t" << sunDirection.z << std::endl;
	glUniform3f(sunDirectionPos, sunDirection.x, sunDirection.y, sunDirection.z);
	glUniform1i(flashlightPos, flashlight);
	//POZOR MAXIMALNI POCET SVETEL SE MUSI MENIT V SHADERU
	glUniform1i(pointLightsCountPos, pointLights.size() / 3);
	glUniform1f(sunAlphaPos, sunAlpha);
	glUniform3fv(pointLightsPos, pointLights.size() / 3,&pointLights[0]);
	CHECK_GL_ERROR();
}

bool CLighting::enableDisableFlashLight() {
	if (flashlight) {
		return flashlight = 0;
	}
	return flashlight = 1;
}
