//----------------------------------------------------------------------------------------
/**
 * \file       CCamera.h
 * \author     Pavel Kriz
 * \date       22/04/2019
 * \brief      Obstarava pohled do sveta v aplikaci
 *
 *  Zajistuje inicializaci pohledu. Jeho aktualizaci, podle klaves ale take podporuje pohyb pomoci mysi. Poskytuje matici projekce a tim pohled. 
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include<pgr.h>
#include<iostream>

/*!
ruzne stavy kamery
*/
enum viewState{
	TRANSITION,	//!< prechod mezi pohledy
	FREE,	//!< volny pohled 
	LOCK_ONE,	//!< prvni staticky pohled
	LOCK_TWO	//!< druhy staticky pohled
};


//!  CCamera class, controls the view
/*!
	Camera class, controls the view
	Ovlada pohled kamery pomoci matice kamery. (vytvari tedy matici projekce)
*/
class CCamera {
	viewState state;	//!< jakym zpusobem se zrovna kamera chova
	viewState nextState;	//!< znaci stav do ktereho se prejde, je li kamera v prechodu
	double transitionBaseTime;	//!< zakladni cas pro mereni delky doby prechodu
	float cameraSpeed;	//!< rychlost pohybu kamery

	glm::mat4 mPerspective;	//!< matice pro perspektivu, nastavi se na zacatku

	glm::vec3 vActualPos;
	glm::vec3 vActualFront;
	glm::vec3 vPosition;	//!< pozice kamery
	glm::vec3 vFront;	//!< smer pohledu kamery
	glm::vec3 vUp;	//!< up vektor kamery
	glm::vec3 vPositionNext;	//!< budouci pozice kamery jestlize je pohled v prechodu 
	glm::vec3 vFrontNext;	//!< budouci smer pohledu kamery jestlize je pohled v prechodu
	glm::vec3 vUpNext;	//!< budouci up vektor pohledu kamery jestlize je pohled v prechodu

	uint32_t cameraMatrixPos;	//!< id/umisteni matice projekce v shaderech
	GLuint eyePosPos;
	GLuint frontPos;
public:
	float yaw;	//<! otoceni kamery, podle toho( jeste s pitchem) se ve free kamere urcuje smer pohledu
	float pitch; //<! naklon kamery, podle toho( jeste s yawem) se ve free kamere urcuje smer pohledu

private:
	//! recalculate Yaw Pitch
	/*!
	 Kdyz je prechod z statickeho pohledu na dynamicky. Tak je potreba znovu vypocist spravny yaw a pitch,
	 jelikoz ve statickych pohledech je pouzit jen front vektor pro smer kamery, zatimco ve free kamere se front vektor vytvari prave z yawu a pitche
	*/
	void checkPosition();
	void recalculateYawPitch();
public:
	//! Konstruktor
	/*!
	inicalizuje instanci kamery - nastavi zakladni potrebne parametry
	\param[in] toPosition urci pocatecni pozici kamery
	\param[in] fov	urci field of view kamery
	\param[in] aspect informace o aspect ratio vykreslovaciho okna
	\param[in] zNear zbuffer near vzdalenost vykreslovani
	\param[in] zFar zbuffer far vzdalenost vykreslovani
	\param[in] toSpead urci zakladni rychlost kamery
	*/
	CCamera(const glm::vec3 & toPosition, float fov, float aspect, float zNear, float zFar, const float toSpeed);
	//! Pohyb dopredu
	/*!
		pohne pohled dopredu
		pohne pohled dopredu
	*/
	void init(GLuint shaders);
	void draw(double time);
	void CameraMoveForward();
	//! Pohyb dozadu
	/*!
		pohne pohled dozadu
	*/
	void CameraMoveBackward();
	//! Pohyb nahoru
	/*!
		pohne pohled nahoru
	*/
	void CameraMoveUp();
	//! Pohyb dolu
	/*!
		pohne pohled dolu
	*/
	void CameraMoveDown();
	//! Pohyb doleva
	/*!
		pohne pohled doleva
	*/
	void CameraMoveLeft();
	//! Pohyb doprava
	/*!
		pohne pohled doprava
	*/
	void CameraMoveRight();
	//! Zmena front vektoru
	/*!
		nastavi vektro podle toFront
		\param[in] toFront podle tohoto nastavi front vektor
	*/
	void setFront(const glm::vec3 & toFront);
	//! Zmena druhu pohledu
	/*!
		zmeni druh pohledu podle which
		\param[in] which informace ktery stav pohledu je povazovan
		\param[in] time informace o case od zacatku programu
	*/
	void changeViewType(viewState which, double time);
	//! Vyrobce matice projekce
	/*!
		podle druhu pohledu nastavi matici projekce a vrati ji
		\return matice projekce
	*/
	glm::mat4 GetViewProjection(double time);
	/*!
		vrati pozici divaka
		\return vektor pozice divaka
	*/
	glm::vec3 getPosition();
	/*!
		vrati smer pohledu divaka
		\return vektor smeru pozice divaka
	*/
	glm::vec3 getDirection();
};