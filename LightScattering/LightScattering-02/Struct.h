#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <gl/glew.h>
#include <gl/glut.h>
#include <glm/glm.hpp>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;

// struct SCameraAttribs
// {
// 	glm::vec4 m_f4CameraPos;
// 	glm::mat4 m_ViewT;
// 	glm::mat4 m_ProjectionT;
// 	glm::mat4 m_ViewProjT;
// };
// 
// struct SLightAttribs
// {
// 	glm::vec4 m_f4DirectionOnSun;
// 	glm::vec4 m_f4SunColorAndIntensityAtGround;
// 	glm::vec4 m_f4AmbientColor;
// 	glm::vec4 m_f4LightScreenPos;
// 
// 	bool m_bIsLightOnScreen;
// };
// 
// struct SPostProcessingAttribs
// {
// 	unsigned int m_NumEpipolarSlices;
// 	unsigned int m_MaxSamplesInSlice;
// 	unsigned int m_InitialSampleStepInSlice;
// 	unsigned int m_EpipolarSamplingDensityFactor;
// 
// 	glm::vec2 m_f2CoordinateTexDim;
// 	glm::vec2 m_f2ScreenResolution;
// 
// 	SPostProcessingAttribs()
// 	{
// 		m_f2CoordinateTexDim = glm::vec2(m_MaxSamplesInSlice, m_NumEpipolarSlices);
// 		m_f2ScreenResolution = glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT);
// 	}
// };


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