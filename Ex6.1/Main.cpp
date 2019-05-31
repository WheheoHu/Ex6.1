#include <stdlib.h>
#include"glut.h"
#include "gltools.h"
#include <gl/GL.h>
#include <gl/GLU.h>

static GLfloat zPos = -60.0f;
#define GL_CLAMP_TO_EDGE 0x812F
constexpr int TEXTURE_BRICK = 0;
constexpr int TEXTURE_FLOOR = 1;
constexpr int TEXTURE_CEILING = 2;
constexpr int TEXTURE_COUNT = 3;

GLuint  textures[TEXTURE_COUNT];
const char *szTextureFiles[TEXTURE_COUNT] = { "brick.tga", "floor.tga", "ceiling.tga" };

//callback func
void ReshapeFuncCB(int w, int h);
void DisplayFuncCB();
void SetupRC();
void ShutdownRC();

int main() {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Textrue");
	glutReshapeFunc(ReshapeFuncCB);
	glutDisplayFunc(DisplayFuncCB);
	
	
	SetupRC();
	glutMainLoop();
	ShutdownRC();

	return 0;
}

void ReshapeFuncCB(int w, int h)
{
	GLfloat fAspect;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	fAspect = (GLfloat)w / (GLfloat)h;

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Produce the perspective projection
	gluPerspective(90.0f, fAspect, 1, 120);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DisplayFuncCB()
{
	GLfloat z;

	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT);

	// Save the matrix state and do the rotations
	glPushMatrix();
	// Move object back and do in place rotation
	glTranslatef(0.0f, 0.0f, zPos);

	// Floor
	for (z = 60.0f; z >= 0.0f; z -= 10)
	{
		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLOOR]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, -10.0f, z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, -10.0f, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10.0f, -10.0f, z - 10.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10.0f, -10.0f, z - 10.0f);
		glEnd();

		// Ceiling
		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CEILING]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10.0f, 10.0f, z - 10.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10.0f, 10.0f, z - 10.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, 10.0f, z);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, 10.0f, z);
		glEnd();


		// Left Wall
		glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_BRICK]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-10.0f, -10.0f, z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-10.0f, -10.0f, z - 10.0f);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-10.0f, 10.0f, z - 10.0f);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-10.0f, 10.0f, z);
		glEnd();


		// Right Wall
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(10.0f, 10.0f, z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(10.0f, 10.0f, z - 10.0f);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(10.0f, -10.0f, z - 10.0f);

		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(10.0f, -10.0f, z);
		glEnd();
	}

	// Restore the matrix state
	glPopMatrix();

	// Buffer swap
	glutSwapBuffers();
}

void SetupRC()
{
	GLbyte *pBytes;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;
	GLint iLoop;

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Textures applied as decals, no lighting or coloring effects
	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	// Load textures
	glGenTextures(TEXTURE_COUNT, textures);
	for (iLoop = 0; iLoop < TEXTURE_COUNT; iLoop++)
	{
		// Bind to next texture object
		glBindTexture(GL_TEXTURE_2D, textures[iLoop]);

		// Load texture, set filter and wrap modes
		pBytes = gltLoadTGA(szTextureFiles[iLoop], &iWidth, &iHeight, &iComponents, &eFormat);
		gluBuild2DMipmaps(GL_TEXTURE_2D, iComponents, iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pBytes);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Don't need original texture data any more
		free(pBytes);
	}
}

void ShutdownRC()
{
	
	glDeleteTextures(TEXTURE_COUNT, textures);
}
