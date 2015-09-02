#include "OutdoorLightScattering.h"

COutdoorLightScattering::COutdoorLightScattering()
{
}

COutdoorLightScattering::~COutdoorLightScattering()
{
}

//******************************************************************
//FUNCTION:
float COutdoorLightScattering::computeScatteredLightSample(const Eigen::Vector3f& vLightPostion, const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget)
{
	__computeCameraDirection(vCameraPosition, vTarget);

	return 1.0;

}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computeCameraDirection(const Eigen::Vector3f& vCameraPosition, const Eigen::Vector3f& vTarget)
{
	vec3f Direction = vTarget - vCameraPosition;
	float Temp = Direction[0] * Direction[0] + Direction[1] * Direction[1] + Direction[2] * Direction[2];
	m_CameraDirection = vec3f(Direction[0]/Temp, Direction[1]/Temp, Direction[2]/Temp);
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
	vec3f EarthCenreToPointDir = vPosition - vEarthCentre;
	float DistToEarthCentre = EarthCenreToPointDir.dot(EarthCenreToPointDir);
	EarthCenreToPointDir /= DistToEarthCentre;

	float HeightAboveSurface = DistToEarthCentre - EARTH_RADIUS;
	float ParticleDensityR = exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_R);
	float ParticleDensityM = exp( -HeightAboveSurface / PARTICLE_SCALE_HEIGHT_M);
	voParticleDensity[0] = ParticleDensityR;
	voParticleDensity[1] = ParticleDensityM;

	float CosSunZenithAngleForCurrPoint = EarthCenreToPointDir.dot(vDirectionOnLight);
	
}

//******************************************************************
//FUNCTION:
void COutdoorLightScattering::__computePointDiffInscattering(vec2f vPraticleDensityIncurrPoint, vec2f vNetParticleDensityFromCam, vec2f vNetParticleDesityToAtmTop, vec3f& voRlghInscatering, vec3f& voMieInscatering)
{
	vec2f TotalParticleDensity = vNetParticleDensityFromCam + vNetParticleDesityToAtmTop;

//	vec3f TotalRlghOpticalDepth = 
}

void COutdoorLightScattering::computeInscatteringIntegral(vec3f vRayStart, vec3f vRayEnd, vec3f vEarthCentre, vec3f vDirOnLight, vec3f& voNetParticleFromCam, vec3f& voRayleighInscattering, vec3f& voMieInscattering, const float vNumSteps)
{
// 	vec3f Steps = (vRayEnd - vRayStart) / vNumSteps;
// 	float StepLen = sqrt(Steps[0]*Steps[0] + Steps[1]*Steps[1] + Steps[2]*Steps[2]);
// 
// 	for (float StepNum = 0.5; StepNum < vNumSteps; StepNum += 1.0)
// 	{
// 		vec3f CurrPos = vRayStart + Steps * StepNum;
// 
// 		vec2f ParticleDensity, NetParticleDensityToAtmTop;
// 		__getAtmosphereProperties(CurrPos, vEarthCentre, vDirOnLight, ParticleDensity, NetParticleDensityToAtmTop);
// 
// 		voNetParticleFromCam += ParticleDensity * StepLen;
// 
// 		vec3f RlghInsctr, MieInsctr;
// 		__computePointDiffInscattering(ParticleDensity, voNetParticleFromCam, NetParticleDensityToAtmTop, RlghInsctr, MieInsctr);
// 
// 		voRayleighInscattering += RlghInsctr * StepLen;
// 		voMieInscattering += MieInsctr * StepLen;
// 	}
}


//******************************************************************
//FUNCTION:
void COutdoorLightScattering::computeScatteringCoefficients()
{
	__computeRayleighScatteringCoefficennts();
	__computeMieScatteringCoefficennts();
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

	float g = m_AirScatteringAttribs.m_AerosolPhaseFuncG;
	m_AirScatteringAttribs.m_CornetteShanks[0] = 3*(1.0f - g*g) / ( 2*(2.0f + g*g) );
	m_AirScatteringAttribs.m_CornetteShanks[1] = 1.0f + g*g;
	m_AirScatteringAttribs.m_CornetteShanks[2] = -2.0f*g;
	m_AirScatteringAttribs.m_CornetteShanks[3] = 1.0f;

	m_AirScatteringAttribs.m_TotalExtinctionCoeff = m_AirScatteringAttribs.m_RayleighExtinctionCoeff + m_AirScatteringAttribs.m_MieExtinctionCoeff;
}
