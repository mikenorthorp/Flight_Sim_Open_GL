
/************************************************************************************

	File: 			FlightSim.c

	Description:	Flight sim

	Author:			Michael Northorp

*************************************************************************************/

// Include headerfile for header, function and variable set up
#include "FlightSim.h"



/************************************************************************

	Function:		main

	Description:	Sets up the openGL rendering context and the windowing
					system, then begins the display loop. Display mode is
					set to double buffering, and idle function and keyboard
					event listeners are set up. glut depth display mode is set.
					Also set up mouse listener and resize function.

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
	// Set up mouse function
	glutPassiveMotionFunc(mousePosition);
	// register redraw function
	glutDisplayFunc(display);
	// Register the resize function
	glutReshapeFunc(myResize);
	// go into a perpetual loop
	glutMainLoop();
}

/************************************************************************

	Function:		fullScreen

	Description:	Handles a fullscreen or non fullscreen of the window.

*************************************************************************/
void fullScreen() {
	// Checks if fullscreen enabled
	if(isFullScreen) {
		// Call glut fullscreen function
		glutFullScreen();
	} else {
		// Reshape window to default
		glutReshapeWindow(640, 640);
		glutPositionWindow(0, 0);
	}
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

	Function:		mousePosition

	Description:	Keeps track of user mouse position and calculates the ratio
					to tilt the plane when the user moves mouse left or right.

*************************************************************************/
void mousePosition(int x, int y) {
	// Where the mouse is
	int left = 0;
	int right = 0;
	int center = 0;
	int	distanceFromCenter = 0;

	// Set max mouse movment and middle of screen
	maxMouseMove = windowWidth/2.0;

	// Check if going to left or right or center
	if(x == (int)maxMouseMove) {
		left = 0;
		right = 0;
	} else if(x > (int)maxMouseMove) {
		center = 0;
		left = 0;
		right = 1;
	} else if(x < (int)maxMouseMove) {
		center = 0;
		left = 1;
		right = 0;
	}

	// Calculate how far to move and tilt the plane
	if(left) {
		// Calculate percentage to the left and move
		distanceFromCenter = (int)maxMouseMove - x;
		// Calculate ratio
		ratioOfTilt = -1 * distanceFromCenter/maxMouseMove;
	}
	else if(right) {
		// Calculate percentage to right and move
		distanceFromCenter = x - (int)maxMouseMove;
		// Calculate ratio
		ratioOfTilt = distanceFromCenter/maxMouseMove;
	}

	// Translate the plane to the left or right depending on ratio, with max tilt being 45 degrees
	sideTilt = 45 * ratioOfTilt;
}

/************************************************************************

	Function:		drawProps

	Description:	Draws the left and right propellers for the plane

*************************************************************************/
void drawProps() {
	// Draw first propeller (left)
	glPushMatrix();
		// Position it in front of plane
		glTranslatef(-0.35, -0.1, -0.05);
		// Rotate so it is facing away
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		// Rotate propeller
		glRotatef(propInterp*360, 1.0f, 0.0f, 0.0f);
		// Translate to origin
		glTranslatef(0, 0.15f, -0.35f);

		// Draw propeller
		glCallList(theProp);
	glPopMatrix();

	// Draw second propeller (right)
	glPushMatrix();
		// Position it in front of plane
		glTranslatef(+0.35, -0.1, -0.05);
		// Rotate so it is facing away
		glRotatef(-90, 0.0f, 1.0f, 0.0f);
		// Rotate propeller
		glRotatef(propInterp*360, 1.0f, 0.0f, 0.0f);
		// Translate to origin
		glTranslatef(0, 0.15f, -0.35f);

		// Draw propeller
		glCallList(theProp);
	glPopMatrix();
}

/************************************************************************

	Function:		moveAllPlane

	Description:	This moves the plane and propellers when tilting or moving
					in the world.

*************************************************************************/
void moveAllPlane() {
		// Move the plane to planes position
		glTranslatef(planePosition[0], planePosition[1], planePosition[2]);

		// Check if we should rotate a certain way depending on where the plane is moving

		// Plane goes up
		if(upPressed) {
			// Update camera position and plane position
			planePosition[1] += 0.05;

			// Add tilt to plane
			glRotatef(8, 1.0f, 0.0f, 0.0f);
		}

		// Plane goes down
		if(downPressed) {
			// Update camera position and plane position
			planePosition[1] -= 0.05;

			// Add tilt to plane
			glRotatef(-8, 1.0f, 0.0f, 0.0f);
		}

		// Plane goes faster
		if(forwardPressed) {
			// Make plane speed faster
			planeSpeed += 0.005;

			// Add tilt to plane
			glRotatef(-5, 1.0f, 0.0f, 0.0f);
		}

		// Plane goes slower
		if(backwardPressed) {
			// Make plane speed slower
			// Limit how slow you can go
			if(planeSpeed >= 0.05) {
				planeSpeed -= 0.005;
			}

			// Add tilt to plane
			glRotatef(5, 1.0f, 0.0f, 0.0f);
		}

		// Always check for the planes tilt and rotation
		// Add planes rotation
		glRotatef(-turnAngle, 0.0f, 1.0f, 0.0f);
		// Adds tilt
		glRotatef(sideTilt*-1, 0.0f, 0.0f, 1.0f);
}

/************************************************************************

	Function:		enableFog

	Description:	This sets up the fog by enabling it and setting all the
					fog stuff up.

*************************************************************************/
void enableFog() {
	// define the color of the fog, pink
	GLfloat fogColor[4] = {0.737255, 0.560784, 0.560784, 1.0};
	// Enable the fog
	glEnable(GL_FOG);
	// set the color of the fog
	glFogfv(GL_FOG_COLOR, fogColor);
	// Set the fog mode to exponential
	glFogf(GL_FOG_MODE, GL_EXP);
	// Set the fog density
	glFogf(GL_FOG_DENSITY, 0.005);
}

/************************************************************************

	Function:		setUpProp

	Description:	This reads in the propeller objects from a file and
					sets them up to be drawn in a display list.

*************************************************************************/
void setUpProp() {
	// Variables to keep track of file stuff
	int i = 0;
	int j = 0;
	int objectCount = -1;
	int isFace = 0;
	char firstChar;
	char *token;

	// Set up a file
	FILE * fileStream;
	// Char array to store
	char string[100];
	fileStream = fopen("prop.txt", "rt");

	// Make sure the file stream is not null
	if (fileStream != NULL)
	{
		// Puts the propeller in a display list
		theProp = glGenLists(1);
	  	glNewList(theProp, GL_COMPILE);

		// Read each file line while it is not null, store in char array
		while(fgets(string, 100, fileStream) != NULL)
		{
			firstChar = ' ';
			// Store the plane vertices as it reads the file
			if(sscanf(string, "v %f %f %f ", &propVertices[i][0], &propVertices[i][1], &propVertices[i][2]) == 3) {
				// Above stores vertices in the plane vertices array
				// Increase i for reading in vertices
				i++;
			} else if(sscanf(string, "n %f %f %f ", &propNormals[j][0], &propNormals[j][1], &propNormals[j][2]) == 3) {
				// Above stores  the normals in the planenormals array
				j++;
			} else if(sscanf(string, "%c", &firstChar) == 1) { // Check if it is a face and tokenize it
				if(firstChar == 'f') {
					// Check for faces
					token = strtok(string, " ");
					// Get next token
					token = strtok(NULL, " ");

					// Draw polygon for this face
					glBegin(GL_POLYGON);
						glLineWidth(1);
						while(token != NULL ) {
							// Draw the normal and point
							glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

							// Set the colors depending on the object
							if(objectCount <= 0) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, orange);
								glMaterialfv(GL_FRONT, GL_AMBIENT, orange);
							} else if(objectCount <= 1) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
								glMaterialfv(GL_FRONT, GL_AMBIENT, red);
							} else {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
								glMaterialfv(GL_FRONT, GL_AMBIENT, yellow);
							}
							// Get normal and draw color
							glNormal3f(propNormals[atoi(token)-1][0], propNormals[atoi(token)-1][1], propNormals[atoi(token)-1][2]);
							glVertex3f(propVertices[atoi(token)-1][0], propVertices[atoi(token)-1][1], propVertices[atoi(token)-1][2]);
							// Get next token
							token = strtok(NULL, " ");
						}
					glEnd(); // End drawing of polygon
				} else if (firstChar == 'g') {
					// Increase object count
					objectCount++;
				}
			}
		}
		// End the display list
		glEndList();
	}
	fclose (fileStream);
}

