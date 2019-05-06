#include "CObject.h"
#include "ObjectLoader.h"

void CObject::initTransformMatrix()
{
	translateScale[3].x = objectPosition.x;
	translateScale[3].y = objectPosition.y;
	translateScale[3].z = objectPosition.z;
	if (objectType == ANANAS_PIECE) {
		translateScale[0].x = 0.1f;
		translateScale[1].y = 0.1f;
		translateScale[2].z = 0.1f;
	}

}

void CObject::setTransformMatrix()
{
	translateScale[3].x = objectPosition.x;
	translateScale[3].y = objectPosition.y;
	translateScale[3].z = objectPosition.z;
}


CObject::CObject(EObjectType toType, std::string fileName, std::string toTextureName) :objectType(toType)
{
	objectPosition = glm::vec3(0.0f,0.0f,0.0f);
	translateScale = glm::mat4(1.0f);
	vFront = glm::vec3();
	ananasDirRotation = glm::mat4(1.0f);
	vertices = std::vector<float>();
	objectLoader(fileName.c_str(), vertices);
	textureName = toTextureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	initTransformMatrix();
	rotationM = glm::mat4(1.0f);
	explosionAlpha = 0;
}

CObject::CObject(EObjectType toType, std::string fileName, std::string toTextureName, glm::vec3 toDefaultPosition) :objectType(toType)
{
	objectPosition = toDefaultPosition;
	translateScale = glm::mat4(1.0f);
	vFront = glm::vec3();
	ananasDirRotation = glm::mat4(1.0f);
	vertices = std::vector<float>();
	objectLoader(fileName.c_str(), vertices);
	textureName = toTextureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	initTransformMatrix();
	rotationM = glm::mat4(1.0f);
	explosionAlpha = 0;
}

CObject::CObject(CObject * image) :objectType(image->objectType) {
	objectPosition = image->objectPosition;
	translateScale = glm::mat4(1.0f);
	vFront = glm::vec3();
	ananasDirRotation = glm::mat4(1.0f);
	vertices = image->vertices;
	textureName = image->textureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	initTransformMatrix();
	arrayBuffer = image->arrayBuffer;
	positionLoc = image->positionLoc;
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
	rotationM = glm::mat4(1.0f);
}

void CObject::init(GLuint shaderProgram)
{

	glGenBuffers(1, &arrayBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	positionLoc = 0;
	glEnableVertexAttribArray(positionLoc);
	glVertexAttribPointer(positionLoc, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);

	//NORMALY
	normalPos = 1;
	//normalPos = glGetAttribLocation(shaderProgram, "vertexShaderNormal");
	glEnableVertexAttribArray(normalPos);
	glVertexAttribPointer(normalPos, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	CHECK_GL_ERROR();

	//UNIFORM
	objectTypePos = glGetUniformLocation(shaderProgram, "objectType");
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
		skyboxSunTexture = pgr::createTexture("sunInSpace.png", true);
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
	glUniformMatrix4fv(transformMatrixPos, 1, GL_FALSE, glm::value_ptr(rotationM * translateScale * ananasDirRotation));
	glUniform1i(explosionAlphaPos, explosionAlpha);

	CHECK_GL_ERROR();

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
	}
	else {
		glUniform1i(objectTypePos, 1);
	}

	CHECK_GL_ERROR();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePos);

	CHECK_GL_ERROR();

	if (objectType == SKYBOX) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, skyboxSunTexture);
	}

	CHECK_GL_ERROR();

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
		ananasDirRotation = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) - vFront, glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		objectPosition = newPos;
		setTransformMatrix();
	}
}

void CObject::rotate(float angle) {
	if (objectType == PIZZA) {
		if (angle < 0.5f) {
			rotationM = glm::rotate(glm::mat4(1.0f), glm::radians(1.0f),glm::vec3(0.0,1.0,0.0)) * rotationM;
		}
	}
	else {
		std::cout << angle << std::endl;
		rotationM = glm::rotate(glm::mat4(), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		std::cout << rotationM[0].x << "\t" << rotationM[1].x << "\t" << rotationM[2].x << "\t" << rotationM[3].x << std::endl;
		std::cout << rotationM[0].y << "\t" << rotationM[1].y << "\t" << rotationM[2].y << "\t" << rotationM[3].y << std::endl;
		std::cout << rotationM[0].z << "\t" << rotationM[1].z << "\t" << rotationM[2].z << "\t" << rotationM[3].z << std::endl;
		std::cout << rotationM[0].w << "\t" << rotationM[1].w << "\t" << rotationM[2].w << "\t" << rotationM[3].w << std::endl;

		std::cout << std::endl;
	}
}