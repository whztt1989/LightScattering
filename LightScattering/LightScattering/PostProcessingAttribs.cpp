#include "PostProcessingAttribs.h"

CPostProcessingAttribs::CPostProcessingAttribs()
{
	m_UseCustomScatteringCoeffs = false;
	m_AerosolDesityScale = 1.0f;
	m_AerosolAbsorbtionScale = 0.1f;
	m_CustomRlghBeta = vec4f(5.8e-6f, 13.5e-6f, 33.1e-6f, 0.0f);
	m_CustomMieBeta = vec4f(2.0e-5f, 2.0e-5f, 2.0e-5f, 0.0f);
}

CPostProcessingAttribs::~CPostProcessingAttribs()
{
}