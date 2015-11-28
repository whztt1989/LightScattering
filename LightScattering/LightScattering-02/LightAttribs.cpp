#include "LightAttribs.h"

CLightAttribs::CLightAttribs()
{
}

CLightAttribs::~CLightAttribs()
{
}

void CLightAttribs::initSunLightForDirectionalLight(SDirectionalLight& vLight)
{

}

//******************************************************************
//FUNCTION:
void CLightAttribs::initSunLightForPointLight(SPointLight& vLight)
{
	m_PointLight = vLight;
	m_f4LightWorldPos = glm::vec4(vLight.m_LightPosition, 1.0);
}

//******************************************************************
//FUNCTION:
void CLightAttribs::initSunLightForSpotLight(SSpotLight& vLight)
{

}

//******************************************************************
//FUNCTION:
void CLightAttribs::setLightScreenPos(glm::vec4 vLightScreenPos)
{
	m_f4LightScreenPos = vLightScreenPos;
}

//******************************************************************
//FUNCTION:
void CLightAttribs::updateShaderUniformForPointLight(CShader* vShader)
{
	vShader->setFloatUniformValue("uPointLight.m_BaseLight.m_AmbientColor", m_PointLight.m_AmbientColor.x, m_PointLight.m_AmbientColor.y, m_PointLight.m_AmbientColor.z);
	vShader->setFloatUniformValue("uPointLight.m_BaseLight.m_DiffuseColor", m_PointLight.m_DiffuseColor.x, m_PointLight.m_DiffuseColor.y, m_PointLight.m_DiffuseColor.z);
	vShader->setFloatUniformValue("uPointLight.m_BaseLight.m_SpecularColor", m_PointLight.m_SpecularColor.x, m_PointLight.m_SpecularColor.y, m_PointLight.m_SpecularColor.z);
	vShader->setFloatUniformValue("uPointLight.m_LightPosition", m_PointLight.m_LightPosition.x, m_PointLight.m_LightPosition.y, m_PointLight.m_LightPosition.z);
	vShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Constant", m_PointLight.Attenuation.m_Constant);
	vShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Linear", m_PointLight.Attenuation.m_Linear);
	vShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Quadratic", m_PointLight.Attenuation.m_Quadratic);
}

//******************************************************************
//FUNCTION:
glm::vec4 CLightAttribs::getDirectionOnSun() const
{
	return m_f4DirectionOnSun;
}

//******************************************************************
//FUNCTION:
glm::vec4 CLightAttribs::getLightWoldPos() const
{
	return m_f4LightWorldPos;
}

//******************************************************************
//FUNCTION:
void CLightAttribs::updateLightPos(glm::vec3 vLightPos)
{
	m_PointLight.m_LightPosition = vLightPos;
	m_f4LightWorldPos = glm::vec4(vLightPos, 1.0);
}
