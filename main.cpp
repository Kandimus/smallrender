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
#include "light_factory.h"
#include "light_ambient.h"
#include "static_mesh.h"

namespace arg
{
const char* WIDTH = "width";
const char* HEIGHT = "height";
const char* INPUT = "input";
const char* OUTPUT = "output";
}

int tests(); //TODO ifndef DEBUG
int stbi_write_png(char const *filename, int w, int h, int comp, const void  *data, int stride_in_bytes);

bool loadObjectsFromGLTF(const tinygltf::Model& model)
{
    for (auto& scene : model.scenes)
    {
        for (auto& nodeId : scene.nodes)
        {
            if (nodeId < 0)
            {
                continue;
            }

            auto& node = model.nodes[nodeId];

            if (node.mesh >= 0)
            {
                auto mesh = Render::createStaticMesh();

                if(!mesh->loadFromTinygltf(node, model))
                {
                    std::cout << "Fault load mesh '" << mesh->name() << "'" << std::endl;
                    return false;
                }

                std::cout << "Load mesh '" << mesh->name() << "', vertices: " << mesh->vertex().size()
                          << ", indices: " << mesh->index().size() << ", tirangles: " << mesh->triangle().size()
                          << ",  obv: " << mesh->obVolume().type() << std::endl;
            }
            else if (node.camera >= 0)
            {
                if (!Render::camera().instance().loadFromTinygltf(node, model))
                {
                    std::cout << "Fault load camera '" << Render::camera().instance().name() << "'" << std::endl;
                    return false;
                }

                std::cout << "Load camera '" << Render::camera().instance().name() << "'" << std::endl;
            }
            if (node.light >= 0)
            {
                auto light = Render::LightFactory::loadFromTinygltf(node, model);

                if (!light)
                {
                    std::cout << "Fault load light '" << node.name << "'" << std::endl;
                    return false;
                }

                Render::addLight(light);
                std::cout << "Load light '" << light->name() << "'" << std::endl;
            }
        }
    }

    return true;
}

int main(int argc, const char** argv)
{
    auto t_start = GetTickCount64();

    rSimpleArgs::instance()
        .addOption(arg::WIDTH, 'w', "640")
        .addOption(arg::HEIGHT, 'h', "480")
        .addOption(arg::INPUT, 'i',
                   "scene6.gltf"
                   //"test_obvolume.gltf"
                   )
        .addOption(arg::OUTPUT, 'o', "smallrender.png");

    rSimpleArgs::instance().parse(argc, argv);
    int w = std::stoi(rSimpleArgs::instance().getOption(arg::WIDTH));
    int h = std::stoi(rSimpleArgs::instance().getOption(arg::HEIGHT));
    int w_3 = w * 3;

    if (rSimpleArgs::instance().isSet(arg::WIDTH) && rSimpleArgs::instance().isSet(arg::HEIGHT))
    {
        Render::camera().frustum().aspect() = REAL(w) / h;
    }

    auto t = tests();
    if (t)
    {
        std::cout << "Fault test: " << t << std::endl;
        return t;
    }


    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;
    tinygltf::Model model;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, rSimpleArgs::instance().getOption(arg::INPUT));

    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
    {
        std::cout << "Failed to load glTF: " << rSimpleArgs::instance().getOption(arg::INPUT) << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Loaded glTF: " << rSimpleArgs::instance().getOption(arg::INPUT) << std::endl;
    }

    Render::camera().reset();

    if (!loadObjectsFromGLTF(model))
    {
        return 1;
    }

    Render::lightAmbient().ambient().x() = Render::ColorRGB::Grey25.red();
    Render::lightAmbient().ambient().y() = Render::ColorRGB::Grey25.green();
    Render::lightAmbient().ambient().z() = Render::ColorRGB::Grey25.blue();
    std::cout << "Create global ambient light" << std::endl;

    //
    Render::init(w, h);

    auto listMesh = Render::staticMeshes();
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
            if (xx == 419 && yy == 272)
            {
                volatile int a = 1;
            }

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
