#include "fbo.h"

CFBO::CFBO()
{
}

CFBO::~CFBO()
{
}

//******************************************************************
//FUNCTION:
void CFBO::__generateTexture(GLuint& vTextureID, unsigned int vWidth, unsigned int vHeight)
{
	glGenTextures(1, &vTextureID);
	glBindTexture(GL_TEXTURE_2D, vTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, vWidth, vHeight, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}