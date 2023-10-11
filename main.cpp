#include <windows.h>

#include <iostream>

#include "simpleargs.h"
#include "raytracer.h"
#include "scene.h"
#include "color_rgb.h"

namespace arg
{
const char* HEIGHT = "height";
const char* INPUT = "in";
const char* OUTPUT = "out";
const char* CAMERA = "camera";
const char* AMBIENT = "ambient";
}

int tests(); //TODO ifndef DEBUG

int main(int argc, const char** argv)
{
    auto t_start = GetTickCount64();

    rSimpleArgs::instance()
        .addOption(arg::HEIGHT, 'h', "480")
        .addOption(arg::INPUT , 'i', "duck.glb")
        .addOption(arg::OUTPUT, 'o', "smallrender.png")
        .addOption(arg::CAMERA, 'c', "")
        .addOption(arg::AMBIENT, 'a', "");
    rSimpleArgs::instance().parse(argc, argv);

    auto t = tests();
    if (t)
    {
        std::cout << "Fault test: " << t << std::endl;
        return t;
    }

    Render::Vector3 ambient = Render::Vector3(Render::ColorRGB::Grey25.toVector3());
    if (rSimpleArgs::instance().isSet(arg::AMBIENT))
    {
        volatile std::string str = rSimpleArgs::instance().getOption(arg::AMBIENT);
        size_t err = 0;
        unsigned int ambient_hex = std::stoul(rSimpleArgs::instance().getOption(arg::AMBIENT), &err, 16);
        ambient = Render::Vector3(Render::ColorRGB(ambient_hex).toVector3());
    }

    Render::Scene scene(ambient, std::cout);
    if (!scene.load(rSimpleArgs::instance().getOption(arg::INPUT)))
    {
        return 1;
    }

    auto outputFile = rSimpleArgs::instance().getOption(arg::OUTPUT);
    std::cout << "Output file '" << outputFile << "'" << std::endl;

    Render::Raytracer rt;

    int h = atoi(rSimpleArgs::instance().getOption(arg::HEIGHT).c_str());
    if (!h)
    {
        std::cout << "Error: height should not be zero" << std::endl;
        return 1;
    }

    if (!rt.renderScene(h, scene, rSimpleArgs::instance().getOption(arg::CAMERA)))
    {
        return 1;
    }

    rt.saveScene(outputFile);

    auto t_end = GetTickCount64();
    auto renderTime = t_end - t_start;

    std::cout << "Render time: " << renderTime << " msec" << std::endl;

    return 0;
}
