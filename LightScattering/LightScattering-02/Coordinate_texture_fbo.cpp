#include "Coordinate_texture_fbo.h"

CCoordinateTextureFBO::CCoordinateTextureFBO()
{
}

CCoordinateTextureFBO::~CCoordinateTextureFBO()
{
}

//******************************************************************
//FUNCTION:
bool CCoordinateTextureFBO::initFBO(unsigned int vWidth, unsigned int vHeight)
{
	__generateTexture(m_tex2DCoordinates, vWidth, vHeight);
	__generateTexture(m_tex2DEpipolarCamSpaceZ, vWidth, vHeight);
	__generateTexture(m_tex2DEpipolarStencil, vWidth, vHeight);

	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex2DCoordinates, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_tex2DEpipolarCamSpaceZ, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_tex2DEpipolarStencil, 0);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Fail to init framebuffer" << std::endl;
		return false;
	}

	GLenum drawBufs[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, drawBufs);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}

//******************************************************************
//FUNCTION:
void CCoordinateTextureFBO::bindForReadingCoordinate(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_tex2DCoordinates);
}

//******************************************************************
//FUNCTION:
void CCoordinateTextureFBO::bindForReadingEpipolarCamSpaceZ(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_tex2DEpipolarCamSpaceZ);
}

//******************************************************************
//FUNCTION:
void CCoordinateTextureFBO::bindForReadingEpipolarStencil(GLenum vTextureUnit)
{
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(vTextureUnit);
	glBindTexture(GL_TEXTURE_2D, m_tex2DEpipolarStencil);
}

//******************************************************************
//FUNCTION:
void CCoordinateTextureFBO::bindForWriting()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);
}
