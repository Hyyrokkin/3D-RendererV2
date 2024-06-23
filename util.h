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
    int triCount;
    vec3d worldPos;
} mesh;

typedef struct mat4x4
{
    float m[4][4];
} mat4x4;

void MakeMatIdentity(mat4x4* matrix);

void MakeMatRotX(mat4x4* matrix, float angleRad);

void MakeMatRotY(mat4x4* matrix, float angleRad);

void MakeMatRotZ(mat4x4* matrix, float angleRad);

void MakeMatTranslate(mat4x4* matrix, const vec3d* pos);

void MakeMatProjection(mat4x4* matrix, float fov, float aspectRatio, float farPlane, float nearPlane);

void MakeMatPointAt(mat4x4* matrix, const vec3d* pos, const vec3d* target, const vec3d* up);

void MatrixQuickInverse(mat4x4* output, mat4x4* toInvert);


void MultiplyMatrixMatrix(mat4x4* output, const mat4x4* input1, const mat4x4* input2);

void MultiplyMatrixVector(vec3d* output, const vec3d* input, const mat4x4* matrix);

void CrossProduct(vec3d* output, const vec3d* i1, const vec3d* i2);

float DotProduct(const vec3d* i1, const vec3d* i2);

float GetLengthVector(const vec3d* v);

void NormalizeVector(vec3d* toNormalize);

void CopyVector(vec3d* out, const vec3d* in);

void AddVector(vec3d* output, const vec3d* i1, const vec3d* i2);

void SubVector(vec3d* output, const vec3d* i1, const vec3d* i2);

void MultiplyVector(vec3d* output, const vec3d* input, float k);

void DivideVector(vec3d* output, const vec3d* input, float k);


void MultiplyTriangleMatrix(vec3d output[3], const vec3d input[3], const mat4x4* matrix);

void AddTriangleVector(vec3d output[3], const vec3d i1[3], const vec3d* i2);


void LineIntersectPlane(vec3d* out, const vec3d* planePoint, const vec3d* planeNormalIn, const vec3d* lineStart, const vec3d* lineEnd);
int TriangleClipWithPlane(vec3d planePoint, vec3d planeNormalIn, triangle* triToCheck, triangle* triOut1, triangle* triOut2);


void SetTriColor(triangle* tri ,unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void SetTriColorFromTri(triangle* triTo ,const triangle* triFrom);

triangle* CopyTriangel(const triangle* toCopy);

int triCompareFunc(const void * a, const void * b);

float Map(float value, float fromStart, float fromFinish, float toStart, float toFinish);

float MapFrom0To1(float value, float start, float finish);