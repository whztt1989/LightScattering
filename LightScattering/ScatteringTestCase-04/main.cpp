#include <iostream>
#include <vector>
#include <Eigen\dense>
#include "OutdoorLightScattering.h"

COutdoorLightScattering OutdoorLightScattering;
vec3f EarthCentre(0.0, 0.0, 0.0);
vec3f RayStart[2], RayEnd[2];
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
void testSameHeight()
{
	setPostion(0.0, 6360000, 0.0, RayStart[0]);
	setPostion(0.0, 6370000, 0.0, RayEnd[0]);

	setPostion(6360000, 0.0, 0.0, RayStart[1]);
	setPostion(6370000, 0.0, 0.0, RayEnd[1]);

	vec2f NetParticleFromCam[2];
	vec3f RayleighInsc[2];
	vec3f MieInsc[2];
	vec3f InscatteringIntegralResult[2];

	OutdoorLightScattering.computeInscatteringIntegral(RayStart[0], RayEnd[0], EarthCentre, LightDir, NetParticleFromCam[0], RayleighInsc[0], MieInsc[0], 7);
	InscatteringIntegralResult[0] = RayleighInsc[0] + MieInsc[0];

	OutdoorLightScattering.computeInscatteringIntegral(RayStart[1], RayEnd[1], EarthCentre, LightDir, NetParticleFromCam[1], RayleighInsc[1], MieInsc[1], 7);
	InscatteringIntegralResult[1] = RayleighInsc[1] + MieInsc[1];

	if (InscatteringIntegralResult[0][0] == InscatteringIntegralResult[1][0] && InscatteringIntegralResult[0][1] == InscatteringIntegralResult[1][1] && InscatteringIntegralResult[0][2] == InscatteringIntegralResult[1][2])
		std::cout << "Compute same height sample(North, East)...Test passed" << std::endl;
	else
		std::cout << "Compute same height sample(North, East)...Test not passed" << std::endl;
}
int main()
{
	testSameHeight();
	system("pause");
	return 1;
}