//----------------------------------------------------------------------------------------
/**
 * \file       CObject.h
 * \author     Pavel Kriz
 * \date       30/04/2019
 * \brief      Represents object in 3d world
 *
 *	Allows load object, init object in shaders and then render object
 *
 *
*/
//----------------------------------------------------------------------------------------

#pragma once
#include<pgr.h>

namespace krizpav6 {

	//!  Represents object in 3d world
	/*!
		Allows load object, init object in shaders and then render object
	*/
	class CObject {
	public:
		/*!
		types of object
		*/
		enum EObjectType {
			UNKNOWN = 0,
			PIZZA = 1,
			SKYBOX = 2,
			ANANAS = 3,
			ANANAS_PIECE = 4,
			EXPLOSION = 5
		};
	private:
		unsigned int objectTypePos;	//!< id(in shaders) of type of object
		EObjectType objectType;	//!< type of object
		glm::vec3 objectPosition;	//!< position of object 
		glm::vec3 vFront;	//!< front vector of object
		glm::mat4 translateScale;	//!< translate and scale matrix of object
		glm::mat4 inWorldRotation;	//!< rotation matrix in world coordinates
		glm::mat4 inModelRotation;	//!< rotation matrix in model coordinates
		std::vector<float>  vertices;	//!< vertices of object
		unsigned int sizeOfVertices;	//!< sizeof(vertices)
		unsigned int countOfVertices;	//!< count of vertices (for each polygon are three vertices)
		GLuint arrayBuffer;	//!< id of array buffer in shaders 
		GLuint vao;	//!< id of vertex array in shaders
		GLuint vertexAtribPointerPos;	//!< id of vertexAtribPointer location in shaders
		unsigned int transformMatrixPos;	//!< id of transform matrix in shaders
		GLuint texturePos;	//!< id of texture object
		GLuint textureCoordsPos;	//!< id of texture coord atribute in shaders
		GLuint textureSamplerPos;	//!< id of texture sampler in shaders
		GLuint normalPos;	//!< id of normal atributes in shaders

		std::string textureName;	//!< name of texture image
		std::string secondTexture;	//!< name of second texture image
		int explosionAlpha;	//!< it is frame( modulo 16 ) of explosion animation
		unsigned int explosionAlphaPos;	//!< id of explosionAlpha in shaders
		unsigned int skyboxSunTexture;	//!< id of skybox sun texture
		unsigned int skyboxTexSamplerPos;	//!<id of skybox sun texture sampler in shaders

		 //! init transform matrix in the beginning of object
		void initTransformMatrix();
		//! sets transform matrix in the middle of object lifetime
		void setTransformMatrix();
		//! generic constructor code
		/*!
		\param[in] toType is type of object
		\param[in] fileName	is name of file containing vertices
		\param[in] toTextureName is name of image which contains texture
		*/
		void defaultConstructor(EObjectType toType, std::string fileName, std::string toTextureName);
	public:
		//! Default Constructor makes empty object
		CObject() : objectType(UNKNOWN) {}
		//! Default non-empty constructor
		/*!
		\param[in] toType is type of object
		\param[in] fileName	is name of file containing vertices
		\param[in] toTextureName is name of image which contains texture
		*/
		CObject(EObjectType toType, std::string fileName, std::string toTextureName) { defaultConstructor(toType, fileName, toTextureName); }
		//! Two textures constructor
		/*!
		construct object with two textures
		\param[in] toType is type of object
		\param[in] fileName	is name of file containing vertices
		\param[in] toTextureName is name of image which contains texture
		\param[in] toSecondTexture is name of image which contains second texture
		*/
		CObject(EObjectType toType, std::string fileName, std::string toTextureName, std::string toSecondTexture) {
			if (toType == SKYBOX) {
				secondTexture = toSecondTexture;
			}
			defaultConstructor(toType, fileName, toTextureName);
		}
		//! Two textures constructor
		/*!
		construct object with two textures
		\param[in] toType is type of object
		\param[in] fileName	is name of file containing vertices
		\param[in] toTextureName is name of image which contains texture
		\param[in] toDefaultPosition is position of the object in beginning
		*/
		CObject(EObjectType toType, std::string fileName, std::string toTextureName, glm::vec3 toDefaultPosition) {
			defaultConstructor(toType, fileName, toTextureName);
			objectPosition = toDefaultPosition;
		}
		//! Copy Constructor
		/*!
		\param[in] image is object which will be copied
		*/
		CObject(CObject * image);
		//! get object type
		/*!
			\return type of object
		*/
		EObjectType getType() { return objectType; }
		//! get position of object
		/*!
			\return position vector of object
		*/
		const glm::vec3 & getPosition() { return objectPosition; }
		//! sets new position
		/*!
			if object type is ANANAS_PIECE it rotate in the direction of move
			\param[in] newPos is new set position
		*/
		void changePosition(const glm::vec3 & newPos);
		//! sets scale
		/*!
			\param[in] scale is new scale
		*/
		void setScale(float scale) {
			translateScale[0].x = scale;
			translateScale[1].y = scale;
			translateScale[2].z = scale;
		}
		//! init object rendering
		/*!
		init objects(vertices, uniforms, textures)
		\param[in] shaderProgram is id of shaders
		*/
		void init(GLuint shaderProgram);
		//! renders object
		void draw();
		//! rotate it by some const rotation in model coords
		void modelRotate();
		//! ads positive rotation in y axis
		/*!
		\param[in] angle by which is object rotated
		*/
		void rotate(float angle);
		//! rotate it by some const rotation in world coors
		void constRotate() { inWorldRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.5f), glm::vec3(0.0, 1.0, 0.0)) * inWorldRotation; }
		//! get matrix of rotation in world coordinates
		/*!
			\return rotation matrix in world coordinates
		*/
		const glm::mat4 & getWorldRotation() { return inWorldRotation; }
		//! set the right frame of dynamic texture
		/*!
		\param[in] frame is number of frame (modulo 16)
		*/
		void setTexFrame(int frame) { explosionAlpha = frame; return; }
	};
}
