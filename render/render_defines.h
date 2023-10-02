#pragma once
#include "defines.h"

namespace Render
{

const REAL MultiplierBackgroundAmbient = 1.1; // trick so that the background does not merge with the shadows
const REAL LightWattToIntensity = 10.0;       // The MAGIC number for covertion light power from blender W to i
const REAL LumensPerWatt = 683.0 ;            // Kv (683) - maximum spectral luminous efficacy of radiation for photoscopic vision

}

#ifdef RENDER_CHECK_INTERSECT_MESH
    #define FOR_EACH_TRIANGLE       { auto& list = m_scene->staticMeshes(); for (auto mesh : list) { if (mesh->intersect(ray)) { for (auto& tmp_triangle_item : mesh->triangle())
    #define END_FOR_EACH_TRIANGLE   }}}
    #define POINTER_TRIANGLE        (&tmp_triangle_item)
#else
    #define FOR_EACH_TRIANGLE       { for (auto tmp_triangle_item : m_scene->triangles())
    #define END_FOR_EACH_TRIANGLE   }
    #define POINTER_TRIANGLE        (tmp_triangle_item)
#endif
