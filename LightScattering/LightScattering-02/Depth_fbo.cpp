#include "Depth_fbo.h"

CDepthFBO::CDepthFBO()
{
	m_Fbo = 0;
	m_LightDepth = 0;
	m_CameraZ = 0;
}

CDepthFBO::~CDepthFBO()
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
bool CDepthFBO::initFBO(unsigned int vWidth, unsigned int vHeight)
{
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
void CDepthFBO::bindForReadingLightDepth(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_LightDepth);
}

//******************************************************************
//FUNCTION:
void CDepthFBO::bindForReadingCameraZ(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_CameraZ);
}

//******************************************************************
//FUNCTION:
void CDepthFBO::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
}
