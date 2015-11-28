#version 400

in float DepthL;
in float CameraZ;

void main()
{
    gl_FragData[0] = vec4(-DepthL, -DepthL, -DepthL, 1.0);
	gl_FragData[1] = vec4(-CameraZ, 0.0, 0.0, 1.0);
	//FragColor = vec4(DepthL, DepthL, DepthL, 1.0);
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}