/************************************************************************

	Function:		setUpPlane()

	Description:	This sets up the plane by reading it in from a file and
					drawing it in a display list.

*************************************************************************/
void setUpPlane() {
	int i = 0;
	int j = 0;
	int objectCount = -1;
	int isFace = 0;
	char firstChar;
	char *token;

	// Set up a file
	FILE * fileStream;
	// Char array to store
	char string[100];
	fileStream = fopen("plane.txt", "rt");

	// Make sure the file stream is not null
	if (fileStream != NULL)
	{
		// Puts the ship in a display list
		thePlane = glGenLists(1);
	  	glNewList(thePlane, GL_COMPILE);

		// Read each file line while it is not null, store in char array
		while(fgets(string, 100, fileStream) != NULL)
		{
			firstChar = ' ';
			// Store the plane vertices as it reads the file
			if(sscanf(string, "v %f %f %f ", &planeVertices[i][0], &planeVertices[i][1], &planeVertices[i][2]) == 3) {
				// Above stores vertices in the plane vertices array
				// Increase i for reading in vertices
				i++;
			} else if(sscanf(string, "n %f %f %f ", &planeNormals[j][0], &planeNormals[j][1], &planeNormals[j][2]) == 3) {
				// Above stores  the normals in the planenormals array
				j++;
			} else if(sscanf(string, "%c", &firstChar) == 1) { // Check if it is a face and tokenize it
				if(firstChar == 'f') {
					// Check for faces
					token = strtok(string, " ");
					// Get next token
					token = strtok(NULL, " ");

					// Draw polygon for this face
					glBegin(GL_POLYGON);
						glLineWidth(1);
						while(token != NULL ) {
							// Draw the normal and point
							glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

							// Colors depend on which object it is
							if(objectCount <= 3) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 5) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 6) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, lightPurple);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 7) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 10) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 11) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 13) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 25) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else if(objectCount <= 32) {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, yellow);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							} else {
								glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
								glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
								glMaterialfv(GL_FRONT, GL_SPECULAR, white);
							}
							// Get normal and draw color
							glNormal3f(planeNormals[atoi(token)-1][0], planeNormals[atoi(token)-1][1], planeNormals[atoi(token)-1][2]);
							glVertex3f(planeVertices[atoi(token)-1][0], planeVertices[atoi(token)-1][1], planeVertices[atoi(token)-1][2]);
							// Get next token
							token = strtok(NULL, " ");
						}
					glEnd(); // End drawing of polygon
				} else if (firstChar == 'g') {
					// Increase object count
					objectCount++;
				}
			}
		}
		// End the display list
		glEndList();
	}
	fclose (fileStream);
}

