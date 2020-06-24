#include "intersectionChecker.h"

intersectionChecker::intersectionChecker()
{
}

float intersectionChecker::dotCalc(XMVECTOR v1, XMVECTOR v2)
{
	float dotCalc = v1.m128_f32[0] * v2.m128_f32[0] + v1.m128_f32[1] * v2.m128_f32[1] + v1.m128_f32[2] * v2.m128_f32[2];

	return dotCalc;
}


float intersectionChecker::collisionCheck(int index, int nrOfShapes, XMVECTOR rayPosition, float& distance, XMVECTOR rayDirection, XMVECTOR positions[], XMVECTOR normals[], std::string shape)
{
	
	if (shape == "Plane" || shape == "plane")
	{
			
		//dp är distance i z
		float theDistance = -100.0f;
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
			
	}

	else if (shape == "Cube" || shape == "cube")
	{
			
		
	}
	return 0;
}
