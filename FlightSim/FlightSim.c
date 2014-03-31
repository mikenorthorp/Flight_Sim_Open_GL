
/************************************************************************************

	File: 			FlightSim.c

	Description:	Flight sim

	Author:			Michael Northorp

*************************************************************************************/

/* include the library header files */
// Freeglut header
#include <GL\freeglut.h>
#include <GL\Gl.h>
#include <windows.h>
// Math header
#include <math.h>
// File read in
#include <stdio.h>
// Time header for random seeds
#include <time.h>

/* Defines */
// Ratio of the circumference to the diameter of a circle
#define PI 3.14159265f
// Conversion multiplier for converting from degrees to Radians for some calculations
#define DEG_TO_RAD PI/180.0f

// Grid size X by X
#define GRID_SIZE 100.0f

/* Global variables */

/* Typedefs and structs */

// Defines an x,y,z point
typedef GLfloat point3[3];
// Defines a RGB color
typedef GLfloat color3[3];

/* Position variables for camera and ship */

// Keep track of current camera position and set the default
GLfloat cameraPosition[] = {6, 2.5, 6, 0, 0, 0};

// Window size parameters
GLfloat windowWidth  = 640.0;
GLfloat windowHeight = 640.0;

// Set up quad objects
GLUquadricObj* quadricCylinder;
GLUquadricObj* quadricDisk;

// Key booleans
// Not full screen by default
GLint isFullScreen = 0;
// Wire rendering on by default
GLint isWireRendering = 1;
// Sea ad sky enabled
GLint isSeaAndSky = 0;

// This handles full screen or getting out of full screen
void fullScreen() {
	if(isFullScreen) {
		glutFullScreen();
	} else {
		glutReshapeWindow(640, 640);
		glutPositionWindow(0, 0);
	}
}

// Sets up the plane 
void setUpPlane() {

}

// Draws the plane
void drawPlane() {

}

// Draw the sky and sea with gluQuad objects
void drawSkyAndSea() {
	// Set up disk and cylinder
	quadricCylinder = gluNewQuadric();
	quadricDisk = gluNewQuadric();

	// Set up draw style to line
	if(isWireRendering) {
		gluQuadricDrawStyle(quadricCylinder, GLU_LINE); 
		gluQuadricDrawStyle(quadricDisk, GLU_LINE); 
	} else {
		gluQuadricDrawStyle(quadricCylinder, GLU_FILL); 
		gluQuadricDrawStyle(quadricDisk, GLU_FILL); 
	}


	// Set up normals
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(quadricCylinder, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricDisk, GLU_SMOOTH); 

	// Draw cylinder
	glPushMatrix();
		glLineWidth(1);
		glTranslatef(0.0, -20.0, 0.0);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		gluCylinder(quadricCylinder, 50, 50, 100, 100, 50);
	glPopMatrix();

	// Draw disk base
	glPushMatrix();
		glLineWidth(1);
		glTranslatef(0.0, -20.0, 0.0);
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		gluDisk(quadricCylinder, 0, 51, 50, 50);
	glPopMatrix();
}


// Enable the fog to be a slight orange
void setUpFog() {
	// define the color of the fog, orange
	GLfloat fogColor[4] = {0.737255, 0.560784, 0.560784, 1.0};
	// Enable the fog
	glEnable(GL_FOG);
	// set the color of the fog
	glFogfv(GL_FOG_COLOR, fogColor);
	// Set the fog mode to exponential
	glFogf(GL_FOG_MODE, GL_EXP);
	// Set the fog density
	glFogf(GL_FOG_DENSITY, 0.05);
}

// Draw the grid and basic frame of reference
void drawFrameReferenceGrid() {
	// Setup for loop
	int i = 0;
	int j = 0;

	// Call draw functions here
	glPushMatrix();

	// Set line width to 1
	glLineWidth(1);

	// Draw the grid of size grid size and translate it to the origin
	glTranslatef(GRID_SIZE/2, 0.0, -GRID_SIZE/2);
		// Enable or disable wirerendering based on button press
		if(isWireRendering) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		// Draw a row in the grid
		for(i = 0;i<GRID_SIZE;i++) {
			glTranslatef(-GRID_SIZE, 0.0, 0.0f);
			glTranslatef(0.0, 0.0, 1.0);
			for(j = 0;j<GRID_SIZE;j++) {
				glTranslatef(1.0, 0.0, 0.0);
				// Draw the grid
				glBegin(GL_QUADS);
					// Loop through to draw each square
					glColor3f(0.8f, 0.8f, 1.0f);
					glVertex3f(0.0f, 0.0f, 0.0f);
					glVertex3f(0.0f, 0.0f, 1.0f);
					glVertex3f(1.0f, 0.0f, 1.0f);
					glVertex3f(1.0f, 0.0f, 0.0f);
				glEnd();
			}
		}
	glPopMatrix();

	// Draw frame of reference for origin
	// X direction
	glPushMatrix();
		// Move slightly above so it draws above grid lines
		glTranslatef(0.0, 0.05, 0.0);
		// Set line width to 5
		glLineWidth(5);

		// Draw the X direction
		glBegin(GL_LINES);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(2.0f, 0.0f, 0.0f);
		glEnd();

		// Y direction
		glBegin(GL_LINES);
			glColor3f(0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 2.0f, 0.0f);
		glEnd();

		// Z direction
		glBegin(GL_LINES);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 2.0f);
		glEnd();

		// Draw circle in middle
		glColor3f(1.0, 1.0, 1.0);
		glutSolidSphere(0.2, 20, 20);
	glPopMatrix();
}

