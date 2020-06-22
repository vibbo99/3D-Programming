#include "Point_Light.h"

void Point_Light::_setBufferDesc(ID3D11Device* device_ptr)
{

	Point_Light_Struct lStruct = this->getAsStruct();
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(Point_Light_Struct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC; //D3D11_USAGE_DEFAULT
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &lStruct;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = device_ptr->CreateBuffer(&cbDesc, &InitData, &light_constant_buffer_ptr);

	if (FAILED(hr))
		assert(FAILED(hr));
}

Point_Light::Point_Light(XMFLOAT3 pos, ID3D11Device* device_ptr, ID3D11DeviceContext* device_context_ptr, Camera* camera)
{
	this->position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
	this->vecPos = XMLoadFloat4(&this->position);
	this->ambient = BASE_AMBIENT;
	this->vecAmbient = XMLoadFloat4(&this->ambient);
	this->diffuse = BASE_DIFFUSE;
	this->vecDiffuse = XMLoadFloat4(&this->diffuse);
	this->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->vecSpecular = XMLoadFloat4(&this->specular);
	this->attenuation = BASE_ATT;
	this->vecAttenuation = XMLoadFloat4(&this->attenuation);

	this->_setBufferDesc(device_ptr);

	this->sphere = new Sphere(pos.x, pos.y, pos.z, LPCWSTR(L"noTex"), camera, device_ptr, device_context_ptr);
}

Point_Light::Point_Light(XMFLOAT3 pos, XMFLOAT3 color, XMFLOAT3 att, float range, ID3D11Device* device_ptr,
	ID3D11DeviceContext* device_context_ptr, Camera* camera)
{
	this->position = XMFLOAT4(pos.x, pos.y, pos.z, 1.0f);
	this->vecPos = XMLoadFloat4(&this->position);
	this->ambient = BASE_AMBIENT;
	this->vecAmbient = XMLoadFloat4(&this->ambient);
	this->diffuse = XMFLOAT4(color.x, color.y, color.z, 1.0f);
	this->vecDiffuse = XMLoadFloat4(&this->diffuse);
	this->specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->vecSpecular = XMLoadFloat4(&this->specular);
	this->attenuation = XMFLOAT4(att.x, att.y, att.z, 1.0f);
	this->vecAttenuation = XMLoadFloat4(&this->attenuation);

	this->_setBufferDesc(device_ptr);

	this->sphere = new Sphere(pos.x, pos.y, pos.z, LPCWSTR(L"noTex"), camera, device_ptr, device_context_ptr);

}

Point_Light_Struct Point_Light::getAsStruct()
{
	Point_Light_Struct returnStruct;
	returnStruct.pos = this->position;
	returnStruct.ambient = this->ambient;
	returnStruct.diffuse = this->diffuse;
	returnStruct.specular = this->specular;
	returnStruct.attenuation = this->attenuation;

	return returnStruct;
}

void Point_Light::setPosition(XMFLOAT4 position)
{
	this->position = position;
	this->vecPos = XMLoadFloat4(&this->position);

	this->sphere->setPosition(position.x, position.y, position.z);

}

void Point_Light::setPosition(float x, float y, float z)
{
	this->position = XMFLOAT4(x, y, z, 1.0f);
	this->vecPos = XMLoadFloat4(&this->position);
	this->sphere->setPosition(x, y, z);
}

void Point_Light::setScale(float x, float y, float z)
{
	this->sphere->scaleSize(x, y, z);
}

void Point_Light::setDiffuse(XMFLOAT4 color)
{
	this->diffuse = color;
	this->vecDiffuse = XMLoadFloat4(&this->diffuse);
}

void Point_Light::setAttenuation(float x, float y, float z)
{
	this->BASE_ATT = XMFLOAT4(x, y, z, 1);
}

void Point_Light::draw()
{
	this->sphere->draw();
}
