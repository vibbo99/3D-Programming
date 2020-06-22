#include "Camera.h"

void Camera::_updateViewMatrix()
{
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);
	camTarget = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, camRotationMatrix);
	

	right = XMVector3Normalize(XMVector3Cross(DEFAULT_UP_VECTOR, camTarget));
	XMVECTOR upDir = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, camRotationMatrix);
	this->viewMatrix = XMMatrixLookAtLH(this->posVector, camTarget + posVector, upDir);
}

Camera::Camera()
{
	this->position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->posVector = XMLoadFloat3(&this->position);
	this->rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->_updateViewMatrix();
}

void Camera::setProjectionValues(float fovDegrees, float aspectRation, float nearZ, float farZ)
{
	float fovRadians = (fovDegrees / 260.0f) * XM_2PI;
	this->projectionMatrix = XMMatrixPerspectiveFovLH(fovRadians, aspectRation, nearZ, farZ);
}

void Camera::processKeyboard(float deltaTime)
{
	float speed = 2.5f;
	auto kb = keyboard->GetState();

	speed *= deltaTime;
	if (kb.W)
	{
		
		this->adjustPosition(XMVector3Normalize(this->camTarget) * 0.05);
	}
	if (kb.S)
	{
		this->adjustPosition(-XMVector3Normalize(this->camTarget) * 0.05);
	}
	if (kb.A)
	{
		this->adjustPosition(this->right * -0.05);
	}
	if (kb.D)
	{
		this->adjustPosition(this->right * 0.05);
	}
	if (kb.Space)
	{
		this->adjustPosition(0.0f, 0.05f, 0.0f);
	}
	if (kb.LeftShift)
	{
		this->adjustPosition(0.0f, -0.05f, 0.0f);
	}
	if (kb.Q)
	{
		this->adjustRotation(0.0f, -0.05f, 0.0f);
	}
	if (kb.E)
	{
		this->adjustRotation(0.0f, 0.05f, 0.0f);
	}

}

void Camera::processMouse(float deltaTime)
{
	float speed = 2.5f;
	auto theMouse = mouse->GetState();
	
	

	speed *= deltaTime;
	//mouse->SetVisible(true);
	
	this-> mouseX = theMouse.x;
	this-> mouseY = theMouse.y;

	this->differenceX = this->mouseX - this->tempValueX;
	this->differenceY = this->mouseY - this->tempValueY;



	//if (mouseX > tempValueX)
	//{
	//	this->adjustRotation(0.0f, 0.005 * differenceX, 0.0f);
	//}

	//else if (mouseX < tempValueX)
	//{
	//	this->adjustRotation(0.0f, -0.005 * differenceX, 0.0f);
	//}

	//if (mouseY > tempValueY)
	//{
	//	this->adjustRotation(0.005 * differenceY, 0.0f, 0.0f);
	//}
	//else if (mouseY < tempValueY)
	//{
	//	this->adjustRotation(-0.005 * differenceY, 0.0f, 0.0f);
	//}
	


	
	tempValueX = this-> mouseX;
	tempValueY = this-> mouseY;

	
	//theMouse.y = 0;
	
	
	
}

void Camera::setupMouse(HWND hwnd)
{
	mouse->SetWindow(hwnd);
	//mouse->SetMode(Mouse::MODE_RELATIVE);

}

const XMMATRIX& Camera::getViewMatrix() const
{
	return this->viewMatrix;
}

const XMMATRIX& Camera::getProjectionMatrix() const
{
	return this->projectionMatrix;
}

const XMVECTOR& Camera::getPositionVector() const
{
	return this->posVector;
}

const XMFLOAT3& Camera::getPositionFloat3() const
{
	return this->position;
}

const XMVECTOR& Camera::getRotationVector() const
{
	return this->rotVector;
}

const XMFLOAT3& Camera::getRotationFloat3() const
{
	return this->rotation;
}

const XMFLOAT3& Camera::getCamTarget() const
{
	XMFLOAT3 returnValue;
	returnValue.x = XMVectorGetX(camTarget);
	returnValue.y = XMVectorGetY(camTarget);
	returnValue.z = XMVectorGetZ(camTarget);

	return returnValue;
}


void Camera::setPosition(const XMVECTOR& pos)
{
	XMStoreFloat3(&this->position, pos);
	this->posVector = pos;
	this->_updateViewMatrix();
}

void Camera::setPosition(float x, float y, float z)
{
	this->position = XMFLOAT3(x, y, z);
	this->posVector = XMLoadFloat3(&this->position);
	this->_updateViewMatrix();
}

void Camera::adjustPosition(const XMVECTOR& pos)
{
	this->posVector += pos;
	XMStoreFloat3(&this->position, this->posVector);
	this->_updateViewMatrix();
}

void Camera::adjustPosition(float x, float y, float z)
{
	this->position.x += x;
	this->position.y += y;
	this->position.z += z;
	this->posVector = XMLoadFloat3(&this->position);
	this->_updateViewMatrix();
}

void Camera::setRotation(const XMVECTOR& rot)
{
	this->rotVector = rot;
	XMStoreFloat3(&this->rotation, rot);
	this->_updateViewMatrix();
}

void Camera::setRotation(float x, float y, float z)
{
	this->rotation = XMFLOAT3(x, y, z);
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->_updateViewMatrix();
}

void Camera::adjustRotation(const XMVECTOR& rot)
{
	this->rotVector += rot;
	XMStoreFloat3(&this->rotation, this->rotVector);
	this->_updateViewMatrix();
}

void Camera::adjustRotation(float x, float y, float z)
{
	this->rotation.x += x;
	this->rotation.y += y;
	this->rotation.z += z;
	this->rotVector = XMLoadFloat3(&this->rotation);
	this->_updateViewMatrix();
}
