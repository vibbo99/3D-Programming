#pragma once
#include "Header.h"
#include "Shader.h"
#include "Camera.h"
using namespace DirectX;

class HeightMap
{
private:
	XMFLOAT3 position;
	XMVECTOR posVec;

	Shader* shader;
	Camera* camera;
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;


	std::vector<VertexData> v;
	std::vector<DWORD> indices;

	struct HeightMapInfo {
		int terrainWidth;
		int terrainHeight;
		XMFLOAT3* heightMap;
	};

	bool _HeightMapLoad(char* filename, HeightMapInfo& hmInfo);
public:
	HeightMap(ID3D11Device* device_ptr, ID3D11DeviceContext* device_context, Camera* camera, XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f));

	void draw();
};
