#include "CHandleScene.h"



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
	}
}

void CHandleScene::draw()
{
	light.draw();
	for (auto & it : objects) {
		it.draw();
	}

	for (auto & it : ananasPieces) {
		it.draw();
	}
}
