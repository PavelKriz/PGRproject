#pragma once
#include<vector>
#include "CLighting.h"
#include "CCamera.h"
#include "CObject.h"

class CHandleScene
{
	std::vector<CObject> objects;
	CLighting light;
public:
	CHandleScene(unsigned int maxCountOfLights);
	~CHandleScene();
	void init(unsigned int shaders);
	void draw();
	
	void addSkybox(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::SKYBOX, toModel, toTexture)); }
	void addAnanas(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::ANANAS, toModel, toTexture)); }
	void addAnanasPiece(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::ANANAS_PIECE, toModel, toTexture)); }
	void addStatic(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::STATIC, toModel, toTexture)); }
	void enableDisableFlashLight() { light.enableDisableFlashLight(); }
};

