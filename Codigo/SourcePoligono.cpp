#include<iostream>
#include <stdlib.h>
#include "libs\glut.h"
#include "cliplines.h"
#include <math.h>

using namespace std;

int cont = 0;

void initRendering() {
	//glColor(R,G,B,alfa)
	glClearColor(0, 0, 0, 0);
}

void dibujarPoligono(int lados){

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < lados; i++) {
		glVertex3f(10*cos(i * 2 * 3.1416 / lados) + 15, 10*sin(i * 2 * 3.1416 / lados) + 15, 0);
		glVertex3f(10*cos((i + 1) * 2 * 3.1416 / lados) + 15, 10*sin((i + 1) * 2 * 3.1416 / lados) + 15, 0);
	}
	glEnd();
}

void key(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	}

	glutPostRedisplay();
}

void Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 200.0);
}

void Display() {
	int i, j;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-15, -15, -40);
	glColor3f(0.3f, 0.3f, 0.3f);

	cout << cont << endl;
	cont++;

	dibujarPoligono(3 + cont / 1000);

	glutSwapBuffers();
	glFlush();
}

static void
idle(void)
{
	glutPostRedisplay();
}


/* Program entry point */

int
main() {
	//Step 1: Initialize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	//Step2: Create Windows
	glutCreateWindow("Cohen-Sutherland Algorithm");
	initRendering();

	//Step3: Handlers functions
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);

	//Step4: Main Loop
	glutMainLoop();

	return EXIT_SUCCESS;
}