#pragma once
#include "Header.h"
using namespace DirectX;
class ObjLoader
{

public:
	ObjLoader();
	~ObjLoader();

	void LoadObj(char path[],
		std::vector<XMFLOAT3> &vertices,
		std::vector<XMFLOAT2> &uvs,
		std::vector<XMFLOAT3> &normals,
		char mtlPath[]);
	void LoadMtl(char path[], XMFLOAT4 &Kd, XMFLOAT4 &Ka, XMFLOAT4 &Ks, char texturePath[]);
	void LoadTexture(LPCWSTR path, ID3D11Device* device_ptr, ID3D11Resource** texture,
		ID3D11ShaderResourceView** shader_resource_view_ptr);
	//void LoadDDSTexture(ID3D11Device* device_ptr, ID3D11ShaderResourceView* smrv);

	
};
