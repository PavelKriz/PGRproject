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
	unsigned int maxOfPointLights;
	glm::vec3 sunDirection;
	float sunAlpha;
	unsigned int sunAlphaPos;
	unsigned int flashlightPos;
	int flashlight;
	unsigned int pointLightsPos;
	unsigned int pointLightsCountPos;
	std::vector<float> pointLights;
	std::vector<std::pair<glm::vec3,glm::mat4 >> pointLightsPositions;
	std::vector<std::pair<bool, int>> isFree;

	void setSunDirectionVector();
	void handlePointLights();
 public:
	CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights);
	~CLighting();

	int addPointLight(glm::vec3 position);
	void updatePointLight(int id, glm::vec3 position);
	void updatePointLight(int id, float angle);
	void endPointLight(int id);

	void init(GLuint shaders);
	void draw(double time);
	//nastaveni reflektoru
	bool enableDisableFlashLight();
};

