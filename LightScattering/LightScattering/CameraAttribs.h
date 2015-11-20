#pragma once
#include "Struct.h"

class SCameraAttribs
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	SCameraAttribs();
	~SCameraAttribs();

private:
	
	vec4f m_CameraPosition;
	float m_NearPlane;
	float m_FarPlane;

	matrix4f m_WorldViewProjT;
	matrix4f m_ViewT;
	matrix4f m_ProjT;
	matrix4f m_ViewProjInvT;
};