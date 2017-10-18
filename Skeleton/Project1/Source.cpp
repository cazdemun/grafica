#include<iostream>
#include "libs\glut.h"
using namespace std;

void initRendering() {
	//glColor(R,G,B,alfa)
	glClearColor(0, 0, 0, 0);
}

void Reshape(int w, int h) {
	//tarea: transformacion de coordenadas(geometria analitica)
	glViewport(0, 0, w, h); //puerto de visualizacion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//modifica la perspectiva en frustum (por revisar)
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 1.0, 200.);
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//Cargar la matriz de identidad
	glLoadIdentity();
	//Realizar una translacion de la escena(model, camara )
	glTranslated(-15, -15, -40);
	glColor3f(0.3f, 0.3f, 0.3f);
	//Inicializa el moto para graficar lineas
	//Grafica lineaas horizontales
	glBegin(GL_LINES);
	for (int i = 0; i <= 30; i++) {
		glVertex3f(0, i, 0); //punto inicial
		glVertex3f(30, i, 0); //punto final
	}
	glEnd();
	//Tarea: lineas verticales
	glBegin(GL_LINES);
	for (int i = 0; i <= 30; i++) {
		glVertex3f(i, 0, 0); //punto inicial
		glVertex3f(i, 30, 0); //punto final
	}
	glEnd();
	//swap
	glutSwapBuffers();
	glFlush();
}

int main() {
	//step 1: initilize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	//step 2: Create Window
	glutCreateWindow("Skeleton - ejercicio ejemplo");
	//initRendering();

	//step 3: Callbacks - Setear maquina de estados
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	//step 4: Main loop
	glutMainLoop();

	return(0);
}