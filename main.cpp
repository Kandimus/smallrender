#include <windows.h>
#include <iostream>

#include "simpleargs.h"
#include "tiny_gltf.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "render.h"
#include "triangle.h"

namespace arg
{
const char* WIDTH = "width";
const char* HEIGHT = "height";
const char* INPUT = "input";
const char* OUTPUT = "output";
}

int stbi_write_png(char const *filename, int w, int h, int comp, const void  *data, int stride_in_bytes);


bool loadCamera(const tinygltf::Model& model)
{
    for (auto& node : model.nodes)
    {
        if (node.camera >= 0 && model.cameras[node.camera].type == "perspective")
        {
            auto& cam = model.cameras[node.camera];

            std::cout << "Found camera '" << cam.name << "'" << std::endl;

            if (node.translation.size())
            {
                Render::camera().position() = Render::Vector3(node.translation[0], node.translation[1], -node.translation[2]);
            }

            if (node.rotation.size())
            {
                auto q = Render::Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
                auto a = q.toYPR();

                Render::camera().angleX() = a.x();
                Render::camera().angleY() = a.y();
            }

            Render::camera().frustum().aspect() = 4.0/3.0;//cam.perspective.aspectRatio;
            Render::camera().frustum().fov() = cam.perspective.yfov;
            Render::camera().frustum().farClip() = cam.perspective.zfar;
            Render::camera().frustum().nearClip() = cam.perspective.znear;
            Render::camera().changed();
            Render::camera().update();

            return true;
        }
    }

    return false;
}

bool loadMeshes(const tinygltf::Model& model)
{
    for (auto& node : model.nodes)
    {
        if (node.mesh >= 0)
        {
            auto sm = Render::addStaticMesh();

            if(!sm->loadFromTinygltf(model.meshes[node.mesh], model))
            {
                return false;
            }

            std::cout << "Load mesh '" << sm->name() << "'" << std::endl;
        }
    }

    return true;
}


int main(int argc, const char** argv)
{
    rSimpleArgs::instance()
        .addOption(arg::WIDTH, 'w', "320")
        .addOption(arg::HEIGHT, 'h', "240")
        .addOption(arg::INPUT, 'i', "triangle.gltf")
        .addOption(arg::OUTPUT, 'o', "smallrender.png");

    rSimpleArgs::instance().parse(argc, argv);
    int w = std::stoi(rSimpleArgs::instance().getOption(arg::WIDTH));
    int h = std::stoi(rSimpleArgs::instance().getOption(arg::HEIGHT));

    if (rSimpleArgs::instance().isSet(arg::WIDTH) && rSimpleArgs::instance().isSet(arg::HEIGHT))
    {
        Render::camera().frustum().aspect() = REAL(w) / h;
    }

    Render::init(w, h);

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


    if (!loadCamera(model))
    {
        std::cout << "Error: Load the camera fault" << std::endl;
        return 1;
    }

    if (!loadMeshes(model))
    {
        std::cout << "Error: Load meshes fault" << std::endl;
        return 1;
    }

    auto listMesh = Render::staticMesh();
    auto v = Render::camera().direction();

    for (int yy = 0; yy < Render::image_height(); ++yy)
    {
        for (int xx = 0; xx < Render::image_width(); ++xx)
        {
            auto dir = Render::camera().ray(xx, yy);

            for (auto sm : listMesh)
            {
                auto listTri = sm->triangle();
                for (auto& tri : listTri)
                {
                    if (xx == 0 && yy == 80)
                    {
                        volatile int a = 1;
                        a = 2;
                    }
                    Render::Vector3 p;
                    Render::Vector2 uv;
                    //if (tri.intersect(dir, p, uv))
                    if (tri.intersect(dir))
                    {
                        Render::image()[Render::image_width() * 3 * yy + 3 * xx + 0] = 0xFF;
                        Render::image()[Render::image_width() * 3 * yy + 3 * xx + 1] = 0x40;
                        Render::image()[Render::image_width() * 3 * yy + 3 * xx + 2] = 0xFF;
                    }
                }
            }

        }
    }

    auto dir = Render::camera().centralRay();//180, 120);
    std::cout << "Camera aX " << Render::camera().angleX() << ", aY " << Render::camera().angleY() << ", dir (" << dir.direction().x() << ", " << dir.direction().y() << ", " << dir.direction().z() << ")" << std::endl;
    for (auto sm : listMesh)
    {
        auto listTri = sm->triangle();
        for (auto& tri : listTri)
        {
            Render::Vector3 p;
            Render::Vector2 uv;
            //if (tri.intersect(dir, p, uv))
            if (tri.intersect(dir))
            {
                Render::image()[Render::image_width() * 3 * 120 + 3 * 160 + 0] = 0xFF;
                Render::image()[Render::image_width() * 3 * 120 + 3 * 160 + 1] = 0xFF;
                Render::image()[Render::image_width() * 3 * 120 + 3 * 160 + 2] = 0xFF;
            }
        }
    }


    Render::image()[0] = 0xFF;
    Render::image()[1] = 0xFF;
    Render::image()[2] = 0xFF;
    Render::image()[Render::image_width() * 3 + 3] = 0xFF;
    Render::image()[Render::image_width() * 3 + 4] = 0xFF;
    Render::image()[Render::image_width() * 3 + 5] = 0xFF;
    stbi_write_png(rSimpleArgs::instance().getOption(arg::OUTPUT).c_str(),
                   Render::image_width(), Render::image_height(), 3, Render::image(),
                   Render::image_width() * 3);

    // Камера смотрит строго по оси Z
//    Render::camera().position() = Render::Vector3(-1, 1.99, -100);
//    Render::camera().ray().origin() = Render::Vector3::cY;
//    Render::camera().ray().direction() = Render::Vector3::cZ;

//    Render::Triangle t(Render::Vector3(-1, -1, 100), Render::Vector3(-1, 2, 100), Render::Vector3(2, -1, 100));

//    {
//        auto t_start = GetTickCount64();

//        bool x = true;
//        for (int ii = 0; ii < 10000000; ++ii)
//        {
//            x &= t.intersect(Render::camera());
//        }

//        auto t_end = GetTickCount64();

//        std::cout << "X = " << x << "  time: " << t_end - t_start << std::endl;
//    }

//    {
//        auto t_start = GetTickCount64();

//        bool x = true;
//        Render::Vector3 p;
//        for (int ii = 0; ii < 10000000; ++ii)
//        {
//            x &= t.intersect(Render::camera(), p);
//        }

//        auto t_end = GetTickCount64();

//        std::cout << "X = " << x << "  time: " << t_end - t_start << std::endl;
//    }

    Render::finalize();

    return 0;
}
