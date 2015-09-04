#include <iostream>
#include <vector>
#include <Eigen/dense>
#include "OutdoorLightScattering.h"

SPoint CameraPosition[2], CameraLookat[2], LightPosition[2];
COutdoorLightScattering OutdoorLightScattering;
vec3f EarthCentre(0.0, 0.0, 0.0);

vec3f RayStart = vec3f(0, 6360000.0, 0.0);
vec3f RayEnd(0, 6400000.0, 0.0);

vec3f LightDir(1.0, 0.0, 0.0);

//******************************************************************
//FUNCTION:
void setPostion(float vX, float vY, float vZ, SPoint& vPoint)
{
	vPoint.m_X = vX;
	vPoint.m_Y = vY;
	vPoint.m_Z = vZ;
}

//******************************************************************
//FUNCTION:
void testDifferentCameraDirection()
{
// 	setPostion(0.0, 0.0, 5.0, CameraPosition[0]);
// 	setPostion(0.0, 0.0, 0.0, CameraLookat[0]);
// 	setPostion(0.0, 5.0, 0.0, LightPosition[0]);
// 
// 	setPostion(0.0, 0.0, 5.0, CameraPosition[1]);
// 	setPostion(0.0, -1.0, 0.0, CameraLookat[1]);
// 	setPostion(0.0, 5.0, 0.0, LightPosition[1]);
// 
// 	if ((CameraPosition[0], CameraLookat[0], LightPosition[0]) > computeSample(CameraPosition[1], CameraLookat[1], LightPosition[1]))
// 		std::cout << "Compute DifferentCameraDirection sample...Test passed" << std::endl;
// 	else
// 		std::cout << "Compute DifferentCameraDirection sample...Test not passed" << std::endl;
}

//******************************************************************
//FUNCTION:
void testSameCameraDirection()
{
	setPostion(0.0, 0.0, 5.0, CameraPosition[0]);
	setPostion(0.0, 0.0, 0.0, CameraLookat[0]);
	setPostion(0.0, 5.0, 0.0, LightPosition[0]);

	setPostion(0.0, 0.0, 5.0, CameraPosition[1]);
	setPostion(0.0, 0.0, -1.0, CameraLookat[1]);
	setPostion(0.0, 5.0, 0.0, LightPosition[1]);

	vec2f NetParticleFromCam;
	vec3f RayleighInsc = vec3f(0.0, 0.0, 0.0);
	vec3f MieInsc = vec3f(0.0, 0.0, 0.0);
	
	OutdoorLightScattering.computeInscatteringIntegral(RayStart, RayEnd, EarthCentre, LightDir, NetParticleFromCam, RayleighInsc, MieInsc, 7);


// 
// 	if (OutdoorLightScattering.computeInscatteringIntegral(CameraPosition[0], CameraLookat[0], LightPosition[0]) > OutdoorLightScattering.computeInscatteringIntegral(CameraPosition[1], CameraLookat[1], LightPosition[1]))
// 		std::cout << "Compute SameCameraDirectionDifferentPositions sample...Test passed" << std::endl;
// 	else
// 		std::cout << "Compute SameCameraDirectionDifferentPositions sample...Test not passed" << std::endl;
}

//******************************************************************
//FUNCTION:
void testScatteringSamples()
{
	testSameCameraDirection();
	//testDifferentCameraDirection();
}

int main()
{

	testScatteringSamples();
	system("pause");
	return 1;
}