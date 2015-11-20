#include "shadow_map_fbo.h"

CShaderMapFBO::CShaderMapFBO()
{
	m_Fbo = 0;
	m_LightDepth = 0;
	m_CameraZ = 0;
}

CShaderMapFBO::~CShaderMapFBO()
{
	if (m_Fbo != 0) {
		glDeleteFramebuffers(1, &m_Fbo);
	}

	if (m_LightDepth != 0) {
		glDeleteTextures(1, &m_LightDepth);
	}
}

//******************************************************************
//FUNCTION:
bool CShaderMapFBO::initFBO(unsigned int vWidth, unsigned int vHeight)
{
// 	glGenTextures(1, &m_ShadowMap);
// 	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
// 	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, vWidth, vHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
//
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
// 	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
// 
// 	glGenFramebuffers(1, &m_Fbo);
// 	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
// 	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0);
// 
// 	glDrawBuffer(GL_NONE);
// 	glReadBuffer(GL_NONE);

	glGenRenderbuffers(1, &m_DepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, vWidth, vHeight);

	__generateTexture(m_LightDepth, vWidth, vHeight);
	__generateTexture(m_CameraZ, vWidth, vHeight);

	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRenderBuffer);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LightDepth, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_CameraZ, 0);
 
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Fail to init framebuffer" << std::endl;
		return false;
	}

	GLenum drawBufs[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
	glDrawBuffers(2, drawBufs);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

//******************************************************************
//FUNCTION:
void CShaderMapFBO::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
}

//******************************************************************
//FUNCTION:
void CShaderMapFBO::bindForReading(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_CameraZ);
}

//******************************************************************
//FUNCTION:
void CShaderMapFBO::__generateTexture(GLuint& vTextureID, unsigned int vWidth, unsigned int vHeight)
{
	glGenTextures(1, &vTextureID);
	glBindTexture(GL_TEXTURE_2D, vTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, vWidth, vHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void CShaderMapFBO::getTextureID() const
{

}
