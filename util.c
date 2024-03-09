#include <math.h>
#include <stdlib.h>

#include "util.h"

void InitMatToZero(mat4x4* toZero)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            toZero->m[i][j] = 0;
        }
    }
}

void MakeMatIdentity(mat4x4* matrix)
{
    InitMatToZero(matrix);

    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
}

void MakeMatRotX(mat4x4* matrix, float angleRad)
{
    InitMatToZero(matrix);

    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = cosf(angleRad);
    matrix->m[1][2] = sinf(angleRad);
    matrix->m[2][1] = -sinf(angleRad);
    matrix->m[2][2] = cosf(angleRad);
    matrix->m[3][3] = 1.0f;
}

void MakeMatRotY(mat4x4* matrix, float angleRad)
{
    InitMatToZero(matrix);

    matrix->m[0][0] = cosf(angleRad);
    matrix->m[0][2] = sinf(angleRad);
    matrix->m[2][0] = -sinf(angleRad);
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = cosf(angleRad);
    matrix->m[3][3] = 1.0f;
}

void MakeMatRotZ(mat4x4* matrix, float angleRad)
{
    InitMatToZero(matrix);

    matrix->m[0][0] = cosf(angleRad);
    matrix->m[0][1] = sinf(angleRad);
    matrix->m[1][0] = -sinf(angleRad);
    matrix->m[1][1] = cosf(angleRad);
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
}

void MakeMatTranslate(mat4x4* matrix, vec3d pos)
{
    InitMatToZero(matrix);

    matrix->m[0][0] = 1.0f;
    matrix->m[1][1] = 1.0f;
    matrix->m[2][2] = 1.0f;
    matrix->m[3][3] = 1.0f;
    matrix->m[3][0] = pos.x;
    matrix->m[3][1] = pos.y;
    matrix->m[3][2] = pos.z;
}

void MakeMatProjection(mat4x4* matrix, float fov, float aspectRatio,  float nearPlane, float farPlane)
{
    float fovRad = 1.0f / tanf(fov * 0.5f / 180.0f * PI);

    matrix->m[0][0] = aspectRatio * fovRad;
    matrix->m[1][1] = fovRad;
    matrix->m[2][2] = farPlane / (farPlane - nearPlane);
    matrix->m[3][2] = (-farPlane * nearPlane) / (farPlane - nearPlane);
    matrix->m[2][3] = 1.0f;
    matrix->m[3][3] = 0.0f;
}

void MakeMatPointAt(mat4x4* matrix, vec3d pos, vec3d target, vec3d up)
{
    InitMatToZero(matrix);

    vec3d newForward;
    SubVector(&newForward, target, pos);
    NormalizeVector(&newForward);

    vec3d upChange, newUp;
    MultiplyVector(&upChange, newForward, DotProduct(up, newForward));
    SubVector(&newUp, up, upChange);
    NormalizeVector(&newUp);

    vec3d newRight;
    CrossProduct(&newRight, newUp, newForward);

    matrix->m[0][0] = newRight.x;	matrix->m[0][1] = newRight.y;	matrix->m[0][2] = newRight.z;	matrix->m[0][3] = 0.0f;
    matrix->m[1][0] = newUp.x;		matrix->m[1][1] = newUp.y;		matrix->m[1][2] = newUp.z;		matrix->m[1][3] = 0.0f;
    matrix->m[2][0] = newForward.x;	matrix->m[2][1] = newForward.y;	matrix->m[2][2] = newForward.z;	matrix->m[2][3] = 0.0f;
    matrix->m[3][0] = pos.x;		matrix->m[3][1] = pos.y;		matrix->m[3][2] = pos.z;		matrix->m[3][3] = 1.0f;
}

void MatrixQuickInverse(mat4x4* output, mat4x4* toInvert) // Only for Rotation/Translation Matrices
{
    output->m[0][0] = toInvert->m[0][0]; output->m[0][1] = toInvert->m[1][0]; output->m[0][2] = toInvert->m[2][0]; output->m[0][3] = 0.0f;
    output->m[1][0] = toInvert->m[0][1]; output->m[1][1] = toInvert->m[1][1]; output->m[1][2] = toInvert->m[2][1]; output->m[1][3] = 0.0f;
    output->m[2][0] = toInvert->m[0][2]; output->m[2][1] = toInvert->m[1][2]; output->m[2][2] = toInvert->m[2][2]; output->m[2][3] = 0.0f;
    output->m[3][0] = -(toInvert->m[3][0] * output->m[0][0] + toInvert->m[3][1] * output->m[1][0] + toInvert->m[3][2] * output->m[2][0]);
    output->m[3][1] = -(toInvert->m[3][0] * output->m[0][1] + toInvert->m[3][1] * output->m[1][1] + toInvert->m[3][2] * output->m[2][1]);
    output->m[3][2] = -(toInvert->m[3][0] * output->m[0][2] + toInvert->m[3][1] * output->m[1][2] + toInvert->m[3][2] * output->m[2][2]);
    output->m[3][3] = 1.0f;
}


