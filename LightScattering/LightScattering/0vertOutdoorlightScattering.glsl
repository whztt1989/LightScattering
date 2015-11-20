#version 430 compatibility

layout(location = 0) in vec3 Position;

//in vec3 iCameraPosition;
//in vec3 iCameraRayEnd;
//in vec3 iDirOnLight;

vec2 ProjToUV(const in vec2 vProjToUVXY)
{
	return vec2(0.5, 0.5) + vec2(0.5, 0.5) * vProjToUVXY;
}

vec2 UVToProj(const in vec2 vUV)
{
	return vec2(-1.0, -1.0) + vec2(2.0, 2.0) * vUV;
}

void main()
{
	//RayStart = iCameraPosition;
	//RayEnd = iCameraRayEnd;
	//DirOnLight = iDirOnLight;

	gl_Position = ftransform();
}