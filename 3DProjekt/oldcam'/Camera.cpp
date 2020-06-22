#include "Camera.h"

void Camera::_updateCameraVectors()
{

	



}

Camera::Camera(DirectX::XMFLOAT4 position, DirectX::XMFLOAT4 up, float yaw, float pitch)
{
	this->position = position;
	this->worldUp = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	this->up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	this->right = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	this->yaw = yaw;
	this->pitch = pitch;

	this->front = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	this->movementSpeed = SPEED;
	this->mouseSensitivity = SENSITIVITY;
	this->zoom = ZOOM;
	//this->_updateCameraVectors();

}

Camera::~Camera()
{
}

void Camera::processKeyboard(float deltaTime)
{
	
	using namespace DirectX;
	float velocity = movementSpeed * deltaTime;

	auto kb = keyboard->GetState();
	if (kb.W)
	{
		XMStoreFloat4(&this->position, XMLoadFloat4(&this->position) + XMLoadFloat4(&this->front) * velocity);
	}
	if (kb.S)
	{
		XMStoreFloat4(&this->position, XMLoadFloat4(&this->position) - XMLoadFloat4(&this->front) * velocity);
	}
	if (kb.A)
	{
		XMStoreFloat4(&this->position, XMLoadFloat4(&this->position) - XMLoadFloat4(&this->right) * velocity);
	}
	if (kb.D)
	{
		XMStoreFloat4(&this->position, XMLoadFloat4(&this->position) + XMLoadFloat4(&this->right) * velocity);

	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	this->_updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
	{
		zoom -= yOffset;
	}
	if (zoom <= 1.0f)
	{
		zoom = 1.0f;
	}
	if (zoom >= 45.0f)
	{
		zoom = 45.0f;
	}
}

DirectX::XMMATRIX Camera::getViewMatrix()
{
	using namespace DirectX;
	XMVECTOR eye = XMVectorSet(this->position.x, this->position.y, this->position.z, 0.0f);
	XMVECTOR lookAt = getLookAt();
	XMVECTOR up = XMVectorSet(this->up.x, this->up.y, this->up.z, 0.0f);
	DirectX::XMMATRIX mat = DirectX::XMMatrixLookAtLH(eye, lookAt, up);
	return mat;
}

DirectX::XMVECTOR Camera::getLookAt()
{
	using namespace DirectX;
	DirectX::XMMATRIX mat = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
	DirectX::XMVECTOR temp = DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4(&this->front), mat);

	temp += DirectX::XMLoadFloat4(&this->position);
	return temp;
	
}

void Camera::setPosition(DirectX::XMFLOAT4 position)
{
	this->position = position;
}


