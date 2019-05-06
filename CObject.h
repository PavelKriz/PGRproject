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
public:
	enum EObjectType {
		UNKNOWN = 0,
		PIZZA = 1,
		SKYBOX = 2,
		ANANAS = 3,
		ANANAS_PIECE = 4,
		EXPLOSION = 5
	};
private:
	glm::vec3 objectPosition;
	glm::vec3 vFront;
	glm::mat4 translateScale;
	glm::mat4 rotationM;
	glm::mat4 ananasDirRotation;
	unsigned int skyboxSunTexture;
	unsigned int skyboxTexSamplerPos;
	float ananasPieceRotation;
	unsigned int objectTypePos;
	std::vector<float>  vertices;	//!< pole s vrcholy ulozenymi ve floatech
	unsigned int sizeOfVertices;	//!< velikost bufferu, pocet jeho floatu
	unsigned int countOfVertices;	//!< pocet vrcholu (pro kazdy polygon jsou tri), nepocitaji se jako identicke
	GLuint arrayBuffer;	//!< id  bufferu s daty
	GLuint vao;	//!< id vertex array pro objekt
	GLuint positionLoc;
	unsigned int transformMatrixPos;
	GLuint texturePos;	//!< id/pozice textur 
	GLuint textureCoordsPos;	//!< id atributu texturovych souradnic v saderu
	GLuint textureSamplerPos;	//!< id sampleru textury v shaderu
	GLuint normalPos;	//!< pozice normal v shaderu
	std::string textureName;	//!< nazev textury objektu
	unsigned int explosionAlphaPos;
	int explosionAlpha;
	EObjectType objectType;

	void initTransformMatrix();
	void setTransformMatrix();
public:
	//! Konstruktor
	/*!
	podle jmen souboru textury a souboru s vrcholy nacte a inicalizuje objekt
	\param[in] fileName soubor s vrcholy
	\param[in] toTextureName jmeno souboru s texturami
	*/
	CObject() : objectType(UNKNOWN){}
	CObject(EObjectType toType, std::string fileName, std::string toTextureName);
	CObject(EObjectType toType, std::string fileName, std::string toTextureName, glm::vec3 toDefaultPosition);
	CObject(CObject * image);
	EObjectType getType() { return objectType; }
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
	void changePosition(const glm::vec3 & newPos);
	void rotate(float angle);
	void constRotate() { rotationM = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0)) * rotationM; }
	const glm::mat4 & getRotationM() { return rotationM; }
	void setTexFrame(int frame) { explosionAlpha = frame; return; }
};
