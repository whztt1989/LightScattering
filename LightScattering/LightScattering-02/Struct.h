#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <gl/glew.h>
#include <gl/glut.h>
#include <glm/glm.hpp>

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