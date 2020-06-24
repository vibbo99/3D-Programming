#pragma once
#include "Header.h"
#include "Camera.h"


using namespace DirectX;

class mousePicking
{

private:

	int mouseX;
	int mouseY;

	XMMATRIX proj;
	XMMATRIX view;

	XMFLOAT2 mouseNormalized;
	XMFLOAT4 clip;
	XMFLOAT4 eye;

	XMVECTOR world;
	XMVECTOR rayPos;

	XMMATRIX worldInverse;

	float aspectRatio;

	XMVECTOR mouseRay;

	Camera* aCamera;

	//Denna kan bara finnas p� ett st�lle. S� ska kameran kunna flyttas med hj�lp av denna m�ste man h�mta den h�rifr�n.
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();
	
	
public:

	mousePicking();
	mousePicking(Camera *aCamera);
	void processMouse();

	void update(Camera *aCamera);

	void mouseToCoords(float x, float y);


	XMVECTOR getWorldInverse();
	XMVECTOR getRayPos();

	//XMFLOAT4 toEye(XMFLOAT4 aClip);
	//XMVECTOR toWorld(XMFLOAT4 anEye);

	//XMVECTOR getWorld();

	//XMFLOAT2 normalizeMouse(float x, float y);

};