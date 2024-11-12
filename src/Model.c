#include "Model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local functions for this file only

void get_vertex_vector(Vector3f *out, char *line, int len);
void get_faces(int out[5], char *line, int len);

void load_model(Model *model, const char* filepath)
{
    FILE *file_ptr;
    printf("opening file\n");
    file_ptr = fopen(filepath, "rb");
    printf("File open\n");
    if(file_ptr != NULL)
    {
        char line_buffer[128];
        int v_i = 0;
        int f_i = 0;
        int v = 0;
        int f = 0;
        // Count number of vertices
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
            if(strcmp(keyword, "v") == 0) v++;
        }

        printf("Vertex count: %d\n", v);
        fseek(file_ptr, 0, SEEK_SET); // Reset file reader to start of file
        
        // Count number of faces
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
            if(strcmp(keyword, "f") == 0) f++;
        }

        printf("Face count: %d\n", f);
        fseek(file_ptr, 0, SEEK_SET); // Reset file reader to start of file


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
            else if(strcmp(keyword, "f") == 0)
            {
                get_faces(model->faces[f_i], line_buffer, strlen(line_buffer));
                f_i++;
            }

        }
        
        model->vertex_count = v_i + 1;
        model->face_count = f_i + 1;
        printf("End of file\n");
        fclose(file_ptr);
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

void get_faces(int out[5], char* line, int len)
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

            char *str = strtok(data, "/");

            // Save vertex to out
            out[j] = (int)strtol(str, &str + strlen(str) , 10);
            if(line[i] == '\n')
                break;
        }

        // Subtract 1 from all to be used as indexes.
        // -1 means that there is nothing to connect to
        for(int k = 0; k < 5; k++)
        {
            out[k] -= 1;
        }
    }
}
