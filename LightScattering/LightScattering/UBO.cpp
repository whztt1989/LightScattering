#include "UBO.h"

int CUBO::m_OffsetIndex = -1;

CUBO::CUBO()
{
}

CUBO::~CUBO()
{
}

//******************************************************************
//FUNCTION:
void CUBO::initUBOBlock(const GLuint vProgramID, const std::string& vBlockName, int vBlockElementNums, const char* vBlockElementNames[])
{
	_ASSERT(!vBlockName.empty() || vProgramID != 0);
	m_BlockIndex = glGetUniformBlockIndex(vProgramID, vBlockName.c_str());

	glGetActiveUniformBlockiv(vProgramID, m_BlockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &m_BlockSize);
	m_pBlockBuffer = (GLubyte*)malloc(m_BlockSize);

	if (m_pBlockBuffer == NULL)
	{
		std::cout << "Fail to allocate buffer" << std::endl;
		exit(1);
	}

	m_pIndices = (GLuint*)malloc(vBlockElementNums * sizeof(GLuint));
	glGetUniformIndices(vProgramID, vBlockElementNums, vBlockElementNames, m_pIndices);

	m_pOffset = (GLint*)malloc(vBlockElementNums * sizeof(GLint));
	glGetActiveUniformsiv(vProgramID, vBlockElementNums, m_pIndices, GL_UNIFORM_OFFSET, m_pOffset);
}

//******************************************************************
//FUNCTION:
void CUBO::setUBOBlockValue1f(const float& vElement)
{
	m_OffsetIndex++;
	memcpy(m_pBlockBuffer + m_pOffset[m_OffsetIndex], &vElement, 1 * sizeof(float));
}

//******************************************************************
//FUNCTION:
void CUBO::setUBOBlockValue4f(const vec4f& vElement)
{
	m_OffsetIndex++;
	memcpy(m_pBlockBuffer + m_pOffset[m_OffsetIndex], &vElement, 1 * sizeof(vec4f));
}

//******************************************************************
//FUNCTION:
void CUBO::createUBO()
{
	glGenBuffers(1, &m_UBOHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOHandle);
	glBufferData(GL_UNIFORM_BUFFER, m_BlockSize, m_pBlockBuffer, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, m_BlockIndex, m_UBOHandle);

	free(m_pBlockBuffer);
}