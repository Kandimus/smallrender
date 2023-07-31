#pragma once

#include "ray.h"
#include "vector2.h"

namespace Render
{

class Camera : public Ray
{
public:
    virtual ~Camera() = default;
    static Camera& instance() { static Camera singleton; return singleton; }

private:
    Camera();
    Camera(const Camera&);
    Camera& operator=(Camera&);


public:
    Vector3& position() { return m_position; }
    Ray& ray() {return *this; }

    const Vector3& position() const { return m_position; }
    const Ray& ray() const { return *this; }

    void setViewport(const Vector2& v) { m_fov = v; }
    void setViewport(const float fLeft, const float fRight, const float fTop, const float fBottom);

protected:
    Vector3 m_position;
    Vector2 m_fov;
//    int m_width = 320;
//    int m_height = 240;
};

}
