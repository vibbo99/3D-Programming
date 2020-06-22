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
	XMFLOAT3 world;
	XMFLOAT3 mouseRay;

	//Denna kan bara finnas på ett ställe. Så ska kameran kunna flyttas med hjälp av denna måste man hämta den härifrån.
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();
	
	
public:

	mousePicking();
	mousePicking(Camera *aCamera);
	void processMouse();

	void update(Camera aCamera);

	XMFLOAT4 toEye(XMFLOAT4 aClip);
	XMFLOAT3 toWorld(XMFLOAT4 anEye);

	XMFLOAT3 getWorld();

	XMFLOAT2 normalizeMouse(float x, float y);

};