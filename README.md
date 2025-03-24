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
### Shading method - Flat shading
There is one problem with the flat shading. The program works best when you use faces constructed of 3 vertices. If the faces have more than 3 vertices and a complex shape, one that is not flat, the shading will be messed up.
One such example can be seen here.

<img src="/Images/03.png" width="400">

You can see there is an almost, if not completely black face around the left cheek. Now, moving the mouse just a little bit results in this

<img src="/Images/04.png" width="400">

The face has now turned completely white. This happens because the algorithm that is being used is made to work with flat faces made up of 3 vertices, and complex vertices don't work great with it.
The solution to this is to either break up the face into more faces in the program, or to fix the actual model.
#### Root of this problem
The root of this problem is the algorithm that is at work when doing the flat shading.
This is the code that applies the flat shading:
```c
Model.c -- The line numbers might not match
246             // Apply flat shading
247             for(int k = 0; k < 3; k++)
248             {
249                 int vertex_index = face[0].x;
250                 if(k != 0)
251                     vertex_index = face[j-1 + k].x;
252 
253                 // Get scale for shading.
254                 float scale = model->face_scale[index];
255                 g_vertices[k].color.r = 255 * scale;
256                 g_vertices[k].color.g = 255 * scale;
257                 g_vertices[k].color.b = 255 * scale;
258             }
```
this code however **only** applies the shading and does not calculate it.
The code that calculates *model->face_scale* (the variable that describes the shade of the face) looks like this:
```c
Model.c -- The line numbers might not match
141         // Calculate face normal -- FLAT SHADING
142         for(int j = 1; j < model->face_data[i]; j++)
143         {
144             if(j+1 >= model->face_data[i])
145                 break;
146 
147             // A face consists of triangles. Each one starts at vertex 0 of the face
148             Vector3f a = model->rotated_vertices[model->faces[i][0].x];
149             Vector3f b = model->rotated_vertices[model->faces[i][j].x];
150             Vector3f c = model->rotated_vertices[model->faces[i][j+1].x];
151 
152             // Calculate Vectors from points
153             // Calculation of normals is as such N = (ABxAC)/mag(ABxAC)
154             // Calculating Vector AB
155             Vector3f ab = {0};   
156             Sub3f(b, a, &ab);
157             // Calculating Vector AC
158             Vector3f ac = {0};
159             Sub3f(c, a, &ac);
160 
161             // Normalized cross product of (b-a, c-a) returns the normal vector 
162             Cross3f(ab, ac, &tmp_normal);
163             //printf("After cross (%f, %f, %f)\n", tmp_normal.x, tmp_normal.y, tmp_normal.z);
164             Normalize3f(tmp_normal, &tmp_normal);
165 
166             // Calculate and save the average direction
167             Vector3f *p_face_normal = (Vector3f*)&model->face_normal[i];
168             p_face_normal->x += tmp_normal.x;
169             p_face_normal->y += tmp_normal.y;
170             p_face_normal->z += tmp_normal.z;
171         }
172         // Calculate the face scale. Angle between camera_direction and normal vector.
173         // a = -1 means that they face eachother
174         // a > 0 -> means that they are not facing each other or are perpendicular
175         
176         int triangles = (model->face_data[i]-3)+1;
177         model->face_normal[i].x /= triangles;
178         model->face_normal[i].y /= triangles;
179         model->face_normal[i].z /= triangles;
(...)
184         Vector3f camera_direction = {0, 0, 1};
185         //float dot = Dot3f(camera_direction, model->face_normal[i]);
186         float angle = Angle3f(camera_direction, model->face_normal[i]);
(...)
191         //model->face_scale[i] = dot;
192         model->face_scale[i] = angle;

```
Looking at lines **167-170** and **176-179** we can see that there is an average being calculated. This is done because the algorithm calculates the shading by faces constructed of 3 vertices at a time.
The whole face is cycled trough by first calculating 3 vertices and then the starting vertex is indexed by 1 until the last 3 vertices are reached and an average normal direction is calculated for the whole face.
<br>

It's a simple way to calculate a face normal, but it only works when the face is flat, meaning that every set of 3 *neighbouring **vertices*** has the same, or almost the same normal direction. In case one of the 3
sets of vertices looks too far in a different direction, the face_scale will be messed up in relation to the *neighbouring **faces***
<br>

The main reason why this was done is because it leads to a less complicated structure to store the shading data...
```c
Model.h -- The line numbers might not match
 12 typedef struct
 13 {
 (...)
 20     /* Vector array containing faces and their data as a vector [FACES_SIZE][n] where
 21      * n is the numbert of vertices in the face 
 22      * faces[i][j] - i is the index of the face, j is the index/order of the vertex in the face
 23      * faces[i][j].x - Vertex index from *vertices
 24      * faces[i][j].y - Texture vertices -- TODO: not yet implemented
 25      * faces[i][j].z - Vertex normal -- TODO: not really needed as it is calculated on runtime
 26      */
 27     Vector3 **faces;
 28 
 29     // Array that stores the number of vertices in a face.               
 30     // The index i in this array is the index i of the face in **faces   
 31     int *face_data;
 32     Vector3f *face_normal;
 (...)
 54 } Model;

```
##### The fix to this problem
This could be fixed by making an array that stores the shading data for each set of 3 vertices in a face, but it's a bit more complicated as a data structure. Color blending could be used when drawing the face to not have sharp shading transitions between the sets of 3 vertices in each face, but it's a complication that I was not willing to implement yet.
<br>
Another way of fixing this is to just ignore the set that differs too much from the others. The final shading result might not be perfect but it would look better.

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