void MultiplyMatrixMatrix(mat4x4* output, const mat4x4 input1, const mat4x4 input2)
{
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            output->m[i][j] = input1.m[i][0] * input2.m[0][j] + input1.m[i][1] * input2.m[1][j] + input1.m[i][2] * input2.m[2][j] + input1.m[i][3] * input2.m[3][j];
        }
    }
}

void MultiplyMatrixVector(vec3d* output, const vec3d input, const mat4x4 matrix)
{
    output->x = input.x * matrix.m[0][0] + input.y * matrix.m[1][0] + input.z * matrix.m[2][0] + input.w * matrix.m[3][0];
    output->y = input.x * matrix.m[0][1] + input.y * matrix.m[1][1] + input.z * matrix.m[2][1] + input.w * matrix.m[3][1];
    output->z = input.x * matrix.m[0][2] + input.y * matrix.m[1][2] + input.z * matrix.m[2][2] + input.w * matrix.m[3][2];
    output->w = input.x * matrix.m[0][3] + input.y * matrix.m[1][3] + input.z * matrix.m[2][3] + input.w * matrix.m[3][3];
}

void CrossProduct(vec3d* output, const vec3d i1, const vec3d i2)
{
    output->x = i1.y * i2.z - i1.z * i2.y;
    output->y = i1.z * i2.x - i1.x * i2.z;
    output->z = i1.x * i2.y - i1.y * i2.x;
}

float DotProduct(const vec3d i1, const vec3d i2)
{
    return i1.x * i2.x + i1.y * i2.y + i1.z * i2.z;
}

float GetLengthVector(const vec3d v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

void NormalizeVector(vec3d* toNormalize)
{
    float l = GetLengthVector(*toNormalize);
    if(l != 0)
    {
        toNormalize->x /= l;
        toNormalize->y /= l;
        toNormalize->z /= l;
    }
}

void CopyVector(vec3d* out, const vec3d in)
{
    out->x = in.x;
    out->y = in.y;
    out->z = in.z;
    out->w = in.w;
}

void AddVector(vec3d* output, const vec3d i1, const vec3d i2)
{
    output->x = i1.x + i2.x;
    output->y = i1.y + i2.y;
    output->z = i1.z + i2.z;
}

void SubVector(vec3d* output, const vec3d i1, const vec3d i2)
{
    output->x = i1.x - i2.x;
    output->y = i1.y - i2.y;
    output->z = i1.z - i2.z;
}

void MultiplyVector(vec3d* output, const vec3d input, const float k)
{
    output->x = input.x * k;
    output->y = input.y * k;
    output->z = input.z * k;
}

void DivideVector(vec3d* output, const vec3d input, const float k)
{
    if(k != 0)
    {
        output->x = input.x / k;
        output->y = input.y / k;
        output->z = input.z / k;
    }
}

void MultiplyTriangleMatrix(vec3d output[3], const vec3d input[3], const mat4x4 matrix)
{
    MultiplyMatrixVector(&output[0], input[0], matrix);
    MultiplyMatrixVector(&output[1], input[1], matrix);
    MultiplyMatrixVector(&output[2], input[2], matrix);
}

void AddTriangleVector(vec3d output[3], const vec3d i1[3], const vec3d i2)
{
    AddVector(&output[0], i1[0], i2);
    AddVector(&output[1], i1[1], i2);
    AddVector(&output[2], i1[2], i2);
}


float Map(const float value, const float fromStart, const float fromFinish, const float toStart, const float toFinish)
{
    return (toStart + (value-fromStart)*(toFinish-toStart)/(fromFinish-fromStart));
}

float MapFrom0To1(const float value, const float start, const float finish)
{
    return Map(value, 0.0f, 1.0f, start, finish);
}

void SetTriColor(triangle* tri , const unsigned char r, const unsigned char g, const unsigned char b, const unsigned char a)
{
    tri->triColor.r = r;
    tri->triColor.g = g;
    tri->triColor.b = b;
    tri->triColor.a = a;
}

void SetTriColorFromTri(triangle* triTo , const triangle* triFrom)
{
    SetTriColor(triTo, triFrom->triColor.r, triFrom->triColor.g, triFrom->triColor.b, triFrom->triColor.a);
}

triangle* CopyTriangel(const triangle* toCopy)
{
    triangle* output = (triangle*) malloc(sizeof(triangle));
    output->triColor = toCopy->triColor;
    output->p[0] = toCopy->p[0];
    output->p[1] = toCopy->p[1];
    output->p[2] = toCopy->p[2];

    return output;
}

//for the painters sort algo
int triCompareFunc(const void * a, const void * b)
{
    triangle t1 = **((triangle**) a);
    triangle t2 = **((triangle**) b);
    float t1z = ((t1.p[0].z + t1.p[1].z + t1.p[2].z)/3);
    float t2z = ((t2.p[0].z + t2.p[1].z + t2.p[2].z)/3);

    if (t1z == t2z)
    {
        return 0;
    }
    else if (t1z > t2z)
    {
        return -1;
    }
    else
    {
        return 1;
    }
}