/************************************************************************

	Function:		drawPlane

	Description:	This draws the plane and accounts for movment by user.

*************************************************************************/
void drawPlane() {

	// Draw plane
	glPushMatrix();

		// Move plane and propellers depending on buttons pressed and
		// mouse position
		moveAllPlane();

		// Draw propellers
		drawProps();

		// Rotate the ship so it is facing away
		glRotatef(-90, 0.0f, 1.0f, 0.0f);

		// Draw the plane from display list
		glCallList(thePlane);
	glPopMatrix();
}

/************************************************************************

	Function:		wireRenderingCheck

	Description:	This checks to see which rendering is enabled and enables
					it

*************************************************************************/
void wireRenderingCheck() {
	// Enable or disable wirerendering based on button press
	if(isWireRendering) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

/************************************************************************

	Function:		drawSkyAndSea

	Description:	This draws the quadric objects for the sky and sea
					as well as maps the textures to them.

*************************************************************************/
void drawSkyAndSea() {
	// Set up disk and cylinder
	quadricCylinder = gluNewQuadric();
	quadricDisk = gluNewQuadric();

	// Enable or disable wirerendering based on button press
	wireRenderingCheck();

	// Set up normals
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(quadricCylinder, GLU_SMOOTH);
	gluQuadricDrawStyle(quadricDisk, GLU_SMOOTH);

	// Draw cylinder
	glPushMatrix();
		// Set line width
		glLineWidth(1);
		// Rotate it to correct position
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		// Set the colors
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, orange);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
		// Set the size (obj, inner, outer, height, slices, stacks)
		gluCylinder(quadricCylinder, 500, 500, 500, 250, 100);
	glPopMatrix();

	// Draw disk base
	glPushMatrix();
		// Enable fog for sea only
		enableFog();
		// Line width is 1
		glLineWidth(1);
		// Move it to correct position
		glRotatef(-90, 1.0f, 0.0f, 0.0f);
		// Set up colors
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seaBlue);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
		// Set up the size (obj, inner, outer, slices, stacks)
		gluDisk(quadricCylinder, 0, 501, 250, 300);
	glPopMatrix();

	// Disable the fog after drawing the disk base
	glDisable(GL_FOG);
}

