#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "meshLoader.h"

//Load a mesh from a file path
mesh* LoadMesh(const char* meshName)
{
    //TODO Unsafe :(
    FILE* fptr;
    fptr = fopen(meshName, "r");

    //TODO change it so it will do this stuff dynamically
    vec3d* tmpVerts = malloc(sizeof(vec3d*) * 100000);
    int tmpVertsCount = 0;

    triangle * tmpTris = malloc(sizeof(triangle*) * 100000);
    int tmpTrisCount = 0;

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
            char *ptr = strtok(line , delim);
            ptr = strtok(NULL, delim);
            float f1 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f2 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f3 = strtof(ptr, NULL);

            tmpVertsCount++;
            tmpVerts[tmpVertsCount] = (vec3d){f1, f2, f3, 1};
        }
        //line describes a triangle
        else if(line[0] == 'f')
        {
            char* delim = {" "};

            //TODO do this stuff right
            char *ptr = strtok(line , delim);
            ptr = strtok(NULL, delim);
            int i1 = atoi(ptr);
            ptr = strtok(NULL, delim);
            int i2 = atoi(ptr);
            ptr = strtok(NULL, delim);
            int i3 = atoi(ptr);

            tmpTris[tmpTrisCount].p[0] = tmpVerts[i1];
            tmpTris[tmpTrisCount].p[1] = tmpVerts[i2];
            tmpTris[tmpTrisCount].p[2] = tmpVerts[i3];
            tmpTrisCount++;
        }
    }

    fclose(fptr);
    free(tmpVerts);

    //Build the actual Mesh
    mesh* m = malloc(sizeof(mesh));

    m->triCount = tmpTrisCount;
    m->tris = malloc(sizeof(triangle) * m->triCount);
    m->worldPos.x = 0.0f;
    m->worldPos.y = 0.0f;
    m->worldPos.z = 0.0f;
    m->worldPos.w = 1.0f;

    //Copy all the data into the Mesh
    for (int i = 0; i < tmpTrisCount; ++i)
    {
        memcpy(&m->tris[i], &tmpTris[i], sizeof(triangle));
    }

    return m;
}

void FreeMesh(mesh* mesh)
{
    free(mesh->tris);
    free(mesh);
}