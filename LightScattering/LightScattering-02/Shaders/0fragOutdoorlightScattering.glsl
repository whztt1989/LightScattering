#version 430 compatibility

//in vec3 NormalEye;
in vec2 TexCoord;
in vec3 PositionW;
in vec3 PositionE;
in mat4 ViewE;
//in vec4 PositionL;
out vec4 FragColor;

struct SBaseLight 
{
	vec3 m_AmbientColor;
	vec3 m_DiffuseColor;
	vec3 m_SpecularColor;
};

struct SAttenuation
{
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

struct SPointLight
{
	SBaseLight m_BaseLight;
	vec3 m_LightPosition;
	SAttenuation m_Attenuation;
};

uniform SPointLight uPointLight;
//uniform sampler2D uShadowMapSampler;
uniform sampler2D uTextureSampler;
uniform float uShininess = 128;
//uniform mat4 uLightProjectionMatrix;


//uniform vec3 uRayStart;
//uniform vec3 uRayEnd;
uniform vec3 uLightDir;
uniform vec3 uEarthCentre;
//uniform float uNumSteps;

uniform uAirScatteringAttribs{
	vec4 u_AngularRayleighScatteringCoeff;
	vec4 u_TotalRayleighScatteringCoeff;
	vec4 u_RayleighExtinctionCoeff;
	vec4 u_AngularMieScatteringCoeff;
	vec4 u_TotalMieScatteringCoeff;
	vec4 u_MieExtinctionCoeff;
	vec4  u_CornetteShanks;
	vec4  u_TotalExtinctionCoeff;
	float u_AerosolPhaseFuncG;
	float u_Turbidity;
};

float EARTH_RADIUS = 6360000.0;
float ATM_TOP_HEIGHT = 80000.0;
float ATM_TOP_RADIUS = EARTH_RADIUS + ATM_TOP_HEIGHT;
float PARTICLE_SCALE_HEIGHT_R = 7994.0;
float PARTICLE_SCALE_HEIGHT_M = 1200.0;

//******************************************************************
//FUNCTION:
void computeCurPointHeightAndNormal(in vec3 vPos, in vec3 vEarthCentre, inout float voHeight, inout vec3 voNormal)
{
	vec3 EarthCentreToPointDir = vPos - vEarthCentre;
	float DistToEarthCentre = length(EarthCentreToPointDir);
	
	voNormal = EarthCentreToPointDir / DistToEarthCentre;
	voHeight = DistToEarthCentre - EARTH_RADIUS;
}

//******************************************************************
//FUNCTION:
void getAtmosphereProperties(in vec3 vPosition, in vec3 vEarthCentre, in vec3 vDirectionOnLight, inout vec2 voParticleDensity, inout vec2 voNetParticleDensityToAtmTop)
{
	vec3 EarthCentreToPointDir;
	float HeightAboveSurface;
	computeCurPointHeightAndNormal(vPosition, vEarthCentre, HeightAboveSurface, EarthCentreToPointDir);

	float ParticleDensityR = exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_R);
	float ParticleDensityM = exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_M);
	voParticleDensity[0] = ParticleDensityR;
	voParticleDensity[1] = ParticleDensityM;

// 	float CosSunZenithAngleForCurrPoint = computeCosBetha(EarthCentreToPointDir, -vDirectionOnLight);
// 	voNetParticleDensityToAtmTop = __getNetParticleDensity(HeightAboveSurface, CosSunZenithAngleForCurrPoint, true);
}

//******************************************************************
//FUNCTION:
void getRaySphereIntersection(in vec3 vRayOrigin, in vec3 vRayDirection, in vec3 vSphereCenter, in float vSphereRadius, inout vec2 voIntersection)
{
	vRayOrigin -= vSphereCenter;

	float A = dot(vRayDirection, vRayDirection);
	float B = 2 * dot(vRayOrigin, vRayDirection);
	float C = dot(vRayOrigin, vRayOrigin) - vSphereRadius * vSphereRadius;
	float D = B * B - 4 * A * C;

	if (D < 0)
	{
		voIntersection = vec2(-1);
	}
	else
	{
		D = sqrt(D);
		voIntersection = vec2((-B - D) / 2 * A, (-B + D) / 2 * A);
	}
}

