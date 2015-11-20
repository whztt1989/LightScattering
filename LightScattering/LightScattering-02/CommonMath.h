#pragma once
#include "Struct.h"

void normalizeVector(glm::vec3& voVector)
{
	float Length = sqrt(glm::dot(voVector, voVector));
	voVector /= Length;
}

float computeCosBetha(glm::vec3 vVector1, glm::vec3 vVector2)
{
	normalizeVector(vVector1);
	normalizeVector(vVector2);

	float CosBetha = glm::dot(vVector1, vVector2);
	return CosBetha;
}