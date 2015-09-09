#include <iostream>
#include <vector>
#include <Eigen\dense>
#include "OutdoorLightScattering.h"

COutdoorLightScattering OutdoorLightScattering;
vec3f EarthCentre(0.0, 0.0, 0.0);
vec3f RayStart, RayEnd;
vec3f LightDir = vec3f(-1.0, -1.0, 0.0);

//******************************************************************
//FUNCTION:
void setPostion(float vX, float vY, float vZ, vec3f& vPoint)
{
	vPoint[0] = vX;
	vPoint[1] = vY;
	vPoint[2] = vZ;
}

//******************************************************************
//FUNCTION:
void testInscatteringRGB()
{
	setPostion(0.0, 6360000, 0.0, RayStart);
	setPostion(5000.0, 6368000, 0.0, RayEnd);

	vec2f NetParticleFromCam;
	vec3f RayleighInsc, MieInsc;
	vec3f InscatteringIntegralResult;

	OutdoorLightScattering.computeInscatteringIntegral(RayStart, RayEnd, EarthCentre, LightDir, NetParticleFromCam, RayleighInsc, MieInsc, 7);
	InscatteringIntegralResult = RayleighInsc + MieInsc;

	if (InscatteringIntegralResult[0] < InscatteringIntegralResult[1] && InscatteringIntegralResult[1] < InscatteringIntegralResult[2])
		std::cout << "Compute in scattering rgb...r < g < b, Test passed" << std::endl;
	else
		std::cout << "Compute in scattering rgb...not satisfy r < g < b, Test not passed" << std::endl;
}

int main()
{
	testInscatteringRGB();
	system("pause");
	return 1;
}