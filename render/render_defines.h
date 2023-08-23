#pragma once

#ifdef RENDER_USING_CHECK_INTERSECT_MESH
    #define FOR_EACH_TRIANGLE       { auto& list = Render::staticMeshes(); for (auto mesh : list) { if (mesh->intersect(ray)) { for (auto& tmp_triangle_item : mesh->triangle())
    #define END_FOR_EACH_TRIANGLE   }}}
    #define POINTER_TRIANGLE        (&tmp_triangle_item)
#else
    #define FOR_EACH_TRIANGLE       { for (auto tmp_triangle_item : Render::triangles())
    #define END_FOR_EACH_TRIANGLE   }
    #define POINTER_TRIANGLE        (tmp_triangle_item)
#endif
