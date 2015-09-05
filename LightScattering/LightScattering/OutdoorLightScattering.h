#pragma once
#include "LightScatteringExport.h"
#include "Struct.h"
#include "LightScatteringPostProcess.h"

class LightScattering_DLL_EXPORT COutdoorLightScattering
{
public:
	COutdoorLightScattering();
	~COutdoorLightScattering();

	 void  computeScatteringCoefficients();
	 void  computeInscatteringIntegral(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, vec3f vDirOnLight, vec2f& voNetParticleFromCam, vec3f& voRayleighInscattering, vec3f& voMieInscattering, const float vNumSteps);
	 float computeScatteredLightSample(const Eigen::Vector3f& vLightPostion, const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget);

private:
	void  __computeRayleighScatteringCoefficennts();
	void  __computeMieScatteringCoefficennts();
	void  __computePointDiffInscattering(vec2f vPraticleDensityIncurrPoint, vec2f vNetParticleDensityFromCam, vec2f vNetParticleDesityToAtmTop, vec3f& voRlghInscatering, vec3f& voMieInscatering);
	void  __computeCameraDirection(const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget);
	float __computeCosBetha(vec3f vVector1, vec3f vVector2);
	void  __computeCurPointHeightAndNormal(vec3f vPos, vec3f vEarthCentre, float& voHeight, vec3f& voNormal);


	void __getRaySphereIntersection(vec3f vRayOrigin, vec3f vRayDirection, vec3f vSphereCenter, float vSphereRadius, vec2f& voIntersection);
	void __getAtmosphereProperties(vec3f vPosition, vec3f vEarthCentre, vec3f vDirectionOnLight, vec2f& voParticleDensity, vec2f& voNetParticleDensityToAtmTop);
	vec2f __getNetParticleDensity(float vHeightAboveSurface, float vCosZenithAngle, const bool vIsOccluded);
	vec2f __getNetParticleDensity(vec3f vPos, vec3f vEarthCentre, vec3f vRayDir, const bool vIsOccluded);
	vec2f __IntegrateParticleDensityAlongRay(vec3f vPos, vec3f vRayDir,  vec3f vEarthCentre);
	vec2f __IntegrateParticleDensity(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCenre, const float vStepsNum = 200);
	vec3f __Normalize3v(vec3f vVector);
	
	void __applyPhaseFunction(vec3f& voRayleighScattering, vec3f& voMieScattering, float vCosTheta);
	

	vec3f  m_CameraPosition;
	vec3f  m_LightPosition;
	vec3f  m_CameraLookat;
	vec3f  m_CameraDirection;

	CAirScatteringAttribs  m_AirScatteringAttribs;
	CPostProcessingAttribs m_PostProcessingAttribs;

	vec3f m_TotalExtinctionTest;
};