/*
* Al Lansley 10/12/2008
*
* Tunnel that changes colours
*/

#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <time.h>
#include <math.h>
#include "libs\glut.h"
#include "SDL.h"

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* screen width, height, and bit depth */
#define SCREEN_WIDTH     800
#define SCREEN_HEIGHT    600
#define SCREEN_BPP       24

#define PI 3.14159

// Tunnel definitiions
#define TUNNEL_POINTS        30        // Number of points making the circle
#define TUNNEL_SLICES        20        // Number of slices in a tunnel, think cucumber ;)
#define TUNNEL_SPEED         2.0f      // Amount Z value of tunnel is moved towards origin per frame
#define TUNNEL_SIZE          25.0f     // Radius of tunnel
#define TUNNEL_DEPTH         -800.0f   // How deep the rabbithole goes...
#define TUNNEL_PULSE_SPEED   4.0f      // The speed of the Point-of-View-esque pulsations

GLfloat tunnelX[TUNNEL_SLICES][TUNNEL_POINTS];
GLfloat tunnelY[TUNNEL_SLICES][TUNNEL_POINTS];
GLfloat tunnelZ[TUNNEL_SLICES][TUNNEL_POINTS];
GLfloat tunnelXAdder[TUNNEL_SLICES][TUNNEL_POINTS];
GLfloat tunnelYAdder[TUNNEL_SLICES][TUNNEL_POINTS];
GLfloat tunnelSize[TUNNEL_SLICES];
GLfloat tunnelSlicePulseOffset[TUNNEL_SLICES];
GLfloat tunnelSliceSpacing[TUNNEL_SLICES];
GLfloat tunnelSpeed = TUNNEL_SPEED;

//Set initial colour values
GLfloat colourOne[] = { 1.0f, 1.0f, 1.0f };        // Current colours..
GLfloat colourTwo[] = { 1.0f, 1.0f, 0.0f };
GLfloat colourOneTarget[] = { 1.0f, 0.0f, 1.0f };  // Colours to shift to...
GLfloat colourTwoTarget[] = { 0.0f, 1.0f, 1.0f };

// Tunnel state vars
int pointsToDrawCount = TUNNEL_POINTS; // For when you don't want to draw all available points [though they are still calc'ed]
int slicesToDrawCount = TUNNEL_SLICES; // For when you don't want to draw all available slices [though they are still calc'ed]
int pulseScale = 1.0f;          // Magnitude of tunnel "forward/back" pulses
float frameCount = 0.0f;          // Frame counter used for sin/cos of pulses

								  /* Lighting values */
GLfloat  whiteLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat  sourceLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat  lightPos[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/* This is our SDL surface */
SDL_Surface *surface;

/* function to release/destroy our resources and restoring the old desktop */
void Quit(int returnCode)
{
	/* clean up the window */
	SDL_Quit();

	/* and exit appropriately */
	exit(returnCode);
}

/* function to reset our viewport after a window resize */
int resizeWindow(int width, int height)
{
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height == 0)
		height = 1;

	ratio = (GLfloat)width / (GLfloat)height;

	/* Setup our viewport. */
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	/* change to the projection matrix and set our viewing volume. */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Set our perspective (POV angle, window ratio, start of viewport z, depth of items to display [if too shallow, "deep" items will not be displayed] */
	gluPerspective(45.0f, ratio, 0.1f, -TUNNEL_DEPTH);

	/* Make sure we're changing the model view and not the projection */
	glMatrixMode(GL_MODELVIEW);

	/* Reset The View */
	glLoadIdentity();

	return(TRUE);
}

float deg2rad(float deg) {

	// Calc angle in rads between each point on tunnel slice    
	float rad = deg * (PI / 180.0f);

	return rad;
}

/* function to handle key press events */
void handleKeyPress(SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		/* ESC key was pressed */
		Quit(0);
		break;
	case SDLK_F1:
		/* F1 key was pressed
		* this toggles fullscreen mode
		*/
		SDL_WM_ToggleFullScreen(surface);
		break;

	default:
		break;
	}

	return;
}

