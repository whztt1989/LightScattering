#include "Shader.h"
#include "VBO.h"
#include "Pipeline.h"
#include "Texture.h"
#include "Light.h"
#include "Mesh.h"
#include "OutdoorLightScattering.h"
#include "UBO.h"
#include "LightAttribs.h"
#include "CameraAttribs.h"
#include "Depth_fbo.h"
#include "Coordinate_texture_fbo.h"

GLfloat g_LookPos[3] = {0.0, 0.0, 10.0};
GLfloat g_LookAt[3] = {0.0, 0.0, 0.0};
GLfloat g_Up[3] = {0.0, 1.0, 0.0};

GLint g_MouseX, g_MouseY;
GLint g_MotionX, g_MotionY;
GLboolean g_Rotated = false;

GLuint g_tex2DSliceEndPoints;
const int g_NumEpipolarSlices = 32;

SDirectionalLight DirectionalLight;
SPointLight PointLight;

CMesh* pMeshTerrainHouse;
CDepthFBO g_DepthFBO;
CCoordinateTextureFBO g_CoordinateTexFBO;

CShader* g_DepthShader = new CShader;
CShader* g_RenderShader = new CShader;
CShader* g_LightScatteringShader = new CShader;

glm::mat4 MVPLightSpace;
glm::mat4 LightViewMatrix;
glm::mat4 LightProjectionMatrix;

bool g_DepthFlag = false;
float g_angle = 0.707;
bool g_stopLightMove = true;

CUBO g_UBO;
COutdoorLightScattering* g_pOutdoorLightScattering = NULL;
CLightAttribs* g_pLight = new CLightAttribs;;
CCameraAttribs* g_pViewCamera = new CCameraAttribs;
CCameraAttribs* g_pLightCamera = new CCameraAttribs;


//******************************************************************
//FUNCTION:
void initGlew()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Fail to initialize GLEW" << std::endl;
		exit(0);
	}
}

//******************************************************************
//FUNCTION:
void initLight()
{
	PointLight.m_AmbientColor = glm::vec3(0.3, 0.3, 0.3);
	PointLight.m_DiffuseColor = glm::vec3(0.9, 0.9, 0.9);
	PointLight.m_SpecularColor = glm::vec3(1.0, 1.0, 1.0);
	PointLight.m_LightPosition = glm::vec3(3.0, 3.0, 3.0);
	PointLight.Attenuation.m_Constant = 1.0;
	PointLight.Attenuation.m_Linear = 0.1;
	PointLight.Attenuation.m_Quadratic = 0.0;

//	glm::vec4 LightDirection = glm::vec4(0.1, -0.1, 0.2, 0.0);
//	LightDirection = glm::normalize(LightDirection);
//	glm::vec4 LightWorldPos = glm::vec4(10.0, 10.0, 10.0, 1.0);;
	g_pLight->initSunLightForPointLight(PointLight);	
}


//******************************************************************
//FUNCTION:
void initOutdoorLightScattering()
{
	g_pOutdoorLightScattering = COutdoorLightScattering::getInstance();
	g_pOutdoorLightScattering->computeScatteringCoefficients();
}

//******************************************************************
//FUNCTION:
void initUBO()
{
	const char* pName[] = { "u_AngularRayleighScatteringCoeff",
		"u_TotalRayleighScatteringCoeff",
		"u_RayleighExtinctionCoeff",
		"u_AngularMieScatteringCoeff",
		"u_TotalMieScatteringCoeff",
		"u_MieExtinctionCoeff",
		"u_CornetteShanks",
		"u_TotalExtinctionCoeff",
		"u_AerosolPhaseFuncG",	
		"u_Turbidity"};
	g_UBO.initUBOBlock(g_LightScatteringShader->getProgramID(), "uAirScatteringAttribs", 10, pName);	
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_AngularRayleighScatteringCoeff);
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_TotalRayleighScatteringCoeff);
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_RayleighExtinctionCoeff);

	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_AngularMieScatteringCoeff);
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_TotalMieScatteringCoeff);
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_MieExtinctionCoeff);

	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_CornetteShanks);
	g_UBO.setUBOBlockValue4f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_TotalExtinctionCoeff);

	g_UBO.setUBOBlockValue1f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_AerosolPhaseFuncG);
	g_UBO.setUBOBlockValue1f(g_pOutdoorLightScattering->getAirScatteringAttribs().m_Turbidity);

	g_UBO.createUBO();
}

