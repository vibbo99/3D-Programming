#pragma once
#include "Header.h"
using namespace DirectX;

class Camera
{
private:

	XMVECTOR posVector;
	XMVECTOR rotVector;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	XMVECTOR camTarget;
	XMVECTOR right;

	int mouseX = 0;
	int mouseY = 0;
	int tempValueX = 0;
	int tempValueY = 0;
	int differenceX = 0;
	int differenceY = 0;

	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	void _updateViewMatrix();
	std::unique_ptr<DirectX::Keyboard> keyboard = std::make_unique<DirectX::Keyboard>();
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();
public:
	Camera();
	void setProjectionValues(float fovDegrees, float aspectRation, float nearZ, float farZ);
	void processKeyboard(float deltaTime);
	void processMouse(float deltaTime);
	void setupMouse(HWND hwnd);

	//Getters
	const XMMATRIX& getViewMatrix() const;
	const XMMATRIX& getProjectionMatrix() const;

	const XMVECTOR& getPositionVector() const;
	const XMFLOAT3& getPositionFloat3() const;
	const XMVECTOR& getRotationVector() const;
	const XMFLOAT3& getRotationFloat3() const;

	//Setters
	void setPosition(const XMVECTOR& pos);
	void setPosition(float x, float y, float z);
	void adjustPosition(const XMVECTOR& pos);
	void adjustPosition(float x, float y, float z);
	void setRotation(const XMVECTOR& rot);
	void setRotation(float x, float y, float z);
	void adjustRotation(const XMVECTOR& rot);
	void adjustRotation(float x, float y, float z);


	

};
