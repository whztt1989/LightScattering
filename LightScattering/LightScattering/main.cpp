#include "Shader.h"
#include "UBO.h"
#include "OutdoorLightScattering.h"
#pragma comment(lib, "glew32.lib")

GLfloat g_Lookat[9];
GLint g_MouseX, g_MouseY;
GLint g_MotionX, g_MotionY;
GLboolean g_Rotated = false;
CShader g_Shader;
CUBO g_UBO;
GLuint g_VBO;
COutdoorLightScattering* g_pOutdoorLightScattering = NULL;

// vec3f g_RayStart = vec3f(0.0, 6360000, 0.0);
// vec3f g_RayEnd = vec3f(5000.0, 6368000, 0.0);
vec3f g_RayStart = vec3f(0.0, 0.0, 6380000.0);
vec3f g_RayEnd = vec3f(0.0, 0.0, 6360000.0);
vec3f g_EarthCentre = vec3f(0.0, 0.0, 0.0);
vec3f g_LightDir = vec3f(-1.0, -1.0, 0.0);

//******************************************************************
//FUNCTION:
void initGlew()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew init fail" << std::endl;
		exit(0);
	}
}

//******************************************************************
//FUNCTION:
void initCamera()
{
	g_Lookat[0] = 0.0;
	g_Lookat[1] = 0.0;
	g_Lookat[2] = 10.0;
	g_Lookat[3] = 0.0;
	g_Lookat[4] = 0.0;
	g_Lookat[5] = 0.0;
	g_Lookat[6] = 0.0;
	g_Lookat[7] = 1.0;
	g_Lookat[8] = 0.0;
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
	g_UBO.initUBOBlock(g_Shader.getProgramID(), "uAirScatteringAttribs", 10, pName);	
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
void createVertexBuffer()
{
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	
}

//******************************************************************
//FUNCTION:
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
//	glClearColor(0.0, 0.0, 0.0, 1.0);
	initGlew();
	initCamera();

	initOutdoorLightScattering();
	
	g_Shader.initShader("0vertOutdoorlightScattering.glsl", "0fragOutdoorlightScattering.glsl");
	initUBO();
}

//******************************************************************
//FUNCTION:
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(g_Lookat[0], g_Lookat[1], g_Lookat[2], g_Lookat[3], g_Lookat[4], g_Lookat[5], g_Lookat[6], g_Lookat[7], g_Lookat[8]);

	glPushMatrix();
	glRotatef(g_MotionX, 1.0, 0.0, 0.0);
	glRotatef(g_MotionY, 0.0, 1.0, 0.0);
	//glutSolidCube(1);
	g_Shader.useShader();
	g_Shader.setFloatUniformValue("uRayStart", g_RayStart[0], g_RayStart[1], g_RayStart[2]);
	g_Shader.setFloatUniformValue("uRayEnd", g_RayEnd[0], g_RayEnd[1], g_RayEnd[2]);
	g_Shader.setFloatUniformValue("uLightDir", g_LightDir[0], g_LightDir[1], g_LightDir[2]);
	g_Shader.setFloatUniformValue("uEarthCentre", g_EarthCentre[0], g_EarthCentre[1], g_EarthCentre[2]);
	glutWireSphere(1.0, 30, 40);
	//glutSolidSphere(1.0, 30, 40);
	g_Shader.banShader();
	glPopMatrix();
	glutSwapBuffers();
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
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		g_Lookat[1] += 0.1;
		break;
	case 'a':
		g_Lookat[0] -= 0.1;
		break;
	case 's':
		g_Lookat[1] -= 0.1;
		break;
	case 'd':
		g_Lookat[0] += 0.1;
		break;
	case 'q':
		g_Lookat[2] += 0.1;
		break;
	case 'e':
		g_Lookat[2] -= 0.1;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

//******************************************************************
//FUNCTION:
void reshape(int width, int height)
{
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLdouble)width/(GLdouble)height, 1, 100);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow(argv[0]);

	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutMainLoop();
	return 1;
}