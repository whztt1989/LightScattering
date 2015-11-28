#pragma once
#include <boost/thread/thread.hpp>
#include "AirScatteringAttribs.h"
#include "PostProcessingAttribs.h"
#include "CameraAttribs.h"
#include "LightAttribs.h"

class COutdoorLightScattering
{
public:
	static COutdoorLightScattering* getInstance();
	void  computeScatteringCoefficients();
	void  computeInscatteringIntegral(glm::vec3 vRayStart, glm::vec3 vRayEnd, glm::vec3 vEarthCentre, glm::vec3 vDirOnLight, glm::vec2& voNetParticleFromCam, glm::vec3& voRayleighInscattering, glm::vec3& voMieInscattering, const float vNumSteps = 7);
	const CAirScatteringAttribs getAirScatteringAttribs() const;
	
	void render(CCameraAttribs& vViewCamera, CLightAttribs& vSunLight, CPostProcessingAttribs& vPPAttribs);

private:
	COutdoorLightScattering();
	~COutdoorLightScattering();

	void  __computeRayleighScatteringCoefficennts();
	void  __computeMieScatteringCoefficennts();
	void  __computePointDiffInscattering(glm::vec2 vPraticleDensityIncurrPoint, glm::vec2 vNetParticleDensityFromCam, glm::vec2 vNetParticleDesityToAtmTop, glm::vec3& voRlghInscatering, glm::vec3& voMieInscatering);
	void  __computeCurPointHeightAndNormal(glm::vec3 vPos, glm::vec3 vEarthCentre, float& voHeight, glm::vec3& voNormal);
	void __getRaySphereIntersection(glm::vec3 vRayOrigin, glm::vec3 vRayDirection, glm::vec3 vSphereCenter, float vSphereRadius, glm::vec2& voIntersection);
	void __getAtmosphereProperties(glm::vec3 vPosition, glm::vec3 vEarthCentre, glm::vec3 vDirectionOnLight, glm::vec2& voParticleDensity, glm::vec2& voNetParticleDensityToAtmTop);
	void __applyPhaseFunction(glm::vec3& voRayleighScattering, glm::vec3& voMieScattering, float vCosTheta);
	
	glm::vec2 __getNetParticleDensity(float vHeightAboveSurface, float vCosZenithAngle, const bool vIsOccluded);
	glm::vec2 __getNetParticleDensity(glm::vec3 vPos, glm::vec3 vEarthCentre, glm::vec3 vRayDir, const bool vIsOccluded);
	glm::vec2 __IntegrateParticleDensityAlongRay(glm::vec3 vPos, glm::vec3 vRayDir,  glm::vec3 vEarthCentre);
	glm::vec2 __IntegrateParticleDensity(glm::vec3 vRayStart, glm::vec3 vRayEnd, glm::vec3 vEarthCenre, const float vStepsNum = 200);

	CAirScatteringAttribs  m_AirScatteringAttribs;
	CPostProcessingAttribs m_PostProcessingAttribs;
};