#pragma once
#include "Struct.h"

class CPostProcessingAttribs
{
public:
	CPostProcessingAttribs();
	~CPostProcessingAttribs();

private:
	bool m_UseCustomScatteringCoeffs;
	float m_AerosolDesityScale;
	float m_AerosolAbsorbtionScale;
	vec4f m_CustomRlghBeta;
	vec4f m_CustomMieBeta;

	friend class COutdoorLightScattering;
;
};
