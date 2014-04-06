/*
 * FlightSim.h
 * Mike Northorp
 * Flight Simulator Project main headerfile contains all varaible and function
 * set up.
 */

#ifndef FLIGHTSIM_H_
#define FLIGHTSIM_H_
/* Header files */
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
 // Include stdlib
 #include <stdlib.h>

/* Defines */

// Ratio of the circumference to the diameter of a circle
#define PI 3.14159265f
// Conversion multiplier for converting from degrees to Radians for some calculations
#define DEG_TO_RAD PI/180.0f
// Grid size X by X
#define GRID_SIZE 100.0f

// Number of mountains
#define NUM_MOUNTAINS 50

/* Global variables */

/* Typedefs and structs */

// Defines an x,y,z point
typedef GLfloat point3[3];
// Defines a RGB color
typedef GLfloat color4[4];

/* Initial positions of camera, light and plane */

// Keep track of current camera position and set the default
GLfloat cameraPosition[] = {0, 0, 0, 0, 0, 0};

// Set light position
GLfloat lightPosition[] = {0.0, 60.0, 0.0, 1.0};

// Set initial plane position
GLfloat planePosition[] = {0, 2.0, 10.0};

// Window size parameters
GLfloat windowWidth  = 640.0;
GLfloat windowHeight = 640.0;

/* Quadric pointers */
// Set up quadric objects
GLUquadricObj* quadricCylinder;
GLUquadricObj* quadricDisk;

// Array of cones for mountains
GLUquadricObj* quadricCone[NUM_MOUNTAINS];

/* Display list variables */
// Set up display list for the plane
GLuint thePlane = 0;

// Set up display list for propeller
GLuint theProp = 0;

// Sets up the grid for frame reference
GLuint theGrid = 0;

/* Vertex and normal arrays for plane and propeller */

// This is an array of all the vertices for the plane
point3 planeVertices[6763];
// This is an array of all normals for the plane
point3 planeNormals[6763];

// This is an array of all the vertices for the plane
point3 propVertices[6763];
// This is an array of all normals for the plane
point3 propNormals[6763];

/* Interp and dynamic values */

// Interp for propeller spinning
GLfloat propInterp = 0.0f;

// Plane speed, this is the plane speed
GLfloat planeSpeed = 0.05;

// Get the amount to tilt the plane
GLfloat sideTilt = 0.0;

// Set the turnspeed to 0
GLfloat turnSpeed = 0.0;

// Global mouse position of x
float mouseX = 0.0;

// Gets the ratio of the tilt from 0 - 1 of the maximum allowed tilt on the plane
GLfloat ratioOfTilt = 0.0;

// Maximum distance from center mouse can move (this is also middle of screen)
GLfloat maxMouseMove = 0.0;

// This is the angle to turn the plane by based on mouse position
GLfloat turnAngle = 0.0;

// This is the the roll interp when doing a roll trick
GLfloat rollAmount = 0.0;

// Roll height to interp too
GLfloat rollHeight = 0.0;

// Random height, width and x and z position for mountains
int randHeightList[NUM_MOUNTAINS];
int baseWidthList[NUM_MOUNTAINS];
int randXList[NUM_MOUNTAINS];
int randZList[NUM_MOUNTAINS];

/* Key checks to see if pressed or not */

// Not full screen by default
GLint isFullScreen = 0;
// Wire rendering on by default
GLint isWireRendering = 1;
// Sea ad sky enabled
GLint isSeaAndSky = 0;
// Fog enabled
GLint isFog = 1;
// If roll is enabled
GLint rollEnabled = 0;
// Crazy roll
GLint crazyRollEnabled = 0;
// Mountain textures on or off
GLint mountainTextureEnabled = 0;

// Toggles for directions key pressed and not pressed
GLint upPressed = 0;
GLint downPressed = 0;
GLint forwardPressed = 0;
GLint backwardPressed = 0;

/* Set up colors for open gl */
// Define colors for plane and propeller
color4 yellow = {1.0, 1.0, 0.0, 1.0};
color4 black = {0.0, 0.0, 0.0, 1.0};
color4 lightPurple = {0.87, 0.58, 0.98, 1.0};
color4 blue = {0.0, 0.0, 1.0, 1.0};
color4 red = {1.0, 0.0, 0.0, 1.0};
color4 green = {0.0, 1.0, 0.0, 1.0};
color4 white = {1.0, 1.0, 1.0, 1.0};
color4 grey = {0.05, 0.05, 0.05, 1.0};
color4 seaBlue = {0.0, 0.3, 0.8, 1.0};
color4 orange = {1.0, 0.5, 0.0, 1.0};
color4 clear = {1.0, 1.0, 1.0, 0.0};

/* Define lighting colors */

// Set up light colors
GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};

// Set global ambient
GLfloat globalAmbient[] = {0.05, 0.05, 0.05, 1.0};

/* Set up image stuff for loading in PPM */

// Image sizes for sea and sky
int imageWidthSea, imageHeightSea, imageWidthSky, imageHeightSky, imageWidthMountain, imageHeightMountain;

// Int ids for the textures
GLuint seaTextureID;
GLuint skyTextureID;
GLuint mountainTextureID;

// Pointers to store the texture data
GLubyte *imageDataSea;
GLubyte *imageDataSky;
GLubyte *imageDataMountain;


// Function name list

// Setup stuff
void setUpMountains();
void setUpTexture();
void loadSea();
void loadSky();
void loadMountain();
void lightingSetUp();
void setUpProp();
void setUpPlane();
void setUpFrameReferenceGrid();

// Move objects
void planeTricks();
void moveAllPlane();

// Drawing functions
void drawPlane();
void drawSkyAndSea();
void drawFrameReferenceGrid();
void enableFog();
void drawProps();

// Keyboard and mouse listeners
void normalKeys(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void specialKeysReleased(int key, int x, int y);
void mousePosition(int x, int y);

// Other
void printOutControls();
void myResize(int newWidth, int newHeight);
void fullScreen();
void wireRenderingCheck();

// Main functions
void init(void);
void myIdle(void);
void display(void);

#endif /* FLIGHTSIM_H_ */