#include "Model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------ Local functions for this file only ------*/
void get_vertex_vector(Vector3f *out, char *line, int len);

// Return the number of vertices in the face
int get_faces(Vector3 *out, char *line, int len);

// Get the normal vector and save it to the *out pointer
void get_normal_vector(Vector3f *out, char *line, int len);

// Return the number of times "key" appears in a file
// This DOESN'T reset the file cursor to the start <--------------- IMPORTANT
int get_keyword_count(const char *key, FILE *file_ptr);

/*------ End of local function definities ------*/

void load_model(Model *model, const char* filepath)
{
    FILE *file_ptr;
    printf("Opening file\n");
    file_ptr = fopen(filepath, "rb");
    printf("File open\n");
    if(file_ptr != NULL)
    {
        char line_buffer[512];
        // These are counters, used when reading the file, to keep track
        // of how many vertices and faces have been loaded into the model
        int v_i = 0;
        int f_i = 0;
        // Count number of vertices
        model->vertex_count = get_keyword_count("v", file_ptr);
        // Allocate memory for vertices
        model->vertices = malloc(model->vertex_count * sizeof *model->vertices);
        memset(model->vertices, 0, model->vertex_count* sizeof *model->vertices);

        model->rotated_vertices = malloc(model->vertex_count*sizeof *model->rotated_vertices);
        memset(model->rotated_vertices, 0, model->vertex_count *sizeof *model->rotated_vertices);

        printf("Vertex count: %d\n", model->vertex_count);

        // Count number of faces
        model->face_count = get_keyword_count("f", file_ptr);
        
        model->faces = malloc(model->face_count*sizeof *model->faces);

        model->face_data = malloc(model->face_count*sizeof *model->face_data);
        memset(model->face_data, 0, model->face_count*sizeof *model->face_data);

        model->face_normal = malloc(model->face_count*sizeof *model->face_normal);
        memset(model->face_normal, 0, model->face_count*sizeof *model->face_normal);

        model->face_scale = malloc(model->face_count*sizeof *model->face_scale);
        memset(model->face_scale, 0, model->face_count*sizeof *model->face_scale);
    
        model->ordered_faces = malloc(model->face_count*sizeof *model->ordered_faces);
        memset(model->ordered_faces, 0, model->face_count*sizeof *model->ordered_faces);

        for(int j = 0; j < model->face_count; j++)
        {
            //model->face_data[j] = 0;
            //model->face_normal[j] = (Vector3f){0};
            model->faces[j] = malloc(5*sizeof(Vector3));
            //model->ordered_faces[j] = (Vector2f){0};

            for(int k = 0; k < 5; k++)
            {
                model->faces[j][k] = (Vector3){0};
            }
        }

        printf("Faces count: %d\n", model->face_count);

        while(fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL)
        {
            if(v_i >= MODEL_SIZE-1)
                v_i = -1;
            // Extract the keyword from the line
            char keyword[6];
            int i = 0;
            while(line_buffer[i] != ' ' && i < strlen(line_buffer))
            {
                keyword[i] = line_buffer[i];
                i++;
            }
            keyword[i] = '\0';


            if(strcmp(keyword, "v") == 0 && v_i != -1)
            {
                get_vertex_vector(&model->vertices[v_i], line_buffer, strlen(line_buffer));
                v_i++;
            }
            else if(strcmp(keyword, "f") == 0)
            {
                model->face_data[f_i] = get_faces(model->faces[f_i], line_buffer, strlen(line_buffer));
                f_i++;
            }
        }
        printf("Loaded %d faces\n", f_i);

        printf("End of file\n");
        fclose(file_ptr);
        printf("File closed\n");
    } 
    else
    {
        fprintf(stderr, "Unable to open file!\n");
        return;
    }
}

