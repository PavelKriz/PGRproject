//----------------------------------------------------------------------------------------
/**
 * \file       ObjectLoader.h
 * \author     Pavel Kriz
 * \date       22/04/2019
 * \brief      Parsuje .obj (Wavefront) soubory
 *
 *  Umi nacist z .obj (Wavefront) pozice vrcholu, jejich texturovaci souradnice a jejich normaly.
 *
*/
//----------------------------------------------------------------------------------------
#pragma once
#include <iostream>
#include <vector>
#include <pgr.h>


/*!
 Umi nacist z .obj (Wavefront) pozice vrcholu, jejich texturovaci souradnice a jejich normaly.
 \param[in] path cesta k .obj souboru
 \param[out] returnVector vektor do ktereho se nastrkaji vrcholy a jejich informace z .obj  souboru
 \return povedlo se/nepovedlo se
*/
bool objectLoader(const char * path, std::vector<float> & returnVector) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::vector< glm::vec3 > returnVertices;
	std::vector< glm::vec2 > returnUvs;
	std::vector< glm::vec3 > returnNormals;

	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int normalIndex = normalIndices[i];
		unsigned int uvIndex = uvIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 texture = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		returnVertices.push_back(vertex);
		returnUvs.push_back(texture);
		returnNormals.push_back(normal);
	}

	for (unsigned int i = 0; i < returnVertices.size(); ++i) {
		returnVector.push_back(returnVertices[i].x);
		returnVector.push_back(returnVertices[i].y);
		returnVector.push_back(returnVertices[i].z);
		returnVector.push_back(returnUvs[i].x);
		returnVector.push_back(returnUvs[i].y);
		returnVector.push_back(returnNormals[i].x);
		returnVector.push_back(returnNormals[i].y);
		returnVector.push_back(returnNormals[i].z);
	}

	return true;
}
