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

	XMVECTOR world;
	XMVECTOR rayPos;

	XMMATRIX viewInverse;

	float aspectRatio;

	XMVECTOR mouseRay;

	Camera* aCamera;

	//Denna kan bara finnas på ett ställe. Så ska kameran kunna flyttas med hjälp av denna måste man hämta den härifrån.
	
	
	
public:

	mousePicking();
	mousePicking(Camera *aCamera);
	void processMouse(float x, float y);

	void update(Camera *aCamera, float x, float y);

	void mouseToCoords(float x, float y);

	//Getters

	XMVECTOR getRayDir();
	XMVECTOR getRayPos();

	XMFLOAT3 getRayDirFloat3();
	XMFLOAT3 getRayPosFloat3();

	float getMouseX();
	float getMouseY();

};