void sort_faces(Model *model)
{
    // Clear ordered_faces array
    //memset(model->ordered_faces, 0, model->face_count * sizeof *model->ordered_faces);

    // Set index and corresponding depth value for each face at index i
    for(int i = 0; i < model->face_count; i++)
    {
        model->ordered_faces[i].x = i;
        
        float average_depth = 0;
        float scale = 0;
        // Calculate average depth value
        for(int j = 0; j < model->face_data[i]; j++)
        {
            // Get z-depth of vertex
            average_depth += model->screen[model->faces[i][j].x].z; // Depth value of vertex
        }
        average_depth /= model->face_data[i]; // Divide by number of vertices to get average


        Vector3f tmp_normal = {0};
        //printf("Face %d\n", i);
        model->face_normal[i] = (Vector3f){0};
        // Calculate face normal -- FLAT SHADING
        for(int j = 1; j < model->face_data[i]; j++)
        {
            if(j+1 >= model->face_data[i])
                break;

            // A face consists of triangles. Each one starts at vertex 0 of the face
            Vector3f a = model->rotated_vertices[model->faces[i][0].x];
            Vector3f b = model->rotated_vertices[model->faces[i][j].x];
            Vector3f c = model->rotated_vertices[model->faces[i][j+1].x];

            // Calculate Vectors from points
            // Calculation of normals is as such N = (ABxAC)/mag(ABxAC)
            // Calculating Vector AB
            Vector3f ab = {0};
            Sub3f(b, a, &ab);
            // Calculating Vector AC
            Vector3f ac = {0};
            Sub3f(c, a, &ac);

            // Normalized cross product of (b-a, c-a) returns the normal vector 
            Cross3f(ab, ac, &tmp_normal);
            //printf("After cross (%f, %f, %f)\n", tmp_normal.x, tmp_normal.y, tmp_normal.z);
            Normalize3f(tmp_normal, &tmp_normal);

            // Calculate and save the average direction
            Vector3f *p_face_normal = (Vector3f*)&model->face_normal[i];
            p_face_normal->x += tmp_normal.x;
            p_face_normal->y += tmp_normal.y;
            p_face_normal->z += tmp_normal.z;
        }
        // Calculate the face scale. Angle between camera_direction and normal vector.
        // a = -1 means that they face eachother
        // a > 0 -> means that they are not facing each other or are perpendicular
        
        int triangles = (model->face_data[i]-3)+1;
        model->face_normal[i].x /= triangles;
        model->face_normal[i].y /= triangles;
        model->face_normal[i].z /= triangles;

        // TODO: Camera direction is not in -z but in z, maybe this is affected because of the 
        // calculation of the dot product, or maybe the vectors are inverted. Worst case it's 
        // one of the matrices.
        Vector3f camera_direction = {0, 0, 1};
        //float dot = Dot3f(camera_direction, model->face_normal[i]);
        float angle = Angle3f(camera_direction, model->face_normal[i]);
        //TODO: I don't like how this is the angle at which the face is facing the camera.
        //      Perhaps I have misunderstood how the formula works, because it didn't use 
        //      the dot product, to my knowledge at least. Altough I think dot product is
        //      the angle between these vectors...idk. I have to check it out later.
        //model->face_scale[i] = dot;
        model->face_scale[i] = angle;

        //model->ordered_faces[i].y = model->screen[model->faces[i][0].x].z;
        model->ordered_faces[i].y = average_depth;
    }

    // Sort by largest depth number
    for(int i = 0; i < model->face_count; i++)
    {
        for(int j = i + 1; j < model->face_count; j++)
        {
            if(model->ordered_faces[i].y < model->ordered_faces[j].y)
            {
                Vector2f temp = {0};
                temp.x = model->ordered_faces[i].x;
                temp.y = model->ordered_faces[i].y;

                model->ordered_faces[i].x = model->ordered_faces[j].x;
                model->ordered_faces[i].y = model->ordered_faces[j].y;

                model->ordered_faces[j].x = temp.x;
                model->ordered_faces[j].y = temp.y;
            }
        }
    }
}

