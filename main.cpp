#include <windows.h>

#include <iostream>
#include <limits>

#include "simpleargs.h"
#include "tiny_gltf.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "render.h"
#include "render_defines.h"
#include "triangle.h"
#include "color_rgb.h"
#include "light_ambient.h"
#include "static_mesh.h"
#include "material.h"

namespace arg
{
const char* WIDTH = "width";
const char* HEIGHT = "height";
const char* INPUT = "in";
const char* OUTPUT = "out";
}

int tests(); //TODO ifndef DEBUG
int stbi_write_png(char const *filename, int w, int h, int comp, const void  *data, int stride_in_bytes);

int main(int argc, const char** argv)
{
    auto t_start = GetTickCount64();

    rSimpleArgs::instance()
        .addOption(arg::WIDTH, 'w', "0")
        .addOption(arg::HEIGHT, 'h', "480")
        .addOption(arg::INPUT, 'i', "duck.glb")
        .addOption(arg::OUTPUT, 'o', "smallrender.png");

    rSimpleArgs::instance().parse(argc, argv);

    auto t = tests();
    if (t)
    {
        std::cout << "Fault test: " << t << std::endl;
        return t;
    }

    std::string out = "";
    auto result = Render::loadScene(rSimpleArgs::instance().getOption(arg::INPUT), out);
    std::cout << out;

    if (!result)
    {
        return 1;
    }

    Render::lightAmbient().ambient().x() = Render::ColorRGB::Grey25.red();
    Render::lightAmbient().ambient().y() = Render::ColorRGB::Grey25.green();
    Render::lightAmbient().ambient().z() = Render::ColorRGB::Grey25.blue();
    std::cout << "Create global ambient light" << std::endl;

    // Render init
    int w = std::stoi(rSimpleArgs::instance().getOption(arg::WIDTH));
    int h = std::stoi(rSimpleArgs::instance().getOption(arg::HEIGHT));
    Render::init(w, h);
    int w_3 = Render::image_width() * 3;

    // Render
    Render::Ray ray;
    Render::ColorRGB color;
    Render::ColorRGB c;
    Render::ColorRGB bg = Render::MultiplierBackgroundAmbient * Render::lightAmbient().ambient(); //NOTE trick so that the background does not merge with the shadows
    REAL deep = std::numeric_limits<REAL>::infinity();

    volatile Render::Ray rr = Render::camera().centralRay();

    for (int yy = 0; yy < Render::image_height(); ++yy)
    {
        for (int xx = 0; xx < Render::image_width(); ++xx)
        {
            Render::gDebugIntX = xx;
            Render::gDebugIntY = yy;

            ray = Render::camera().ray(xx, yy);
            color = bg;
            deep = REAL_MAXIMUM;

            FOR_EACH_TRIANGLE
            {
                const Render::Triangle* t = POINTER_TRIANGLE;
                REAL d = calculatePoint(ray, *t, c);

                if (d < deep)
                {
                    deep = d;
                    color = c;
                }
            }
            END_FOR_EACH_TRIANGLE

//            auto& list = Render::staticMeshes(); for (auto mesh : list) { if (mesh->intersect(ray)) { color = Render::ColorRGB::White; break; } }

            Render::image()[w_3 * yy + 3 * xx + 0] = color.redHex();
            Render::image()[w_3 * yy + 3 * xx + 1] = color.greenHex();
            Render::image()[w_3 * yy + 3 * xx + 2] = color.blueHex();
        }
    }

    stbi_write_png(rSimpleArgs::instance().getOption(arg::OUTPUT).c_str(),
                   Render::image_width(), Render::image_height(), 3, Render::image(),
                   Render::image_width() * 3);

    auto t_end = GetTickCount64();

    std::cout << "Calculate time: " << t_end - t_start << " msec" << std::endl;

    Render::finalize();

    return 0;
}
