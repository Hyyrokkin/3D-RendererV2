#pragma once

#include "raylib.h"

typedef struct vec3d
{
    float x, y, z, w;
} vec3d;

typedef struct triangle
{
    vec3d p[3];
    Color triColor;
} triangle;

typedef struct mesh
{
    triangle* tris;
    vec3d worldPos;
} mesh;

typedef struct mat4x4
{
    float m[4][4];
} mat4x4;

void MakeMatIdentity(mat4x4 matrix[static 1]);

void MakeMatRotX(mat4x4 matrix[static 1], float angleRad);

void MakeMatRotY(mat4x4 matrix[static 1], float angleRad);

void MakeMatRotZ(mat4x4 matrix[static 1], float angleRad);

void MakeMatTranslate(mat4x4 matrix[static 1], const vec3d pos[static 1]);

void MakeMatProjection(mat4x4 matrix[static 1], float fov, float aspectRatio, float farPlane, float nearPlane);

void MakeMatPointAt(mat4x4 matrix[static 1], const vec3d pos[static 1], const vec3d target[static 1], const vec3d up[static 1]);

void MatrixQuickInverse(mat4x4 output[static 1], const mat4x4 toInvert[static 1]);


void MultiplyMatrixMatrix(mat4x4 output[static 1], const mat4x4 input1[static 1], const mat4x4 input2[static 1]);

void MultiplyMatrixVector(vec3d output[static 1], const vec3d input[static 1], const mat4x4 matrix[static 1]);

void CrossProduct(vec3d output[static 1], const vec3d i1[static 1], const vec3d i2[static 1]);

float DotProduct(const vec3d i1[static 1], const vec3d i2[static 1]);

float GetLengthVector(const vec3d vector[static 1]);

void NormalizeVector(vec3d toNormalize[static 1]);

void CopyVector(vec3d out[static 1], const vec3d in[static 1]);

void AddVector(vec3d output[static 1], const vec3d i1[static 1], const vec3d i2[static 1]);

void SubVector(vec3d output[static 1], const vec3d i1[static 1], const vec3d i2[static 1]);

void MultiplyVector(vec3d output[static 1], const vec3d input[static 1], float k);

void DivideVector(vec3d output[static 1], const vec3d input[static 1], float k);


void MultiplyTriangleMatrix(vec3d output[static 3], const vec3d input[static 3], const mat4x4 matrix[static 1]);

void AddTriangleVector(vec3d output[static 3], const vec3d i1[static 3], const vec3d i2[static 1]);


void LineIntersectPlane(vec3d out[static 1], const vec3d planePoint[static 1], const vec3d planeNormalIn[static 1], const vec3d lineStart[static 1], const vec3d lineEnd[static 1]);
size_t TriangleClipWithPlane(vec3d planePoint[static 1], vec3d planeNormalIn[static 1], triangle triToCheck[static 1], triangle triOut1[static 1], triangle triOut2[static 1]);


void SetTriColor(triangle tri[static 1], unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void SetTriColorFromTri(triangle triTo[static 1], const triangle triFrom[static 1]);

int TriCompareFunc(const void * a, const void * b);

float Map(float value, float fromStart, float fromFinish, float toStart, float toFinish);

float Map0To1(float value, float start, float finish);