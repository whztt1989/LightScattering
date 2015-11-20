#pragma once
#include "Struct.h"

class CAirScatteringAttribs
{
public:
	CAirScatteringAttribs();
	~CAirScatteringAttribs();

 	glm::vec4 m_AngularRayleighScatteringCoeff;
 	glm::vec4 m_TotalRayleighScatteringCoeff;
 	glm::vec4 m_RayleighExtinctionCoeff;
 
 	glm::vec4 m_AngularMieScatteringCoeff;
 	glm::vec4 m_TotalMieScatteringCoeff;
	glm::vec4 m_MieExtinctionCoeff;

	glm::vec4 m_CornetteShanks;
	glm::vec4 m_TotalExtinctionCoeff;
	float m_AerosolPhaseFuncG;
	float m_Turbidity;
};
