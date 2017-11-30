#include <stdlib.h>
#include <math.h>
#include "libs\glut.h"

int spin = 0;

void init(void)
{

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	// PASO 1: ENABLE LIGHTING
	glEnable(GL_LIGHTING);
	// PASO 2: SELECT LIGHT SOURCES
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.0);

	glPushMatrix();
	glRotated((GLdouble)spin, 0.0, 1.0, 0.0);

	// PASO 3: SET LIGHT POSITION
	GLfloat position[] = { 1.0, 1.0, 1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glTranslated(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 1.0);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	glutSolidTorus(0.275, 0.85, 8, 15);
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			spin = (spin + 30) % 360;
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

int main()
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Lights 2");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
