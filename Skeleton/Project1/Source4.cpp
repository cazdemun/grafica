#include "libs/glut.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

#define TIMERSECS 5000

using namespace std;

static int slices = 1;
static int stacks = 1;
int ds = 1;

float w;
float h;
float near = 0.1;
float far = 50;
double dd = -20;
double d = dd;
double dx = 0.01;
double st = -6;

double c1 = 0;
double c2 = 0.2;
double c3 = 0.5;
double dc1 = 0.01;
double dc2 = 0.02;
double dc3 = 0.03;

int lados = 2;

bool flag = false;

int r = 0;

class Meteor {

	float x, y, z;
	float dz;

	Meteor();

};

static void
resize(int width, int height) {
	const float ar = (float)width / (float)height;
	w = width;
	h = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, ar, near, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void dibujar_tunel(double d, double a) {

	int lad = 10;

	int depth = 16;

	for (int i = 0; i < lad; i++) {
		for (int j = -depth; j <= depth; j++)
		{
			glPushMatrix();
			//glRotated(a, 0, 0, 1);
			
			glRotated(r, 0, 0, 1);
			//glRotated(20, 0, 1, 0);
			//glRotated(a, 0, 1, 0);
			//glRotated(a, 1, 0, 0);
			glTranslated(2 * cos(i * 2 * 3.1416 / lad), 2 * sin(i * 2 * 3.1416 / lad), -6 + d + 2 * j);
			glRotated(r, 0, 1, 0);
			if (flag == true) {
				glutWireSphere(1, slices, stacks);
				glutWireCube(1);
			}
			else {
				glutSolidCube(1);
			}
			glPopMatrix();
		}
	}

}

static void
display(void)
{
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*90.0;

	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (flag == true) {
		r += 1;
		c1 += dc1;
		c2 += dc2;
		c3 += dc3;
		st += 0.004;
		glColor3d(c1, c2, c3);
		glPushMatrix();
		glTranslated(0, 0, st);
		glRotated(a, 1, 1, 1);
		glutWireSphere(0.5, slices + 1, stacks + 1);
		glutSolidTorus(0.1, 0.5, slices + 1, stacks + 1);
		//glutWireTeapot(1.0f);
		glPopMatrix();

	}
	else
	{
		if (r % 180 != 0) r += 1;
		
		glColor3d(c1, c2, c3);
		glPushMatrix();
		glTranslated(0, 0, -6);
		glRotated(a, 1, 1, 1);
		glutWireSphere(0.5, slices + 1, stacks + 1);
		//glutSolidTorus(0.1, 0.5, slices + 1, stacks + 1);
		//glutWireTeapot(1.0f);
		glPopMatrix();

		c1 += dc1 / 5;
		c2 += dc2 / 5;
		c3 += dc3 / 5;
	}

	d += dx;

	//system("cls");
	//cout << c1 << " " << c2 << " " << c3;

	if (c1 > 1 || c1 < 0) dc1 *= -1;
	if (c3 > 1 || c3 < 0) dc3 *= -1;
	if (c2 > 1 || c2 < 0) dc2 *= -1;


	glColor3d(c1, c2, c3);

	if (d >= 4) { d = dd; }
	//if (r >= 360 / 2) {
	//	r = r % 180;
	//	//flag = false;
	//}
	dibujar_tunel(d, a);

	glutSwapBuffers();
}

static void
idle(void)
{
	glutPostRedisplay();
}

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };
//const GLfloat light_position[] = { 0.0f, 0.0f, 5.0f, 0.0f };
const GLfloat light_position[] = { 0.0f, 0.1f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

void animate(int value) {
	glutTimerFunc(TIMERSECS, animate, 1);
	flag = true;
	glutPostRedisplay();
}

void slow(int value);

void speed(int value) {
	glutTimerFunc(TIMERSECS, slow, 1);

	slices += ds;
	stacks += ds;
	system("cls");
	cout << slices << '\n';
	cout << glutGet(GLUT_ELAPSED_TIME) / 1000;
	if (slices >= 10 || slices <= 1) ds *= -1;
	
	flag = true;
	dx = 0.05;
	glutPostRedisplay();
}

void slow(int value) {
	glutTimerFunc(TIMERSECS, speed, 1);
	flag = false;
	dx = 0.01;
	st = -6;

	glutPostRedisplay();
}

int main() {
	glutInitWindowSize(800, 420);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("FreeGLUT Shapes");

	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	//glutTimerFunc(TIMERSECS, animate, 0);
	glutTimerFunc(TIMERSECS, speed, 0);
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
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	glutMainLoop();

	return EXIT_SUCCESS;
}