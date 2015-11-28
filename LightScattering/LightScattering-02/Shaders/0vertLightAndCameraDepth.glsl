#version 400

layout (location = 0) in vec3 _Position;

uniform mat4 uLightModel;
uniform mat4 uLightView;
uniform mat4 uLightProjection;

uniform mat4 uCameraModel;
uniform mat4 uCameraView;
//uniform mat4 uCameraProjection;

out float DepthL;
out float CameraZ;

void main()
{
	vec4 Depth =  uLightView * uLightModel * vec4(_Position, 1.0);
	DepthL = Depth.z;

	vec4 Camera = uCameraView * uCameraModel * vec4(_Position, 1.0);
	CameraZ = Camera.z;

	gl_Position = uLightProjection * uLightView * uLightModel * vec4(_Position, 1.0);
	//gl_Position = uCameraProjection * uCameraView * uCameraModel * vec4(_Position, 1.0);
}