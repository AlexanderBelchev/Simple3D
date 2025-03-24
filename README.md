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

## Downsides
### Face draw order
This usually isn't a problem in models with many faces **however** if we have a model that doesn't have many faces but the faces are long, we could get results similar to this:

<img src="/Images/05.png" width="400">

Right under the tail of the plane, we can see where the occlusion culling has failed. This is the engine exhaust hole, it should not be visible through the fuselage of the plane. 
The root of the right wing can also be seen through the fuselage, and the whole rear horizontal sabilizer is visible through the vertical stabilizer.
#### Root of this problem
The root of this problem is the face sorting method that is being used.
<br>

When drawing a model to the screen, the code that should be written looks like this:
```c
main.c -- The line numbers might not match
 77         calculate_vertices(&monkey, &transform_matrix, &projection_matrix);
 78         sort_faces(&monkey);
 79         draw_model(&monkey, renderer);
```
We can see that we first calculate vertices - meaning that we apply all projection, rotation and transformation matrices.<br>
Then we **sort** the faces and finally draw the model. Sorting the faces in draw order is done inside the *sort_faces()* function. <br>
The code is big so here is a shortened version:
```c
117 void sort_faces(Model *model)
118 {
119     // Clear ordered_faces array
120     //memset(model->ordered_faces, 0, model->face_count * sizeof *model->ordered_faces);
121 
122     // Set index and corresponding depth value for each face at index i
123     for(int i = 0; i < model->face_count; i++)
124     {
125         model->ordered_faces[i].x = i;
126         
127         float average_depth = 0;
128         float scale = 0;
129         // Calculate average depth value
130         for(int j = 0; j < model->face_data[i]; j++)
131         {
132             // Get z-depth of vertex
133             average_depth += model->screen[model->faces[i][j].x].z; // Depth value of vertex
134         }
135         average_depth /= model->face_data[i]; // Divide by number of vertices to get average
136 
137 
138         Vector3f tmp_normal = {0};
139         //printf("Face %d\n", i);
140         model->face_normal[i] = (Vector3f){0};
141         // Calculate face normal -- FLAT SHADING
(...)
192 
193         //model->ordered_faces[i].y = model->screen[model->faces[i][0].x].z;
194         model->ordered_faces[i].y = average_depth;
195     }
196 
197     // Sort by largest depth number
198     for(int i = 0; i < model->face_count; i++)
199     {
200         for(int j = i + 1; j < model->face_count; j++)
201         {
202             if(model->ordered_faces[i].y < model->ordered_faces[j].y)
203             {
204                 Vector2f temp = {0};
205                 temp.x = model->ordered_faces[i].x;
206                 temp.y = model->ordered_faces[i].y;
207 
208                 model->ordered_faces[i].x = model->ordered_faces[j].x;
209                 model->ordered_faces[i].y = model->ordered_faces[j].y;
210 
211                 model->ordered_faces[j].x = temp.x;
212                 model->ordered_faces[j].y = temp.y;
213             }
214         }
215     }
216 }

```
Looking at lines **129-135** we can see that an average depth of all vertices is calculated. This depth is the distance from the vertex to the camera and is a result of the application of the projection matrix.
Getting the average depth of the face is the easiest way to get which face to draw first and last. 

##### The fix to this problem
There is an inherent problem with this method. Crossing faces can never be drawn correctly because 3D graphics are usually drawn **per-pixel** and not **per-face**.
Since this is a CPU based renderer, calculating every pixel on the screen with these algorithms and implementations is very slow, so **per-face** rendering is the only viable solution that I came up with.
<br>
Unless the whole program is altered, this issue can only be fixed by making models that do not have any crossing faces. Also, faces should not be too long as to not result in an average position that could be farther from the camera than it should be.
