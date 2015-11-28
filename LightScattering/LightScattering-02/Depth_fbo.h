#pragma once
#include "fbo.h"

class CDepthFBO : public CFBO
{
public:
	CDepthFBO();
	virtual ~CDepthFBO();

	bool initFBO(unsigned int vWidth, unsigned int vHeight) override;
	void bindForWriting() override;
	void bindForReadingLightDepth(GLenum vTextureUnit);
	void bindForReadingCameraZ(GLenum vTextureUnit);

private:
	GLuint m_Fbo;
	GLuint m_DepthRenderBuffer;
	GLuint m_LightDepth;
	GLuint m_CameraZ;
};