//******************************************************************
//FUNCTION:
void computeEndPointsOfEpipolarLine()
{
	_ASSERT((g_NumEpipolarSlices % 4) == 0);
	glm::vec4 f4SliceExitPoints[g_NumEpipolarSlices];

	f4SliceExitPoints[0] = glm::vec4(-1.0, -1.0, 0.0, 0.0);
	float Delta = (1.0-(-1.0)) / (g_NumEpipolarSlices/4);
	unsigned int Temp = g_NumEpipolarSlices / 4;

	for (unsigned int i=1; i<g_NumEpipolarSlices; i++)
	{
		if(i < Temp)
		{
			f4SliceExitPoints[i] = glm::vec4(-1.0+Delta*(i%8), -1.0, 0.0, 0.0);
		}
		else if(i<Temp*2)
		{
			f4SliceExitPoints[i] = glm::vec4(1.0, -1.0+Delta*(i%8), 0.0, 0.0);
		}
		else if(i<Temp*3)
		{
			f4SliceExitPoints[i] = glm::vec4(1.0-Delta*(i%8), 1.0, 0.0, 0.0);
		}
		else
		{
			f4SliceExitPoints[i] = glm::vec4(-1.0, 1.0-Delta*(i%8), 0.0, 0.0);
		}
	}

	glGenTextures(1, &g_tex2DSliceEndPoints);
	glBindTexture(GL_TEXTURE_1D, g_tex2DSliceEndPoints);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, WINDOW_WIDTH, 0, GL_RGBA32F, GL_FLOAT, f4SliceExitPoints);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_1D, 0);
}

//******************************************************************
//FUNCTION:
void initEyeCamera()
{
	g_pViewCamera->initCamera(glm::vec3(g_LookPos[0], g_LookPos[1], g_LookPos[2]), glm::vec3(g_LookAt[0], g_LookAt[1], g_LookAt[2]), glm::vec3(g_Up[0], g_Up[1], g_Up[2]));
}

//******************************************************************
//FUNCTION:
void init()
{
	//glClearColor(0.1, 0.1, 0.1, 1.0);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	initGlew();
	initLight();
	initEyeCamera();
//	computeEndPointsOfEpipolarLine();
// 
	pMeshTerrainHouse = new CMesh;
	if (!pMeshTerrainHouse->loadMesh("../Model/TerrainHouse.obj"))
		return;

	g_DepthShader->initShader("Shaders/0vertLightAndCameraDepth.glsl", "Shaders/0fragLightAndCameraDepth.glsl");
	g_RenderShader->initShader("Shaders/1vertrender.glsl", "Shaders/1fragrender.glsl");
//	g_LightScatteringShader->initShader("Shaders/0vertOutdoorlightScattering.glsl", "Shaders/0fragOutdoorlightScattering.glsl");
	if (!g_DepthFBO.initFBO(WINDOW_WIDTH, WINDOW_HEIGHT))
		return;
}

