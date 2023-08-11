#include <windows.h>
#include <iostream>

#include "simpleargs.h"
#include "tiny_gltf.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "render.h"
#include "triangle.h"
#include "color_rgb.h"
#include "light.h"
#include "static_mesh.h"

namespace arg
{
const char* WIDTH = "width";
const char* HEIGHT = "height";
const char* INPUT = "input";
const char* OUTPUT = "output";
}

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
                auto mesh = Render::addStaticMesh();

                if(!mesh->loadFromTinygltf(node, model))
                {
                    std::cout << "Fault load mesh '" << mesh->name() << "'" << std::endl;
                    return false;
                }

                std::cout << "Load mesh '" << mesh->name() << "'" << std::endl;
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
                auto light = Render::addLight();

                if(!light->loadFromTinygltf(node, model))
                {
                    std::cout << "Fault load light '" << light->name() << "'" << std::endl;
                    return false;
                }

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
        .addOption(arg::WIDTH, 'w', "320")
        .addOption(arg::HEIGHT, 'h', "240")
        .addOption(arg::INPUT, 'i', "triangle.gltf")
        .addOption(arg::OUTPUT, 'o', "smallrender.png");

    rSimpleArgs::instance().parse(argc, argv);
    int w = std::stoi(rSimpleArgs::instance().getOption(arg::WIDTH));
    int h = std::stoi(rSimpleArgs::instance().getOption(arg::HEIGHT));
    int w_3 = w * 3;

    if (rSimpleArgs::instance().isSet(arg::WIDTH) && rSimpleArgs::instance().isSet(arg::HEIGHT))
    {
        Render::camera().frustum().aspect() = REAL(w) / h;
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

    auto ambient = Render::addLight();
    ambient->createAmbient(Render::Vector3::c1, Render::Vector3::c1, Render::ColorRGB::Grey25); // Такой цвет по дефаулту в блендере
    std::cout << "Create global ambient light" << std::endl;

    //
    Render::init(w, h);

    auto listMesh = Render::staticMeshes();
    Render::ColorRGB color;

    for (int yy = 0; yy < Render::image_height(); ++yy)
    {
        for (int xx = 0; xx < Render::image_width(); ++xx)
        {
            auto ray = Render::camera().ray(xx, yy);

            for (auto sm : listMesh)
            {
                auto listTri = sm->triangle();
                for (auto& tri : listTri)
                {
                    color = calculatePoint(ray, tri);

                    Render::image()[w_3 * yy + 3 * xx + 0] = color.redHex();
                    Render::image()[w_3 * yy + 3 * xx + 1] = color.greenHex();
                    Render::image()[w_3 * yy + 3 * xx + 2] = color.blueHex();
                }
            }

        }
    }

    stbi_write_png(rSimpleArgs::instance().getOption(arg::OUTPUT).c_str(),
                   Render::image_width(), Render::image_height(), 3, Render::image(),
                   Render::image_width() * 3);

    auto t_end = GetTickCount64();

    std::cout << "Calculate time: " << t_end - t_start << " msec" << std::endl;

    color = Render::calculatePoint(Render::camera().centralRay(), Render::staticMeshes()[0]->triangle()[0]);

//

//        bool x = true;
//        Render::Vector3 p;
//        for (int ii = 0; ii < 10000000; ++ii)
//        {
//            x &= t.intersect(Render::camera(), p);
//        }

//


//    }

    Render::finalize();

    return 0;
}