int initVars(GLvoid)
{
	/* initialize random seed: */
	srand(time(NULL));

	// Set random colours and target colours for the chequers
	for (int i = 0; i < 3; i++) {
		colourOne[i] = (float)rand() / ((double)RAND_MAX + 1);
		colourTwo[i] = (float)rand() / ((double)RAND_MAX + 1);
		colourOneTarget[i] = (float)rand() / ((double)RAND_MAX + 1);
		colourTwoTarget[i] = (float)rand() / ((double)RAND_MAX + 1);
	}

	// Define initial points
	// For each tunnel slice...
	for (int i = 1; i <= TUNNEL_SLICES; i++) {

		// Set initial tunnelSize array all equal to defined tunnel size constant
		tunnelSize[i] = TUNNEL_SIZE;

		// Initialise pulse offsets
		tunnelSlicePulseOffset[i] = (deg2rad(360.0f) / TUNNEL_SLICES) * i;

		// Set initial tunnelSliceSpacing to be an even distance off depth / slices
		tunnelSliceSpacing[i] = TUNNEL_DEPTH / TUNNEL_SLICES;

		// For each point on the tunnel slice...
		for (int j = 1; j <= TUNNEL_POINTS; j++) {

			tunnelX[i][j] = tunnelSize[i];             // Set all X points to initially be the size of the tunnel (ready for rotation)
			tunnelY[i][j] = 0.0f;		       // Set Y point to be on Y axis
			tunnelZ[i][j] = tunnelSliceSpacing[i] * i; // Set initial depth of tunnel slice to be spacing * slice number

		} // End of per point loop...

	} // End of per slice loop...


	  // Calc angle in rads between each point on tunnel slice    
	float angle = (360.0f / TUNNEL_POINTS) * (PI / 180.0f);


	// Rotate points
	// For each tunnel slice...
	for (int i = 1; i <= TUNNEL_SLICES; i++)
	{
		// For each point on the tunnel slice...
		for (int j = 1; j <= TUNNEL_POINTS; j++) {

			// Calc Sin + Cos of rotation angle multiplied by point number
			float cosAng = cos(angle * (float)j);
			float sinAng = sin(angle * (float)j);

			// Calc new X and Y value of rotated point [i][j] using tunnelSize array
			tunnelX[i][j] = (tunnelSize[i] * cosAng) - (tunnelSize[i] * sinAng);
			tunnelY[i][j] = (tunnelSize[i] * sinAng) + (tunnelSize[i] * cosAng);

		} // End of per tunnel point loop

	} // End of per tunnel slice loop

	return(TRUE);
}

/* general OpenGL initialization function */
int initGL(GLvoid)
{

	// Setup fog 
	GLfloat fogColour[] = { 0.0f, 0.0f, 0.0f };
	glFogfv(GL_FOG_COLOR, fogColour);
	//glFogf(GL_FOG_START, (TUNNEL_DEPTH * -0.2f));
	glFogf(GL_FOG_START, (TUNNEL_DEPTH * -0.1f));
	glFogf(GL_FOG_END, (TUNNEL_DEPTH * -0.9f));
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glEnable(GL_FOG);

	/* Enable lighting */
	glEnable(GL_LIGHTING);

	/* Setup and enable light 0 */
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHT0);

	/* Enable color tracking */
	glEnable(GL_COLOR_MATERIAL);

	/* Set Material properties to follow glColor values */
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	/* Enable smooth shading */
	//glShadeModel( GL_SMOOTH);
	glShadeModel(GL_FLAT);

	/* Set the background black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/* Depth buffer setup */
	glClearDepth(1.0f);

	/* Enables Depth Testing */
	glEnable(GL_DEPTH_TEST);

	/* The Type Of Depth Test To Do */
	glDepthFunc(GL_LEQUAL);

	/* Really Nice Perspective Calculations */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	/* Enable multi-sampling to smooth jaggies */
	glEnable(GL_MULTISAMPLE);


	return(TRUE);
}

