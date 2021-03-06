#pragma once
#include "Struct.h"
#include "AirScatteringAttribs.h"

class CUBO
{
public:
	CUBO();
	~CUBO();

	void initUBOBlock(const GLuint vProgramID, const std::string& vBlockName, int vBlockElementNums, const char* vBlockElementNames[]);
	void setUBOBlockValue1f(const float& vElement);
	void setUBOBlockValue4f(const vec4f& vElement);
	void createUBO();

private:
	int m_BlockIndex;
	int m_BlockSize;	
	GLint* m_pOffset;
	GLubyte* m_pBlockBuffer;
	GLuint m_UBOHandle;
	GLuint* m_pIndices;
	vec4f m_Element;
	static int m_OffsetIndex;
};