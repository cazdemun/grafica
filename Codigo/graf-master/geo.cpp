#include "libs\glut.h"
#include <stdlib.h>
#include <math.h>

#include "operations.h"

#define DEGTORAD (2 * 3.14159 / 360)

class operations {
#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

#include "libs\glut.h"
	enum {
		CSG_A,
		CSG_B,
		CSG_A_OR_B,
		CSG_A_AND_B,
		CSG_A_SUB_B,
		CSG_B_SUB_A,
		CSG_C
	};

	static void
		_one(void(*dso)(void)) {
		glEnable(GL_DEPTH_TEST);
		dso(); // 
		glDisable(GL_DEPTH_TEST);
	}

	static void
		firstInsideSecond(void(*a)(void), void(*b)(void), GLenum face, GLenum test) {
		glEnable(GL_DEPTH_TEST);
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glCullFace(face);
		a();

		glDepthMask(GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glCullFace(GL_BACK);
		b();

		glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		glCullFace(GL_FRONT);
		b();
		glDepthMask(GL_TRUE);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glStencilFunc(test, 0, 1);
		glDisable(GL_DEPTH_TEST);

		glCullFace(face);
		a();                  /* draw the part of a that's in b */
	}

	static void
		fixDepth(void(*a) (void)) {
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDepthFunc(GL_ALWAYS);
		a();                  /* draw the front face of a, fixing the depth buffer */
		glDepthFunc(GL_LESS);
	}

	////////////////////////////////////////////////////////////////////////////////
	/// AND, OR and SUBSTRACT OPERATION SUPPORT
	////////////////////////////////////////////////////////////////////////////////

	// OR Operation: Draw both objects with depth buffering on
	static void
		_or(void(*a)(void), void(*b)(void)) {
		glEnable(GL_DEPTH_TEST);
		a();
		b();
		glDisable(GL_DEPTH_TEST);
	}

	// AND Operation:* "and" two objects together
	static void
		_and(void(*a)(void), void(*b)(void)) {
		firstInsideSecond(a, b, GL_BACK, GL_NOTEQUAL);
		fixDepth(b);////
		firstInsideSecond(b, a, GL_BACK, GL_NOTEQUAL);
		glDisable(GL_STENCIL_TEST);  /* reset things */
	}

	// Subtract Operation: substract b from a
	static void
		_sub(void(*a)(void), void(*b) (void)) {
		firstInsideSecond(a, b, GL_FRONT, GL_NOTEQUAL);
		fixDepth(b); /////
		firstInsideSecond(b, a, GL_BACK, GL_EQUAL);
		glDisable(GL_STENCIL_TEST);  /* reset things */
	}

#endif
};


enum { SPHERE = 1, CONE, TORUS};
int csg_op = CSG_A; //default csg operation
GLfloat viewangle;

/////////////////////////////////////////////////////////////////
//Drawing Two Objects 
/////////////////////////////////////////////////////////////////

// Draw a sphere
GLfloat sphereX = 0.f, sphereY = 0.f, sphereZ = 0.f;
void sphere(void){
	glPushMatrix();
	glTranslatef(sphereX, sphereY, sphereZ);
	glCallList(SPHERE);
	glPopMatrix();
}

GLfloat touruX = 0.f, touruY = 0.f, touruZ = 0.f;
void touru(void) {
	glPushMatrix();
	glTranslatef(touruX, touruY, touruZ);
	glRotated(45, 1, 0, 0);
	glCallList(TORUS);
	glPopMatrix();
}

// Draw a cone
GLfloat coneX = 0.f, coneY = 0.f, coneZ = 0.f;
void cone(void){
	glPushMatrix();
	glTranslatef(coneX, coneY, coneZ);
	glTranslatef(0.f, 0.f, -30.f);
	glCallList(CONE);
	glPopMatrix();
}
/////////////////////////////////////////////////////////////////
//Animation Support
/////////////////////////////////////////////////////////////////
enum { ANIM_LEFT, ANIM_RIGHT };
int animDirection = ANIM_LEFT;

static void
anim(void){
	if (animDirection == ANIM_LEFT)
		viewangle -= 3.f;
	else
		viewangle += 3.f;
	glutPostRedisplay();
}

///////////////////////////////////////////////////////////////////////////
//Callbacks
///////////////////////////////////////////////////////////////////////////

static void
menu(int csgop){
	csg_op = csgop;
	glutPostRedisplay();
}

static void
display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	glPushMatrix();
		glRotatef(viewangle, 0.f, 1.f, 0.f);
		switch (csg_op) {
			case CSG_A: _one(cone);   break;
			case CSG_B:	_one(sphere); break;
			case CSG_C: _one(touru); break;
			case CSG_A_OR_B:  _or(cone, sphere);  break;
			case CSG_A_AND_B: _and(cone, sphere); break;
			case CSG_A_SUB_B: _sub(cone, sphere); break;
			case CSG_B_SUB_A: _sub(sphere, cone); break;
		}
	glPopMatrix();
	glutSwapBuffers();
}

