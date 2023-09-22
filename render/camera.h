#pragma once

#include <string>

#include "i_object.h"
#include "ray.h"
#include "quaternion.h"
#include "vector2.h"

namespace tinygltf
{
struct Node;
class Model;
}

namespace Render
{
class CameraInternal;

 //TODO Переделать или на UVN вектора (как в Vers) или на кватернионы
class Camera : public IObject
{
public:
    Camera();
    virtual ~Camera();

public:
    std::string& name() { return m_name; }
    const std::string& name() const { return m_name; }

    Vector3& position() { m_isChangedPosition = true; return m_position; }

    const Quaternion& orientation() const { return m_orientation; }
    const Vector3& position() const { return m_position; }
    const Vector3& direction() const { return m_direction; }
    const Vector3& right() const { return m_right; }
    const Vector3& up() const { return m_up; }

    REAL aspect() const { return m_aspect; }

    void setViewport(REAL left, REAL right, REAL top, REAL bottom);
    void setViewport(int h);
    void setScreen(int h);

    Ray ray(int screen_x, int screen_y) const;
    Ray centralRay() const;

    void setChanged() {  m_isChangedPosition = m_isChangedDirection = true; }
    bool isChanged() const {  return m_isChangedPosition || m_isChangedDirection; }
    void update();
    void reset();

    void setDirectionInverse() { m_direction = -m_direction; } // TODO убрать, и разобраться почему камера в бинарнике и в тексте ведет себя по разному

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

    // IObject
    virtual bool intersect(const Ray& ray) const override { return false; }
    virtual void tranformation(const Matrix4& m4) override;
    virtual std::string toString() const override;

private:
    void updatePosition()
    {
        if (!m_isChangedPosition)
        {
            return;
        }
    }

    void updateDirection()
    {
        if (!m_isChangedDirection)
        {
            return;
        }
    }

protected:
    std::string m_name = "camera";

    Vector3 m_position;
    Vector3 m_direction;
    Vector3 m_up;
    Vector3 m_right;
    Quaternion m_orientation;

    REAL m_portLeft; // 0..1
    REAL m_portRight; // 0..1
    REAL m_portTop; // 0..1
    REAL m_portBottom; // 0..1

    int  m_width = 640;
    int  m_height = 480;

    // Frustum
    REAL m_fov;
    REAL m_near;
    REAL m_far;
    REAL m_aspect;

    // Frustum precalculated
    CameraInternal* d;

    bool m_isChangedPosition;
    bool m_isChangedDirection;
};

}
