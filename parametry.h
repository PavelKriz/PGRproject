//----------------------------------------------------------------------------------------
/**
 * \file       parametry.h
 * \author     Pavel Kriz
 * \date       22/04/2019
 * \brief      Zakladni nastavitelne parametry programu
 *
 *  Umoznuje nastavit vlastnosti zobrazovani, ovladani, shaderu. 
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
namespace krizpav6 {
	const int WINDOW_WIDTH = 1440;					//!< sirka oknna pro glut
	const int WINDOW_HEIGHT = 900;					//!< vyska oknna pro glut
	const std::string WINDOW_TITLE = "PGR project x";	//!< nazev vyskakovaciho okna
	const int refreshTimeMs = 33;					//!< jak rychly ma byt refresh
	const std::string vertexShader = "vertexShader.frag";	//!< jmeno souboru s vertex shaderem
	const std::string fragmentShader = "fragmentShader.frag";	//! jmeno souboru s fragment shaderem
	const float basicCameraSpeed = 0.1f;			//!< slouzi pro incializaci rychlosti kamery
	const int maximumNumberOfPointLights = 6;
	const char moveForwardKey = 'w';	//!< normal key only
	const char moveForwardKey2 = GLUT_KEY_UP;	//!< special key only
	const char moveBackwardKey = 's';	//!< normal key only
	const char moveBackwardKey2 = GLUT_KEY_DOWN; //!< special key only
	const char moveUpKey = 'q';	//!< normal key only
	const char moveDownKey = 'e';	//!< normal key only
	const char moveLeftKey = 'a';	//!< normal key only
	const char moveLeftKey2 = GLUT_KEY_LEFT; //!< special key only
	const char moveRightKey = 'd';	//!< normal key only
	const char moveRightKey2 = GLUT_KEY_RIGHT;	//!< special key only
	const char freeCameraKey = '1';	//!< normal key only
	const char firstViewKey = '2';	//!< normal key only
	const char secondViewKey = '3';	//!< normal key only
	const char flashLightOnOff = 'l';	//!< normal key only
	const int crazyModKey1 = GLUT_KEY_F1;	//!< special key only
	const int crazyModKey2 = GLUT_KEY_F2;	//!< special key only
	const int crazyModKey3 = GLUT_KEY_F3;	//!< special key only
	const int crazyModKey4 = GLUT_KEY_F4;	//!< special key only

	const std::string pizzaOBJ = "data/pizza.obj";
	const std::string pizzaTEX = "data/pizza2kb.png";
	const std::string skyboxOBJ = "data/spaceBox.obj";
	const std::string skyboxTEX = "data/bigSpaceNoSun.jpg";
	const std::string skyboxTEX2 = "data/sunInSpace.png";
	const std::string ananasOBJ = "data/ananas.obj";
	const std::string ananasTEX = "data/ananas4k.png";
	const std::string ananasPieceOBJ = "data/ananasPiece.obj";
	const std::string ananasPieceTEX = "data/ananasPiece.png";
	const std::string explosionOBJ = "data/explosion.obj";
	const std::string explosionTEX = "data/explosion2.png";

	const float lightR = 1.0f;	//!< RGB light - R
	const float lightG = 0.9f;	//!< RGB light - G
	const float lightB = 0.9f;	//!< RGB light - B
	const float AMBIENT_REGULATOR = 0.8f;	//!< lower or higher ambient

	const int ANANASPIECES_MAX_COUNT = 150;	//!< max count of ananases

	const glm::vec3 bezierControlPoint4 = glm::vec3(0.0f, -0.23f, 2.5f); //!< bezier control point 4
	const glm::vec3 bezierControlPoint3 = glm::vec3(0.0f, 2.0f, 2.5f);	//!< bezier control point 3
	const glm::vec3 bezierControlPoint2 = glm::vec3(0.0f, 2.0f, 1.5f);	//!< bezier control point 2
	const glm::vec3 bezierControlPoint1 = glm::vec3(0.0f, 0.7f, 0.7f);	//!< bezier control point 1
}