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
		//float speed = 2.5f;
		auto theMouse = mouse->GetState();

		//speed *= deltaTime;
		//mouse->SetVisible(true);

		this->mouseX = theMouse.x;
		this->mouseY = theMouse.y;


		mouseToCoords(mouseX, mouseY);
		//mouseNormalized = normalizeMouse(this->mouseX, this->mouseY);

		////convert to clip coords
		//this->clip.x = mouseNormalized.x;
		//this->clip.y = mouseNormalized.y;
		//this->clip.z = -1.f;
		//this->clip.w = 1.f;

		////convert to eye coords
		//this->eye = toEye(clip);

		////convert to world coords
		//this->world = toWorld(eye);
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
	viewSpace.m128_f32[2] = -1.f;

	XMVECTOR pos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	XMVECTOR dir = XMVectorSet(viewSpace.m128_f32[0], viewSpace.m128_f32[1], viewSpace.m128_f32[2], 1.0f);


	this->rayPos = pos;

	//To world space

	this->worldInverse = XMMatrixInverse(NULL, this->view);

	XMVector3TransformCoord(viewSpace, worldInverse);
	XMVector3TransformNormal(dir, worldInverse);

	//this->world = XMVectorSet(worldInverse.r->m128_f32[0], worldInverse.r->m128_f32[1], worldInverse.r->m128_f32[2], 1.0f);
	this->world = dir;
}

XMVECTOR mousePicking::getRayDir()
{
	return this->world;
}

XMVECTOR mousePicking::getRayPos()
{
	return this->rayPos;
}

//XMFLOAT4 mousePicking::toEye(XMFLOAT4 aClip)
//{
//	XMMATRIX inversProj = XMMatrixInverse(NULL, proj);
//
//	//Förstår inte hur jag ska göra transform med matrix och en XMFLOAT4..
//	XMMATRIX eye = inversProj * clip;
//
//	
//	XMFLOAT4 returnValue;
//
//	returnValue.x = eye.r->m128_f32[0]; //x
//	returnValue.y = eye.r->m128_f32[1]; //y
//	returnValue.z = -1.0f;
//	returnValue.w = 0.0f;
//
//	return returnValue;
//}
//
//XMVECTOR mousePicking::toWorld(XMFLOAT4 anEye)
//{
//	XMMATRIX inverseView = XMMatrixInverse(NULL, view);
//
//	XMMATRIX newEye;
//
//	newEye.r->m128_f32[0] = eye.x;
//	newEye.r->m128_f32[1] = eye.y;
//	newEye.r->m128_f32[2] = eye.z;
//	newEye.r->m128_f32[3] = eye.w;
//
//	XMMATRIX ray = inverseView * newEye;
//
//	mouseRay.m128_f32[0] = ray.r->m128_f32[0];
//	mouseRay.m128_f32[1] = ray.r->m128_f32[1];
//	mouseRay.m128_f32[2] = ray.r->m128_f32[2];
//
//	XMVector3Normalize(mouseRay);
//	
//	return this->mouseRay;
//}
//
//
//XMVECTOR mousePicking::getWorld()
//{
//	return this->world;
//}
//
//XMFLOAT2 mousePicking::normalizeMouse(float x, float y)
//{
//	// -1 i x, + 1 i y. P.g.a directX kordinat-system.
//	return XMFLOAT2((x*2.f) / WIDTH - 1, -((y*2.f) / HEIGHT + 1));
//}