static void
special(int key, int x, int y){
	switch (key) {
	case GLUT_KEY_LEFT:
		glutIdleFunc(anim);
		animDirection = ANIM_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		glutIdleFunc(anim);
		animDirection = ANIM_RIGHT;
		break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		glutIdleFunc(0);
		break;
	}
}

static void
key(unsigned char key, int x, int y){
	switch (key) {
	case 'a':
		viewangle -= 10.f;
		glutPostRedisplay();
		break;
	case 's':
		viewangle += 10.f;
		glutPostRedisplay();
		break;
	case '\033': /*Escape*/
		exit(0);
	}
}

int picked_object;
int xpos = 0, ypos = 0;
int newxpos, newypos;
int startx, starty;

static void
mouse(int button, int state, int x, int y){
	if (state == GLUT_UP) {
		picked_object = button;
		xpos += newxpos;
		ypos += newypos;
		newxpos = 0;
		newypos = 0;
	}
	else {              /* GLUT_DOWN */
		startx = x;
		starty = y;
	}
}

static void
motion(int x, int y){
	GLfloat r, objx, objy, objz;

	newxpos = x - startx;
	newypos = starty - y;

	r = (newxpos + xpos) * 50.f / 512.f;
	objx = r * cos(viewangle * DEGTORAD);
	objy = (newypos + ypos) * 50.f / 512.f;
	objz = r * sin(viewangle * DEGTORAD);

	switch (picked_object) {
	case CSG_A:
		coneX = objx;
		coneY = objy;
		coneZ = objz;
		break;
	case CSG_B:
		sphereX = objx;
		sphereY = objy;
		sphereZ = objz;
		break;
	}
	glutPostRedisplay();
}


//Main Operation
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(512, 512);
	glutInitDisplayMode(GLUT_STENCIL | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("CSG Example Operations");
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutSpecialFunc(special);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	//create a Menu
	glutCreateMenu(menu);
	glutAddMenuEntry("A only", CSG_A);
	glutAddMenuEntry("B only", CSG_B);
	glutAddMenuEntry("C", CSG_C);
	glutAddMenuEntry("A or B", CSG_A_OR_B);
	glutAddMenuEntry("A and B", CSG_A_AND_B);
	glutAddMenuEntry("A sub B", CSG_A_SUB_B);
	glutAddMenuEntry("B sub A", CSG_B_SUB_A);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	////
	static GLfloat lightpos[] = { -20.f, 0.f, 90.f, 0.0f };
	static GLfloat sphere_mat[] = { 1.f, .0f, 0.f, 1.f };
	static GLfloat cone_mat[] = { 0.f, .5f, 1.f, 1.f };

	GLUquadricObj *sphere, *cone, *base;

	///
	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


	glNewList(SPHERE, GL_COMPILE);
	sphere = gluNewQuadric();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	gluSphere(sphere, 20.f, 64, 64);
	gluDeleteQuadric(sphere);
	glEndList();

	glNewList(CONE, GL_COMPILE);
	cone = gluNewQuadric();
	base = gluNewQuadric();

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, cone_mat);
	gluQuadricOrientation(base, GLU_INSIDE);
	gluDisk(base, 0., 15., 64, 1);
	gluCylinder(cone, 15., 0., 60., 64, 64);
	gluDeleteQuadric(cone);
	gluDeleteQuadric(base);
	glEndList();

	glNewList(TORUS, GL_COMPILE);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sphere_mat);
	glutWireTorus(4, 5, 16, 16);
	glEndList();

	glMatrixMode(GL_PROJECTION);
	glOrtho(-50., 50., -50., 50., -50., 50.);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();
	return 0;
}
