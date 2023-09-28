
#include <filesystem>
#include <limits>
#include <iostream>

#include "render_defines.h"
#include "raytracer.h"

#include "camera.h"
#include "color_argb.h"
#include "i_light.h"
#include "material.h"
#include "scene.h"
#include "static_mesh.h"
#include "triangle.h"
#include "util.h"

#include "tiny_gltf.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace Render
{

Raytracer::~Raytracer()
{
    if (m_image)
    {
        delete[] m_image;
        m_image = nullptr;
    }
}


REAL Raytracer::calculatePoint(const Ray& ray, const Triangle& triangle, ColorRGB& c)
{
    Intersection ti;
    Vector3 t_normal = triangle.normal();

    c = ColorRGB::Black;

    REAL len = triangle.intersect(ray, ti);
    if (len < 0)
    {
        return REAL_MAXIMUM;
    }

    if (m_debugIntX == 317 && m_debugIntY == (480 - 337))
    {
        volatile int a = 1;
    }

#ifdef RENDER_CALCULATED_NORMALS
    t_normal = triangle.origin() * ti.w + triangle.point1() * ti.u + triangle.point2() * ti.v;
    t_normal.normalize();
#endif

    //return 1;

    // Проверка на попадание с обратной стороны треугольника
    //TODO тут нужно будет сделать зависимость от материала, или рисуем вторую сторону
    //     или делаем вид, что мы не пересекали этот треугольник. Если рисуем, то
    //     нужно обратить нормаль
    auto rn = ray.direction() & t_normal;
    if (rn > MATH_EPS)
    {
        if (triangle.material().isDoubleSided())
        {
            t_normal = -t_normal;
        }
        else
        {
            return REAL_MAXIMUM;
        }
    }

    // ILxxx - начальная интенсивность источника
    // CLxxx - цвет источника
    // IMxxx - интенсивность материала
    // CMxxx - цвет материала
    //
    // C = RSambient * ILambient + RSemmision + RSdiffuse * SUM(IL(i)diffuse * (ni & li)) + RSspecular * SUM(IL(i)specular * max(ri & vi, 0) * ((ni & li) > 0 ? 1 : 0))

    Vector3 diffuse = Vector3::c0;

    for (auto light : m_scene->lights())
    {
        if (!light->enable())
        {
            continue;
        }

        // проверка наличие препятствия между точкой и истоником света
        bool intersected = false;
        Ray ray_to_light = light->ray(ti.point);
        Intersection li;

        FOR_EACH_TRIANGLE
        {
            const Triangle* t = POINTER_TRIANGLE;
            if (t == &triangle) continue;

            if (t->intersect(ray_to_light, li) >= MATH_EPS)
            {
                intersected = true;
                break;
            }
        }
        END_FOR_EACH_TRIANGLE

        if (!intersected)
        {
            diffuse += light->intensity(ray, ti.point, t_normal);
        }
    }

    Vector2 textCoord = triangle.vertex0().texCoord(0) * ti.w + triangle.vertex1().texCoord(0) * ti.u + triangle.vertex2().texCoord(0) * ti.v;
    //textCoord.normalize();

    ColorARGB mat_color = triangle.material().diffuse(textCoord);
    c = m_scene->ambient() * mat_color + diffuse * mat_color;
    c.scaleByMax();

    return len;
}

int Raytracer::renderScene(int h, Scene& scene, const std::string& cameraName)
{
    m_scene = &scene;

    m_scene->setHeight(h);

    m_camera = m_scene->findCamera(cameraName);

    m_width = h * m_camera->aspect();
    m_height = h;
    m_size = m_width * m_height * 3;
    m_image = new unsigned char[m_size];

    Util::fastMemClear(m_image, m_size);

    //--- RENDER ------------------------------------
    int w_3 = m_width * 3;
    Render::Ray ray;
    Render::ColorRGB color;
    Render::ColorRGB c;
    Render::ColorRGB bg = MultiplierBackgroundAmbient * m_scene->colorAmbient();
    REAL deep = std::numeric_limits<REAL>::infinity();

    volatile Render::Ray rr = m_camera->centralRay(); // TODO for tests

    for (int yy = 0; yy < m_height; ++yy)
    {
        for (int xx = 0; xx < m_width; ++xx)
        {
            m_debugIntX = xx;
            m_debugIntY = yy;

            ray = m_camera->ray(xx, yy);
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

            // auto& list = Render::staticMeshes(); for (auto mesh : list) { if (mesh->intersect(ray)) { color = Render::ColorRGB::White; break; } }
            m_image[w_3 * yy + 3 * xx + 0] = color.redHex();
            m_image[w_3 * yy + 3 * xx + 1] = color.greenHex();
            m_image[w_3 * yy + 3 * xx + 2] = color.blueHex();
        }
    }

    return true;
}


int Raytracer::saveScene(const std::string& filename)
{
    stbi_write_png(filename.c_str(), m_width, m_height, 3, m_image, m_width * 3);
}

}

