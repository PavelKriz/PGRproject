#pragma once
#include <pgr.h>
#include <vector>

class CLighting
{
	GLuint shaderProgram;
	unsigned int sunDirectionPos;
	glm::vec3 sunDirection;
	unsigned int flashlightPos;
	int flashlight;
	unsigned int pointLightsPos;
	unsigned int pointLightsCountPos;
	std::vector<float> pointLights;
	unsigned int maxOfPointLights;
public:
	CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights);
	~CLighting();
	void init(GLuint shaders);
	void draw();
	//nastavit smerove
	bool setDirectionLight(glm::vec3 Position);
	//nastaveni reflektoru
	bool enableDisableFlashLight();
	//pridani/aktualizace bodoveho
};

