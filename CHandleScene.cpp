#include "CHandleScene.h"

glm::vec3 CHandleScene::cubicBezier(float u, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3,const glm::vec3 & cp4) {
	glm::vec3 ret;
	float u3 = u * u* u;
	float u2 = u * u;
	float oneMinusU = (1 - u);
	float oneMinusU2 = (1 - u) * (1 - u);
	float oneMinusU3 = (1 - u) * (1 - u) * (1 - u);

	ret.x = cp4.x *u3 +		3 * cp3.x * u2 * oneMinusU +	3 * cp2.x * u * oneMinusU2 +	cp1.x * oneMinusU3;
	ret.y = cp4.y *u3 +		3 * cp3.y * u2 * oneMinusU +	3 * cp2.y * u * oneMinusU2 +	cp1.y * oneMinusU3;
	ret.z = cp4.z *u3 +		3 * cp3.z * u2 * oneMinusU +	3 * cp2.z * u * oneMinusU2 +	cp1.z * oneMinusU3;

	return ret;
}



CHandleScene::CHandleScene(unsigned int maxCountOfLights) :light(glm::vec3(3.0, 1.0, 0.0), maxCountOfLights)
{
	objects = std::vector<CObject>();
}


CHandleScene::~CHandleScene()
{
}

void CHandleScene::init(unsigned int shaders)
{
	light.init(shaders);
	for (auto & it : objects) {
		it.init(shaders);
	}

	for (auto & it : ananasPieces) {
		it.init(shaders);
		it.rotate((float)(std::rand() % 360));
	}
}

bool start = true;
const glm::vec3 p4 = glm::vec3(0.0f, -0.2f, 2.5f);
const glm::vec3 p3 = glm::vec3(0.0f, 2.0f, 2.5f);
const glm::vec3 p2 = glm::vec3(0.0f, 2.0f, 1.5f);
const glm::vec3 p1 = glm::vec3(0.0f, 0.7f, 0.7f);

void CHandleScene::draw(float time)
{
	static float startTime;
	if (start) {
		startTime = time;
		start = false;
	}

	float u = (time - startTime) / 2;
	if (u > 1.0f) {
		u = 0.0f;
		start = true;
	}


	light.draw(time);
	for (auto & it : objects) {
		it.draw();
	}

	for (auto & it : ananasPieces) {
		glm::vec3 tmp = cubicBezier(u, p1, p2, p3, p4);
		it.changePosition(tmp);
		it.draw();
	}
}
