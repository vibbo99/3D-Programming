#include "intersectionChecker.h"

intersectionChecker::intersectionChecker()
{
}

float intersectionChecker::dotCalc(XMVECTOR v1, XMVECTOR v2)
{
	float dotCalc = v1.m128_f32[0] * v2.m128_f32[0] + v1.m128_f32[1] * v2.m128_f32[1] + v1.m128_f32[2] * v2.m128_f32[2];

	return dotCalc;
}


float intersectionChecker::collisionCheck(int index, XMVECTOR rayPosition, float& distance, XMVECTOR rayDirection, XMVECTOR normals[], std::string shape, XMVECTOR center[], float radius[])
{
	
	if (shape == "Plane" || shape == "plane")
	{			
		//dp är distance i z
		float theDistance = -1.f;
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

	else if (shape == "Cube" || shape == "cube")
	{
		return 0;		
	}
}
