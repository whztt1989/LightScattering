#pragma once
#include "struct.h"

class CFBO
{
public:
	CFBO();
	virtual ~CFBO();

	virtual bool initFBO(unsigned int vWidth, unsigned int vHeight) = 0; 
	virtual void bindForWriting() = 0;
	
protected:
	void __generateTexture(GLuint& vTextureID, unsigned int vWidth, unsigned int vHeight);
};
