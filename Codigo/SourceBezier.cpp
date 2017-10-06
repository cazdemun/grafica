#include <iostream>
#include <stdlib.h>
#include "libs\glut.h"
#include <Windows.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include "Punto2D.h"

using namespace std;

int SCREEN_HEIGHT = 480;
int NUMPOINTS = 0;
vector<Punto2D> puntosControl;

void myInit() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(4.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void DibujaPunto(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

void DibujaLinea(Punto2D p1, Punto2D p2) {
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
	glFlush();
}

int Factorial(int x) {
	int fac = 1;
	for (int i = 2;i <= x;i++) fac *= i;
	return fac;
}

int CoeficienteBinomial(int n, int i) {
	int a = Factorial(n);
	int b = Factorial(i);
	int c = Factorial(n - i);
	int cb = a / (b*c);
	return cb;
}

Punto2D DibujaBezier(double t) {
	Punto2D P;
	P.init();
	int n = NUMPOINTS - 1;
	for (int i = 0;i < NUMPOINTS;i++) {
		P.x += CoeficienteBinomial(n, i)*(puntosControl[i].x)*(pow(1 - t, n - i))*(pow(t, i));
		P.y += CoeficienteBinomial(n, i)*(puntosControl[i].y)*(pow(1 - t, n - i))*(pow(t, i));
	}
	return P;
}

void MyKey(int key, int x, int y) {
	if (key == GLUT_KEY_F1 && puntosControl.size() > 2) {
		glColor3f(1.0, 1.0, 1.0);
		for (int i = 0; i < NUMPOINTS - 1;i++)
			DibujaLinea(puntosControl[i], puntosControl[i + 1]);

		glColor3f(0.0, 0.0, 1.0);
		Punto2D POld = puntosControl[0];
		for (double t = 0.0;t <= 1.0;t += 0.005) {
			Punto2D P = DibujaBezier(t);
			DibujaLinea(POld, P);
			POld = P;
		}

		glColor3f(1.0, 0.0, 0.0);
	}
	if (key == GLUT_KEY_F2) {
		puntosControl = vector<Punto2D>();
		NUMPOINTS = 0;
	}
}

void MyMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Punto2D aux;
		aux.setxy((float)x, (float)(SCREEN_HEIGHT - y));
		puntosControl.push_back(aux);
		NUMPOINTS++;

		DibujaPunto(x, SCREEN_HEIGHT - y);
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Curva de Bezier: ");

	glutMouseFunc(MyMouse);
	glutDisplayFunc(myDisplay);
	glutSpecialFunc(MyKey);

	myInit();
	glutMainLoop();
	return 0;
}