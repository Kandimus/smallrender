#pragma once

#include "camera.h"

namespace Render
{

Camera& camera(void);

unsigned char* image();
int image_width();
int image_height();
void init(int w, int h);

}
