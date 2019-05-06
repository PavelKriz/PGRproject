#pragma once
#include <pgr.h>
#include <vector>
#include <utility>
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
	std::vector<std::pair<glm::vec3,glm::mat4 >> pointLightsPositions;
	std::vector<std::pair<bool, int>> isFree;
	unsigned int maxOfPointLights;
	unsigned int sunAlphaPos;
	void setSunDirectionVector();
	void getPointLights();
 public:
	CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights);
	~CLighting();

	int addPointLight(glm::vec3 position);
	void udpate(int id, glm::vec3 position);
	void udpate(int id, float angle);
	void endLightPoint(int id);

	void init(GLuint shaders);
	void draw(double time);
	//nastavit smerove
	bool setDirectionLight(glm::vec3 Position);
	//nastaveni reflektoru
	bool enableDisableFlashLight();
	//pridani/aktualizace bodoveho
};

