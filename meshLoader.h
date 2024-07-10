#pragma once

#include "util.h"

mesh* LoadMesh(const char meshName[static 1]);
void FreeMesh(mesh mesh[static 1]);
