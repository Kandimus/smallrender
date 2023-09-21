#pragma once

#include <string>

#include "defines.h"

namespace Render
{

class Camera;
class ColorRGB;
class Matrix4;
class Ray;
class Scene;
class Triangle;

class Raytracer
{
public:
    Raytracer() = default;
    virtual ~Raytracer();

    unsigned char* image() const { return m_image; }
    int image_width() const { return m_width; }
    int image_height() const { return m_height; }

    int renderScene(int h, const Scene& scene, const std::string& cameraName = "");
    int saveScene(const std::string& filename);

private:
    REAL calculatePoint(const Ray& ray, const Triangle& t, ColorRGB& c);

private:
    unsigned char* m_image = nullptr;
    unsigned int m_size = 0;
    int m_width = 320;
    int m_height = 240;

    const Camera* m_camera = nullptr;
    const Scene* m_scene = nullptr;

    int m_debugIntX = 0;
    int m_debugIntY = 0;
};

}
