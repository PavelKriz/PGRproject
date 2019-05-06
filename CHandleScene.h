#pragma once
#include<vector>
#include<math.h>
#include "CLighting.h"
#include "CCamera.h"
#include "CObject.h"

const int ANANASPIECES_MAX_COUNT = 20;

class CHandleScene
{
	struct SAnanasPiece {
		int light;
		bool alive;
		float angle;
		CObject piece;
		bool move;
		double startTime;
		float u;
		glm::vec3 p4;
		glm::vec3 p3;
	};

	struct SExplosion {
		CObject explosion;
		double startTime;
		int frame;
	};
	bool pizzaRotation;
	int aCounter;
	std::vector<CObject> objects;
	std::vector<SExplosion> explosions;
	CObject referencePiece;
	CObject referenceExplosion;
	SAnanasPiece ananasPieces[ANANASPIECES_MAX_COUNT];
	CLighting light;


	double rand0812();
	glm::vec3 cubicBezier(float u, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3, const glm::vec3 & cp4);
	void setBezierAlfa(SAnanasPiece * piece,double time);
	void handleExplosions(double time);
	void bornExplosion(float angle, double time);
	void bornExplosionOnPizza(float angle, double time, glm::vec3  position);
	void bornAnanasPiece(double time);
	void killAnanasPiece(SAnanasPiece * piece);
	void checkLife(SAnanasPiece * piece, double time);
	void handleLife(unsigned int shaderProgram, double time);
public:
	CHandleScene(unsigned int maxCountOfLights);
	~CHandleScene();
	void objectEcho(int id, double time);
	void init(unsigned int shaders);
	void draw(unsigned int shaders, double time);
	
	void addSkybox(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::SKYBOX, toModel, toTexture)); }
	void addAnanas(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::ANANAS, toModel, toTexture)); }
	/*void addAnanasPiece(std::string toModel, std::string toTexture, glm::vec3 toPosition) {
		ananasPieces.push_back(CObject(CObject::EObjectType::ANANAS_PIECE, toModel, toTexture,toPosition));
	}*/
	void addPizza(std::string toModel, std::string toTexture) { objects.push_back(CObject(CObject::EObjectType::PIZZA, toModel, toTexture)); }
	void enableDisableFlashLight() { light.enableDisableFlashLight(); }
};

