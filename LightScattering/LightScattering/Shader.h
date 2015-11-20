#pragma once
#include <iostream>
#include <fstream>
#include <gl/glew.h>
#include <gl/glut.h>

class CShader
{
public:
	CShader();
	~CShader();

	void initShader(const GLchar* vVertex, const GLchar* vFragment);
	void useShader();
	void banShader();
	const GLuint getProgramID() const;

	void setTextureUniformValue(const char *name, unsigned int n);
	void setMatUniformValue(const char *name, float Matrix[16]);

	void setFloatUniformValue(const char* name, GLfloat v0);
	void setFloatUniformValue(const char* name, GLfloat v0,  GLfloat v1);
	void setFloatUniformValue(const char* name, GLfloat v0,  GLfloat v1, GLfloat v2);
	void setFloatUniformValue(const char* name, GLfloat v0,  GLfloat v1, GLfloat v2, GLfloat v3);
	void setFloatUnifromValue3v(const char* vName, GLsizei vCount, const GLfloat* vValues);

	void setIntUniformValue(const char *name, GLint v0);
	void setIntUniformValue(const char *name, GLint v0,  GLint v1);
	void setIntUniformValue(const char *name, GLint v0,  GLint v1, GLint v2);
	void setIntUniformValue(const char *name, GLint v0,  GLint v1, GLint v2, GLint v3);

private:
	void __compileShader(GLuint& vShader);
	void __linkProgram(GLuint& vProgram);
	const GLchar* const __loadShaderText(const GLchar* vFileName);

	GLuint m_VertexShaderID;
	GLuint m_FragmentShaderID;
	GLuint m_ProgramID;
};