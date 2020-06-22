#pragma once
#include "Header.h"
#include "Shader.h"
#include "Camera.h"
using namespace DirectX;

class Sphere
{
private:
	XMFLOAT3 position;
	XMVECTOR posVec;
	std::vector<XMFLOAT3> vertices;
	std::vector<XMFLOAT2> texCoords;
	std::vector<XMFLOAT3> normals;

	ID3D11Resource* texture = NULL;
	ID3D11Buffer* vertex_buffer_ptr = NULL;

	Shader* shader;

	Camera* camera;
	ID3D11Device* device_ptr;
	ID3D11DeviceContext* device_context_ptr;

	UINT vertex_stride = 0;
	UINT vertex_offset = 0;
	UINT vertex_count = 0;
public:
	Sphere(float x, float y, float z, LPCWSTR texturePath, Camera* camera,
		ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr);

	//Setters
	void setPosition(float x, float y, float z);
	void scaleSize(float x, float y, float z);
	Shader* getShader() { return this->shader; }

	void draw(ID3D11Buffer* light_constant_buffer = NULL, ID3D11Buffer* lightNr_constant_buffer = NULL,
		ID3D11Buffer* cam_pos_buffer = NULL);

};



