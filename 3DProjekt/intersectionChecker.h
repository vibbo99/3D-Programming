#pragma once
#include "Header.h"
#include <string>


using namespace DirectX;

class intersectionChecker
{


private:


	int lowestValue;

	float planeDistance = 0.0f;

	XMVECTOR rayDirection;

	float dotCalc(XMVECTOR v1, XMVECTOR v2);


public:

	intersectionChecker();


	float collisionCheck(int index, XMVECTOR rayPosition, float& distance, XMVECTOR rayDirection, XMVECTOR normals[], std::string shape, XMVECTOR center[], float radius[]);

	





};