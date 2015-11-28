#pragma once
#include "fbo.h"

class CCoordinateTextureFBO : public CFBO
{
public:
	CCoordinateTextureFBO();
	virtual ~CCoordinateTextureFBO();
	
	bool initFBO(unsigned int vWidth, unsigned int vHeight) override;
	void bindForWriting() override;

	void bindForReadingCoordinate(GLenum vTextureUnit);
	void bindForReadingEpipolarCamSpaceZ(GLenum vTextureUnit);
	void bindForReadingEpipolarStencil(GLenum vTextureUnit);

private:
	GLuint m_Fbo;
	GLuint m_tex2DCoordinates;
	GLuint m_tex2DEpipolarCamSpaceZ;
	GLuint m_tex2DEpipolarStencil;
};