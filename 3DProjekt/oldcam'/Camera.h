#pragma once
#include "Header.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
	//DirectX::XMVECTOR vector = DirectX::XMVectorSet(1.0f, 1.0f, 1.f, 1.0f);
	//DirectX::XMFLOAT3
	//DirectX::XMMATRIX
private:
	//Camera Attributes
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 front;
	DirectX::XMFLOAT4 up;
	DirectX::XMFLOAT4 right;
	DirectX::XMFLOAT4 worldUp;

	//Euler angles
	float yaw;
	float pitch;

	//Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

	void _updateCameraVectors();

	std::unique_ptr<DirectX::Keyboard> keyboard = std::make_unique<DirectX::Keyboard>();
public:
	Camera(DirectX::XMFLOAT4 position = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4 up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH);
	~Camera();


	void processKeyboard(float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yOffset);

	DirectX::XMMATRIX getViewMatrix();
	DirectX::XMFLOAT4 getPosition() { return this->position; }
	float getZoom() { return this->zoom; }
	DirectX::XMFLOAT4 getUp() { return this->up; }
	DirectX::XMFLOAT4 getRight() { return this->right; }

	DirectX::XMVECTOR getLookAt();
	void setPosition(DirectX::XMFLOAT4 position);
	

};
