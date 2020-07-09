#include "mousePicking.h"

mousePicking::mousePicking()
{
}

mousePicking::mousePicking(Camera *aCamera)
{	
	this->aCamera = aCamera;
	this->proj = this->aCamera->getProjectionMatrix();
	this->view = this->aCamera->getViewMatrix();

	this->aspectRatio = WIDTH / HEIGHT;
}

void mousePicking::processMouse()
{
		auto theMouse = mouse->GetState();
		mouse->SetVisible(true);
	
		//SetCursorPos(700, 430);
		//mouse->SetVisible(true);
		this->mouseX = theMouse.x;
		this->mouseY = theMouse.y;

		auto leftClickCheck = mouse->GetState().leftButton;
		
		//if (leftClickCheck == pressed)
		//{
			mouseToCoords(mouseX, mouseY);
		//}
		
		 
}

void mousePicking::update(Camera* aCamera)
{
	this->view = this->aCamera->getViewMatrix();
	
	processMouse();
}


void mousePicking::mouseToCoords(float x, float y)
{
	XMVECTOR viewSpace;

	float projX = 1 / (aspectRatio * tan(this->aCamera->getFieldOfView() / 2));
	float projY = 1 / tan(this->aCamera->getFieldOfView() / 2);

	viewSpace.m128_f32[0] = (x * 2.f) / WIDTH - 1 / projX;
	viewSpace.m128_f32[1] = -(y * 2.f) / HEIGHT + 1 / projY;
	viewSpace.m128_f32[2] = 1.f;

	XMVECTOR pos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR dir = XMVectorSet(viewSpace.m128_f32[0], viewSpace.m128_f32[1], viewSpace.m128_f32[2], 1.0f);

	//To world space

	
	this->viewInverse = XMMatrixInverse(&XMMatrixDeterminant(this->view), this->view);

	pos = XMVector3TransformCoord(pos, viewInverse);
	dir = XMVector3TransformNormal(dir, viewInverse);

	this->world = dir;
	this->rayPos = pos;
}

XMVECTOR mousePicking::getRayDir()
{
	return this->world;
}

XMVECTOR mousePicking::getRayPos()
{
	return this->rayPos;
}

XMFLOAT3 mousePicking::getRayDirFloat3()
{
	XMFLOAT3 direction;
	direction.x = this->world.m128_f32[0];
	direction.y = this->world.m128_f32[1];
	direction.z = this->world.m128_f32[2];

	return direction;
}

XMFLOAT3 mousePicking::getRayPosFloat3()
{
	XMFLOAT3 position;
	position.x = this->rayPos.m128_f32[0];
	position.y = this->rayPos.m128_f32[1];
	position.z = this->rayPos.m128_f32[2];

	return position;
}

float mousePicking::getMouseX()
{
	return this->mouseX;
}

float mousePicking::getMouseY()
{
	return this->mouseY;
}

