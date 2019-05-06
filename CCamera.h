//----------------------------------------------------------------------------------------
/**
 * \file       CCamera.h
 * \author     Pavel Kriz
 * \date       22/04/2019
 * \brief      Handles view
 *
 *	Handles view init and view updating according to keyboard actions and mouse movement. It provide projection matrix.
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
	TRANSITION,	//!< state between other states
	FREE,	//!< free camera state
	LOCK_ONE,	//!< first lock camera state
	LOCK_TWO	//!< second lock camera state
};


//!  CCamera class, controls the view
/*!
	Camera class, controls the view
	Handles view init and view updating according to keyboard actions and mouse movement. It provide projection matrix.
*/
class CCamera {
	viewState state;	//!< represent view method
	viewState nextState;	//!< represent next view method
	double transitionBaseTime;	//!< startTime of generic action
	float cameraSpeed;	//!< camera speed

	glm::mat4 mPerspective;	//!< perspective matrix, it is initialised in the beginning

	glm::vec3 vActualPos;	//!< actual position, even in transition
	glm::vec3 vActualFront;	//!< actual front, even in transition
	glm::vec3 vPosition;	//!< camera last positition, in transition represents old positition
	glm::vec3 vFront;	//!< camera last front, in transition represents old front
	glm::vec3 vUp;	//!< camera last up, in transition represents old up
	glm::vec3 vPositionNext;	//!< camera next positition, in transition represents future positition
	glm::vec3 vFrontNext;	//!< camera next front, in transition represents future front
	glm::vec3 vUpNext;	//!< camera next up, in transition represents future up

	uint32_t cameraMatrixPos;	//!< id of camera projectin matrix in shaders
	GLuint eyePosPos;	//!< id of camera position vector in shaders
	GLuint frontPos;	//!< id of camera fron vector in shaders
public:	
	float yaw;	//!< yaw of camera, with this (and with pitch) is in free camera calculated vfront
	float pitch; //!< pitch of camera, with this (and with yaw) is in free camera calculated vfront

private:
	//! recalculate Yaw Pitch
	/*!
	 Kdyz je prechod z statickeho pohledu na dynamicky. Tak je potreba znovu vypocist spravny yaw a pitch,
	 jelikoz ve statickych pohledech je pouzit jen front vektor pro smer kamery, zatimco ve free kamere se front vektor vytvari prave z yawu a pitche
	*/
	void recalculateYawPitch();
	//! check and get free camera in allowed area
	/*!
	 it checks whether is camera in camera allowed area (globe around center in distance 20),
	 if it isnt, than it teleport camera to other side of globe
	*/
	void checkPosition();
public:
	//! Constructor
	/*!
	inicialize camera
	\param[in] toPosition position of camera in beginning
	\param[in] fov	set field of camera view
	\param[in] aspect information about screen aspect ratio 
	\param[in] zNear zbuffer near distance
	\param[in] zFar zbuffer far distance
	\param[in] toSpead set default camera speed
	*/
	CCamera(const glm::vec3 & toPosition, float fov, float aspect, float zNear, float zFar, const float toSpeed);
	//! init camera in shaders
	/*!
		set camera uniforms in shader
	*/
	void init(GLuint shaders);
	//! draw camera in shaders
	/*!
		set values of camera uniforms in shaders
	*/
	void draw(double time);
	//! camera move forward
	void CameraMoveForward();
	//! camera move backward
	void CameraMoveBackward();
	//! camera move up
	void CameraMoveUp();
	//! camera move down
	void CameraMoveDown();
	//! camera move left
	void CameraMoveLeft();
	//! camera move right
	void CameraMoveRight();
	//! sets front
	/*!
		sets front of camera
		\param[in] toFront is set as new front of camera
	*/
	void setFront(const glm::vec3 & toFront);
	//! change view method
	/*!
		changes view method(camera states)
		\param[in] which defines wanted state
		\param[in] time information about time from start of program
	*/
	void changeViewType(viewState which, double time);
	//! getter for view projection
	/*!
		according to camera state it return projection matrix
		\return projection matrix
	*/
	glm::mat4 GetViewProjection(double time);
};