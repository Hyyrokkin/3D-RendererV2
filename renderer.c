#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#include "renderer.h"
#include "util.h"
#include "meshLoader.h"
#include "settings.h"

mesh** meshList;
int meshCount;
mat4x4 matProj;

vec3d camera = {0.0f, 0.0f, 0.0f, 1.0f};
vec3d lookDir;

float theta = 0.0f;
float  yaw = 0.0f;

void RenderMesh(mesh* meshToRender, int meshId);

void Setup()
{
    printf("[3D Render] - Started Setup\n");

    meshCount = 1;
    meshList = malloc(sizeof(mesh*)*meshCount);

    meshList[0] = LoadMesh("C:\\Users\\Morit\\Downloads\\ProgrammShit\\C Shit\\3D_Render\\meshes\\axis.obj");
    meshList[0]->worldPos.z = 3.0f;

    float aspectRatio = WINDOW_HEIGHT / WINDOW_WIDTH;
    MakeMatProjection(&matProj, FOV, aspectRatio, NEAR_PLANE, FAR_PLANE);

    printf("[3D Render] - Finished Setup\n");
}

void Update()
{
    ClearBackground(DARKGRAY);
    theta += GetFrameTime();

    if(IsKeyDown(KEY_SPACE))
        camera.y += VERT_MOVE_SPEED * GetFrameTime();

    if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
        camera.y -= VERT_MOVE_SPEED * GetFrameTime();

    vec3d forward;
    MultiplyVector(&forward, lookDir, MOVE_SPEED * GetFrameTime());

    vec3d right;
    CrossProduct(&right, lookDir, (vec3d) {0.0f, 1.0f, 0.0f, 1.0f});
    MultiplyVector(&right, right, MOVE_SPEED * GetFrameTime());

    if(IsKeyDown(KEY_W))
        AddVector(&camera, camera, forward);

    if(IsKeyDown(KEY_S))
        SubVector(&camera, camera, forward);

    if(IsKeyDown(KEY_A))
        SubVector(&camera, camera, right);

    if(IsKeyDown(KEY_D))
        AddVector(&camera, camera, right);

    if(IsKeyDown(KEY_Q))
        yaw -= TURN_SPEED * GetFrameTime();

    if(IsKeyDown(KEY_E))
        yaw += TURN_SPEED * GetFrameTime();

    for (int i = 0; i < meshCount; ++i) {
        RenderMesh(meshList[i], i);
    }
}

void RenderMesh(mesh* meshToRender, int meshId)
{
    triangle** trisToRaster = malloc(sizeof(triangle*) * meshToRender->triCount);
    int trisToRasterCount = 0;

    mat4x4 matRotZ, matRotY, matRotX, matTrans;

    MakeMatRotZ(&matRotZ, 0.0f);
    MakeMatRotY(&matRotY, 0.0f);
    MakeMatRotX(&matRotX, 0.0f);

    MakeMatTranslate(&matTrans, meshToRender->worldPos.x, meshToRender->worldPos.y, meshToRender->worldPos.z);

    mat4x4 matWorld;
    MakeMatIdentity(&matWorld);
    MultiplyMatrixMatrix(&matWorld, matWorld, matRotZ);
    MultiplyMatrixMatrix(&matWorld, matWorld, matRotY);
    MultiplyMatrixMatrix(&matWorld, matWorld, matRotX);
    MultiplyMatrixMatrix(&matWorld, matWorld, matTrans);

    vec3d up = {0.0f, -1.0f, 0.0f, 1.0f};
    NormalizeVector(&up);

    vec3d target = {0.0f, 0.0f, 1.0f, 1.0f};
    mat4x4 matCameraRot;
    MakeMatRotY(&matCameraRot, yaw);
    MultiplyMatrixVector(&lookDir, target, matCameraRot);
    NormalizeVector(&lookDir);
    AddVector(&target, camera, lookDir);

    mat4x4 matCamera, matView;
    MakeMatPointAt(&matCamera, camera, target, up);
    MatrixQuickInverse(&matView, &matCamera);

    for (int i = 0; i < meshToRender->triCount; ++i)
    {
        triangle triTransformed, triViewed, triProjected;

        MultiplyTriangleMatrix(triTransformed.p, meshToRender->tris[i].p, matWorld);

        vec3d normal, line1, line2, cameraRay;

        SubVector(&line1, triTransformed.p[1], triTransformed.p[0]);
        SubVector(&line2, triTransformed.p[2], triTransformed.p[0]);

        CrossProduct(&normal, line1, line2);
        NormalizeVector(&normal);

        SubVector(&cameraRay, triTransformed.p[0], camera);

        if(DotProduct(normal, cameraRay) < 0.0f)
        {
            vec3d lightDir = {0.0f, 0.0f , -1.0f, 1.0f};
            NormalizeVector(&lightDir);

            int lightIntensity = (int)MapFrom0To1(fabsf(DotProduct(lightDir, normal)), 20.0f, 255.0f);
            SetTriColor(&triTransformed, lightIntensity, lightIntensity, lightIntensity, 255);

            MultiplyTriangleMatrix(triViewed.p, triTransformed.p, matView);
            SetTriColorFromTri(&triViewed, &triTransformed);

            MultiplyTriangleMatrix(triProjected.p, triViewed.p, matProj);
            SetTriColorFromTri(&triProjected, &triViewed);

            DivideVector(&triProjected.p[0], triProjected.p[0], triProjected.p[0].w);
            DivideVector(&triProjected.p[1], triProjected.p[1], triProjected.p[1].w);
            DivideVector(&triProjected.p[2], triProjected.p[2], triProjected.p[2].w);

            vec3d offset = {1.0f, 1.0f, 0.0f, 1.0f};
            AddTriangleVector(triProjected.p, triProjected.p, offset);

            triProjected.p[0].x *= 0.5f * (float) WINDOW_WIDTH;
            triProjected.p[0].y *= 0.5f * (float) WINDOW_HEIGHT;
            triProjected.p[1].x *= 0.5f * (float) WINDOW_WIDTH;
            triProjected.p[1].y *= 0.5f * (float) WINDOW_HEIGHT;
            triProjected.p[2].x *= 0.5f * (float) WINDOW_WIDTH;
            triProjected.p[2].y *= 0.5f * (float) WINDOW_HEIGHT;

            trisToRaster[trisToRasterCount] = CopyTriangel(&triProjected);
            trisToRasterCount++;
        }
    }

    qsort(trisToRaster, trisToRasterCount, sizeof(triangle*), triCompareFunc);

    for (int i = 0; i < trisToRasterCount; ++i) {
        triangle toRaster = *trisToRaster[i];

        Vector2 d0 = {toRaster.p[0].x, toRaster.p[0].y};
        Vector2 d1 = {toRaster.p[1].x, toRaster.p[1].y};
        Vector2 d2 = {toRaster.p[2].x, toRaster.p[2].y};

        DrawTriangle(d0, d1, d2, toRaster.triColor);
        //DrawTriangleLines(d0, d1, d2, BLACK);
    }

    for (int i = 0; i < trisToRasterCount; ++i) {
        free(trisToRaster[i]);
    }
    free(trisToRaster);
}

void Exit()
{
    for (int i = 0; i < meshCount; ++i) {
        FreeMesh(meshList[i]);
    }
    free(meshList);
}