// TODO: Sometimes this causes a crash. Some array goes of out bounds perhaps
void draw_model(Model *model, SDL_Renderer *renderer)
{
    //for(int i = model->face_count-1; i >= 0; i--)
    for(int i = 0; i < model->face_count; i++)
    {
        int index = (int)model->ordered_faces[i].x;
        Vector3 *face;
        face = model->faces[index];
        int vertices_number = model->face_data[index];

        // Convert points to triangles
        // Using a simple "triangle transformation"(?I think it's called that?)
        int n = 1;
        SDL_Vertex g_vertices[3];

        for(int j = 1; j < vertices_number; j++)
        {
            if(j+1 >= vertices_number)
                break;

            g_vertices[0] = model->screen_vertices[face[0].x];
            g_vertices[1] = model->screen_vertices[face[j].x];
            g_vertices[2] = model->screen_vertices[face[j+1].x];


            // Apply flat shading
            for(int k = 0; k < 3; k++)
            {
                int vertex_index = face[0].x;
                if(k != 0)
                    vertex_index = face[j-1 + k].x;

                // Get scale for shading.
                float scale = model->face_scale[index];
                g_vertices[k].color.r = 255 * scale;
                g_vertices[k].color.g = 255 * scale;
                g_vertices[k].color.b = 255 * scale;
            }

            // Extract vectors for vector calculation
            Vector2f a = {0}, b = {0}, c = {0};
            a.x = g_vertices[0].position.x;
            a.y = g_vertices[0].position.y;
            b.x = g_vertices[1].position.x;
            b.y = g_vertices[1].position.y;
            c.x = g_vertices[2].position.x;
            c.y = g_vertices[2].position.y;

            Vector2f mod_a = {0};
            Sub(b, a, &mod_a);

            Vector2f mod_b = {0};
            Sub(c, a, &mod_b);

            // Back face culling (very simple method of drawing too)
            // If the cross product between the modified vectors is less than 0, then they should not
            // be rendered.
            if(Cross(mod_a, mod_b) >= 0.1)
            {
                SDL_RenderGeometry(renderer, NULL, g_vertices, 3, NULL, 0);
            }
        }
    }
}

void calculate_vertices(Model *model, Matrix4x4 *transform_matrix, Matrix4x4 *projection_matrix)
{
    for(int i = 0; i < model->vertex_count; i++)
    {
        Vector3f result_vertex = {0};
        Vector3f position_result = {0};
        Vector3f transform_result_vertex = {0};
        Vector3f *vertex = &model->vertices[i];

        // Apply x-rotation matrix
        Matrix4x4 x_rotation_matrix = {0};
        Vector3f x_rotation_result = {0};
        SetXRotationMatrix(&x_rotation_matrix, model->rotation.x);
        MultiplyPointBy4x4(vertex, &x_rotation_matrix, &x_rotation_result);

        // Apply y-rotation matrix
        Matrix4x4 y_rotation_matrix = {0};
        Vector3f y_rotation_result = {0};
        SetYRotationMatrix(&y_rotation_matrix, model->rotation.y);
        MultiplyPointBy4x4(&x_rotation_result, &y_rotation_matrix, &y_rotation_result);

        // Apply z-rotation matrix
        Matrix4x4 z_rotation_matrix = {0};
        Vector3f z_rotation_result = {0};
        SetZRotationMatrix(&z_rotation_matrix, model->rotation.z);
        MultiplyPointBy4x4(&y_rotation_result, &z_rotation_matrix, &z_rotation_result);


        // Update camera matrix with model position
        Matrix4x4 transform_matrix = {0};
        SetTransformMatrix(&transform_matrix, model->position);
        //SetTransformMatrix(&transform_matrix, position);
        // Apply camera matrix 
        MultiplyPointBy4x4(&z_rotation_result, &transform_matrix, &transform_result_vertex);

        model->rotated_vertices[i] = (Vector3f)transform_result_vertex;
        // Apply projection
        MultiplyPointBy4x4(&transform_result_vertex, projection_matrix, &result_vertex);

        // Let's see if the vertex point is visible
        if(result_vertex.x > 1 || result_vertex.x < -1 ||
                result_vertex.y > 1 || result_vertex.y < -1 ||
                result_vertex.z < 0);
            //continue;   // Point isn't visible

        // Convert to screen space
        int x = (result_vertex.x + 1) * 0.5 * 1920;
        int y = (result_vertex.y + 1) * 0.5 * 1080;

        float depth = result_vertex.z;

        Vector3f *v = &model->screen[i];
        SDL_Vertex *vert = &model->screen_vertices[i];
        vert->position.x = x;
        vert->position.y = y;
        v->z = depth;

        vert->color.r = 255;
        vert->color.g = 255;
        vert->color.b = 255;
        vert->color.a = 255;
    }
}