/************************************************************************

	Function:		normalKeys

	Description:	This checks when a key is pressed that controls one of the
					features in the program.

*************************************************************************/
void normalKeys(unsigned char key, int x, int y) {
	switch(key) {
		// Toggle full screen
		case 'f':
			// Set full screen to opposite
			isFullScreen = !isFullScreen;
			// Call fullscreen function
			fullScreen();
			break;
		// Toggle full screen
		case 'w':
			// Set full screen to opposite
			isWireRendering = !isWireRendering;
			break;
		case 's':
			// Set full screen to opposite
			isSeaAndSky = !isSeaAndSky;
			break;
		// Quit the program gracefully
		case 'q':
			exit(0);
			break;
		default:
			break;
	}
}

/************************************************************************

	Function:		specialKeys

	Description:	This checks when a special key is pressed to move the
					ship.

*************************************************************************/
void specialKeys(int key, int x, int y) {
	
}

/************************************************************************

	Function:		specialKeysReleased

	Description:	This checks when a special key is released

*************************************************************************/
void specialKeysReleased(int key, int x, int y) {
	
}

/************************************************************************

	Function:		printOutcontrols

	Description:	This prints out all of the controls for the program.

*************************************************************************/
void printOutControls() {
	printf("Scene Controls\n--------------\n");
	printf("w: Toggle between wireframe and solid draw mode\n");
	printf("f: Toggle between fullscreen and non fullscreen\n");
	printf("s: Toggle between sea and sky and frame reference grid\n");
	printf("q: Quit the program\n");
}


/************************************************************************

	Function:		init

	Description:	This performs the main setup for openGl, it sets the
					projection mod to gl_projection and modelview. It also sets
					the perspective and performs a number of setup functions.
					These display the controls, set up the planets, stars and ships.
*************************************************************************/
void init(void)
{
	// Set initial window position and size
	glutReshapeWindow(windowWidth, windowHeight);
	glutPositionWindow(0, 0);

	// Enable the depth test
	glEnable(GL_DEPTH_TEST);

	// Set the background to be black
	glClearColor(0, 0, 0, 1.0);

	// Set to projection mode so we can use camera functions
    glMatrixMode(GL_PROJECTION);

	// Load identity matrix onto projection matrix
    glLoadIdentity();

    // gluPerspective(fovy, aspect, near, far)
    gluPerspective(90, windowWidth/windowHeight, 0.1, 40000);

    // change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);

	// Draw the fog
	setUpFog();

	// Print out the controls
	printOutControls();
}

/************************************************************************

	Function:		myIdle

	Description:	This runs whenever the program is idle. It handles most
					of the dynamic functionality of the program.
					This includes rotating planets, changing interp values for
					shield and klingons moving.

*************************************************************************/
void myIdle(void)
{
	// Force a redraw in OpenGL
	glutPostRedisplay();
}

/************************************************************************

	Function:		myResize

	Description:	Handles a user resize of the window. 

*************************************************************************/
void myResize(int newWidth, int newHeight)
{

	// Update width of window
	windowWidth = newWidth;
	// Update height of window
	windowHeight = newHeight;

	// Update the viewport to still be all of the window
	glViewport (0, 0, windowWidth, windowHeight);

	// Change camera properties
    glMatrixMode(GL_PROJECTION);

	// Load identity matrix
    glLoadIdentity();

    // Modify the gluPerspective (fovy, aspect, near, far)
    gluPerspective(45, windowWidth/windowHeight, 0.1, 40000); 

    // Back into modelview
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************

	Function:		display

	Description:	Clears color and depth buffer. Sets up the camera position
					and look at position. Draws planets and other objects based
					on button toggles. Always draws planets and ship.

*************************************************************************/
void display(void)
{
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load the indentity matrix
	glLoadIdentity();

	// Set up the camera position
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraPosition[3], cameraPosition[4], cameraPosition[5], 0, 1, 0);

	// Draw the frame of reference and basic grid
	if(isSeaAndSky) {
		// Draw sky and sea and enable the fog
		drawSkyAndSea();
		setUpFog();
	} else {
		drawFrameReferenceGrid();
		// Disable the fog
		glDisable(GL_FOG);
	}

	// Swap the drawing buffers here
	glutSwapBuffers();
}


/************************************************************************

	Function:		main

	Description:	Sets up the openGL rendering context and the windowing
					system, then begins the display loop. Display mode is
					set to double buffering, and idle function and keyboard
					event listeners are set up. glut depth display mode is set.

*************************************************************************/
void main(int argc, char** argv)
{
	// initialize the toolkit
	glutInit(&argc, argv);
	// set display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// set window size
	glutInitWindowSize(windowHeight, windowWidth);
	// open the screen window
	glutCreateWindow("Flight Sim");
	//initialize the rendering context
	init();
	// register the idle function
	glutIdleFunc(myIdle);
	// This handles keyboard input for normal keys
	glutKeyboardFunc(normalKeys);
	// This handles keyboard input for special keys
	glutSpecialFunc(specialKeys);
	// This checks when a special key is released
	glutSpecialUpFunc(specialKeysReleased);
	// register redraw function
	glutDisplayFunc(display);
	// Register the resize function
	glutReshapeFunc(myResize); 
	// go into a perpetual loop
	glutMainLoop();
}