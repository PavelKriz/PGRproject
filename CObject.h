//----------------------------------------------------------------------------------------
/**
 * \file       CObject.h
 * \author     Pavel Kriz
 * \date       22/04/2019
 * \brief      Trida ktera predstavuje objekt ve svete
 *
 *  Umoznuje nacist objekt, inicializovat ho v programu a nasledne ho zobrazovat
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include<pgr.h>

//!  CObject class, ovlada a zpracovava objekt ve svete
/*!
	Umi objekty nacist nasledovne je inicializovat do bufferu a potom je vykreslovat
*/
class CObject {
	std::vector<float>  vertices;	//!< pole s vrcholy ulozenymi ve floatech
	unsigned int sizeOfVertices;	//!< velikost bufferu, pocet jeho floatu
	unsigned int countOfVertices;	//!< pocet vrcholu (pro kazdy polygon jsou tri), nepocitaji se jako identicke
	GLuint arrayBuffer;	//!< id  bufferu s daty
	GLuint vao;	//!< id vertex array pro objekt
	GLuint positionLoc;
	GLuint texturePos;	//!< id/pozice textur 
	GLuint textureCoordsPos;	//!< id atributu texturovych souradnic v saderu
	GLuint textureSamplerPos;	//!< id sampleru textury v shaderu
	GLuint normalPos;	//!< pozice normal v shaderu
	std::string textureName;	//!< nazev textury objektu
public:
	//! Konstruktor
	/*!
	podle jmen souboru textury a souboru s vrcholy nacte a inicalizuje objekt
	\param[in] fileName soubor s vrcholy
	\param[in] toTextureName jmeno souboru s texturami
	*/
	CObject(std::string fileName, std::string toTextureName);
	//! Inicializace
	/*!
	incializuje objekt, nahraje jeho textury a vrcholy
	\param[in] shaderProgram pro to aby mohl nabindovat objekty s promenymi v shaderech
	*/
	void init(GLuint shaderProgram);
	//! Vykresleni souboru
	/*!
	vykresli objekt podle informaci co ma
	*/
	void draw();
};
