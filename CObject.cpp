#include "CObject.h"
#include "ObjectLoader.h"

void CObject::initTransformMatrix()
{
	translateScale[3].x = objectPosition.x;
	translateScale[3].y = objectPosition.y;
	translateScale[3].z = objectPosition.z;
}

void CObject::setTransformMatrix()
{
	translateScale[3].x = objectPosition.x;
	translateScale[3].y = objectPosition.y;
	translateScale[3].z = objectPosition.z;
}

const float skyboxVertices[]{
	1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 ,
	-1 , 0 , -1 , 1 , 1 , 0 , 1 , 0 ,
	-1 , 0 , 1 , 0 , 1 , 0 , 1 , 0 ,
	1 , 0 , 1 , 0 , 0 , 0 , 1 , 0 ,
	1 , 0 , -1 , 1 , 0 , 0 , 1 , 0 ,
	-1 , 0 , -1 , 1 , 1 , 0 , 1 , 0 ,
};

void CObject::defaultConstructor(EObjectType toType, std::string fileName, std::string toTextureName)
{
	objectType = toType;
	objectPosition = glm::vec3(0.0f,0.0f,0.0f);
	translateScale = glm::mat4(1.0f);
	vFront = glm::vec3();
	inModelRotation = glm::mat4(1.0f);
	vertices = std::vector<float>();
	objectLoader(fileName.c_str(), vertices);
	if (objectType == EXPLOSION) {
		vertices.clear();
		for (int i = 0; i < 48; ++i) {
			vertices.push_back(skyboxVertices[i]);
		}
	}
	textureName = toTextureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	initTransformMatrix();
	inWorldRotation = glm::mat4(1.0f);
	explosionAlpha = 0;
}

CObject::CObject(CObject * image) :objectType(image->objectType) {
	objectPosition = image->objectPosition;
	translateScale = glm::mat4(1.0f);
	vFront = glm::vec3();
	inModelRotation = glm::mat4(1.0f);
	vertices = image->vertices;
	textureName = image->textureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	initTransformMatrix();
	arrayBuffer = image->arrayBuffer;
	vertexAtribPointerPos = image->vertexAtribPointerPos;
	normalPos = image->normalPos;
	objectTypePos = image->objectTypePos;
	transformMatrixPos = image->transformMatrixPos;
	texturePos = image->texturePos;
	textureSamplerPos = image->textureSamplerPos;
	textureCoordsPos = image->textureCoordsPos;
	skyboxSunTexture = image->skyboxSunTexture;
	skyboxTexSamplerPos = image->skyboxTexSamplerPos;
	explosionAlphaPos = image->explosionAlphaPos;
	vao = image->vao;
	explosionAlpha = 0;
	inWorldRotation = glm::mat4(1.0f);
}

void CObject::init(GLuint shaderProgram)
{

	glGenBuffers(1, &arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	vertexAtribPointerPos = 0;
	glEnableVertexAttribArray(vertexAtribPointerPos);
	glVertexAttribPointer(vertexAtribPointerPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	//NORMALY
	normalPos = 1;
	//normalPos = glGetAttribLocation(shaderProgram, "vertexShaderNormal");
	glEnableVertexAttribArray(normalPos);
	glVertexAttribPointer(normalPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	CHECK_GL_ERROR();

	//UNIFORM
	objectTypePos = glGetUniformLocation(shaderProgram, "objectType");
	transformMatrixPos = glGetUniformLocation(shaderProgram, "transform");
	explosionAlphaPos = glGetUniformLocation(shaderProgram, "explosionAlpha");



	//OBRAZEK
	texturePos = pgr::createTexture(textureName, true);
	if (texturePos == 0) {
		pgr::dieWithError("Texture loading failed.");
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePos);
	textureSamplerPos = glGetUniformLocation(shaderProgram, "MTexture");

	glUseProgram(shaderProgram);
	glUniform1i(textureSamplerPos, 0);

	textureCoordsPos = 2;
	glVertexAttribPointer(textureCoordsPos, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(textureCoordsPos);

	if (objectType == SKYBOX) {
		skyboxSunTexture = pgr::createTexture(secondTexture.c_str(), true);
		if (skyboxSunTexture == 0) {
			pgr::dieWithError("Texture loading failed.");
		}
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skyboxSunTexture);
		skyboxTexSamplerPos = glGetUniformLocation(shaderProgram, "skyboxSun");

		glUseProgram(shaderProgram);
		glUniform1i(skyboxTexSamplerPos, 1);

	}

	CHECK_GL_ERROR();
}





void CObject::draw()
{
	glUniformMatrix4fv(transformMatrixPos, 1, GL_FALSE, glm::value_ptr(inWorldRotation * translateScale * inModelRotation));

	if (objectType == SKYBOX) {
		glUniform1i(objectTypePos, 2);
	}
	else if (objectType == ANANAS) {
		glUniform1i(objectTypePos, 3);
	}
	else if (objectType == ANANAS_PIECE) {
		glUniform1i(objectTypePos, 4);
	}
	else if (objectType == EXPLOSION) {
		glUniform1i(objectTypePos, 5);
		glUniform1i(explosionAlphaPos, explosionAlpha);
	}
	else {
		glUniform1i(objectTypePos, 1);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePos);

	if (objectType == SKYBOX) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skyboxSunTexture);
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, countOfVertices);

	if (vao > 1000) {
		sin(5);
	}

	CHECK_GL_ERROR();
}


void CObject::changePosition(const glm::vec3 & newPos) {
	if (objectType == ANANAS_PIECE) {
		vFront = normalize(newPos - objectPosition);
		inModelRotation = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) - vFront, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		objectPosition = newPos;
		setTransformMatrix();
	}
	else {
		objectPosition = newPos;
		setTransformMatrix();
	}
}

void CObject::modelRotate(){
	inModelRotation = glm::lookAt(	glm::vec3(0.0f, 0.0f, 0.0f),
									glm::vec3(0.0f, 0.0f, 0.0f) - glm::normalize(glm::vec3(0.0f, 0.7f, 0.7f)),
									glm::vec3(0.0f, 1.0f, 0.0f) );
}

void CObject::rotate(float angle) {
		inWorldRotation = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)) * inWorldRotation;
}