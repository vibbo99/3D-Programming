#include "mousePicking.h"

mousePicking::mousePicking()
{
}

mousePicking::mousePicking(Camera *aCamera)
{	
	this->proj = aCamera->getProjectionMatrix();
	this->view = aCamera->getViewMatrix();
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

		//convert to clip coords
		this->clip.x = mouseNormalized.x;
		this->clip.y = mouseNormalized.y;
		this->clip.z = -1.f;
		this->clip.w = 1.f;

		//convert to eye coords
		this->eye = toEye(clip);

		//convert to world coords
		this->world = toWorld(eye);
}

void mousePicking::update(Camera aCamera)
{
	this->view = aCamera.getViewMatrix();
	processMouse();
}

XMFLOAT4 mousePicking::toEye(XMFLOAT4 aClip)
{
	XMMATRIX inversProj = XMMatrixInverse(NULL, proj);

	XMMATRIX newClip;

	newClip.r->m128_f32[0] = clip.x;
	newClip.r->m128_f32[1] = clip.y;
	newClip.r->m128_f32[2] = clip.z;
	newClip.r->m128_f32[3] = clip.w;

	XMMATRIX eye = inversProj * newClip;
	
	XMFLOAT4 returnValue;

	returnValue.x = eye.r->m128_f32[0]; //x
	returnValue.y = eye.r->m128_f32[1]; //y
	returnValue.z = -1.0f;
	returnValue.w = 0.0f;

	return returnValue;
}

XMFLOAT3 mousePicking::toWorld(XMFLOAT4 anEye)
{
	XMMATRIX inverseView = XMMatrixInverse(NULL, view);

	XMMATRIX newEye;

	newEye.r->m128_f32[0] = eye.x;
	newEye.r->m128_f32[1] = eye.y;
	newEye.r->m128_f32[2] = eye.z;
	newEye.r->m128_f32[3] = eye.w;

	XMMATRIX ray = inverseView * newEye;

	this->mouseRay.x = ray.r->m128_f32[0];
	this->mouseRay.y = ray.r->m128_f32[1];
	this->mouseRay.z = ray.r->m128_f32[2];

	
	return this->mouseRay;
}


XMFLOAT3 mousePicking::getWorld()
{
	return this->world;
}

XMFLOAT2 mousePicking::normalizeMouse(float x, float y)
{
	// -1 i x, + 1 i y. P.g.a directX kordinat-system.
	return XMFLOAT2((x*2.f) / WIDTH - 1.f, (y*2.f) / HEIGHT + 1.f);
}
