#include "HeightMap.h"

bool HeightMap::_HeightMapLoad(char* filename, HeightMapInfo& hmInfo)
{
	FILE* filePtr;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, index;
	unsigned char height;

	//Open file
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return 0;

	//Read bitmaps header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	//Read the info header
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	//Get height and width of image
	hmInfo.terrainWidth = bitmapInfoHeader.biWidth;
	hmInfo.terrainHeight = bitmapInfoHeader.biHeight;

	//Size of image in bytes
	imageSize = hmInfo.terrainWidth * hmInfo.terrainHeight * 3;

	//Initialize array
	unsigned char* bitmapImage = new unsigned char[imageSize];

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//Store image data
	fread(bitmapImage, 1, imageSize, filePtr);

	//close file
	fclose(filePtr);

	//Init array
	hmInfo.heightMap = new XMFLOAT3[hmInfo.terrainWidth * hmInfo.terrainHeight];

	int k = 0;

	float heightFactor = 10.0f;

	for (int j = 0; j < hmInfo.terrainHeight; j++)
	{
		for (int i = 0; i < hmInfo.terrainWidth; i++)
		{
			height = bitmapImage[k];
			index = (hmInfo.terrainHeight * j) + i;

			hmInfo.heightMap[index].x = (float)i;
			hmInfo.heightMap[index].y = (float)height / heightFactor;
			hmInfo.heightMap[index].z = (float)j;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

HeightMap::HeightMap(ID3D11Device* device_ptr, ID3D11DeviceContext* device_context, Camera* camera, XMFLOAT3 position)
{
	HeightMapInfo hmInfo;
	char path[126] = "heightmap01.bmp";
	_HeightMapLoad(path, hmInfo);

	int cols = hmInfo.terrainWidth;
	int rows = hmInfo.terrainHeight;

	//Create grid
	int NumVertices = rows * cols;
	int NumFace = (rows - 1) * (cols - 1) * 2;

	//std::vector<VertexData> v(NumVertices);
	v.size = NumVertices;


	for (DWORD i = 0; i < rows; ++i)
	{
		for (DWORD j = 0; j < cols; ++j)
		{
			v[i * cols + j].position = hmInfo.heightMap[i * cols + j];
			v[i * cols + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

		}
	}

	//std::vector<DWORD> indices(NumFace * 3);
	indices.size = NumFace * 3;

	int k = 0;
	int texUIndex = 0;
	int texVIndex = 0;
	for (DWORD i = 0; i < rows - 1; i++)
	{
		for (DWORD j = 0; j < cols - 1; j++)
		{
			indices[k] = i * cols + j;        // Bottom left of quad
			v[i * cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 1.0f);

			indices[k + 1] = i * cols + j + 1;        // Bottom right of quad
			v[i * cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 2] = (i + 1) * cols + j;    // Top left of quad
			v[(i + 1) * cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);


			indices[k + 3] = (i + 1) * cols + j;    // Top left of quad
			v[(i + 1) * cols + j].texCoord = XMFLOAT2(texUIndex + 0.0f, texVIndex + 0.0f);

			indices[k + 4] = i * cols + j + 1;        // Bottom right of quad
			v[i * cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 1.0f);

			indices[k + 5] = (i + 1) * cols + j + 1;    // Top right of quad
			v[(i + 1) * cols + j + 1].texCoord = XMFLOAT2(texUIndex + 1.0f, texVIndex + 0.0f);

			k += 6; // next quad

			texUIndex++;
		}
		texUIndex = 0;
		texVIndex++;
	}
}
