#pragma once
#include "Struct.h"

class SCameraAttribs
{
public:
	SCameraAttribs();
	~SCameraAttribs();

private:
	
	glm::vec4 m_CameraPosition;
	float m_NearPlane;
	float m_FarPlane;

	glm::mat4 m_WorldViewProjT;
	glm::mat4 m_ViewT;
	glm::mat4 m_ProjT;
	glm::mat4 m_ViewProjInvT;
};