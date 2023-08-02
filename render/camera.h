#pragma once

#include "ray.h"
#include "quaternion.h"
#include "vector2.h"

namespace Render
{

class Camera : public Ray
{
public:
    virtual ~Camera() = default;
    static Camera& instance() { static Camera singleton; return singleton; }

private:
    Camera() : Ray() {  };
    Camera(const Camera&);
    Camera& operator=(Camera&);


public:
//    const Matrix4& matix() const { return m_matrix; }
//    Matrix4& matix() { return m_matrix; }


    Vector3& position() { return m_position; }
    Ray& ray() {return *this; }
    Quaternion& rotation() { return m_rotation; }

    const Vector3& position() const { return m_position; }
    const Ray& ray() const { return *this; }
    const Quaternion& rotation() const { return m_rotation; }

    void setViewport(const Vector2& v) { m_fov = v; }
    void setViewport(const float fLeft, const float fRight, const float fTop, const float fBottom);

protected:
    Vector3 m_position;
    Quaternion m_rotation;

    Vector2 m_fov;
//    Matrix4 m_matrix;

};

}
