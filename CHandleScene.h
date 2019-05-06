#pragma once
#include<vector>
#include<math.h>
#include "CLighting.h"
#include "CCamera.h"
#include "CObject.h"
#include "parametry.h"

class CHandleScene
{
	int randomRange;	//!< sets the range of numbers genereted
	int randomPick;	//!< sets the lucky number which is picked from range

	/*!
		struct for maintain life of ananas piece
	*/
	struct SAnanasPiece {
		CObject piece;
		bool alive;
		bool move;
		int pointLightId;
		float angle;
		double startTime;
		float bezierT;
		glm::vec3 bezierControlPoint4;
		glm::vec3 bezierControlPoint3;
	};

	/*!
		struct for maintain life of explosion
	*/
	struct SExplosion {
		CObject explosion;
		double startTime;
	};

	bool pizzaRotation;	//!< information if the pizza is rotating
	int aCounter;	//!< count born ananas pieces from beginning
	CLighting light;	//!< light in scene
	std::vector<CObject> objects;	//!< regular(only one entity of each) objects
	SAnanasPiece ananasPieces[ANANASPIECES_MAX_COUNT];	//!< holds ananas pieces
	std::vector<SExplosion> explosions;	//!< holds explosions
	CObject referencePiece;	//!< is reference for creating ananas pieces
	CObject referenceExplosion;	//!< is reference for creating explosions

	//! return random value from <0.8, 1.2>
	/*!
		\return double precision random value from <0.8, 1.2>
	*/
	double rand0812();
	//! evaluate position on bezier curve
	/*!
		\param[in] bezierT defines relative position on curve
		\param[in] cp1 is control point 1
		\param[in] cp2 is control point 2
		\param[in] cp3 is control point 3
		\param[in] cp4 is control point 4
		\return position on bezier curve
	*/
	glm::vec3 cubicBezier(float bezierT, const glm::vec3 & cp1, const glm::vec3 & cp2, const glm::vec3 & cp3, const glm::vec3 & cp4);
	//! set right bezierAlpha
	/*!
		\param[in] piece in which will be set new bezierT (bezier Alfa)
		\param[in] time is time from beginning of program
	*/
	void setBezierAlfa(SAnanasPiece * piece,double time);
	//! handles explosions lifes
	/*!
		\param[in] time is time from beginning of program
	*/
	void handleExplosions(double time);
	//! spawn explosions by ananas at given angle
	/*!
	\param[in] angle angle where the explosion will spawn around ananas
	\param[in] time is time from beginning of program
	*/
	void bornExplosionOnAnanas(float angle, double time);
	//! spawn explosions on pizza
	/*!
	when ananas fell on pizza, this explosion should appear
	\param[in] angle angle where the explosion will spawn around pizza
	\param[in] time is time from beginning of program
	\param[in] position is place where the explosion will spawn according to distance from ananas
	*/
	void bornExplosionOnPizza(float angle, double time, glm::vec3  position);
	//! spawn ananas piece
	/*!
	ananas piece will spawn at random place around ananas topa
	\param[in] time is time from beginning of program
	*/
	void bornAnanasPiece(double time);
	//! something like destructor for ananas piece
	/*!
	\param[in] piece piece which will be destruct
	*/
	void killAnanasPiece(SAnanasPiece * piece);
	//! checks if there isnt ananas piece too long
	/*!
	\param[in] piece which will be checked if it should be destructed
	\param[in] time is time from beginning of program
	*/
	void checkAnanasLife(SAnanasPiece * piece, double time);
	//! handles moving parts of game
	/*!
	\param[in] time is time from beginning of program
	*/
	void handleGameLife( double time);
public:
	//! default constructor
	/*!
	\param[in] maxCountOfLights is maximum count of point lights in scene
	*/
	CHandleScene(unsigned int maxCountOfLights);
	~CHandleScene();
	//! echo from clicking on screen
	/*!
	\param[in] id is id of object that has been clicked
	\param[in] time is time from beginning of program
	*/
	void objectEcho(int id, double time);
	//! init object rendering
	/*!
	\param[in] shaders where it will be init
	*/
	void init(unsigned int shaders);
	//! object rendering
	/*!
	\param[in] time is time from beginning of program
	*/
	void draw( double time);
	
	//! adds all object to scene, its loading names of files from parameters
	void addObjects() {
		objects.push_back(CObject(CObject::EObjectType::PIZZA, pizzaOBJ, pizzaTEX));
		objects.push_back(CObject(CObject::EObjectType::SKYBOX, skyboxOBJ, skyboxTEX, skyboxTEX2));
		objects.push_back(CObject(CObject::EObjectType::ANANAS, ananasOBJ, ananasTEX));
		referencePiece = CObject(CObject::EObjectType::ANANAS_PIECE, ananasPieceOBJ, ananasPieceTEX, glm::vec3(-3.0f, 1.0f, 0.0f));
		referenceExplosion = CObject(CObject::EObjectType::EXPLOSION, explosionOBJ, explosionTEX, glm::vec3(0.0f, 0.75f, 0.75f));
	}
	//! anable or disable flashlight in scene
	void enableDisableFlashLight() { light.enableDisableFlashLight(); }
	//! sets ananas piece generation speed
	/*!
	\param[in] toRandomRange sets the range of numbers genereted
	\param[in] toRandomPick sets the lucky number which is picked from range
	*/
	void howCrazy(int toRandomRange,int toRandomPick) {
		randomRange = toRandomRange;
		randomPick = toRandomPick;
	}
};

