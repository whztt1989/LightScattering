#pragma once
#include "Struct.h"
#include "Shader.h"
#include "Light.h"

class CLightAttribs
{
public:
	CLightAttribs();
	~CLightAttribs();

	void initSunLightForDirectionalLight(SDirectionalLight& vLight);
	void initSunLightForPointLight(SPointLight& vLight);
	void initSunLightForSpotLight(SSpotLight& vLight);

	void setLightScreenPos(glm::vec4 vLightScreenPos);
	void updateShaderUniformForPointLight(CShader* vShader);
	void updateLightPos(glm::vec3 vLightPos);

	glm::vec4 getDirectionOnSun() const;
	glm::vec4 getLightWoldPos() const;

private:
	glm::vec4 m_f4DirectionOnSun;
	glm::vec4 m_f4SunColorAndIntensityAtGround;
	glm::vec4 m_f4AmbientColor;
	glm::vec4 m_f4LightScreenPos;
	glm::vec4 m_f4LightWorldPos;

	SPointLight m_PointLight;

	bool m_bIsLightOnScreen;

	friend class COutdoorLightScattering;
};