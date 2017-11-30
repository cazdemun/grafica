#include "libs/glut.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

#define TIMERSECS 5000

using namespace std;

enum AXIS { X, Y, Z };
int cAxis = Z; //current axis

float w;
float h;
float near = 0.1;
float far = 50;

float axis[3] = { 0.0, 0.0, 1.0 };

bool specular = false;
bool lightposition = true;

static void
resize(int width, int height) {
	const float ar = (float)width / (float)height;
	w = width;
	h = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, ar, near, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void
hud(){
	system("cls");
	cout << "Eje actual " << cAxis << endl;
	cout << "Eje x: " << axis[X] << endl;
	cout << "Eje y: " << axis[Y] << endl;
	cout << "Eje z: "<< axis[Z] << endl;
	cout << "Specular: " << specular << endl;
	cout << "Light: " << lightposition << endl;

}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.5f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void
dibujarCubos(double a) {

	if (!lightposition) a = 45;

	glPushMatrix();
	glTranslated(0, -2, -6);
	glRotated(a, 0, 1, 0);
	glColor3d(1.0, 0.0, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2, -6);
	glRotated(a, 0, 1, 0);
	glColor3d(1.0, 0.0, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(2, 0, -6);
	glRotated(a, 0, 1, 0);
	glColor3d(1.0, 0.0, 1.0);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2, 0, -6);
	glRotated(a, 0, 1, 0);
	glColor3d(1.0, 0.0, 1.0);
	glutSolidCube(2);
	glPopMatrix();
}

static void
display(void)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*90.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat light_position[] = { axis[X], axis[Y], axis[Z], (float)lightposition };
	if(specular) glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glPushMatrix();
	glTranslated(axis[X], axis[Y], axis[Z]);
	glColor3d(1.0, 1.0, 1.0);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	dibujarCubos(a);

	glutSwapBuffers();
}

static void
idle(void)
{
	glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key) {
	case ' ':
		if (cAxis == Z)
			cAxis = Y;
		else
			cAxis = Z;
		break;
	case 's':
		specular = !specular;
		break;
	case 'd':
		lightposition = !lightposition;
		break;
	}
	hud();
	glutPostRedisplay();
}

void upAndDown(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		axis[cAxis] += 1 * ((cAxis == Z) ? -1 : 1);
		break;
	case GLUT_KEY_DOWN:
		axis[cAxis] -= 1 * ((cAxis == Z) ? -1 : 1);
		break;
	case GLUT_KEY_RIGHT:
		axis[X] += 1;
		break;
	case GLUT_KEY_LEFT:
		axis[X] -= 1;
		break;
	}
	hud();
	glutPostRedisplay();
}

int main() {
	hud();
	glutInitWindowSize(800, 420);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("FreeGLUT Shapes");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(upAndDown);
	//glutTimerFunc(TIMERSECS, animate, 0);

	glClearColor(0, 0, 0, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

	glutMainLoop();

	return EXIT_SUCCESS;
}