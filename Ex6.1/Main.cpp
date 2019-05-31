#include <stdlib.h>
#include"glut.h"

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