#include "OutdoorLightScattering.h"
#include "CommonMath.h"

COutdoorLightScattering::COutdoorLightScattering()
{
}

COutdoorLightScattering::~COutdoorLightScattering()
{
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__getRaySphereIntersection(vec3f vRayOrigin, vec3f vRayDirection, vec3f vSphereCenter, float vSphereRadius, vec2f& voIntersection)
{
	vRayOrigin -= vSphereCenter;

	float A = vRayDirection.dot(vRayDirection);
	float B = 2 * vRayOrigin.dot(vRayDirection);
	float C = vRayOrigin.dot(vRayOrigin) - vSphereRadius * vSphereRadius;
	float D = B * B - 4 * A * C;

	if (D < 0)
	{
		voIntersection = vec2f(-1);
	}
	else
	{
		D = sqrt(D);
		voIntersection = vec2f((-B - D) / 2 * A, (-B + D) / 2 * A);
	}
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__getAtmosphereProperties(vec3f vPosition, vec3f vEarthCentre, vec3f vDirectionOnLight, vec2f& voParticleDensity, vec2f& voNetParticleDensityToAtmTop)
{
	vec3f EarthCentreToPointDir;
	float HeightAboveSurface;
	__computeCurPointHeightAndNormal(vPosition, vEarthCentre, HeightAboveSurface, EarthCentreToPointDir);

	float ParticleDensityR = (float)exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_R);
	float ParticleDensityM = (float)exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_M);
	voParticleDensity[0] = ParticleDensityR;
	voParticleDensity[1] = ParticleDensityM;

// 	float CosSunZenithAngleForCurrPoint = computeCosBetha(EarthCentreToPointDir, -vDirectionOnLight);
// 	voNetParticleDensityToAtmTop = __getNetParticleDensity(HeightAboveSurface, CosSunZenithAngleForCurrPoint, true);
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::computeScatteringCoefficients()
{
	__computeRayleighScatteringCoefficennts();
	__computeMieScatteringCoefficennts();

	float g = m_AirScatteringAttribs.m_AerosolPhaseFuncG;
	m_AirScatteringAttribs.m_CornetteShanks[0] = 3*(1.0f - g*g) / ( 2*(2.0f + g*g) );
	m_AirScatteringAttribs.m_CornetteShanks[1] = 1.0f + g*g;
	m_AirScatteringAttribs.m_CornetteShanks[2] = -2.0f * g;
	m_AirScatteringAttribs.m_CornetteShanks[3] = 1.0f;

	m_AirScatteringAttribs.m_TotalExtinctionCoeff = m_AirScatteringAttribs.m_RayleighExtinctionCoeff + m_AirScatteringAttribs.m_MieExtinctionCoeff;
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computeRayleighScatteringCoefficennts()
{
	//RayleighBetha(lambda = (680nm, 550nm, 440nm) ) = (5.8, 13.5, 33.1)e-6
	static const double WaveLengths[] = {680e-9, 550e-9, 440e-9};

	double n = 1.0003;    // - Refractive index of air in the visible spectrum
	double N = 2.545e+25; // - Number of molecules per unit volume
	double Pn = 0.035;    // - Depolarization factor for air which exoresses corrections, due to anisotropy of air molecules

	double RayleighConst = 8.0*PI*PI*PI * (n*n - 1.0) * (n*n - 1.0) / (3.0 * N) * (6.0 + 3.0*Pn) / (6.0 - 7.0*Pn);
	
	for (int WaveNum = 0; WaveNum < 3; WaveNum++)
	{
		double ScatteringCoeff = 0.0;

		if (m_PostProcessingAttribs.m_UseCustomScatteringCoeffs)
		{
			ScatteringCoeff = m_AirScatteringAttribs.m_TotalMieScatteringCoeff[WaveNum] = m_PostProcessingAttribs.m_CustomRlghBeta[WaveNum];
		}
		else
		{
			double Lambda2 = WaveLengths[WaveNum] * WaveLengths[WaveNum];
			double Lambda4 = Lambda2 * Lambda2;

			ScatteringCoeff = RayleighConst / Lambda4;
			m_AirScatteringAttribs.m_TotalRayleighScatteringCoeff[WaveNum] = static_cast<float>(ScatteringCoeff);
		}

		m_AirScatteringAttribs.m_AngularRayleighScatteringCoeff[WaveNum] = static_cast<float>(3.0 / (16*PI) * ScatteringCoeff);
	}

	m_AirScatteringAttribs.m_RayleighExtinctionCoeff = m_AirScatteringAttribs.m_TotalRayleighScatteringCoeff;
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computeMieScatteringCoefficennts()
{
	static const double WaveLengths[] = {680e-9, 550e-9, 440e-9};
	if (m_PostProcessingAttribs.m_UseCustomScatteringCoeffs)
	{
		m_AirScatteringAttribs.m_TotalMieScatteringCoeff = m_PostProcessingAttribs.m_CustomMieBeta * m_PostProcessingAttribs.m_AerosolDesityScale;
	}
	else
	{
		const bool UsePrethamMethod = false;
		if (UsePrethamMethod)
		{
			double K[] = {0.68455, 0.678781, (0.68455 + 0.678781) / 2.0};
			assert(m_AirScatteringAttribs.m_Turbidity >= 1.0);

			double C = (0.6544 * m_AirScatteringAttribs.m_Turbidity - 0.6510) * 1E-16;  //Concentration factor
			const double V = 4; //Junge's exponent

			double TotalMieBetaTerm = 0.434 * C * PI *pow(2.0 * PI, V-2);
			for (int WaveNum = 0; WaveNum < 3; WaveNum++)
			{
				double Lambdav_minus_2 = pow(WaveLengths[WaveNum], V-2);
				double TotalMieScatteringCoeff = TotalMieBetaTerm * K[WaveNum] / Lambdav_minus_2;
				m_AirScatteringAttribs.m_TotalMieScatteringCoeff[WaveNum] = static_cast<float>(TotalMieScatteringCoeff);
			}
		}
		else
		{
			const float MieBethaBN08 = 2e-5 * m_PostProcessingAttribs.m_AerosolDesityScale;
			m_AirScatteringAttribs.m_TotalMieScatteringCoeff = vec4f(MieBethaBN08, MieBethaBN08, MieBethaBN08, 0);
		}
	}

	for (int WaveNum = 0; WaveNum < 3; WaveNum++)
	{
		m_AirScatteringAttribs.m_AngularMieScatteringCoeff[WaveNum] = m_AirScatteringAttribs.m_TotalMieScatteringCoeff[WaveNum] / static_cast<float>(4.0 * PI);
		m_AirScatteringAttribs.m_MieExtinctionCoeff[WaveNum] = m_AirScatteringAttribs.m_TotalMieScatteringCoeff[WaveNum] * (1.0 + m_PostProcessingAttribs.m_AerosolAbsorbtionScale);
	}
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computePointDiffInscattering(vec2f vPraticleDensityIncurrPoint, vec2f vNetParticleDensityFromCam, vec2f vNetParticleDesityToAtmTop, vec3f& voRlghInscatering, vec3f& voMieInscatering)
{
	vec2f TotalParticleDensity = vNetParticleDensityFromCam + vNetParticleDesityToAtmTop;

	vec3f RayleighExtinctionCoeffRGB = vec3f( m_AirScatteringAttribs.m_RayleighExtinctionCoeff[0],  m_AirScatteringAttribs.m_RayleighExtinctionCoeff[1],  m_AirScatteringAttribs.m_RayleighExtinctionCoeff[2]);
	vec3f TotalRlghOpticalDepth =  RayleighExtinctionCoeffRGB * TotalParticleDensity[0];

	vec3f MieExtinctionCoeffRGB = vec3f( m_AirScatteringAttribs.m_MieExtinctionCoeff[0],  m_AirScatteringAttribs.m_MieExtinctionCoeff[1],  m_AirScatteringAttribs.m_MieExtinctionCoeff[2]);
	vec3f TotalMieOpticalDepth =  MieExtinctionCoeffRGB * TotalParticleDensity[1];

	//Total extinction for the current integration point
	vec3f TotalExtinction;
	TotalExtinction[0] = exp(-(TotalRlghOpticalDepth[0] + TotalMieOpticalDepth[0]));
	TotalExtinction[1] = exp(-(TotalRlghOpticalDepth[1] + TotalMieOpticalDepth[1]));
	TotalExtinction[2] = exp(-(TotalRlghOpticalDepth[2] + TotalMieOpticalDepth[2]));

	voRlghInscatering = vPraticleDensityIncurrPoint[0] * TotalExtinction;
	voMieInscatering = vPraticleDensityIncurrPoint[1] * TotalExtinction;
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::computeInscatteringIntegral(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, vec3f vDirOnLight, vec2f& voNetParticleFromCam, vec3f& voRayleighInscattering, vec3f& voMieInscattering, const float vNumSteps /*= 7*/)
{
	voNetParticleFromCam[0] = voNetParticleFromCam[1] = 0.0;
	voRayleighInscattering[0] = voRayleighInscattering[1] = voRayleighInscattering[2] = 0.0;
	voMieInscattering[0] = voMieInscattering[1] = voMieInscattering[2] = 0.0;
	normalizeVector(vDirOnLight);

	computeScatteringCoefficients();
	vec3f Steps = (vRayEnd - vRayStart) / vNumSteps;
	float StepLen = sqrt(Steps[0]*Steps[0] + Steps[1]*Steps[1] + Steps[2]*Steps[2]);

	for (float StepNum = 0.5; StepNum < vNumSteps; StepNum += 1.0)
	{
		vec3f CurrPos = vRayStart + Steps * StepNum;

		vec2f ParticleDensity, NetParticleDensityToAtmTop;
		__getAtmosphereProperties(CurrPos, vEarthCentre, vDirOnLight, ParticleDensity, NetParticleDensityToAtmTop);

		NetParticleDensityToAtmTop = __IntegrateParticleDensityAlongRay(CurrPos, vDirOnLight, vEarthCentre);
		voNetParticleFromCam += ParticleDensity * StepLen;

		vec3f RlghInsctr, MieInsctr;
		RlghInsctr[0] = RlghInsctr[1] = RlghInsctr[2] = 0.0;
		MieInsctr[0] = MieInsctr[1] = MieInsctr[2] = 0.0;
		__computePointDiffInscattering(ParticleDensity, voNetParticleFromCam, NetParticleDensityToAtmTop, RlghInsctr, MieInsctr);

		voRayleighInscattering += RlghInsctr * StepLen;
		voMieInscattering += MieInsctr * StepLen;
	}

	float CosBetha = computeCosBetha(vRayStart - vRayEnd, vDirOnLight);
	__applyPhaseFunction(voRayleighInscattering, voMieInscattering, CosBetha);

	vec3f LightInScattering = voRayleighInscattering + voMieInscattering;
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__applyPhaseFunction(vec3f& voRayleighScattering, vec3f& voMieScattering, float vCosTheta)
{
	vec4f AngularRayleighScatteringCoeff = m_AirScatteringAttribs.m_AngularRayleighScatteringCoeff * (1 + vCosTheta * vCosTheta);
	voRayleighScattering[0] *= AngularRayleighScatteringCoeff[0];
	voRayleighScattering[1] *= AngularRayleighScatteringCoeff[1];
	voRayleighScattering[2] *= AngularRayleighScatteringCoeff[2];

	// Apply Cornette-Shanks phase function (see Nishita et al. 93):
	// F(theta) = 1/(4*PI) * 3*(1-g^2) / (2*(2+g^2)) * (1+cos^2(theta)) / (1 + g^2 - 2g*cos(theta))^(3/2)
	// m_CornetteShanks = ( 3*(1-g^2) / (2*(2+g^2)), 1+g^2, -2g, 1 )
	vec2f Temp = vec2f(m_AirScatteringAttribs.m_CornetteShanks[1], m_AirScatteringAttribs.m_CornetteShanks[2]);
	float Denom = 1 / sqrt(Temp.dot(vec2f(1.0, vCosTheta)));  // 1 / (1 + g^2 - 2g*cos(theta))^(1/2)
	float CornettePhaseFunc = m_AirScatteringAttribs.m_CornetteShanks[0] * (Denom * Denom * Denom) * (1+ vCosTheta * vCosTheta);

	vec4f AngularMieScatteringCoeff = m_AirScatteringAttribs.m_AngularMieScatteringCoeff * CornettePhaseFunc;
	voMieScattering[0] *= AngularMieScatteringCoeff[0];
	voMieScattering[1] *= AngularMieScatteringCoeff[1];
	voMieScattering[2] *= AngularMieScatteringCoeff[2];
}

//******************************************************************
//FUNCTION:
vec2f COutdoorLightScattering::__getNetParticleDensity(float vHeightAboveSurface, float vCosZenithAngle, const bool vIsOccluded)
{
	float RelativeHeightAboveSurface = vHeightAboveSurface / ATM_TOP_HEIGHT;

	return vec2f(RelativeHeightAboveSurface, vCosZenithAngle * 0.5 + 0.5);
}

//******************************************************************
//FUNCTION:
vec2f COutdoorLightScattering::__IntegrateParticleDensityAlongRay(vec3f vPos, vec3f vRayDir, vec3f vEarthCentre)
{
	vec2f RayAtmTopIsecs;
	__getRaySphereIntersection(vPos, -vRayDir, vEarthCentre, ATM_TOP_RADIUS, RayAtmTopIsecs);
	
	float IntegrationDist = 0.0;
	if (RayAtmTopIsecs[1] > 0.0)
	  IntegrationDist = RayAtmTopIsecs[1];

	vec3f RayEnd = vPos + (-vRayDir) * IntegrationDist;

	return __IntegrateParticleDensity(vPos, RayEnd, vEarthCentre);
}

//******************************************************************
//FUNCTION:
vec2f COutdoorLightScattering::__IntegrateParticleDensity(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, const float vStepsNum /*= 200*/)
{
	vec3f Steps = (vRayEnd - vRayStart) / vStepsNum;
	float StepLen = sqrt(Steps.dot(Steps));

	vec2f ParticleNetDensity = vec2f(0.0, 0.0);
	for (float StepNum = 0.5; StepNum < vStepsNum; StepNum += 1.0)
	{
		vec3f CurrPos = vRayStart + Steps * StepNum;
		float HeightAboveSurface = 0.0;
		vec3f EarthCentreToPointDir;
		__computeCurPointHeightAndNormal(CurrPos, vEarthCentre, HeightAboveSurface, EarthCentreToPointDir);
		vec2f ParticleDensity;
		ParticleDensity[0] = exp(-HeightAboveSurface / PARTICLE_SCALE_HEIGHT_R);
		ParticleDensity[1] = exp(-HeightAboveSurface / PARTICLE_SCALE_HEIGHT_M);

		ParticleNetDensity += ParticleDensity * StepLen;
	}

	return ParticleNetDensity;
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computeCurPointHeightAndNormal(vec3f vPos, vec3f vEarthCentre, float& voHeight, vec3f& voNormal)
{
	vec3f EarthCentreToPointDir = vPos - vEarthCentre;
	float DistToEarthCentre = sqrt(EarthCentreToPointDir.dot(EarthCentreToPointDir));
	
	voNormal = EarthCentreToPointDir / DistToEarthCentre;
	voHeight = DistToEarthCentre - EARTH_RADIUS;
}

//******************************************************************
//FUNCTION:
COutdoorLightScattering* COutdoorLightScattering::getInstance()
{	
	static COutdoorLightScattering* pOutdoorLightScattering = NULL;

	if (NULL == pOutdoorLightScattering)
	{
		boost::mutex Mutex;
		boost::mutex::scoped_lock Lock(Mutex);
		if (NULL == pOutdoorLightScattering)
		{
			pOutdoorLightScattering = new COutdoorLightScattering;
		}
	}
	return pOutdoorLightScattering;
}

//******************************************************************
//FUNCTION:
const CAirScatteringAttribs COutdoorLightScattering::getAirScatteringAttribs() const
{
	return m_AirScatteringAttribs;
}
