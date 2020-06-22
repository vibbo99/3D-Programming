#pragma once
#include "Header.h"
#include "Sphere.h"
using namespace DirectX;

class Point_Light
{
private:

	XMVECTOR vecPos;
	XMFLOAT4 position;
	XMVECTOR vecAmbient;
	XMFLOAT4 ambient;
	XMVECTOR vecDiffuse;
	XMFLOAT4 diffuse;
	XMVECTOR vecSpecular;
	XMFLOAT4 specular;
	XMVECTOR vecAttenuation;
	XMFLOAT4 attenuation;

	ID3D11Buffer* light_constant_buffer_ptr = NULL;
	Sphere* sphere;

	const XMFLOAT4 BASE_AMBIENT = { 0.1f, 0.1f, 0.1f, 1.0f };
	const XMFLOAT4 BASE_DIFFUSE = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 BASE_ATT		= { 1.0f, 0.07f, 0.017f, 1.0f };

	void _setBufferDesc(ID3D11Device* device_ptr);
public:
	Point_Light(XMFLOAT3 pos, ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr, Camera* camera);
	Point_Light(XMFLOAT3 pos, XMFLOAT3 color, XMFLOAT3 att, float range,
		ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr,
		Camera* camera);

	//Getters
	XMFLOAT4 getPosition() { return this->position; }
	XMFLOAT4 getColor(){ return this->diffuse; }
	Point_Light_Struct getAsStruct();

	ID3D11Buffer* getBufferPtr() { return this->light_constant_buffer_ptr; }

	//Setters
	void setPosition(XMFLOAT4 position);
	void setPosition(float x, float y, float z);
	void setScale(float x, float y, float z);
	void setDiffuse(XMFLOAT4 color);
	void setAttenuation(float x, float y, float z);

	void draw();
};