/************************************************************************

	Function:		drawFrameReferenceGrid

	Description:	This draws the initial frame of reference and the axis
					for debugging purposes.

*************************************************************************/
void drawFrameReferenceGrid() {
	// Setup for loop
	int i = 0;
	int j = 0;

	// Call draw functions here
	glPushMatrix();
	// Rotate at an angle
	glRotatef(-45, 0.0f, 1.0f, 0.0f);
	// Set line width to 1
	glLineWidth(1);

	// Draw the grid of size grid size and translate it to the origin
	glTranslatef(GRID_SIZE/2, 0.0, -GRID_SIZE/2);
		// Enable or disable wirerendering based on button press
		wireRenderingCheck();

		// Draw a row in the grid
		for(i = 0;i<GRID_SIZE;i++) {
			// Offset the column drawn
			glTranslatef(-GRID_SIZE, 0.0, 0.0f);
			glTranslatef(0.0, 0.0, 1.0);
			// Draw column
			for(j = 0;j<GRID_SIZE;j++) {
				glTranslatef(1.0, 0.0, 0.0);
				// Draw the grid
				glBegin(GL_QUADS);
					// Loop through to draw each square
					glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
					glMaterialfv(GL_FRONT, GL_AMBIENT, white);

					// Get the normals and vertex for each square
					glNormal3f(0.0f, 0.0f, 0.0f);
					glVertex3f(0.0f, 0.0f, 0.0f);

					glNormal3f(0.0f, 0.0f, 1.0f);
					glVertex3f(0.0f, 0.0f, 1.0f);

					glNormal3f(1.0f, 0.0f, 1.0f);
					glVertex3f(1.0f, 0.0f, 1.0f);

					glNormal3f(1.0f, 0.0f, 0.0f);
					glVertex3f(1.0f, 0.0f, 0.0f);
				glEnd();
			}
		}
	glPopMatrix();

	// Draw frame of reference for origin

	// X direction
	glPushMatrix();
		glRotatef(-45, 0.0f, 1.0f, 0.0f);
		// Move slightly above so it draws above grid lines
		glTranslatef(0.0, 0.05, 0.0);

		// Set line width to 5
		glLineWidth(5);

		// Draw the X direction
		glBegin(GL_LINES);
			// Color red
			glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
			glMaterialfv(GL_FRONT, GL_AMBIENT, red);
			glNormal3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glNormal3f(2.0f, 0.0f, 0.0f);
			glVertex3f(2.0f, 0.0f, 0.0f);
		glEnd();

		// Y direction
		glBegin(GL_LINES);
			// Color green
			glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
			glMaterialfv(GL_FRONT, GL_AMBIENT, green);
			glNormal3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glNormal3f(0.0f, 2.0f, 0.0f);
			glVertex3f(0.0f, 2.0f, 0.0f);
		glEnd();

		// Z direction
		glBegin(GL_LINES);
			// Color blue
			glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
			glMaterialfv(GL_FRONT, GL_AMBIENT, blue);
			glNormal3f(0.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 0.0f, 0.0f);

			glNormal3f(0.0f, 0.0f, 2.0f);
			glVertex3f(0.0f, 0.0f, 2.0f);
		glEnd();

		// Draw circle in middle
		glMaterialfv(GL_FRONT, GL_DIFFUSE, grey);
		glMaterialfv(GL_FRONT, GL_AMBIENT, grey);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glutSolidSphere(0.2, 20, 20);
		glLineWidth(1);
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
		// Move up
		if(key == GLUT_KEY_UP) {
			// Set the key to be pressed
			upPressed = 1;
		}
		// Move down
		if(key == GLUT_KEY_DOWN) {
			// Set the key to be pressed
			downPressed = 1;
		}
		// Move forward
		if(key == GLUT_KEY_PAGE_UP) {
			// Set the key to be pressed
			forwardPressed = 1;
		}
		// Move down
		if(key == GLUT_KEY_PAGE_DOWN) {
			// Set the key to be pressed
			backwardPressed = 1;
		}
}

