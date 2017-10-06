#include<iostream>
#include "libs\glut.h"
#include "Poligono2D.h"
#include "Malla2D.h"
using namespace std;

float v_x, v_y, v_z;
int gridX, gridY;
int ips, ivs;
Punto2D base;
Malla2D M;
Poligono2D P;

int ipb;
int tipoTransformacion;
int rotar;
int espesor;

void dibujarArista(Punto2D* v1, Punto2D* v2)
{
	glBegin(GL_LINES);
	glVertex3d(v1->getX(), v1->getY(), 0);
	glVertex3d(v2->getX(), v2->getY(), 0);
	glEnd();
}

void dibujarPoligono(Poligono2D P)
{
	int i;
	for (i = 0; i<P.CantidadVertices() - 1;i++) 
		dibujarArista(&P.getVertice(i), &P.getVertice(i + 1));
	dibujarArista(&P.getVertice(i), &P.getVertice(0));
	
}

void dibujarMalla(Malla2D M)
{
	int i;
	for (i = 0; i<M.getCantidadPoligonos();i++)
		dibujarPoligono(M.getPoligono(i));
}

void initRendering() {
	//glColor(R,G,B,alfa)
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
}

void Reshape(int w, int h) {
	//tarea: transformacion de coordenadas(geometria analitica)
	glViewport(0, 0, w, h); //puerto de visualizacion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//modifica la perspectiva en frustum (por revisar)
	gluPerspective(45.0, (GLdouble)w / (GLdouble)h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); // matriz de la camara
	//Cargar la matriz de identidad
	glLoadIdentity(); // setear la matriz
	//Realizar una translacion de la escena(model, camara)
	glTranslatef(v_x, v_y, v_z);
	glColor3f(0.3f, 0.3f, 0.3f);
	//Inicializa el moto para graficar lineas


	//Grafica lineaas horizontales
	glBegin(GL_LINES);
	for (int i = 0; i <= 30; i++) {
		glVertex3f(0, i, 0); //punto inicial
		glVertex3f(30, i, 0); //punto final
	}
	glEnd();
	 //lineas verticales
	glBegin(GL_LINES);
	for (int i = 0; i <= 30; i++) {
		glVertex3f(i, 0, 0); //punto inicial
		glVertex3f(i, 30, 0); //punto final
	}
	glEnd();



	base.set(M.getPoligono(ips).getVertice(ivs));
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex3d(base.getX() - 0.5, base.getY() - 0.5, 0);
	glVertex3d(base.getX() + 0.5, base.getY() - 0.5, 0);
	glVertex3d(base.getX() + 0.5, base.getY() + 0.5, 0);
	glVertex3d(base.getX() - 0.5, base.getY() + 0.5, 0);
	glEnd();

	base.set(M.getPoligono(ips).getVertice(ivs));

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex3d(base.getX() - 0.5f, base.getY() - 0.5f, 0);
	glVertex3d(base.getX() + 0.5f, base.getY() - 0.5f, 0);
	glVertex3d(base.getX() + 0.5f, base.getY() + 0.5f, 0);
	glVertex3d(base.getX() - 0.5f, base.getY() + 0.5f, 0);
	glEnd();
	
	glClear(GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 0);

	dibujarMalla(M);

	glClear(GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);
	dibujarPoligono(M.getPoligono(ips));


	glutSwapBuffers();
	glFlush();
	cout << "hola";
}

void keyPressed(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_END:
		exit(0);
		break;
	case GLUT_KEY_F1:
		ivs = (ivs + 1) % M.getPoligono(ips).CantidadVertices();
		break;
	case GLUT_KEY_F2:
		ips = (ips + 1) % M.getCantidadPoligonos();
		break;
	case GLUT_KEY_PAGE_UP:
		v_z = v_z - 1;
		break;
	case GLUT_KEY_PAGE_DOWN:
			v_z = v_z + 1;
		break;
	case GLUT_KEY_RIGHT:
			M.Trasladar(base, 0.1, 0);
		break;
	case GLUT_KEY_LEFT:
			M.Trasladar(base, -0.1, 0);
		break;
	case GLUT_KEY_UP:
			M.Trasladar(base, 0, 0.1);
		break;
	case GLUT_KEY_DOWN:
			M.Trasladar(base, 0, -0.1);
		break;
	}
	glutPostRedisplay();
}

void idle(void)
{
	glutPostRedisplay();
}

int main() {
	//step 1: initilize GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);

	gridX = 30;
	gridY = 30;
	v_x = -gridX / 2;
	v_y = -gridY / 2;
	v_z = -40;


	// 0
	P = Poligono2D(3);
	P.setVertice(0, 3, 2);
	P.setVertice(1, 8, 10);
	P.setVertice(2, 3, 11);
	M.addPoligono(P);

	// 1
	P = Poligono2D(3);
	P.setVertice(0, 3, 2);
	P.setVertice(1, 13, 2);
	P.setVertice(2, 8, 10);
	M.addPoligono(P);

	// 2
	P = Poligono2D(3);
	P.setVertice(0, 8, 10);
	P.setVertice(1, 13, 2);
	P.setVertice(2, 13, 12);
	M.addPoligono(P);

	// 3
	P = Poligono2D(3);
	P.setVertice(0, 13, 2);
	P.setVertice(1, 21, 15);
	P.setVertice(2, 13, 12);
	M.addPoligono(P);

	// 4
	P = Poligono2D(3);
	P.setVertice(0, 13, 2);
	P.setVertice(1, 28, 2);
	P.setVertice(2, 21, 15);
	M.addPoligono(P);

	// 5
	P = Poligono2D(3);
	P.setVertice(0, 3, 11);
	P.setVertice(1, 8, 10);
	P.setVertice(2, 6, 15);
	M.addPoligono(P);

	// 6
	P = Poligono2D(3);
	P.setVertice(0, 8, 10);
	P.setVertice(1, 13, 12);
	P.setVertice(2, 6, 15);
	M.addPoligono(P);

	// 7
	P = Poligono2D(3);
	P.setVertice(0, 13, 12);
	P.setVertice(1, 21, 15);
	P.setVertice(2, 6, 15);
	M.addPoligono(P);

	// 8
	P = Poligono2D(3);
	P.setVertice(0, 3, 11);
	P.setVertice(1, 6, 15);
	P.setVertice(2, 3, 23);
	M.addPoligono(P);

	// 9
	P = Poligono2D(3);
	P.setVertice(0, 6, 15);
	P.setVertice(1, 13, 23);
	P.setVertice(2, 3, 23);
	M.addPoligono(P);

	// 10
	P = Poligono2D(3);
	P.setVertice(0, 6, 15);
	P.setVertice(1, 21, 15);
	P.setVertice(2, 13, 23);
	M.addPoligono(P);

	ips = 0;
	ivs = 0;

	//step 2: Create Window
	glutCreateWindow("Skeleton - ejercicio ejemplo");
	initRendering();

	//step 3: Callbacks - Setear maquina de estados
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(keyPressed);
	glutIdleFunc(idle);

	//step 4: Main loop
	glutMainLoop();

	return(0);
}
