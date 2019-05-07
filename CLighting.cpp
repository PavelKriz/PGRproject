#include "CLighting.h"

namespace krizpav6 {
	//================================================================================================

	void CLighting::setSunDirectionVector() {
		float tmpAlpha = sunAlpha - 0.25f;
		if (tmpAlpha < 0.0f) {
			tmpAlpha += 1.0f;
		}
		sunDirection.x = cos(tmpAlpha * 2 * M_PI);
		sunDirection.y = sin(tmpAlpha * 2 * M_PI);
		sunDirection.z = 0.0f;
	}

	//================================================================================================

	void CLighting::handlePointLights() {
		pointLights.clear();
		for (unsigned int i = 0; i < pointLightsPositions.size(); ++i) {
			if (isFree[i].first == false) {
				glm::vec3 tmp = (pointLightsPositions[i].second * glm::vec4(pointLightsPositions[i].first.x, pointLightsPositions[i].first.y,
					pointLightsPositions[i].first.z, 1.0f));
				pointLights.push_back(tmp.x);
				pointLights.push_back(tmp.y);
				pointLights.push_back(tmp.z);
				pointLights.push_back(pointLightsPositions[i].first.w);
			}
		}
	}

	//================================================================================================

	CLighting::CLighting(glm::vec3 toLightColor, float toAmbientRefulator, glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights)
	{
		ambientRegulator = toAmbientRefulator;
		lightColor = toLightColor;
		sunDirection = defaultSunDirection;
		maxOfPointLights = setMaxOfPointLights;
		sunStart = true;
		flashlight = 0;
	}

	//================================================================================================

	CLighting::~CLighting()
	{
	}

	//================================================================================================

	int CLighting::addPointLight(glm::vec3 position) {
		for (auto & it : isFree) {
			if (it.first) {
				it.first = false;
				pointLightsPositions[it.second].second = glm::mat4(1.0f);
				pointLightsPositions[it.second].first = glm::vec4(0.0f, 0.0f, 0.0f, (float)(0.1f + 2.0f * (((float)(rand() % 11)) / 100)));
				return it.second;
			}
		}
		return -1;
	}

	//================================================================================================

	void CLighting::updatePointLight(int id, glm::vec3 position) {
		pointLightsPositions[id].first = glm::vec4(position, pointLightsPositions[id].first.w);
	}

	//================================================================================================

	void CLighting::updatePointLight(int id, float angle) {
		pointLightsPositions[id].second = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * pointLightsPositions[id].second;
	}

	//================================================================================================

	void CLighting::endPointLight(int id) {
		isFree[id].first = true;
	}

	//================================================================================================

	void CLighting::init(GLuint shaders) {
		/*
		for (int i = 0; i < 9; ++i) {
			pointLights.push_back(points[i]);
		}*/
		for (unsigned int i = 0; i < maxOfPointLights; ++i) {
			isFree.push_back(std::make_pair(true, i));
			pointLightsPositions.push_back(std::make_pair(glm::vec4(), glm::mat4(1.0f)));
		}

		shaderProgram = shaders;
		sunDirectionPos = glGetUniformLocation(shaderProgram, "sunDirection");
		flashlightPos = glGetUniformLocation(shaderProgram, "flashlight");
		pointLightsCountPos = glGetUniformLocation(shaderProgram, "countOflights");
		pointLightsPos = glGetUniformLocation(shaderProgram, "lights");
		sunAlphaPos = glGetUniformLocation(shaderProgram, "sunAlpha");
		lightColorPos = glGetUniformLocation(shaderProgram, "lightColor");
		ambientRegulatorPos = glGetUniformLocation(shaderProgram, "ambientRegulator");
		CHECK_GL_ERROR();
	}

	//================================================================================================

	void CLighting::draw(double time) {
		static double startTime;
		if (sunStart) {
			sunStart = false;
			startTime = time;
		}

		sunAlpha = (float)(time - startTime) / 60;
		if (sunAlpha > 1.0f) {
			sunAlpha = 0.0f;
			startTime = time;
			sunStart = true;
		}
		setSunDirectionVector();

		handlePointLights();

		glUniform3f(sunDirectionPos, sunDirection.x, sunDirection.y, sunDirection.z);
		glUniform3f(lightColorPos, lightColor.x, lightColor.y, lightColor.z);
		glUniform1f(ambientRegulatorPos, ambientRegulator);
		glUniform1i(flashlightPos, flashlight);
		//POZOR MAXIMALNI POCET SVETEL SE MUSI MENIT V SHADERU
		glUniform1i(pointLightsCountPos, pointLights.size() / 4);
		glUniform1f(sunAlphaPos, sunAlpha);
		CHECK_GL_ERROR();
		if (pointLights.size() > 0) {
			glUniform4fv(pointLightsPos, pointLights.size() / 4, &pointLights[0]);
		}
		CHECK_GL_ERROR();
	}

	//================================================================================================

	bool CLighting::enableDisableFlashLight() {
		if (flashlight) {
			return flashlight = 0;
		}
		return flashlight = 1;
	}

	//================================================================================================

}