/************************************************************************

	Function:		specialKeysReleased

	Description:	This checks when a special key is released

*************************************************************************/
void specialKeysReleased(int key, int x, int y) {
		if(key == GLUT_KEY_UP) {
			// Set the key to be depressed
			upPressed = 0;
		}
		// Move down
		if(key == GLUT_KEY_DOWN) {
			// Set the key to be depressed
			downPressed = 0;
		}
		// Move forward
		if(key == GLUT_KEY_PAGE_UP) {
			// Set the key to be depressed
			forwardPressed = 0;
		}
		// Move down
		if(key == GLUT_KEY_PAGE_DOWN) {
			// Set the key to be depressed
			backwardPressed = 0;
		}
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

	Function:		lightingSetUp

	Description:	This sets up all the lighting and light sources

*************************************************************************/
void lightingSetUp() {
	// Enable lighting
	glEnable(GL_LIGHTING);

	// Enable light source
	glEnable(GL_LIGHT0);

	// Enable smooth shading
	glShadeModel(GL_SMOOTH);

	// Enable normalization
	glEnable(GL_NORMALIZE);

	// Set up light colors
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Global ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

	// Shade both sides
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/************************************************************************

	Function:		init

	Description:	This performs the main setup for openGl, it sets the
					projection mod to gl_projection and modelview. It also sets
					the perspective and performs a number of setup functions.
					Lighting is enabled here too.

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

    lightingSetUp();

    // change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);

	// Setup plane
	setUpPlane();

	// Setup propeller
	setUpProp();

	// Print out the controls
	printOutControls();
}

/************************************************************************

	Function:		myIdle

	Description:	This runs whenever the program is idle. It handles most
					of the dynamic functionality of the program.
					This helps for turning, tilting and spinning propellers.

*************************************************************************/
void myIdle(void)
{
	// Rotation speed of the plane
	if(propInterp >= 1.0) {
		propInterp = 0;
	} else {
		propInterp += 0.05;
	}

	// Make sure turn speed starts at 0
	turnSpeed = 0.0;

	// Calculate the rotation speed of turning to be double the ratio of
	// the tilt
	turnSpeed += ratioOfTilt * 2;

	// Increase the angle of turning by the turn speed
	turnAngle += turnSpeed;

	// Reset turn angle if goes over 360
	if(turnAngle > 360) {
		turnAngle = 0;
	}

	// Force a redraw in OpenGL
	glutPostRedisplay();
}

/************************************************************************

	Function:		positionScene

	Description:	This positions the plane and the camera to trail behinde
					it.

*************************************************************************/
void positionScene() {
	// Update plane position and account for the turn angle and plane speed
	planePosition[0] += sin(turnAngle * (PI/180.0f)) * planeSpeed;
	planePosition[2] -= cos(turnAngle * (PI/180.0f)) * planeSpeed;


	// Set up the camera position to trail behinde the plane
	// Based off the plane position
	cameraPosition[0] = planePosition[0] + sin(turnAngle * (PI/180.0f)) * -4;
	cameraPosition[1] = 1.2 + planePosition[1];
	cameraPosition[2] = planePosition[2] - cos(turnAngle * (PI/180.0f)) * -4;

	// Set where to look at (the plane)
	cameraPosition[3] = planePosition[0];
	cameraPosition[4] = planePosition[1];
	cameraPosition[5] = planePosition[2];
}



/************************************************************************

	Function:		display

	Description:	Clears color and depth buffer. Sets up the camera position
					and look at position as well as plane position. Draws world and plane.

*************************************************************************/
void display(void)
{
	// Clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load the indentity matrix
	glLoadIdentity();

	// Sets up camera position and plane position
	positionScene();

	// Tell the camera where to position and lookat
	gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2], cameraPosition[3], cameraPosition[4], cameraPosition[5], 0, 1, 0);

	// Set light position to whatever the lightPosition is
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// Draw everything except plane so we can move world around the plane
	glPushMatrix();
		// Draw sea and sky or the frame reference grid
		if(isSeaAndSky) {
			// Draw sky and sea and enable the fog for sea
			drawSkyAndSea();
		} else {
			// Draw frame and refercne grid
			drawFrameReferenceGrid();
		}
	glPopMatrix();

	// Always draw the plane
	glPushMatrix();
		// Draw plane
		drawPlane();
	glPopMatrix();

	// Swap the drawing buffers here
	glutSwapBuffers();
}