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



public:

	intersectionChecker();



	float collisionCheck(int index, int nrOfShapes, XMVECTOR rayPosition, float& distance, XMVECTOR rayDirection, XMVECTOR positions[], XMVECTOR normals[], std::string shape);

	float dotCalc(XMVECTOR v1, XMVECTOR v2);





};