#pragma once
#include <iostream>
#include <Eigen/dense>

typedef Eigen::Vector2f vec2f;
typedef Eigen::Vector3f vec3f;
typedef Eigen::Vector4f vec4f;

typedef Eigen::Vector2d vec2d;
typedef Eigen::Vector3d vec3d;
typedef Eigen::Vector4d vec4d;

#define PI 3.14159265358979323846

#ifndef EARTH_RADIUS
#   define EARTH_RADIUS 6360000.f
#endif

#ifndef ATM_TOP_HEIGHT
#   define ATM_TOP_HEIGHT 80000.f
#endif

#ifndef ATM_TOP_RADIUS
#   define ATM_TOP_RADIUS (EARTH_RADIUS + ATM_TOP_HEIGHT)
#endif

#ifndef PARTICLE_SCALE_HEIGHT_R
#   define PARTICLE_SCALE_HEIGHT_R 7994.0
#endif

#ifndef PARTICLE_SCALE_HEIGHT_M
#   define PARTICLE_SCALE_HEIGHT_M 1200.0
#endif

#ifndef ENABLE_LIGHT_SHAFTS
#   define ENABLE_LIGHT_SHAFTS 1
#endif

#ifndef IS_32BIT_MIN_MAX_MAP
#   define IS_32BIT_MIN_MAX_MAP 0
#endif

struct SPoint
{
	float m_X;
	float m_Y;
	float m_Z;
};