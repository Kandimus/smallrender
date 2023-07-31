#include <windows.h>
#include <iostream>

#include "render.h"
#include "triangle.h"

int main()
{
    Render::Vector3 v1(1.0, 0.0, 1.0);
    Render::Vector3 v2(0.0, 0.0, 0.0);
    Render::Vector3 v3;

    v2 = Render::Vector3::cY();
    v3 = v1 + v2;

    // Камера смотрит строго по оси Z
    Render::camera().origin() = Render::Vector3(-1, 1.99, -100);
    Render::camera().direction() = Render::Vector3::cZ();

    Render::Triangle t(Render::Vector3(-1, -1, 100), Render::Vector3(-1, 2, 100), Render::Vector3(2, -1, 100));

    {
        auto t_start = GetTickCount64();

        bool x = true;
        for (int ii = 0; ii < 10000000; ++ii)
        {
            x &= t.intersect(Render::camera());
        }

        auto t_end = GetTickCount64();

        std::cout << "X = " << x << "  time: " << t_end - t_start << std::endl;
    }

    {
        auto t_start = GetTickCount64();

        bool x = true;
        Render::Vector3 p;
        for (int ii = 0; ii < 10000000; ++ii)
        {
            x &= t.intersect(Render::camera(), p);
        }

        auto t_end = GetTickCount64();

        std::cout << "X = " << x << "  time: " << t_end - t_start << std::endl;
    }

    return 0;
}
