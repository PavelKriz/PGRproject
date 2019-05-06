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
const int WINDOW_WIDTH = 1024;					//!< sirka oknna pro glut
const int WINDOW_HEIGHT = 1024;					//!< vyska oknna pro glut
const std::string WINDOW_TITLE = "PGR project x";	//!< nazev vyskakovaciho okna
const int refreshTimeMs = 33;					//!< jak rychly ma byt refresh
const std::string vertexShader = "vertexShader.glsl";	//!< jmeno souboru s vertex shaderem
const std::string fragmentShader = "fragmentShader.glsl";	//! jmeno souboru s fragment shaderem
const float basicCameraSpeed = 0.1f;			//!< slouzi pro incializaci rychlosti kamery
const int maximumNumberOfPointLights = 6;
const char moveForwardKey = 'w';
const char moveBackwardKey = 's';
const char moveUpKey = 'q';
const char moveDownKey= 'e';
const char moveLeftKey = 'a';
const char moveRightKey = 'd';
const char freeCameraKey = '1';
const char firstViewKey = '2';
const char secondViewKey = '3';
const char flashLightOnOff = 'l';
const int crazyModKey1 = GLUT_KEY_F1;
const int crazyModKey2 = GLUT_KEY_F2;
const int crazyModKey3 = GLUT_KEY_F3;
const int crazyModKey4 = GLUT_KEY_F4;

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

const int ANANASPIECES_MAX_COUNT = 50;

const glm::vec3 bezierControlPoint4 = glm::vec3(0.0f, -0.23f, 2.5f);
const glm::vec3 bezierControlPoint3 = glm::vec3(0.0f, 2.0f, 2.5f);
const glm::vec3 bezierControlPoint2 = glm::vec3(0.0f, 2.0f, 1.5f);
const glm::vec3 bezierControlPoint1 = glm::vec3(0.0f, 0.7f, 0.7f);
