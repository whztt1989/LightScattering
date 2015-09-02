#pragma once
#include "Struct.h"
#include "LightScatteringPostProcess.h"

class COutdoorLightScattering
{
public:
	COutdoorLightScattering();
	~COutdoorLightScattering();
	
	void computeScatteringCoefficients();
	void computeInscatteringIntegral(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, vec3f vDirOnLight, vec3f& voNetParticleFromCam, vec3f& voRayleighInscattering, vec3f& voMieInscattering, const float vNumSteps);
	float computeScatteredLightSample(const Eigen::Vector3f& vLightPostion, const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget);

private:
	void __computeRayleighScatteringCoefficennts();
	void __computeMieScatteringCoefficennts();
	void __applyPhaseFunction(vec3f& voRayleighScattering, vec3f& voMieScattering, float vCosTheta);

	void __computePointDiffInscattering(vec2f vPraticleDensityIncurrPoint, vec2f vNetParticleDensityFromCam, vec2f vNetParticleDesityToAtmTop, vec3f& voRlghInscatering, vec3f& voMieInscatering);
	
	void __computeCameraDirection(const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget);

	void __getRaySphereIntersection(vec3f vRayOrigin, vec3f vRayDirection, vec3f vSphereCenter, float vSphereRadius, vec2f& voIntersection);
	void __getAtmosphereProperties(vec3f vPosition, vec3f vEarthCentre, vec3f vDirectionOnLight, vec2f& voParticleDensity, vec2f& voNetParticleDensityToAtmTop);

	vec3f  m_CameraPosition;
	vec3f  m_LightPosition;
	vec3f  m_CameraLookat;
	vec3f  m_CameraDirection;

	CAirScatteringAttribs  m_AirScatteringAttribs;
	CPostProcessingAttribs m_PostProcessingAttribs;
};