void get_vertex_vector(Vector3f *out, char *line, int len)
{
    char keyword[6];
    int i = 0;
    while(line[i] != ' ' && i < len)
    {
        keyword[i] = line[i];
        i++;
    }
    keyword[i] = '\0';
    // Check if the line given is for this function
    if(strcmp(keyword, "v") == 0)
    {
        // To read numerical values
        char *start_ptr, *end_ptr;
        i++;
        // Read x value
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->x = strtof(start_ptr, &end_ptr);
        //printf("X: %f ", out->x);

        // Read y value
        i++;
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->y = strtof(start_ptr, &end_ptr);
        //printf("Y: %f ", out->y);

        // Read z value
        i++;
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->z = strtof(start_ptr, &end_ptr);
        //printf("Z: %f \n", out->z);
    }
}

void get_normal_vector(Vector3f *out, char *line, int len)
{
    char keyword[6];
    int i = 0;
    while(line[i] != ' ' && i < len)
    {
        keyword[i] = line[i];
        i++;
    }
    keyword[i] = '\0';
    // Check if the line given is for this function
    if(strcmp(keyword, "vn") == 0)
    {
        // To read numerical values
        char *start_ptr, *end_ptr;
        i++;
        // Read x value
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->x = strtof(start_ptr, &end_ptr);

        // Read y value
        i++;
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->y = strtof(start_ptr, &end_ptr);

        // Read z value
        i++;
        start_ptr = &line[i];
        while(line[i] != ' ') i++;
        end_ptr = &line[i];
        out->z = strtof(start_ptr, &end_ptr);
    }
}

int get_faces(Vector3 *out, char *line, int len)
{
    char keyword[6];
    int i = 0;
    while(line[i] != ' ' && i < len)
    {
        keyword[i] = line[i];
        i++;
    }
    keyword[i] = '\0';
    if(strcmp(keyword, "f") == 0)
    {
        int count = 0;
        int j = 0;
        while(line[i] != '\n')
        {
            char *start_ptr, *end_ptr;
            i++;

            if(i >= len) break;

            start_ptr = &line[i];
            while(line[i] != ' ' && line[i] != '\n') i++;
            if(i >= len) break;
            end_ptr = &line[i];

            // Get vertex index
            char data[32];
            int data_len = (end_ptr - start_ptr);
            strncpy(data, start_ptr, data_len);
            data[data_len] = '\0';

            char *str = strtok(data, "/");
            // Save vertex to out
            out[j].x = (int)strtol(str, &str + strlen(str) , 10);

            // Remove the first character of the line, to prepare for the get_split_value function

            count++;    //<--------------------------IF SOMETHING CAUSES A SF, IT'S THIS
            j++;
        }
        // Subtract 1 from all to be used as indexes.
        // -1 means that there is nothing to connect to
        for(int k = 0; k < count; k++)
        {
            out[k].x -= 1;
        }
        //model->face_data[i] = count;
        //printf("Vertex count in this face: %d\n", count);
        return count;
    }
}

int get_keyword_count(const char* key, FILE *file_ptr)
{
    int count = 0;
    char line_buffer[128];
    while(fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL)
    {
        char keyword[6];
        int i = 0;
        while(line_buffer[i] != ' ' && i < strlen(line_buffer))
        {
            keyword[i] = line_buffer[i];
            i++;
        }
        keyword[i] = '\0';
        if(strcmp(keyword, key) == 0) count++;
    }
    fseek(file_ptr, 0, SEEK_SET);
    return count;
}
