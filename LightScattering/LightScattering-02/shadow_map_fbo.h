#pragma once
#include <gl/glew.h>
#include <iostream>

class CShaderMapFBO
{
public:
	CShaderMapFBO();
	~CShaderMapFBO();

	bool initFBO(unsigned int vWidth, unsigned int vHeight);
	void bindForWriting();
	void bindForReading(GLenum vTextureUnit);
	void getTextureID() const;

private:
	void __generateTexture(GLuint& vTextureID, unsigned int vWidth, unsigned int vHeight);

	GLuint m_Fbo;
	GLuint m_LightDepth;
	GLuint m_CameraZ;
	GLuint m_DepthRenderBuffer;
};