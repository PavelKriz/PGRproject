#pragma once
#include<vector>
#include<math.h>
#include "CLighting.h"
#include "CCamera.h"
#include "CObject.h"

class CHandleScene
{
	std::vector<CObject> objects;
	std::vector<CObject> ananasPieces;
	CLighting light;
	glm::vec3 cubicBezier(float u, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3, const glm::vec3 & cp4);
public:
	CHandleScene(unsigned int maxCountOfLights);
	~CHandleScene();
	void init(unsigned int shaders);
	void draw(float time);
	
	void addSkybox(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::SKYBOX, toModel, toTexture)); }
	void addAnanas(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::ANANAS, toModel, toTexture)); }
	void addAnanasPiece(std::string toModel, std::string toTexture, glm::vec3 toPosition) {
		ananasPieces.push_back(CObject(CObject::EObjectType::ANANAS_PIECE, toModel, toTexture,toPosition));
	}
	void addStatic(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::STATIC, toModel, toTexture)); }
	void enableDisableFlashLight() { light.enableDisableFlashLight(); }
};

