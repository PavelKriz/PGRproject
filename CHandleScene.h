#pragma once
#include<vector>
#include<math.h>
#include "CLighting.h"
#include "CCamera.h"
#include "CObject.h"

const std::string pizzaOBJ = "pizza.obj";
const std::string pizzaTEX = "pizza2kb.png";
const std::string skyboxOBJ = "spaceBox.obj";
const std::string skyboxTEX = "bigSpaceNoSun.jpg";
const std::string skyboxTEX2 = "sunInSpace.png";
const std::string ananasOBJ = "ananas.obj";
const std::string ananasTEX = "ananas4k.png"; 
const std::string ananasPieceOBJ = "ananasPiece.obj";
const std::string ananasPieceTEX = "ananasPiece.png";
const std::string explosionOBJ = "explosion.obj";
const std::string explosionTEX = "explosion2.png";

const int ANANASPIECES_MAX_COUNT = 20;

class CHandleScene
{
	struct SAnanasPiece {
		CObject piece;
		bool alive;
		bool move;
		int pointLightId;
		float angle;
		double startTime;
		float bezierT;
		glm::vec3 p4;
		glm::vec3 p3;
	};

	struct SExplosion {
		CObject explosion;
		double startTime;
	};

	bool pizzaRotation;
	int aCounter;
	CLighting light;
	std::vector<CObject> objects;
	std::vector<SExplosion> explosions;
	SAnanasPiece ananasPieces[ANANASPIECES_MAX_COUNT];
	CObject referencePiece;
	CObject referenceExplosion;


	double rand0812();
	glm::vec3 cubicBezier(float bezierT, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3, const glm::vec3 & cp4);
	void setBezierAlfa(SAnanasPiece * piece,double time);
	void handleExplosions(double time);
	void bornExplosionOnAnanas(float angle, double time);
	void bornExplosionOnPizza(float angle, double time, glm::vec3  position);
	void bornAnanasPiece(double time);
	void killAnanasPiece(SAnanasPiece * piece);
	void checkAnanasLife(SAnanasPiece * piece, double time);
	void handleGameLife(unsigned int shaderProgram, double time);
public:
	CHandleScene(unsigned int maxCountOfLights);
	~CHandleScene();
	void objectEcho(int id, double time);
	void init(unsigned int shaders);
	void draw(unsigned int shaders, double time);
	
	void addObjects() {
		objects.push_back(CObject(CObject::EObjectType::PIZZA, pizzaOBJ, pizzaTEX));
		objects.push_back(CObject(CObject::EObjectType::SKYBOX, skyboxOBJ, skyboxTEX, skyboxTEX2));
		objects.push_back(CObject(CObject::EObjectType::ANANAS, ananasOBJ, ananasTEX));
		referencePiece = CObject(CObject::EObjectType::ANANAS_PIECE, ananasPieceOBJ, ananasPieceTEX, glm::vec3(-3.0f, 1.0f, 0.0f));
		referenceExplosion = CObject(CObject::EObjectType::EXPLOSION, explosionOBJ, explosionTEX, glm::vec3(0.0f, 0.75f, 0.75f));
	}

	void enableDisableFlashLight() { light.enableDisableFlashLight(); }
};

