#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libs/stb/stb_ds.h"

#include "meshLoader.h"

//Load a mesh from a file path
mesh* LoadMesh(const char meshName[static 1])
{
    //Build the actual Mesh
    mesh* m = malloc(sizeof(mesh));

    m->tris = NULL;
    m->worldPos.x = 0.0f;
    m->worldPos.y = 0.0f;
    m->worldPos.z = 0.0f;
    m->worldPos.w = 1.0f;

    //TODO Unsafe :(
    FILE* fptr;
    fptr = fopen(meshName, "r");

    vec3d* tmpVerts = NULL;
    arrput(tmpVerts, ((vec3d){ 0 }));

    //Reads over every line of the file
    char line[256];
    while (fgets(line, sizeof(line), fptr))
    {
        line[strcspn(line, "\n")] = 0;

        //Line describes a vertex
        if(line[0] == 'v')
        {
            char* delim = {" "};

            //TODO do this stuff right
            strtok(line , delim);
            char *ptr = strtok(NULL, delim);
            float f1 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f2 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f3 = strtof(ptr, NULL);

            arrput(tmpVerts, ((vec3d){.x=f1, .y=f2, .z=f3, .w=1}));
        }
        //line describes a triangle
        else if(line[0] == 'f')
        {
            char* delim = {" "};

            //TODO do this stuff right
            strtok(line , delim);
            char *end;
            char *ptr = strtok(NULL, delim);
            int i1 = strtol(ptr, &end, 10);
            ptr = strtok(NULL, delim);
            int i2 = strtol(ptr, &end, 10);
            ptr = strtok(NULL, delim);
            int i3 = strtol(ptr, &end, 10);

            triangle tmp = {.p[0]=tmpVerts[i1],.p[1]=tmpVerts[i2],.p[2]=tmpVerts[i3]};
            arrput(m->tris, tmp);
        }
    }

    fclose(fptr);
    arrfree(tmpVerts);

    return m;
}

void FreeMesh(mesh mesh[static 1])
{
    arrfree(mesh->tris);
    free(mesh);
}