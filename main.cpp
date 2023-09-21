#include <windows.h>

#include <iostream>

#include "simpleargs.h"
#include "raytracer.h"
#include "scene.h"

namespace arg
{
const char* HEIGHT = "height";
const char* INPUT = "in";
const char* OUTPUT = "out";
const char* CAMERA = "camera";
}

int tests(); //TODO ifndef DEBUG

int main(int argc, const char** argv)
{
    auto t_start = GetTickCount64();

    rSimpleArgs::instance()
        .addOption(arg::HEIGHT, 'h', "480")
        .addOption(arg::INPUT , 'i', "duck.glb")
        .addOption(arg::OUTPUT, 'o', "smallrender.png")
        .addOption(arg::CAMERA, 'c', "");
    rSimpleArgs::instance().parse(argc, argv);

    auto t = tests();
    if (t)
    {
        std::cout << "Fault test: " << t << std::endl;
        return t;
    }

    Render::Scene scene(std::cout);
    if (!scene.load(rSimpleArgs::instance().getOption(arg::INPUT)))
    {
        return 1;
    }

    std::cout << "Output file '" << rSimpleArgs::instance().getOption(arg::OUTPUT) << "'" << std::endl;

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

    rt.saveScene(rSimpleArgs::instance().getOption(arg::OUTPUT));

    auto t_end = GetTickCount64();

    std::cout << "Render time: " << t_end - t_start << " msec" << std::endl;

    return 0;
}
