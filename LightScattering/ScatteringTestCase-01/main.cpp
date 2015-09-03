#include <iostream>
#include <vector>
#include <Eigen/dense>

struct SPoint
{
	float m_X;
	float m_Y;
	float m_Z;
};
SPoint CameraPosition[2], CameraLookat[2], LightPosition[2];

float computeSample(const SPoint& vCameraPosition, const SPoint& vCameraLookat, const SPoint& vLightPosition)
{
	// TO do...
	return 1.0;
}

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
	setPostion(0.0, 0.0, 5.0, CameraPosition[0]);
	setPostion(0.0, 0.0, 0.0, CameraLookat[0]);
	setPostion(0.0, 5.0, 0.0, LightPosition[0]);

	setPostion(0.0, 0.0, 5.0, CameraPosition[1]);
	setPostion(0.0, -1.0, 0.0, CameraLookat[1]);
	setPostion(0.0, 5.0, 0.0, LightPosition[1]);

	if (computeSample(CameraPosition[0], CameraLookat[0], LightPosition[0]) > computeSample(CameraPosition[1], CameraLookat[1], LightPosition[1]))
		std::cout << "Compute DifferentCameraDirection sample...Test passed" << std::endl;
	else
		std::cout << "Compute DifferentCameraDirection sample...Test not passed" << std::endl;
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

	if (computeSample(CameraPosition[0], CameraLookat[0], LightPosition[0]) > computeSample(CameraPosition[1], CameraLookat[1], LightPosition[1]))
		std::cout << "Compute SameCameraDirectionDifferentPositions sample...Test passed" << std::endl;
	else
		std::cout << "Compute SameCameraDirectionDifferentPositions sample...Test not passed" << std::endl;
}

//******************************************************************
//FUNCTION:
void testScatteringSamples()
{
	testSameCameraDirection();
	testDifferentCameraDirection();
}

int main()
{
	Eigen::Vector3f Test(1, 2, 3);
	Eigen::Vector3f Test2 = Test;
	Eigen::Matrix3f Test3(3, 3);
	Test3 << 1, 2, 3, 4, 5, 6, 7, 8, 9;
	float Temp = Test[0];

	testScatteringSamples();
	return 1;
}

//abc
//bcdef