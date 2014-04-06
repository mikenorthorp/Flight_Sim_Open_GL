README
======

README and code base best viewed on [Github](https://github.com/mikenorthorp/Flight_Sim_Open_GL)

This is the Project for my OpenGL Animation class, written in C and OpenGL
It is a flight simulator that has a plane flying around in a 3D environment. The basic environment when you load up the
program is a simple wireframe with a frame of reference. You can switch to the sea and sky environment with the controls
below and fly around an area with lighting, mountains, sea and sky. You can also toggle between wireframe drawing and full drawing.
See a full list of the controls below.

Requirements
------------

This program requires `C`, `freeGlut`, and `GLEW`.

This program is made to run in Visual Studio 2012, and you will have to configure the
freeglut libraries and linkers when running this on your own copy most likely.

A basic tutorial on how to get set up is found here [Setup](http://www.wyzant.com/resources/blogs/226103/opengl_freeglut_and_visual_studio_2012)

Installation
------------

1. Open the project in Visual studio, or grab the FlightSim.c and FlightSim.h files and set them up in
   your own environment.
2. Ensure you have `freeGlut`, and `GLEW` installed properly. As well as all includes and static libraries linked

Making the Program Do Things
----------------------------

1. Run the FlightSim.exe found in the main debug folder or run it in visual studio as a project. Or use the source code in the FlightSim
   folder called FlightSim.c and FlightSim.h
2. View the output to console to see the various keys that control the program.


- Scene Controls
- w: Toggle between wireframe and solid draw mode
- f: Toggle between fullscreen and non fullscreen
- s: Toggle between sea and sky and frame reference grid
- b: Toggle between fog on and off when in sea and sky mode
- t: Toggle between mountain textures on or off
- q: Quit the program


- Plane Controls
- Up Arrow: Go up in height
- Down Arrow: Go down in height
- Page Up: Speed up
- Page Down: Slow down
- Mouse left or right: Turn left or right


- Bonuses
- r: Do a barrel roll with the plane
- c: Do a crazy roll with the plane

3. To exit the program hit the q button or the button in the corner of the window

Bonus
-----

The bonus for this assignment is two different plane tricks. One is a barrel roll that spins the plane around and the other
is a more inverted spin that looks very immpressive. The controls for the button are "r" and "c".


Citations
=========
Lecture notes used. Code examples in class used in some parts of program, but modifed slightly.