//******************************************************************
//FUNCTION:
vec2 integrateParticleDensity(in vec3 vRayStart, in vec3 vRayEnd, in vec3 vEarthCentre, const float vStepsNum = 200)
{
	vec3 Steps = (vRayEnd - vRayStart) / vStepsNum;
	float StepLen = length(Steps);

	vec2 ParticleNetDensity = vec2(0.0, 0.0);
	for (float StepNum = 0.5; StepNum < vStepsNum; StepNum += 1.0)
	{
		vec3 CurrPos = vRayStart + Steps * StepNum;
		float HeightAboveSurface = 0.0;
		vec3 EarthCentreToPointDir;
		computeCurPointHeightAndNormal(CurrPos, vEarthCentre, HeightAboveSurface, EarthCentreToPointDir);
		vec2 ParticleDensity;
		ParticleDensity[0] = exp(-HeightAboveSurface / PARTICLE_SCALE_HEIGHT_R);
		ParticleDensity[1] = exp(-HeightAboveSurface / PARTICLE_SCALE_HEIGHT_M);

		ParticleNetDensity += ParticleDensity * StepLen;
	}

	return ParticleNetDensity;
}

//******************************************************************
//FUNCTION:
vec2 integrateParticleDensityAlongRay(in vec3 vPos, in vec3 vRayDir, in vec3 vEarthCentre)
{
	vec2 RayAtmTopIsecs;
	getRaySphereIntersection(vPos, -vRayDir, vEarthCentre, ATM_TOP_RADIUS, RayAtmTopIsecs);
	
	float IntegrationDist = 0.0;
	if (RayAtmTopIsecs[1] > 0.0)
	  IntegrationDist = RayAtmTopIsecs[1];

	vec3 RayEnd = vPos + (-vRayDir) * IntegrationDist;

	return integrateParticleDensity(vPos, RayEnd, vEarthCentre);
}

//******************************************************************
//FUNCTION:
void computePointDiffInscattering(in vec2 vPraticleDensityIncurrPoint, in vec2 vNetParticleDensityFromCam, in vec2 vNetParticleDesityToAtmTop, inout vec3 voRlghInscatering, inout vec3 voMieInscatering)
{
	vec2 TotalParticleDensity = vNetParticleDensityFromCam + vNetParticleDesityToAtmTop;

	vec3 RayleighExtinctionCoeffRGB = vec3(u_RayleighExtinctionCoeff[0], u_RayleighExtinctionCoeff[1], u_RayleighExtinctionCoeff[2]);
	vec3 TotalRlghOpticalDepth =  RayleighExtinctionCoeffRGB * TotalParticleDensity[0];

	vec3 MieExtinctionCoeffRGB = vec3(u_MieExtinctionCoeff[0], u_MieExtinctionCoeff[1],u_MieExtinctionCoeff[2]);
	vec3 TotalMieOpticalDepth =  MieExtinctionCoeffRGB * TotalParticleDensity[1];

	//Total extinction for the current integration point
	vec3 TotalExtinction;
	TotalExtinction[0] = exp(-(TotalRlghOpticalDepth[0] + TotalMieOpticalDepth[0]));
	TotalExtinction[1] = exp(-(TotalRlghOpticalDepth[1] + TotalMieOpticalDepth[1]));
	TotalExtinction[2] = exp(-(TotalRlghOpticalDepth[2] + TotalMieOpticalDepth[2]));

	voRlghInscatering = vPraticleDensityIncurrPoint[0] * TotalExtinction;
	voMieInscatering = vPraticleDensityIncurrPoint[1] * TotalExtinction;
}

