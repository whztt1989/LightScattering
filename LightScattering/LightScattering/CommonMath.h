#pragma once
#include "Struct.h"

void normalizeVector(vec3f& voVector)
{
	float Length = sqrt(voVector.dot(voVector));
	voVector /= Length;
}

float computeCosBetha(vec3f vVector1, vec3f vVector2)
{
	normalizeVector(vVector1);
	normalizeVector(vVector2);

	float CosBetha = vVector1.dot(vVector2);
	return CosBetha;
}