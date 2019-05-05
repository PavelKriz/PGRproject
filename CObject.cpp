#include "CObject.h"
#include "ObjectLoader.h"

void CObject::setTransformMatrix()
{
	transform[3].x = objectPosition.x;
	transform[3].y = objectPosition.y;
	transform[3].z = objectPosition.z;
	if (objectType == ANANAS_PIECE) {
		transform[0].x = 0.1f;
		transform[1].y = 0.1f;
		transform[2].z = 0.1f;
	}
}

CObject::CObject(EObjectType toType, std::string fileName, std::string toTextureName)
{
	objectPosition = glm::vec3(0.0f,0.0f,0.0f);
	transform = glm::mat4(1.0f);
	objectType = toType;
	vertices = std::vector<float>();
	objectLoader(fileName.c_str(), vertices);
	textureName = toTextureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	setTransformMatrix();
}

CObject::CObject(EObjectType toType, std::string fileName, std::string toTextureName, glm::vec3 toDefaultPosition)
{
	objectPosition = toDefaultPosition;
	transform = glm::mat4(1.0f);
	objectType = toType;
	vertices = std::vector<float>();
	objectLoader(fileName.c_str(), vertices);
	textureName = toTextureName;
	sizeOfVertices = vertices.size() * sizeof(float);
	countOfVertices = vertices.size() / 8;
	setTransformMatrix();
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
	transformMatrixPos = glGetUniformLocation(shaderProgram, "transform");


	//OBRAZEK
	texturePos = pgr::createTexture(textureName, true);
	if (texturePos == 0) {
		pgr::dieWithError("Texture loading failed.");
	}
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePos);
	textureSamplerPos = glGetUniformLocation(shaderProgram, "MTexture");

	glUseProgram(shaderProgram);
	glUniform1i(textureSamplerPos, 0);

	textureCoordsPos = 2;
	glVertexAttribPointer(textureCoordsPos, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(textureCoordsPos);



	CHECK_GL_ERROR();
}

void CObject::draw()
{

	glUniformMatrix4fv(transformMatrixPos, 1, GL_FALSE, glm::value_ptr(transform));

	if (objectType == SKYBOX) {
		glUniform1i(objectTypePos, 2);
	}
	else if (objectType == ANANAS) {
		glUniform1i(objectTypePos, 3);
	}
	else {
		glUniform1i(objectTypePos, 1);
	}
	glBindTexture(GL_TEXTURE_2D, texturePos);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLES, 0, countOfVertices);
}