//******************************************************************
//FUNCTION:
void depthPass()
{
	g_DepthFBO.bindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	CPileline LightPileline;
	LightPileline.lookAt(glm::vec3(g_pLight->getLightWoldPos()), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	LightPileline.rotate(g_MotionX, 1.0, 0.0, 0.0);
	LightPileline.rotate(g_MotionY, 0.0, 1.0, 0);
	LightPileline.scale(0.01, 0.01, 0.01);
	LightPileline.perspective(60, 1, 1, 100);
	
	LightViewMatrix = LightPileline.getViewMat();
	LightProjectionMatrix = LightPileline.getProjectMat();

	glm::mat4 LightModel = LightPileline.getModelMat();
	glm::mat4 LightView = LightPileline.getViewMat();
	glm::mat4 LightProjection = LightPileline.getProjectMat();
	glm::mat4 LightMVP = LightProjection * LightView * LightModel;
	MVPLightSpace = LightMVP;

	CPileline CameraPileline;
	CameraPileline.lookAt(glm::vec3(g_LookPos[0], g_LookPos[1], g_LookPos[2]), glm::vec3(g_LookAt[0], g_LookAt[1], g_LookAt[2]), glm::vec3(0.0, 1.0, 0.0));
	CameraPileline.rotate(g_MotionX, 1.0, 0.0, 0.0);
	CameraPileline.rotate(g_MotionY, 0.0, 1.0, 0);
	CameraPileline.scale(0.01, 0.01, 0.01);
	CameraPileline.perspective(60, 1, 1, 100);
	glm::mat4 CameraModel = CameraPileline.getModelMat();
	glm::mat4 CamreaView = CameraPileline.getViewMat();
	glm::mat4 CameraProjection = CameraPileline.getProjectMat();

	g_DepthShader->useShader();
	g_DepthShader->setMatUniformValue("uLightModel", &LightModel[0][0]);
	g_DepthShader->setMatUniformValue("uLightView", &LightView[0][0]);
	g_DepthShader->setMatUniformValue("uLightProjection", &LightProjection[0][0]);	

	g_DepthShader->setMatUniformValue("uCameraModel", &CameraModel[0][0]);
	g_DepthShader->setMatUniformValue("uCameraView", &CamreaView[0][0]);
//	g_DepthShader->setMatUniformValue("uCameraProjection", &CameraProjection[0][0]);	
	pMeshTerrainHouse->render();
	
	g_DepthShader->banShader();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

//******************************************************************
//FUNCTION:
void generateShadow()
{
	CPileline Pileline;
	
	Pileline.lookAt(glm::vec3(g_LookPos[0], g_LookPos[1], g_LookPos[2]), glm::vec3(g_LookAt[0], g_LookAt[1], g_LookAt[2]), glm::vec3(0.0, 1.0, 0.0));
	Pileline.rotate(g_MotionX, 1.0, 0.0, 0.0);
	Pileline.rotate(g_MotionY, 0.0, 1.0, 0);
	Pileline.scale(0.01, 0.01, 0.01);
	Pileline.perspective(60, 1, 1, 100);

	glm::mat4 Model = Pileline.getModelMat();
	glm::mat4 View = Pileline.getViewMat();
	glm::mat4 Projection = Pileline.getProjectMat();
	glm::mat4 ModelView = View * Model;
	glm::mat4 NormalMat = glm::transpose(glm::inverse(ModelView));

	g_RenderShader->useShader();

	g_RenderShader->setTextureUniformValue("uTextureSampler", 0);
	g_DepthFBO.bindForReadingLightDepth(GL_TEXTURE1);
	g_RenderShader->setTextureUniformValue("uShadowMapSampler", 1);

	g_RenderShader->setMatUniformValue("uModel", &Model[0][0]);
	g_RenderShader->setMatUniformValue("uView", &View[0][0]);
	g_RenderShader->setMatUniformValue("uProjection", &Projection[0][0]);
	g_RenderShader->setMatUniformValue("uNormalMatEye", &NormalMat[0][0]);
	g_RenderShader->setMatUniformValue("uLightViewMat", &LightViewMatrix[0][0]);
	g_RenderShader->setMatUniformValue("uLightProjectionMatrix", &LightProjectionMatrix[0][0]);
	g_pLight->updateShaderUniformForPointLight(g_RenderShader);
	pMeshTerrainHouse->render();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	g_RenderShader->banShader();
}

//******************************************************************
//FUNCTION:
void renderPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (g_DepthFlag)
	{
		glLoadIdentity();
		gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		g_DepthFBO.bindForReadingLightDepth(GL_TEXTURE0);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);glVertex2f(-1.0, -1.0);
		glTexCoord2f(1.0, 0.0);glVertex2f(1.0, -1.0);
		glTexCoord2f(1.0, 1.0);glVertex2f(1.0, 1.0);
		glTexCoord2f(0.0, 1.0);glVertex2f(-1.0, 1.0);
		glEnd();
	}
	else
	{
		generateShadow();
	}
}