// *** pulsate function -- pulsates the size of the tunnel slices - is really just a multiplication of X and Y, no Z manipulation ***
int pulsate(GLvoid)
{
	float adderRad = 0.0f;
	float sinAdderRad = 0.0f;

	// For each tunnel slice...
	for (int i = 1; i <= TUNNEL_SLICES; i++)
	{

		// Get radian value to use for adders
		adderRad = deg2rad(frameCount + tunnelSlicePulseOffset[i]) * TUNNEL_PULSE_SPEED;

		// Sin goes from -1 to 1, if you always add 2 to the returned value it'll never be less than 1 (i.e. tunnels expand and shrink, but not less than orig size) - this'll go from 1 to 3.0f 
		sinAdderRad = sin(adderRad) + 2.0f;

		// For each tunnel point...
		for (int j = 1; j <= TUNNEL_POINTS; j++) {

			// Actually set the adders
			tunnelXAdder[i][j] = (sinAdderRad * pulseScale);// * 0.1f;
			tunnelYAdder[i][j] = (sinAdderRad * pulseScale);// * 0.1f;

															// Disable the pulsing for debugging if need be...
															//tunnelXAdder[i][j] = 1.0f;
															//tunnelYAdder[i][j] = 1.0f;

		} // End of per point loop [j]

	} // End of per slice loop [i]

	return(TRUE);

} // End of pulsate procedure

  /* Here goes our drawing code */
