#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#include "renderer.h"
#include "util.h"
#include "meshLoader.h"
#include "settings.h"

static mesh** meshList = { 0 };
static int meshCount = { 0 };
static mat4x4 matProj = { 0 };

static vec3d camera = {.x=0.0F, .y=0.0F, .z=0.0F, .w=1.0F};
static vec3d lookDir = { 0 };

static float theta = { 0.0F };
static float  yaw = { 0.0F };
static float  pitch = { 0.0F };

static void RenderMesh(mesh meshToRender[static 1]);
static void HandleInputs(void);

//Function gets called only once at Project startup
void Setup(void)
{
    printf("[3D Render] - Started Setup\n");

    meshCount = 1;
    meshList = malloc(sizeof(mesh*)*meshCount);

    meshList[0] = LoadMesh("C:\\Users\\Morit\\Downloads\\ProgrammShit\\C Shit\\3D_Render\\meshes\\lambo.obj");
    meshList[0]->worldPos.z = 3.0f;

    float aspectRatio = WINDOW_HEIGHT / WINDOW_WIDTH;
    MakeMatProjection(&matProj, FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);

    printf("[3D Render] - Finished Setup\n");
}

//Function that gets called every Frame
void Update(void)
{
    ClearBackground(DARKGRAY);
    theta += GetFrameTime();

    HandleInputs();

    //Render All Meshes
    for (int i = 0; i < meshCount; ++i)
    {
        RenderMesh(meshList[i]);
    }
}

static void HandleInputs(void)
{
    vec3d forward = { 0 };
    vec3d right = { 0 };
    vec3d  up = { 0 };

    //Get current cam dir vectors
    CopyVector(&forward, &lookDir);
    CrossProduct(&right, &lookDir, &(vec3d) {.x=0.0f, .y=1.0f, .z=0.0f, .w=1.0f});
    CrossProduct(&up, &lookDir, &right);

    NormalizeVector(&forward);
    NormalizeVector(&right);
    NormalizeVector(&up);

    //Scale them by move speed and delta time
    MultiplyVector(&forward, &forward, HOR_MOVE_SPEED * GetFrameTime());
    MultiplyVector(&right, &right, HOR_MOVE_SPEED * GetFrameTime());
    MultiplyVector(&up, &up, VERT_MOVE_SPEED * GetFrameTime());

    //Move stuff if key is pressed
    if(IsKeyDown(KEY_W))
        AddVector(&camera, &camera, &forward);

    if(IsKeyDown(KEY_S))
        SubVector(&camera, &camera, &forward);

    if(IsKeyDown(KEY_A))
        SubVector(&camera, &camera, &right);

    if(IsKeyDown(KEY_D))
        AddVector(&camera, &camera, &right);

    if(IsKeyDown(KEY_SPACE))
        SubVector(&camera, &camera, &up);

    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        AddVector(&camera, &camera, &up);


    if(IsKeyDown(KEY_LEFT))
        yaw -= TURN_SPEED * GetFrameTime();

    if(IsKeyDown(KEY_RIGHT))
        yaw += TURN_SPEED * GetFrameTime();

    if(IsKeyDown(KEY_UP) && pitch < 1.5f)
        pitch += TURN_SPEED * GetFrameTime();

    if(IsKeyDown(KEY_DOWN) && pitch > -1.5f)
        pitch -= TURN_SPEED * GetFrameTime();
}