//******************************************************************
//FUNCTION:
void lightScatteringPass()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	CPileline Pileline;
	Pileline.lookAt(glm::vec3(g_LookPos[0], g_LookPos[1], g_LookPos[2]), glm::vec3(g_LookAt[0], g_LookAt[1], g_LookAt[2]), glm::vec3(0.0, 1.0, 0.0));
//	Pileline.translate(0.0, -2.0, 0.0);
	Pileline.rotate(g_MotionX, 1.0, 0.0, 0.0);
	Pileline.rotate(g_MotionY, 0.0, 1.0, 0);
//	Pileline.rotate(-60, 1.0, 0.0, 0.0);
	Pileline.scale(0.05, 0.05, 0.05);
	Pileline.perspective(60, 1, 1, 100);

// 	glLoadIdentity();
// 	gluLookAt(0.0, 0.0, 20000000, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
// 	glutWireSphere(6360000.0, 200, 400);
	 
 	glm::mat4 Model = Pileline.getModelMat();
 	glm::mat4 View = Pileline.getViewMat();
 	glm::mat4 Projection = Pileline.getProjectMat();
 	glm::mat4 ModelView = View * Model;
 	glm::mat4 NormalMat = glm::transpose(glm::inverse(ModelView));
 
 	g_LightScatteringShader->useShader();
 // 	g_LightScatteringShader->setTextureUniformValue("uTextureSampler", 0);
 // 	g_ShadowMapFBO.bindForReading(GL_TEXTURE1);
 // 	g_LightScatteringShader->setTextureUniformValue("uShadowMapSampler", 1);
 
 	g_LightScatteringShader->setMatUniformValue("uModel", &Model[0][0]);
 	g_LightScatteringShader->setMatUniformValue("uView", &View[0][0]);
 	g_LightScatteringShader->setMatUniformValue("uProjection", &Projection[0][0]);
 //	g_LightScatteringShader->setMatUniformValue("uNormalMatEye", &NormalMat[0][0]);
 
 
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_BaseLight.m_AmbientColor", PointLight.m_AmbientColor.x, PointLight.m_AmbientColor.y, PointLight.m_AmbientColor.z);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_BaseLight.m_DiffuseColor", PointLight.m_DiffuseColor.x, PointLight.m_DiffuseColor.y, PointLight.m_DiffuseColor.z);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_BaseLight.m_SpecularColor", PointLight.m_SpecularColor.x, PointLight.m_SpecularColor.y, PointLight.m_SpecularColor.z);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_LightPosition", PointLight.m_LightPosition.x, PointLight.m_LightPosition.y, PointLight.m_LightPosition.z);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Constant", PointLight.Attenuation.m_Constant);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Linear", PointLight.Attenuation.m_Linear);
 // 	g_LightScatteringShader->setFloatUniformValue("uPointLight.m_Attenuation.m_Quadratic", PointLight.Attenuation.m_Quadratic);
 // 	g_LightScatteringShader->setMatUniformValue("uLightViewMat", &LightViewMatrix[0][0]);
 // 	g_LightScatteringShader->setMatUniformValue("uLightProjectionMatrix", &LightProjectionMatrix[0][0]);
 
	pMeshTerrainHouse->render();
 
 	g_LightScatteringShader->banShader();
}

