#pragma once
#include <pgr.h>
#include <vector>
#include <iostream>
#include <math.h>

class CLighting
{
	GLuint shaderProgram;
	unsigned int sunDirectionPos;
	glm::vec3 sunDirection;
	float sunAlpha;
	unsigned int flashlightPos;
	int flashlight;
	unsigned int pointLightsPos;
	unsigned int pointLightsCountPos;
	std::vector<float> pointLights;
	unsigned int maxOfPointLights;
	unsigned int sunAlphaPos;
	void setSunDirectionVector();
public:
	CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights);
	~CLighting();
	void init(GLuint shaders);
	void draw(double time);
	//nastavit smerove
	bool setDirectionLight(glm::vec3 Position);
	//nastaveni reflektoru
	bool enableDisableFlashLight();
	//pridani/aktualizace bodoveho
};

