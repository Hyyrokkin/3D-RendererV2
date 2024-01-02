#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "meshLoader.h"

mesh* LoadMesh(const char* meshName)
{
    FILE* fptr;
    fptr = fopen(meshName, "r");

    vec3d* tmpVerts = malloc(sizeof(vec3d*) * 100000);
    int tmpVertsCount = 0;

    triangle * tmpTris = malloc(sizeof(triangle*) * 100000);
    int tmpTrisCount = 0;

    char line[256];
    while (fgets(line, sizeof(line), fptr))
    {
        line[strcspn(line, "\n")] = 0;

        if(line[0] == 'v')
        {
            char* delim = {" "};

            char *ptr = strtok(line , delim);
            ptr = strtok(NULL, delim);
            float f1 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f2 = strtof(ptr, NULL);
            ptr = strtok(NULL, delim);
            float f3 = strtof(ptr, NULL);

            tmpVertsCount++;
            tmpVerts[tmpVertsCount].x = f1;
            tmpVerts[tmpVertsCount].y = f2;
            tmpVerts[tmpVertsCount].z = f3;
            tmpVerts[tmpVertsCount].w = 1;
        }
        if(line[0] == 'f')
        {
            char* delim = {" "};

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

    mesh* m = malloc(sizeof(mesh));

    m->triCount = tmpTrisCount;
    m->tris = malloc(sizeof(triangle) * m->triCount);
    m->worldPos.x = 0.0f;
    m->worldPos.y = 0.0f;
    m->worldPos.z = 0.0f;
    m->worldPos.w = 1.0f;

    for (int i = 0; i < tmpTrisCount; ++i) {
        memcpy(&m->tris[i], &tmpTris[i], sizeof(triangle));
        //printf("m[%d]:\n - v1: %f %f %f\n - v2: %f %f %f\n - v3: %f %f %f\n\n", i, m->tris->p[0].x, m->tris->p[0].y, m->tris->p[0].z, m->tris->p[1].x, m->tris->p[1].y, m->tris->p[1].z, m->tris->p[2].x, m->tris->p[2].y, m->tris->p[2].z);
    }

    return m;
}

void FreeMesh(mesh* mesh)
{
    free(mesh->tris);
    free(mesh);
}