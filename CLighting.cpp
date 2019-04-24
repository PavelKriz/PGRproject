#include "CLighting.h"



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
	CHECK_GL_ERROR();
}


void CLighting::draw() {
	glUniform3f(sunDirectionPos, sunDirection.x, sunDirection.y, sunDirection.z);
	glUniform1i(flashlightPos, flashlight);
	//POZOR MAXIMALNI POCET SVETEL SE MUSI MENIT V SHADERU
	glUniform1i(pointLightsCountPos, pointLights.size() / 3);
	glUniform3fv(pointLightsPos, pointLights.size() / 3,&pointLights[0]);
	CHECK_GL_ERROR();
}

bool CLighting::enableDisableFlashLight() {
	if (flashlight) {
		return flashlight = 0;
	}
	return flashlight = 1;
}
