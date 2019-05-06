//----------------------------------------------------------------------------------------
/**
 * \file       CLighting.h
 * \author     Pavel Kriz
 * \date       30/04/2019
 * \brief      Handles light
 *
 *	Handles light init and light updates like point light handling and sun direction during day
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include <pgr.h>
#include <vector>
#include <utility>
#include <iostream>
#include <math.h>


//!  Handles light
/*!
	Camera class, controls the view
	Handles light init and light updates like point light handling and sun direction during day
*/
class CLighting
{
	GLuint shaderProgram;	//!< shaders id
	unsigned int maxOfPointLights;	//!< maximum number of point lights
	unsigned int sunDirectionPos;	//!< id of sun direction vector in shaders 
	glm::vec3 sunDirection;	//!< sun direction vector
	float sunAlpha;	//!< sun angle on sky
	unsigned int sunAlphaPos;	//!< id of sunf angle in shaders
	int flashlight;	//!< information if flashlight is on (0 = off, 1 = on)
	unsigned int flashlightPos;	//!< id of flashlight in shaders
	unsigned int pointLightsPos;	//!< id of array with point lights positions in shaders
	unsigned int pointLightsCountPos;	//!< id of count of pointlights in shaders
	std::vector<float> pointLights;	//!< positions of pointlights in floats
	std::vector<std::pair<glm::vec3,glm::mat4 >> pointLightsPositions;	//!< positions of pointlights in vector and rotation matrix
	std::vector<std::pair<bool, int>> isFree;	//!< checks if is on same index in pointLightsPositions free place for pointlight

	//! set sun direction from sunAlpha
	void setSunDirectionVector();
	//! handle which pointlight is valid and will be rendered
	void handlePointLights();
 public:
	 //! Constructor
	/*!
	inicialize light
	\param[in] defaultSunDirection position of sun in the beginning
	\param[in] setMaxOfPointLights	sets max count of point lights
	*/
	CLighting( glm::vec3 defaultSunDirection, unsigned int setMaxOfPointLights);
	~CLighting();

	//! add point light
	/*!
		\param[in] position sets position of point light in the beginning
		\return -1 if pointLight wasnt added, or id of point light which was added
	*/
	int addPointLight(glm::vec3 position);
	//! updates point light of given id
	/*!
		\param[in] id sets which point light will be updated
		\param[in] position sets position of point light
	*/
	void updatePointLight(int id, glm::vec3 position);
	//! updates point light of given id
	/*!
		\param[in] id sets which point light will be updated
		\param[in] angle rotate point light by given angle
	*/
	void updatePointLight(int id, float angle);
	//! ends point light of given id
	/*!
		\param[in] id sets which point light will be deleted
	*/
	void endPointLight(int id);
	//! init light in shaders 
	/*!
		\param[in] shaders information about id of shaders
	*/
	void init(GLuint shaders);
	//! draw light in shaders 
	/*!
		\param[in] time information about time from the beginning of program
	*/
	void draw(double time);
	//! enables or disables flashlight
	bool enableDisableFlashLight();
};