//******************************************************************
//FUNCTION:
void applyPhaseFunction(inout vec3 voRayleighScattering, inout vec3 voMieScattering, in float vCosTheta)
{
	vec4 AngularRayleighScatteringCoeff = u_AngularRayleighScatteringCoeff * (1 + vCosTheta * vCosTheta);
	voRayleighScattering[0] *= AngularRayleighScatteringCoeff[0];
	voRayleighScattering[1] *= AngularRayleighScatteringCoeff[1];
	voRayleighScattering[2] *= AngularRayleighScatteringCoeff[2];

	// Apply Cornette-Shanks phase function (see Nishita et al. 93):
	// F(theta) = 1/(4*PI) * 3*(1-g^2) / (2*(2+g^2)) * (1+cos^2(theta)) / (1 + g^2 - 2g*cos(theta))^(3/2)
	// m_CornetteShanks = ( 3*(1-g^2) / (2*(2+g^2)), 1+g^2, -2g, 1 )
	vec2 Temp = vec2(u_CornetteShanks[1], u_CornetteShanks[2]);
	float Denom = 1 / sqrt(dot(Temp, vec2(1.0, vCosTheta)));  // 1 / (1 + g^2 - 2g*cos(theta))^(1/2)
	float CornettePhaseFunc = u_CornetteShanks[0] * (Denom * Denom * Denom) * (1+ vCosTheta * vCosTheta);

	vec4 AngularMieScatteringCoeff = u_AngularMieScatteringCoeff * CornettePhaseFunc;
	voMieScattering[0] *= AngularMieScatteringCoeff[0];
	voMieScattering[1] *= AngularMieScatteringCoeff[1];
	voMieScattering[2] *= AngularMieScatteringCoeff[2];
}

//******************************************************************
//FUNCTION:
float computeCosBetha(in vec3 vVector1, in vec3 vVector2)
{
	vVector1 = normalize(vVector1);
	vVector2 = normalize(vVector2);

	float CosBetha = dot(vVector1, vVector2);
	return CosBetha;
}

//******************************************************************
//FUNCTION:
void computeInscatteringIntegral(in vec3 vRayStart, in vec3 vRayEnd, in vec3 vEarthCentre, in vec3 vDirOnLight, inout vec2 voNetParticleFromCam, inout vec3 voRayleighInscattering, inout vec3 voMieInscattering, const float vNumSteps = 7)
{
	voNetParticleFromCam[0] = voNetParticleFromCam[1] = 0.0;
	voRayleighInscattering[0] = voRayleighInscattering[1] = voRayleighInscattering[2] = 0.0;
	voMieInscattering[0] = voMieInscattering[1] = voMieInscattering[2] = 0.0;
	vDirOnLight = normalize(vDirOnLight);

	vec3 Steps = (vRayEnd - vRayStart) / vNumSteps;
	float StepLen = length(Steps);

	for (float StepNum = 0.5; StepNum < vNumSteps; StepNum += 1.0)
	{
		vec3 CurrPos = vRayStart + Steps * StepNum;

		vec2 ParticleDensity, NetParticleDensityToAtmTop;
		getAtmosphereProperties(CurrPos, vEarthCentre, vDirOnLight, ParticleDensity, NetParticleDensityToAtmTop);

		NetParticleDensityToAtmTop = integrateParticleDensityAlongRay(CurrPos, vDirOnLight, vEarthCentre);
		voNetParticleFromCam += ParticleDensity * StepLen;

		vec3 RlghInsctr, MieInsctr;
		RlghInsctr[0] = RlghInsctr[1] = RlghInsctr[2] = 0.0;
		MieInsctr[0] = MieInsctr[1] = MieInsctr[2] = 0.0;
		computePointDiffInscattering(ParticleDensity, voNetParticleFromCam, NetParticleDensityToAtmTop, RlghInsctr, MieInsctr);

		voRayleighInscattering += RlghInsctr * StepLen;
		voMieInscattering += MieInsctr * StepLen;
	}

	float CosBetha = computeCosBetha(vRayStart - vRayEnd, vDirOnLight);
	applyPhaseFunction(voRayleighInscattering, voMieInscattering, CosBetha);
}

void main()
{
	vec2 NetParticleFromCam;
	vec3 RayleighInsc;
	vec3 MieInsc;
//	vec3 CameraPos = uRayStart + vec3(6360000.0);

	vec3 CameraPos = vec3(0.0, 0.0, 6400000.0);
	computeInscatteringIntegral(CameraPos, PositionW * 6360000, uEarthCentre, uLightDir, NetParticleFromCam, RayleighInsc, MieInsc, 7);
	vec3 LightInScattering = RayleighInsc + MieInsc;

	FragColor = vec4(LightInScattering.x*100, 0.0, 0.0, 1.0);

	FragColor = vec4(1.0);
	//if (LightInScattering.x < 0)
	//	FragColor = vec4(LightInScattering.x, 0.0, 0.0, 1.0);
	//else
	//	FragColor = vec4(1, 1, 0, 1.0);
}