#ifndef INC_3D_RENDER_MESHLOADER_H
#define INC_3D_RENDER_MESHLOADER_H

#include "util.h"

mesh* LoadMesh(const char* meshName);
void FreeMesh(mesh* mesh);

#endif //INC_3D_RENDER_MESHLOADER_H
