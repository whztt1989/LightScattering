#pragma once
#include "Struct.h"

void normalizeVector(vec3f& voVector)
{
	float Length = sqrt(voVector.dot(voVector));
	voVector /= Length;
}
