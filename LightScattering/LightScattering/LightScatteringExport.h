#ifdef LightScattering_EXPORTS
#define LightScattering_DLL_EXPORT __declspec(dllexport)
#else
#define LightScattering_DLL_EXPORT __declspec(dllimport)
#endif