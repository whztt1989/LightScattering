#include "Shader.h"

CShader::CShader()
{
}

CShader::~CShader()
{
}

//******************************************************************
//FUNCTION:
void CShader::initShader(const GLchar* vVertex, const GLchar* vFragment)
{
	const GLchar* pVertexShader = __loadShaderText(vVertex);
	const GLchar* pFragmentShader = __loadShaderText(vFragment);

	m_VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	m_FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	
	glShaderSource(m_VertexShaderID, 1, &pVertexShader, NULL);
	glShaderSource(m_FragmentShaderID, 1, &pFragmentShader, NULL);

	__compileShader(m_VertexShaderID);
	__compileShader(m_FragmentShaderID);
	
	m_ProgramID = glCreateProgram();
	glAttachShader(m_ProgramID, m_VertexShaderID);
	glAttachShader(m_ProgramID, m_FragmentShaderID);

	__linkProgram(m_ProgramID);
}

//******************************************************************
//FUNCTION:
const GLchar* const CShader::__loadShaderText(const GLchar* vFileName)
{
	_ASSERT(vFileName);
	GLchar* pShaderText = NULL;
	std::filebuf* pBuffer = NULL;
	unsigned int Size = 0;
	std::ifstream FileIn;

	FileIn.open(vFileName, std::ios::binary);

	if (FileIn.fail())
	{
		std::cout << "File open fail" << std::endl;
		return NULL;
	}

	pBuffer = FileIn.rdbuf();
	Size = pBuffer->pubseekoff(0, std::ios::end, std::ios::in);
	pBuffer->pubseekpos(0, std::ios::in);

	pShaderText = new GLchar[Size+1];
	pBuffer->sgetn(pShaderText, Size+1);
	pShaderText[Size] = '\0';
	FileIn.close();

	return pShaderText;
}

//******************************************************************
//FUNCTION:
void CShader::__compileShader(GLuint& vShader)
{
	glCompileShader(vShader);

	GLint Compile;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &Compile);

	if (!Compile)
	{
		GLint LogLength;
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &LogLength);

		if (LogLength > 0)
		{
			char* pLog = new char[LogLength];
			glGetShaderInfoLog(vShader, LogLength, &LogLength, pLog);
			fprintf(stderr, "Compile log = '%s' \n", pLog);
			delete[] pLog;
		}
	}
}

//******************************************************************
//FUNCTION:
void CShader::__linkProgram(GLuint& vProgram)
{
	glLinkProgram(vProgram);

	GLint Link;
	glGetProgramiv(vProgram, GL_LINK_STATUS, &Link);

	if (!Link)
	{
		GLint LogLength;
		glGetProgramiv(vProgram, GL_INFO_LOG_LENGTH, &LogLength);

		if (LogLength > 0)
		{
			char* pLog = new char[LogLength];
			glGetProgramInfoLog(vProgram, LogLength, &LogLength, pLog);
			fprintf(stderr, "Link log = '%s' \n", pLog);
			delete[] pLog;
		}
	}
}

//******************************************************************
//FUNCTION:
void CShader::useShader()
{
	glUseProgram(m_ProgramID);
}

//******************************************************************
//FUNCTION:
void CShader::banShader()
{
	glUseProgram(0);
}

//******************************************************************
//FUNCTION:
const GLuint CShader::getProgramID() const
{
	return m_ProgramID;
}

//******************************************************************
//FUNCTION:
void CShader::setTextureUniformValue(const char *name, unsigned int n)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform1i(key, n);
	else
		printf("TextureUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setMatUniformValue(const char *name, float Matrix[16])
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniformMatrix4fv(key , 1, GL_FALSE, Matrix);
	else
		printf("MatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform1f(key, v0);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform2f(key, v0, v1);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform3f(key, v0, v1, v2);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setFloatUniformValue(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform4f(key, v0, v1, v2, v3);
	else
		printf("FloatUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform1i(key, v0);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform2i(key, v0, v1);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform3i(key, v0, v1, v2);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setIntUniformValue(const char *name, GLint v0, GLint v1, GLint v2, GLint v3)
{
	GLint key;
	key = glGetUniformLocation(m_ProgramID, name);
	if(key >= 0)
		glUniform4i(key, v0, v1, v2, v3);
	else
		printf("IntUniformValue %s not found\n", name);
}

//******************************************************************
//FUNCTION:
void CShader::setFloatUnifromValue3v(const char* vName, GLsizei vCount, const GLfloat* vValues)
{
	GLint Key;
	Key = glGetUniformLocation(m_ProgramID, vName);

	if (Key >= 0)
		glUniform3fv(Key, vCount, vValues);
	else
		std::cout << "Float uniform value " << vName << " not found" << std::endl;
}
