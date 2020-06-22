#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include "Header.h"


void OBJLoader(char *fileName, std::vector<outputData> &output, std::vector<UINT> &vertexIndicesFinal) {


	//_CRT_SECURE_NO_WARNINGS;
	using namespace std;
	using namespace DirectX;

	UINT numberOfVertices = 0, numberOfNormals = 0, numberOfUVs = 0, numberOfFaces = 0;
	vector<UINT> vIndices, vtIndices, vnIndices;
	vector<XMFLOAT3> vertices;
	vector<XMFLOAT2> uvs;
	vector<XMFLOAT3> normals;

	vector<XMFLOAT3> verticiesOut;
	vector<XMFLOAT3> normalsOut;
	vector<XMFLOAT3> UvsOut;

	FILE * file = fopen(fileName, "r");

	while (true)
	{

		char lineHeader[128];
		int res = fscanf(file, "%s \n", lineHeader);
		if (res == EOF)
			break;

		if (strcmp(lineHeader, "v") == 0)
		{
			XMFLOAT3 vertex;
			int a = fscanf(file, "%f %f %f \n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
			numberOfVertices++;
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			XMFLOAT2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
			numberOfUVs++;
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			XMFLOAT3 normal;
			fscanf(file, "%f %f %f \n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
			numberOfNormals++;
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			UINT vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%i %i %i %i %i %i %i %i %i \n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			//if (matches != 9) {
			//	MessageBox(0, L"File cant be read", 0, 0);
			//	break;
			//}
			vIndices.push_back(vertexIndex[0]);
			vIndices.push_back(vertexIndex[1]);
			vIndices.push_back(vertexIndex[2]);
			vtIndices.push_back(uvIndex[0]);
			vtIndices.push_back(uvIndex[1]);
			vtIndices.push_back(uvIndex[2]);
			vnIndices.push_back(normalIndex[0]);
			vnIndices.push_back(normalIndex[1]);
			vnIndices.push_back(normalIndex[2]);
			numberOfFaces++;
		}

		numberOfFaces *= 3;
		int cont = 0;
		for (int i = 0; i < numberOfFaces; i++) {

			UINT vertexIndex = vIndices[i];
			UINT normalIndex = vnIndices[i];
			UINT uvIndex = vtIndices[i];
			outputData temp;

			temp.pos = vertices[vertexIndex - 1];
			temp.uv = uvs[uvIndex - 1];
			temp.normal = normals[normalIndex - 1];

			output.push_back(temp);

			cont++;
		}
		for (int i = 0; i < numberOfFaces; i++) {
			vIndices[i]--;  //Starts from 1, i need it to start from 0
		}
		int toAdd = 0;
		for (int i = 0; i < numberOfFaces; i += 3) { //Needed to get the indices working for my draw function
			vIndices[i] += toAdd;
			vIndices[i + 1] += toAdd;
			vIndices[i + 2] += toAdd;
			toAdd += 2;
		}
		vertexIndicesFinal = vIndices;


	}
}