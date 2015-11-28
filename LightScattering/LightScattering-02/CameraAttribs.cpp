#include "CameraAttribs.h"

CCameraAttribs::CCameraAttribs()
{
}

CCameraAttribs::~CCameraAttribs()
{
}

//******************************************************************
//FUNCTION:
glm::mat4 CCameraAttribs::getViewT() const
{
	return m_ViewT;
}

//******************************************************************
//FUNCTION:
glm::mat4 CCameraAttribs::getProjectionT() const
{
	return m_ProjT;
}

//******************************************************************
//FUNCTION:
void CCameraAttribs::initCamera(glm::vec3 vPos, glm::vec3 vLookAt, glm::vec3 vUp)
{
	m_f3CameraPos = vPos;
	m_f3CameraLookAt = vLookAt;
	m_f3CameraUp = vUp;


	CPileline Pileline;
	Pileline.lookAt(vPos, vLookAt, vUp);
	Pileline.perspective(60, 1, 1, 100);

	m_ViewT = Pileline.getViewMat();
	m_ProjT = Pileline.getProjectMat();
}

//******************************************************************
//FUNCTION:
void CCameraAttribs::updateCamera(glm::vec3 vPos, glm::vec3 vLookAt, glm::vec3 vUp)
{
	CPileline Pileline;
	Pileline.lookAt(vPos, vLookAt, vUp);

	m_ViewT = Pileline.getViewMat();
}
