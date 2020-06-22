#include "mousePicking.h"

mousePicking::mousePicking(Camera aCamera)
{	
	this->proj = aCamera.getProjectionMatrix();
	this->view = aCamera.getViewMatrix();
}

void mousePicking::processMouse()
{
		//float speed = 2.5f;
		auto theMouse = mouse->GetState();

		//speed *= deltaTime;
		//mouse->SetVisible(true);

		this->mouseX = theMouse.x;
		this->mouseY = theMouse.y;

		mouseNormalized = normalizeMouse(this->mouseX, this->mouseY);
}

void mousePicking::update(Camera aCamera)
{
	this->view = aCamera.getViewMatrix();
	processMouse();
}

XMFLOAT2 mousePicking::normalizeMouse(float x, float y)
{
	// -1 i x, + 1 i y. P.g.a directX kordinat-system.
	return XMFLOAT2((x*2.f) / WIDTH - 1.f, (y*2.f) / HEIGHT + 1.f);
}
