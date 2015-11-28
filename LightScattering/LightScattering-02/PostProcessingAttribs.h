#pragma once
#include "Struct.h"

class CPostProcessingAttribs
{
public:
	CPostProcessingAttribs();
	~CPostProcessingAttribs();

private:
	bool  m_UseCustomScatteringCoeffs;
	float m_AerosolDesityScale;
	float m_AerosolAbsorbtionScale;
	glm::vec4 m_CustomRlghBeta;
	glm::vec4 m_CustomMieBeta;

	unsigned int m_NumEpipolarSlices;
	unsigned int m_MaxSamplesInSlice;
	unsigned int m_InitialSampleStepInSlice;
	unsigned int m_EpipolarSamplingDensityFactor;

	glm::vec2 m_f2CoordinateTexDim;
	glm::vec2 m_f2ScreenResolution;

	friend class COutdoorLightScattering;
};
