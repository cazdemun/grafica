#include "libs/glut.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

static int slices = 16;
static int stacks = 16;

float w;
float h;
float near = 2;

static void
resize(int width, int height) {
	const float ar = (float)width / (float)height;
	w = width;
	h = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	gluPerspective(80, ar, near, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void
dibujarCuadra(double centrox, double centroy, double centroz, double radio ) {
	int zbase = -6;
	int altura = -2;
	double lado = 0.2;

	//El radio es el número de cubos que tiene de radio, no la longitud

	for (int i = -radio; i <= radio; i++)
	{
		glPushMatrix();
		glTranslated(centrox + lado * i, altura, zbase + radio * lado);
		glutWireCube(lado);
		glPopMatrix();
		
		glPushMatrix();
		glTranslated(centrox + lado * i, altura, zbase - radio * lado);
		glutWireCube(lado);
		glPopMatrix();
	}

	for (int i = -radio; i <= radio; i++)
	{
		glPushMatrix();
		glTranslated(radio * lado, altura, zbase + lado * i);
		glutWireCube(lado);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-radio * lado, altura, zbase + lado * i);
		glutWireCube(lado);
		glPopMatrix();
	}
}


static void
display(void)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*90.0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(0, -1.2, 2); //punto inicial
	glVertex3f(0, -1.2, 10); //punto final
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(-400, 0, -6); //punto inicial
	glVertex3f(400, 0, -6); //punto final
	glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glVertex3f(0, -210, -6); //punto inicial
	glVertex3f(0, 210, -6); //punto final
	glEnd();

	glColor3d(1, 1, 1);

	dibujarCuadra(0, -2, -6, 5);

	//1
	glPushMatrix();
	glTranslated(-3.4, 1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutSolidSphere(1, slices, stacks);
	glPopMatrix();
	//2
	glPushMatrix();
	glTranslated(-1, 1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutSolidCone(1, 1, slices, stacks);
	glPopMatrix();
	//3
	glPushMatrix();
	glTranslated(1.4, 1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutSolidTorus(0.2, 0.8, slices, stacks);
	glPopMatrix();


	//4
	glPushMatrix();
	glTranslated(-3.4, -1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutWireSphere(1, slices, stacks);
	glPopMatrix();
	//5
	glPushMatrix();
	glTranslated(-1, -1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutWireCone(1, 1, slices, stacks);
	glPopMatrix();
	//6
	glPushMatrix();
	glTranslated(1.4, -1.2, -6);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutWireTorus(0.2, 0.8, slices, stacks);
	glPopMatrix();
	//7
	glPushMatrix();
	glTranslated(4.4, 1.2, -6);
	glRotated(180, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutSolidTeapot(0.8f);
	glPopMatrix();

	glPushMatrix();
	glTranslated(4.4, -1.2, -6);
	glRotated(90, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	glutWireTeapot(0.8f);
	glPopMatrix();

	glutSwapBuffers();
}


static void
key(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(0);
		break;

	case 'a':
		slices++;
		stacks++;
		break;
	case 'w':
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(80, w/h, ++near, 10.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	case 's':
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(80, w / h, --near, 10.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		break;
	case 'd':
		if (slices>3 && stacks>3)
		{
			slices--;
			stacks--;
		}
		break;
	}

	glutPostRedisplay();
}

static void
idle(void)
{
	glutPostRedisplay();
}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };


int main() {
	glutInitWindowSize(800, 420);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("FreeGLUT Shapes");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	glClearColor(1, 1, 1, 1);
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
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glutMainLoop();

	return EXIT_SUCCESS;
}