int drawGLScene(GLvoid)
{

	pulsate();

	/* rotational vars for the triangle and quad, respectively */
	//static GLfloat fElect1 = 0.0f;

	/* These are to calculate our fps */
	static GLint T0 = 0;
	static GLint Frames = 0;

	/* Clear The Screen And The Depth Buffer */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Move light 50 units into screen
	glTranslatef(0.0f, 0.0f, 50.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	// Reset to identity matrix
	glLoadIdentity();

	// Move 50 units into screen
	glTranslatef(0.0f, 0.0f, 50.0f);


	// Move the tunnel co-ords towards the origin along the Z axis
	// For each tunnel slice...
	for (int i = 1; i <= TUNNEL_SLICES; i++) {

		// For each tunnel point...
		for (int j = 1; j <= TUNNEL_POINTS; j++) {

			// Move tunnel points closer to origin
			tunnelZ[i][j] += TUNNEL_SPEED;

			// If tunnel points hit origin send them back to depth
			if (tunnelZ[i][j] >= 0.0f) {
				tunnelZ[i][j] = TUNNEL_DEPTH;
			}

		} // End of per point loop...

	} // End of per slice loop...


	  // Main drawing loop
	for (int i = 1; i < TUNNEL_SLICES; i++) {

		// Start the triangle strip
		glBegin(GL_TRIANGLE_STRIP);

		// Give the initial point for the triangle strip
		glVertex3f(tunnelX[i + 1][1] * tunnelXAdder[i + 1][1], tunnelY[i + 1][1] * tunnelYAdder[i + 1][1], tunnelZ[i + 1][1]);

		for (int j = 1; j < TUNNEL_POINTS; j++) {

			// Change colour to draw in every other triangle...
			if (j % 2 == 0) { glColor3f(colourOne[0], colourOne[1], colourOne[2]); }
			else { glColor3f(colourTwo[0], colourTwo[1], colourTwo[2]); }

			// If there's no Z wrap-around and we're not on the last point of the tunnel strip...
			if ((tunnelZ[i][j] > tunnelZ[i + 1][j]) & (j != (TUNNEL_POINTS - 1))) {

				glVertex3f(tunnelX[i][j] * tunnelXAdder[i][j], tunnelY[i][j] * tunnelYAdder[i][j], tunnelZ[i][j]);			// Current slice point
				glVertex3f(tunnelX[i + 1][j + 1] * tunnelXAdder[i + 1][j + 1], tunnelY[i + 1][j + 1] * tunnelYAdder[i + 1][j + 1], tunnelZ[i + 1][j + 1]);  // Next point in strip
			}

			// If we've got to the end of the j [points] loop draw wrapping triangles between first and last points to seal the tunnel...
			// This is a pretty sketchy kludge =(
			if ((j == (TUNNEL_POINTS - 1)) & (tunnelZ[i][j] > tunnelZ[i + 1][j])) {

				glVertex3f(tunnelX[i][j] * tunnelXAdder[i][j], tunnelY[i][j] * tunnelYAdder[i][j], tunnelZ[i][j]);
				glVertex3f(tunnelX[i + 1][j + 1] * tunnelXAdder[i + 1][j + 1], tunnelY[i + 1][j + 1] * tunnelYAdder[i + 1][j + 1], tunnelZ[i + 1][j + 1]);

				if (j % 2 == 1) { glColor3f(colourOne[0], colourOne[1], colourOne[2]); }
				else { glColor3f(colourTwo[0], colourTwo[1], colourTwo[2]); }

				glVertex3f(tunnelX[i][j + 1] * tunnelXAdder[i][j + 1], tunnelY[i][j + 1] * tunnelYAdder[i][j + 1], tunnelZ[i][j + 1]);
				glVertex3f(tunnelX[i + 1][1] * tunnelXAdder[i + 1][1], tunnelY[i + 1][1] * tunnelYAdder[i + 1][1], tunnelZ[i + 1][1]);

				if (j % 2 == 0) { glColor3f(colourOne[0], colourOne[1], colourOne[2]); }
				else { glColor3f(colourTwo[0], colourTwo[1], colourTwo[2]); }
				glVertex3f(tunnelX[i][1] * tunnelXAdder[i][1], tunnelY[i][1] * tunnelYAdder[i][1], tunnelZ[i][1]);

			} // End of tunnel patchng kludge

		} // End of points loop

		glEnd(); // Close triangle strip


				 // Draw final awkward last-slice to first-slice strip, also pretty kludgey
		glBegin(GL_TRIANGLE_STRIP);

		// Give the initial point for the triangle strip
		glVertex3f(tunnelX[1][1] * tunnelXAdder[1][1], tunnelY[1][1] * tunnelYAdder[1][1], tunnelZ[1][1]);

		for (int j = 1; j < TUNNEL_POINTS; j++) {

			// Change colour to draw in every other triangle...
			if (j % 2 == 0) { glColor3f(colourOne[0], colourOne[1], colourOne[2]); }
			else { glColor3f(colourTwo[0], colourTwo[1], colourTwo[2]); }

			// If the front slice is closer than the slice behind on Z-wrap...
			if (tunnelZ[TUNNEL_SLICES][j] > tunnelZ[1][j + 1]) {

				glVertex3f(tunnelX[TUNNEL_SLICES][j] * tunnelXAdder[TUNNEL_SLICES][j], tunnelY[TUNNEL_SLICES][j] * tunnelYAdder[TUNNEL_SLICES][j], tunnelZ[TUNNEL_SLICES][j]);
				glVertex3f(tunnelX[1][j + 1] * tunnelXAdder[1][j + 1], tunnelY[1][j + 1] * tunnelYAdder[1][j + 1], tunnelZ[1][j + 1]);

			} // End of draw last strip kludge


			  // Another kludge to seal the sides of the last tunnel strip. Inelegant, shitty code. Poor show.
			if ((j == (TUNNEL_POINTS - 1)) & (tunnelZ[TUNNEL_SLICES][j] > tunnelZ[1][j])) {

				glColor3f(colourOne[0], colourOne[1], colourOne[2]);
				if (tunnelZ[1][j + 1] < tunnelZ[TUNNEL_SLICES][j + 1]) {
					glVertex3f(tunnelX[TUNNEL_SLICES][j] * tunnelXAdder[TUNNEL_SLICES][j], tunnelY[TUNNEL_SLICES][j] * tunnelYAdder[TUNNEL_SLICES][j], tunnelZ[TUNNEL_SLICES][j]);
					glVertex3f(tunnelX[1][j + 1] * tunnelXAdder[1][j + 1], tunnelY[1][j + 1] * tunnelYAdder[1][j + 1], tunnelZ[1][j + 1]);
					glVertex3f(tunnelX[TUNNEL_SLICES][j + 1] * tunnelXAdder[TUNNEL_SLICES][j + 1], tunnelY[TUNNEL_SLICES][j + 1] * tunnelYAdder[TUNNEL_SLICES][j + 1], tunnelZ[TUNNEL_SLICES][j + 1]);
				}

				if (tunnelZ[1][1] < tunnelZ[TUNNEL_SLICES][j + 1]) {
					glVertex3f(tunnelX[1][1] * tunnelXAdder[1][1], tunnelY[1][1] * tunnelYAdder[1][1], tunnelZ[1][1]);
				}

				glColor3f(colourTwo[0], colourTwo[1], colourTwo[2]);
				if (tunnelZ[1][1] < tunnelZ[TUNNEL_SLICES][1]) {
					glVertex3f(tunnelX[TUNNEL_SLICES][1] * tunnelXAdder[TUNNEL_SLICES][1], tunnelY[TUNNEL_SLICES][1] * tunnelYAdder[TUNNEL_SLICES][1], tunnelZ[TUNNEL_SLICES][1]);
				}

			} // End of last strip patching

		} // End of j [points] loop

		glEnd();

	} // End of slices [i] loop

	  /* Draw it to the screen */
	SDL_GL_SwapBuffers();

	/* Gather our frames per second */
	Frames++;
	{
		GLint t = SDL_GetTicks();

		// Every 5 seconds...
		if (t - T0 >= 5000) {
			GLfloat seconds = (t - T0) / 1000.0;
			GLfloat fps = Frames / seconds;
			printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
			T0 = t;
			Frames = 0;

			// Set random colours for the chequers
			for (int i = 0; i < 3; i++) {
				colourOneTarget[i] = (float)rand() / ((double)RAND_MAX + 1);
				colourTwoTarget[i] = (float)rand() / ((double)RAND_MAX + 1);
			}

		} // End of every 5 seconds condition
	}

	// Increment frameCount unless it's 360 then put it back to 0 so we never get range-wrap on the float [used for pulsing of tunnel]
	if (frameCount != 360.0f) {
		frameCount += 0.1f;
	}
	else {
		frameCount = 0.0f;
	}

	// Move cheque colours closer to target colours
	for (int i = 0; i < 3; i++) {

		if (colourOne[i] < colourOneTarget[i]) {
			colourOne[i] += 0.001f;
		}
		else {
			colourOne[i] -= 0.001f;
		}

		if (colourTwo[i] < colourTwoTarget[i]) {
			colourTwo[i] += 0.001f;
		}
		else {
			colourTwo[i] -= 0.001f;
		}
	}

	//glFlush();

	return(TRUE);
}

// Main loop
int main(int argc, char **argv)
{

	/* Initialise glut so we can use glutSphere etc - glut calls crash without this called at beginning of prog */
	glutInit(&argc, argv);

	/* Flags to pass to SDL_SetVideoMode */
	int videoFlags;
	/* main loop variable */
	int done = FALSE;
	/* used to collect events */
	SDL_Event event;
	/* this holds some info about our display */
	const SDL_VideoInfo *videoInfo;
	/* whether or not the window is active */
	int isActive = TRUE;

	/* initialize SDL */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Video initialization failed: %s\n",
			SDL_GetError());
		Quit(1);
	}

	/* Fetch the video info */
	videoInfo = SDL_GetVideoInfo();

	if (!videoInfo)
	{
		fprintf(stderr, "Video query failed: %s\n",
			SDL_GetError());
		Quit(1);
	}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

									   /* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Set up multisampling [ACL]
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	// Set window title - syntax: (WINDOW_TITLE, ICON TITLE)
	SDL_WM_SetCaption("Al Lansley 11/12/2008", "Tunnel");

	/* get a SDL surface */
	surface = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags);

	/* Verify there is a surface */
	if (!surface)
	{
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		Quit(1);
	}

	/* int vars */
	initVars();

	/* initialize OpenGL */
	initGL();

	/* resize the initial window */
	resizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

	/* wait for events */
	while (!done)
	{
		/* handle the events in the queue */

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_ACTIVEEVENT:
				/* Something's happend with our focus
				* If we lost focus or we are iconified, we
				* shouldn't draw the screen
				*/
				if (event.active.gain == 0) {
					//isActive = FALSE;  ** yeah, we should ;) ***
				}
				else {
					isActive = TRUE;
				}
				break;
			case SDL_VIDEORESIZE:
				/* handle resize event */
				surface = SDL_SetVideoMode(event.resize.w,
					event.resize.h,
					16, videoFlags);
				if (!surface)
				{
					fprintf(stderr, "Could not get a surface after resize: %s\n", SDL_GetError());
					Quit(1);
				}
				resizeWindow(event.resize.w, event.resize.h);
				break;
			case SDL_KEYDOWN:
				/* handle key presses */
				handleKeyPress(&event.key.keysym);
				break;
			case SDL_QUIT:
				/* handle quit requests */
				done = TRUE;
				break;
			default:
				break;
			}
		}

		/* draw the scene */
		if (isActive)
			drawGLScene();

		/* delay 1ms */
		usleep(10000);
	}

	/* clean ourselves up and exit */
	Quit(0);

	/* Should never get here */
	return(0);
}
