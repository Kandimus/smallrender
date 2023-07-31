#include "render.h"

namespace Render
{

Ray& camera(void)
{
    static Ray camera;

    return camera;
}

}

