#pragma once
#include "Struct.h"

class CAirScatteringAttribs
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	CAirScatteringAttribs();
	~CAirScatteringAttribs();

 	vec4f m_AngularRayleighScatteringCoeff;
 	vec4f m_TotalRayleighScatteringCoeff;
 	vec4f m_RayleighExtinctionCoeff;
 
 	vec4f m_AngularMieScatteringCoeff;
 	vec4f m_TotalMieScatteringCoeff;
	vec4f m_MieExtinctionCoeff;

	vec4f m_CornetteShanks;
	vec4f m_TotalExtinctionCoeff;
	float m_AerosolPhaseFuncG;
	float m_Turbidity;
};
