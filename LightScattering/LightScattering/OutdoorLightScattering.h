#pragma once
#include <boost/thread/thread.hpp>
#include "AirScatteringAttribs.h"
#include "PostProcessingAttribs.h"


class COutdoorLightScattering
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	static COutdoorLightScattering* getInstance();
	void  computeScatteringCoefficients();
	void  computeInscatteringIntegral(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, vec3f vDirOnLight, vec2f& voNetParticleFromCam, vec3f& voRayleighInscattering, vec3f& voMieInscattering, const float vNumSteps = 7);
	const CAirScatteringAttribs getAirScatteringAttribs() const;


private:
	COutdoorLightScattering();
	~COutdoorLightScattering();

	void  __computeRayleighScatteringCoefficennts();
	void  __computeMieScatteringCoefficennts();
	void  __computePointDiffInscattering(vec2f vPraticleDensityIncurrPoint, vec2f vNetParticleDensityFromCam, vec2f vNetParticleDesityToAtmTop, vec3f& voRlghInscatering, vec3f& voMieInscatering);
	void  __computeCurPointHeightAndNormal(vec3f vPos, vec3f vEarthCentre, float& voHeight, vec3f& voNormal);
	void __getRaySphereIntersection(vec3f vRayOrigin, vec3f vRayDirection, vec3f vSphereCenter, float vSphereRadius, vec2f& voIntersection);
	void __getAtmosphereProperties(vec3f vPosition, vec3f vEarthCentre, vec3f vDirectionOnLight, vec2f& voParticleDensity, vec2f& voNetParticleDensityToAtmTop);
	void __applyPhaseFunction(vec3f& voRayleighScattering, vec3f& voMieScattering, float vCosTheta);
	
	vec2f __getNetParticleDensity(float vHeightAboveSurface, float vCosZenithAngle, const bool vIsOccluded);
	vec2f __getNetParticleDensity(vec3f vPos, vec3f vEarthCentre, vec3f vRayDir, const bool vIsOccluded);
	vec2f __IntegrateParticleDensityAlongRay(vec3f vPos, vec3f vRayDir,  vec3f vEarthCentre);
	vec2f __IntegrateParticleDensity(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCenre, const float vStepsNum = 200);


	CAirScatteringAttribs  m_AirScatteringAttribs;
	CPostProcessingAttribs m_PostProcessingAttribs;
};