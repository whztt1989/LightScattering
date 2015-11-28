#pragma once
#include "Struct.h"
#include "Pipeline.h"

class CCameraAttribs
{
public:
	CCameraAttribs();
	~CCameraAttribs();

	void initCamera(glm::vec3 vPos, glm::vec3 vLookAt, glm::vec3 vUp);
	void updateCamera(glm::vec3 vPos, glm::vec3 vLookAt, glm::vec3 vUp);

	glm::mat4 getViewT() const;
	glm::mat4 getProjectionT() const;

private:	
	glm::vec3 m_f3CameraPos;
	glm::vec3 m_f3CameraLookAt;
	glm::vec3 m_f3CameraUp;

	glm::mat4 m_WorldViewProjT;
	glm::mat4 m_ViewT;
	glm::mat4 m_ProjT;
	glm::mat4 m_ViewProjInvT;
};