//Render one Mesh at a time
//TODO Make them Render all at once (possible?)
static void RenderMesh(mesh meshToRender[static 1])
{
    triangle* trisToRaster = malloc(sizeof(triangle) * meshToRender->triCount * 2);
    int trisToRasterCount = 0;

    //Setup rotation and translation matrices
    mat4x4 matRotZ = { 0 };
    mat4x4 matRotY = { 0 };
    mat4x4 matRotX = { 0 };
    mat4x4 matTrans = { 0 };

    MakeMatRotZ(&matRotZ, 0.0f);
    MakeMatRotY(&matRotY, 0.0f);
    MakeMatRotX(&matRotX, 0.0f);

    MakeMatTranslate(&matTrans, &meshToRender->worldPos);

    mat4x4 matWorld = { 0 };
    MakeMatIdentity(&matWorld);
    MultiplyMatrixMatrix(&matWorld, &matWorld, &matRotZ);
    MultiplyMatrixMatrix(&matWorld, &matWorld, &matRotY);
    MultiplyMatrixMatrix(&matWorld, &matWorld, &matRotX);
    MultiplyMatrixMatrix(&matWorld, &matWorld, &matTrans);


    //Setup Camera and update look dir
    //TODO dont do this every mesh (?)
    vec3d up = {.x=0.0f, .y=-1.0f, .z=0.0f, .w=1.0f};
    NormalizeVector(&up);

    vec3d target = {.x=0.0f, .y=0.0f, .z=1.0f, .w=1.0f};
    mat4x4 matCameraRotY = { 0 };
    mat4x4 matCameraRotX = { 0 };
    MakeMatRotY(&matCameraRotY, yaw);
    MakeMatRotX(&matCameraRotX, pitch);
    MultiplyMatrixVector(&target, &target, &matCameraRotX);
    MultiplyMatrixVector(&lookDir, &target, &matCameraRotY);
    NormalizeVector(&lookDir);
    AddVector(&target, &camera, &lookDir);

    mat4x4 matCamera = { 0 };
    mat4x4 matView = { 0 };
    MakeMatPointAt(&matCamera, &camera, &target, &up);
    MatrixQuickInverse(&matView, &matCamera);

    //Do the render stuff for every triangle in the Mesh
    for (int i = 0; i < meshToRender->triCount; ++i)
    {
        //Move the Triangle from Object space to World space
        triangle triTransformed = { 0 };
        triangle triViewed = { 0 };
        triangle triProjected = { 0 };

        MultiplyTriangleMatrix(triTransformed.p, meshToRender->tris[i].p, &matWorld);

        //Calculate the Normal for the Triangle
        vec3d normal = { 0 };
        vec3d line1 = { 0 };
        vec3d line2 = { 0 };
        vec3d cameraRay = { 0 };

        SubVector(&line1, &triTransformed.p[1], &triTransformed.p[0]);
        SubVector(&line2, &triTransformed.p[2], &triTransformed.p[0]);

        CrossProduct(&normal, &line1, &line2);
        NormalizeVector(&normal);

        SubVector(&cameraRay, &triTransformed.p[0], &camera);

        //Only work on the Triangle if it can be seen from the Camera
        if(DotProduct(&normal, &cameraRay) < 0.0f)
        {
            //Do lighting Stuff
            vec3d lightDir = {.x=0.0f, .y=0.0f , .z=-1.0f, .w=1.0f};
            NormalizeVector(&lightDir);

            int lightIntensity = (int)MapFrom0To1(fabsf(DotProduct(&lightDir, &normal)), 20.0f, 255.0f);
            SetTriColor(&triTransformed, lightIntensity, lightIntensity, lightIntensity, 255);

            //Bring Triangle to View space
            MultiplyTriangleMatrix(triViewed.p, triTransformed.p, &matView);
            SetTriColorFromTri(&triViewed, &triTransformed);

            triangle clipped[2] = { 0 };
            int clippedTris = TriangleClipWithPlane(&(vec3d){.x=0.0f, .y=0.0f, .z=NEAR_PLANE, .w=1.0f},&(vec3d){.x=0.0f, .y=0.0f, .z=1.0f, .w=1.0f}, &triViewed, &clipped[0], &clipped[1]);


            for (int triNum = 0; triNum < clippedTris; triNum++)
            {
                //Project Triangle
                MultiplyTriangleMatrix(triProjected.p, clipped[triNum].p, &matProj);
                SetTriColorFromTri(&triProjected, &clipped[triNum]);

                DivideVector(&triProjected.p[0], &triProjected.p[0], triProjected.p[0].w);
                DivideVector(&triProjected.p[1], &triProjected.p[1], triProjected.p[1].w);
                DivideVector(&triProjected.p[2], &triProjected.p[2], triProjected.p[2].w);

                //Move the Triangle to Screen space
                AddTriangleVector(triProjected.p, triProjected.p, &(vec3d){.x=1.0f, .y=1.0f, .z=0.0f, .w=1.0f});

                triProjected.p[0].x *= 0.5f * WINDOW_WIDTH;
                triProjected.p[0].y *= 0.5f * WINDOW_HEIGHT;
                triProjected.p[1].x *= 0.5f * WINDOW_WIDTH;
                triProjected.p[1].y *= 0.5f * WINDOW_HEIGHT;
                triProjected.p[2].x *= 0.5f * WINDOW_WIDTH;
                triProjected.p[2].y *= 0.5f * WINDOW_HEIGHT;

                //Save Triangle for sorting (will be replaced)
                CopyTriangel(&trisToRaster[trisToRasterCount],&triProjected);
                trisToRasterCount++;
            }
        }
    }

    //Sort the Triangles with the painters algorithm
    qsort(trisToRaster, trisToRasterCount, sizeof(triangle), TriCompareFunc);

    //Raster all the Triangles
    for (int i = 0; i < trisToRasterCount; i++)
    {
        triangle toRaster = trisToRaster[i];

        Vector2 d0 = {toRaster.p[0].x, toRaster.p[0].y};
        Vector2 d1 = {toRaster.p[1].x, toRaster.p[1].y};
        Vector2 d2 = {toRaster.p[2].x, toRaster.p[2].y};

        DrawTriangle(d0, d1, d2, toRaster.triColor);
        #ifdef RENDER_DEBUG
            DrawTriangleLines(d0, d1, d2, BLACK);
        #endif
    }

    //Free all Triangles after drawing
    //TODO Check for memory leaks :3
    free(trisToRaster);
}

//Called at the end of the project to Free stuff
void Exit(void)
{
    for (int i = 0; i < meshCount; ++i)
    {
        FreeMesh(meshList[i]);
    }
    free(meshList);
}