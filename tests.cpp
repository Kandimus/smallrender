#include <windows.h>

#include <vector>

#include <iostream>
#include <limits>

#include "vector3.h"
#include "ob_sphere.h"
#include "ob_box.h"

int test_obSphere()
{
    Render::ObSphere obs(Render::Vector3(1, 1, 1), 3);
    Render::Ray ray(Render::Vector3(0, 1, -1), Render::Vector3::c0);

    // Луч внутри сферы
    ray.direction() = Render::Vector3(1, 1, 1);
    ray.direction().normalize();
    if (true != obs.intersect(ray))
    {
        return 10;
    }

    // Сфера позади луча
    ray.origin() = Render::Vector3(0, 0, -10);
    ray.direction() = Render::Vector3(0, 0, -1);
    ray.direction().normalize();
    if (false != obs.intersect(ray))
    {
        return 11;
    }

    // Сфера перед лучом
    ray.origin() = Render::Vector3(0, 0, -10);
    ray.direction() = Render::Vector3(0, 0, 1);
    ray.direction().normalize();
    if (true != obs.intersect(ray))
    {
        return 12;
    }

    return 0;
}


int test_obBox()
{
    Render::ObBox obb;
    std::vector<Render::Vector3> v = {{0, 3, 0}, {-3, -3, -3}, {3, -3, -3}, {0, -3, 3}};

    obb.create(v);
    return 0;
}

int tests()
{
    int result = 0;

    if ((result = test_obSphere()) > 0) return result;
    if ((result = test_obBox())    > 0) return result;

    return 0;
}

