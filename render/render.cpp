#include "render.h"

namespace Render
{

static unsigned char* gBuffer = nullptr;
static int gWidth = 320;
static int gHeight = 240;

Camera& camera(void)
{
    return Camera::instance();
}

unsigned char* image()
{
    return gBuffer;
}

int image_width()
{
    return gWidth;
}

int image_height()
{
    return gHeight;
}

void init(int w, int h)
{
    if (gBuffer)
    {
        delete[] gBuffer;
    }

    gWidth = w;
    gHeight = h;
    gBuffer = new unsigned char[gWidth * gHeight * 3];

    int max_ii = gWidth * gHeight * 3 / 4;
    int max_ii_2 = gWidth * gHeight * 3 % 4;
    for (int ii = 0; ii < max_ii; ++ii)
    {
        *((int*)gBuffer + ii) = 0;
    }
    for (int ii = 0; ii < max_ii_2; ++ii)
    {
        gBuffer[max_ii + ii] = 0;
    }
}

}

