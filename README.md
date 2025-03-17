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
