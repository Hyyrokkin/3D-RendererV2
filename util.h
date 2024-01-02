#include "raylib.h"

#ifndef INC_3D_RENDER_UTIL_H
#define INC_3D_RENDER_UTIL_H

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

void InitMatToZero(mat4x4* toZero);

void MakeMatIdentity(mat4x4* matrix);

void MakeMatRotX(mat4x4* matrix, float angleRad);

void MakeMatRotY(mat4x4* matrix, float angleRad);

void MakeMatRotZ(mat4x4* matrix, float angleRad);

void MakeMatTranslate(mat4x4* matrix, float x, float y, float z);

void MakeMatProjection(mat4x4* matrix, float fov, float aspectRatio, float farPlane, float nearPlane);

void MakeMatPointAt(mat4x4* matrix, vec3d pos, vec3d target, vec3d up);

void MatrixQuickInverse(mat4x4* output, mat4x4* toInvert);


void MultiplyMatrixMatrix(mat4x4* output, const mat4x4 input1, const mat4x4 input2);

void MultiplyMatrixVector(vec3d* output, const vec3d input, const mat4x4 matrix);

void CrossProduct(vec3d* output, const vec3d i1, const vec3d i2);

float DotProduct(const vec3d i1, const vec3d i2);

float GetLengthVector(const vec3d v);

void NormalizeVector(vec3d* toNormalize);

void AddVector(vec3d* output, const vec3d i1, const vec3d i2);

void SubVector(vec3d* output, const vec3d i1, const vec3d i2);

void MultiplyVector(vec3d* output, const vec3d input, const float k);

void DivideVector(vec3d* output, const vec3d input, const float k);


void MultiplyTriangleMatrix(vec3d output[3], const vec3d input[3], const mat4x4 matrix);

void AddTriangleVector(vec3d output[3], const vec3d i1[3], const vec3d i2);


void SetTriColor(triangle* tri ,const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a);

void SetTriColorFromTri(triangle* triTo ,const triangle* triFrom);

triangle* CopyTriangel(const triangle* toCopy);

int triCompareFunc(const void * a, const void * b);

float Map(const float value, const float fromStart, const float fromFinish, const float toStart, const float toFinish);

float MapFrom0To1(const float value, const float start, const float finish);

#endif //INC_3D_RENDER_UTIL_H
