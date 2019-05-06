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
const char * WINDOW_TITLE = "PGR project x";	//!< nazev vyskakovaciho okna
const int refreshTimeMs = 33;					//!< jak rychly ma byt refresh
const char * vertexShader = "vertexShader.glsl";	//!< jmeno souboru s vertex shaderem
const char * fragmentShader = "fragmentShader.glsl";	//! jmeno souboru s fragment shaderem
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