//******************************************************************
//FUNCTION:
void display()
{
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
// 
// 	glm::vec4 f4DirectionOnSun = g_pLight->getDirectionOnSun();
// 	_ASSERT(f4DirectionOnSun.w == 0);


//	g_pOutdoorLightScattering->render();

	depthPass();
	renderPass();
	//glDisable(GL_TEXTURE_2D);	


	glm::mat4 EyeViewProjT = g_pViewCamera->getProjectionT() * g_pViewCamera->getViewT();
	glm::vec4 f4SunScreenPos = EyeViewProjT * g_pLight->getLightWoldPos();
	f4SunScreenPos = f4SunScreenPos  / f4SunScreenPos.w;

	glPushMatrix();
	glLoadIdentity();
	gluLookAt(g_LookPos[0], g_LookPos[1], g_LookPos[2], g_LookAt[0], g_LookAt[1], g_LookAt[2], 0.0, 1.0, 0.0);
	glTranslatef(g_pLight->getLightWoldPos().x, g_pLight->getLightWoldPos().y, g_pLight->getLightWoldPos().z);
	//glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(0.1, 20, 40);
	glPopMatrix();

//	lightScatteringPass();

	glutSwapBuffers();
}

//******************************************************************
//FUNCTION:
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_LookPos[2] -= 0.1;
		g_LookAt[2] -= 0.1;
		break;
	case 's':
		g_LookPos[2] += 0.1;
		g_LookAt[2] += 0.1;
		break;
	case 'a':
		g_LookPos[0] -= 0.1;
		g_LookAt[0] -= 0.1;
		break;
	case 'd':
		g_LookPos[0] += 0.1;
		g_LookAt[0] += 0.1;
		break;
	case 'q':
		g_LookPos[1] -= 0.1;
		g_LookAt[1] -= 0.1;
		break;
	case 'e':
		g_LookPos[1] += 0.1;
		g_LookAt[1] += 0.1;
		break;
	case 'b':
		g_DepthFlag = !g_DepthFlag;
		break;
	case 'p':
		g_stopLightMove = !g_stopLightMove;
		break;
	default:
		break;
	}

	g_pViewCamera->updateCamera(glm::vec3(g_LookPos[0], g_LookPos[1], g_LookPos[2]), glm::vec3(g_LookAt[0], g_LookAt[1], g_LookAt[2]), glm::vec3(g_Up[0], g_Up[1], g_Up[2]));
	glutPostRedisplay();
}

//******************************************************************
//FUNCTION:

void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			g_MouseX = x;
			g_MouseY = y;
			g_Rotated = true;
		}
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//******************************************************************
//FUNCTION:
void motion(int x, int y)
{
	if (g_Rotated)
	{
		g_MotionX = (g_MotionX + (y - g_MouseY)) % 360;
		g_MotionY = (g_MotionY + (x - g_MouseX)) % 360;

		g_MouseX = x;
		g_MouseY = y;
	}

	glutPostRedisplay();
}

//******************************************************************
//FUNCTION:
void reshape(int vWidth, int vHeight)
{
	glViewport(0, 0, (GLsizei)vWidth, (GLsizei)vHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, vWidth/vHeight, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//******************************************************************
//FUNCTION:
void idleLightPositionChange()
{
	if(!g_stopLightMove)
	{
		g_angle += 3.14/180;
		if( g_angle >= 6.28f )
			g_angle = 0.0f;
	}

	PointLight.m_LightPosition.x = 3.0 * sin(g_angle); 
	PointLight.m_LightPosition.z = 3.0 * cos(g_angle);
	g_pLight->updateLightPos(PointLight.m_LightPosition);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
    Magick::InitializeMagick(*argv);    
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Light Scattering");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutReshapeFunc(reshape);
//	glutIdleFunc(idleLightPositionChange);

	glutMainLoop();
	return 1;
}

