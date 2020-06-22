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

	
	
	std::unique_ptr<DirectX::Mouse> mouse = std::make_unique<DirectX::Mouse>();
	
	
public:

	mousePicking(Camera aCamera);
	void processMouse();

	void update(Camera aCamera);

	XMFLOAT2 normalizeMouse(float x, float y);



};