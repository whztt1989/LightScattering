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

	friend class COutdoorLightScattering;
};
