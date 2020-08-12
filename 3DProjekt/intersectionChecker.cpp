#include "intersectionChecker.h"

intersectionChecker::intersectionChecker()
{
}

float intersectionChecker::dotCalc(XMVECTOR v1, XMVECTOR v2)
{
	float dotCalc = v1.m128_f32[0] * v2.m128_f32[0] + v1.m128_f32[1] * v2.m128_f32[1] + v1.m128_f32[2] * v2.m128_f32[2];

	return dotCalc;
}


bool intersectionChecker::cubeCheck(float cubeWidth, XMFLOAT3 cubePosition, XMVECTOR rayOrigin, XMVECTOR rayDirection)
{
	float t = -1;
	float tmin = -99999999.f;
	float tmax = 99999999.f;
	XMFLOAT3 rayO;
	XMStoreFloat3(&rayO, rayOrigin);
	XMFLOAT3 rayD;
	XMStoreFloat3(&rayD, rayDirection);

	XMFLOAT3 p = XMFLOAT3(cubePosition.x - rayO.x, cubePosition.y - rayO.y, cubePosition.z - rayO.z);
	XMFLOAT3 allSlabs[] = { XMFLOAT3(1.f, 0.f, 0.f), XMFLOAT3(0.f, 1.f, 0.f), XMFLOAT3(0.f, 0.f, 1.f)};
	float allSlabsH[] = { cubeWidth, cubeWidth, cubeWidth };

	for (int i = 0; i < 3; i++)
	{
		float e = allSlabs[i].x * p.x + allSlabs[i].y * p.y + allSlabs[i].z * p.z;
		float f = allSlabs[i].x * rayD.x + allSlabs[i].y * rayD.y + allSlabs[i].z * rayD.z;

		if (abs(f) > 0.00001)
		{
			float t1 = (e + allSlabsH[i]) / f;
			float t2 = (e - allSlabsH[i]) / f;

			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			if (t1 > tmin)
				tmin = t1;

			if (t2 < tmax)
				tmax = t2;

			if (tmin > tmax)
				return false;

			if (tmax < 0)
				return false;
		}
		else if (-e - allSlabsH[i] > 0 || -e + allSlabsH[i] < 0)
			return false;

		if (tmin > 0)
			t = tmin;
		else
			t = tmax;
	}
	return t;
}

float intersectionChecker::collisionCheck(int index, XMVECTOR rayPosition, float& distance, XMVECTOR rayDirection, XMVECTOR normals[], std::string shape, XMVECTOR center[], float radius[])
{
	
	if (shape == "Plane" || shape == "plane")
	{			
		//dp är distance i z
		float theDistance = 2.f;

		float dotCalculation = dotCalc(normals[index], rayPosition);

		float firstPartOfEquation = -theDistance - dotCalculation;
				
		float secondPartOfEquation = dotCalc(normals[index], rayDirection);

		float t = firstPartOfEquation / secondPartOfEquation;

		if (secondPartOfEquation > 0 || secondPartOfEquation < 0)
		{
			return t;
			distance = t;
		}
		else
		{
			return -1;
		}
			//vec3 connectingAt = theRay.origin() + t * theRay.direction();
	}
	

	else if (shape == "Sphere" || shape == "sphere")
	{
		float t = -1;

		XMVECTOR sphereOrigin = rayPosition - center[index];
		float b = dotCalc(rayDirection, sphereOrigin);
		float c = dotCalc(sphereOrigin, sphereOrigin) - pow(radius[index], 2);
		
			if (pow(b, 2) - c > 0)
		{
			float t1 = -b + sqrt(pow(b, 2) - c);
			float t2 = -b - sqrt(pow(b, 2) - c);
			t = t1;
			if (t > t2)
			{
				t = t2;
			}
		}

		distance = t;

		return t;

	}

	//else if (shape == "Cube" || shape == "cube")
	//{
	//	return 0;		
	//}
}
