#pragma once

#include "camera.h"
#include "static_mesh.h"

namespace Render
{
void init(int w, int h);
void finalize();

Camera& camera(void);

unsigned char* image();
int image_width();
int image_height();


StaticMesh* addStaticMesh();

}
