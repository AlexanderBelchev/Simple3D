#include "Model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local functions for this file only

void get_vertex_vector(Vector3f *out, char *line, int len);
// Return the number of vertices in the face
int get_faces(Vector3 *out, char *line, int len);
void get_face_normals(Vector3 *out, char *line, int len);

void get_normal_vector(Vector3f *out, char *line, int len);

int get_keyword_count(const char* key, FILE *file_ptr);

void load_model(Model *model, const char* filepath)
{
    FILE *file_ptr;
    printf("Opening file\n");
    file_ptr = fopen(filepath, "rb");
    printf("File open\n");
    if(file_ptr != NULL)
    {
        char line_buffer[128];
        int v_i = 0;
        int f_i = 0;
        int n_i = 0;
        int v = 0;
        int f = 0;
        int n = 0;
        // Count number of vertices
        v = get_keyword_count("v", file_ptr);
        // Allocate memory for vertices
        model->vertices = malloc(v * sizeof(Vector3f));
        for(int k = 0; k < v; k++)
            model->vertices[k] = (Vector3f){0};

        printf("Vertex count: %d\n", v);

        // Count number of faces
        f = get_keyword_count("f", file_ptr);

        model->normals = malloc(n * sizeof(Vector3f));
        for(int k = 0; k < n; k++)
            model->normals[k] = (Vector3f){0};
        printf("Faces count: %d\n", f);


        // Count number of vertex normals
        n = get_keyword_count("vn", file_ptr);

        model->faces = malloc(f*sizeof(model->faces));
        model->face_data = malloc(f*sizeof *model->face_data);

        for(int j = 0; j < f; j++)
        {
            model->face_data[j] = 0;
            model->faces[j] = malloc(5*sizeof(Vector3));
            for(int k = 0; k < 5; k++)
            {
                model->faces[j][k] = (Vector3){0};
            }
        }


        printf("Normals count: %d\n", n);

        while(fgets(line_buffer, sizeof(line_buffer), file_ptr) != NULL)
        {
            if(v_i >= MODEL_SIZE-1)
                break;
            // Extract the keyword from the line
            char keyword[6];
            int i = 0;
            while(line_buffer[i] != ' ' && i < strlen(line_buffer))
            {
                keyword[i] = line_buffer[i];
                i++;
            }
            keyword[i] = '\0';


            if(strcmp(keyword, "v") == 0)
            {
                get_vertex_vector(&model->vertices[v_i], line_buffer, strlen(line_buffer));
                v_i++;
            }
            else if(strcmp(keyword, "vn") == 0)
            {
                //get_normal_vector(&model->normals[n_i], line_buffer, strlen(line_buffer));
                //n_i++;
            }
            else if(strcmp(keyword, "f") == 0)
            {
                model->face_data[f_i] = get_faces(model->faces[f_i], line_buffer, strlen(line_buffer));
                get_face_normals(model->faces[f_i], line_buffer, strlen(line_buffer));
                f_i++;
            }
        }

        model->vertex_count = v;
        model->face_count = f_i;
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
        return count;
        /*for(int j = 0; j < 5; j++)
          {
            char *start_ptr, *end_ptr;
            i++;

            start_ptr = &line[i];
            while(line[i] != ' ' && line[i] != '\n') i++;
            end_ptr = &line[i];

            // Get vertex index
            char data[32];
            int data_len = (end_ptr - start_ptr);
            strncpy(data, start_ptr, data_len);
            data[data_len] = '\0';

            char *str = strtok(data, "/");
            // Save vertex to out
            out[j].x = (int)strtol(str, &str + strlen(str) , 10);
            //str = strtok(NULL, "/");
            //out[j].z = (int)strtol(str, &str + strlen(str), 10);
            if(line[i] == '\n')
                break;
        }

        // Subtract 1 from all to be used as indexes.
        // -1 means that there is nothing to connect to
        for(int k = 0; k < 5; k++)
        {
        out[k].x -= 1;
        //out[k].z -= 1;
        }*/
    }
}

void get_face_normals(Vector3 *out, char *line, int len)
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
        for(int j = 0; j < 5; j++)
        {
            char *start_ptr, *end_ptr;
            i++;

            start_ptr = &line[i];
            while(line[i] != ' ' && line[i] != '\n') i++;
            end_ptr = &line[i];

            // Get vertex index
            char data[32];
            int data_len = (end_ptr - start_ptr);
            strncpy(data, start_ptr, data_len);
            data[data_len] = '\0';

            char *str = strtok(data, "/"); // first
            str = strtok(NULL, "/");       // second
            str = strtok(NULL, "/");       // third - the normals 

            // Save vertex to out
            out[j].z = (int)strtol(str, &str + strlen(str) , 10);
            if(line[i] == '\n')
                break;
        }

        // Subtract 1 from all to be used as indexes.
        // -1 means that there is nothing to connect to
        for(int k = 0; k < 5; k++)
        {
            out[k].z -= 1;
        }
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
