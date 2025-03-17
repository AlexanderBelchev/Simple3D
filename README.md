# Simple 3D
## What is this project about?
This is a simple 3D renderer written in C. The only external library for this project is SDL.
SDL is used to draw the graphics on the screen.
The program runs on the processor, and does not use miltithreading and it does not run any of the code on the GPU. That could be done with OpenCL but it is not in the scope of this project.
## Setup
Before building the project you might have to setup SDL.
### Linux
Simply running this command

`sudo apt-get install libsdl2-dev`

should install everything you need for this project.
Incase that doesn't work, you can visit [here](https://wiki.libsdl.org/SDL2/Installation) for more information on how to download and setup SDL.
At the time of writing this, this is the most relevant source.
## Building this project
### Windows
I have not yet tested how to build it on windows.
### Linux
To build the program you have to run

`cmake --build .`

An executable named **simple3d_demo** wil be created. Run it with.

`./simple3d_demo`

The program should open without problems.

## Demonstration
After opening the program, depending on the system it might take a few seconds to load.
The current iteration of the project loads a monkey head model from blender, exported as a *.obj file.
The file is being read so until that is finished the program wont continue.
When the file gets loaded, the model will appear on the screen.

<img src="/Images/01.png" width="400">

If you move your mouse around you will notice that the model will also rotated according to the movement of the mouse

<img src="/Images/02.png" width="400">

This works, and you can also see that there is shading on each face of the model, it's not smooth shading, it's an average shade for the whole face, but it still has the desired effect of giving depth to the model.
There is on problem with it. The program works best when you use faces constructed of 3 vertices. If the faces have more than 3 vertices and a complex shape, one that is not flat, the shading will be messed up.
One such example can be seen here.

<img src="/Images/03.png" width="400">

You can see there is an almost, if not completely black face around the left cheek. Now, moving the mouse just a little bit results in this

<img src="/Images/04.png" width="400">

The face has now turned completely white. This happens because the algorithm that is being used is made to work with flat faces made up of 3 vertices, and complex vertices don't work great with it.
The solution to this is to either break up the face into more faces in the program, or to fix the actual model.
